#define _VideoPlayback
#include "joint.h"
#include <d3d9.h>
#include <dshow.h>
#include <vmr9.h>

extern IDirect3DDevice9* RealDevice;
extern BYTE LoadedVideoCount;
DWORD g_userid=0;

class CAllocator : public IVMRSurfaceAllocator9, IVMRImagePresenter9 {
private:
    IDirect3DSurface9* surface;
    IVMRSurfaceAllocatorNotify9 *pAllocNotify;
    ULONG RefCount;
    IDirect3DTexture9* ptex;
public:
    IDirect3DTexture9* tex;
    CAllocator() {
        RefCount=1;
        surface=NULL;
        pAllocNotify=NULL;
        tex=NULL;
        ptex=NULL;
    }
    ULONG _stdcall AddRef() {
        return ++RefCount;
    }
    ULONG _stdcall Release() {
        if(--RefCount==0) {
            TerminateDevice(0);
            if(pAllocNotify) {
                pAllocNotify->Release();
                pAllocNotify=NULL;
            }
        }
        return RefCount;
    }
    HRESULT _stdcall QueryInterface(const IID &riid, void** ppvObject) {
        HRESULT hr = E_NOINTERFACE;

        if( ppvObject == NULL ) {
            hr = E_POINTER;
        } else if( riid == IID_IVMRSurfaceAllocator9 ) {
            *ppvObject = static_cast<IVMRSurfaceAllocator9*>( this );
            AddRef();
            hr = S_OK;
        } else if( riid == IID_IVMRImagePresenter9 ) {
            *ppvObject = static_cast<IVMRImagePresenter9*>( this );
            AddRef();
            hr = S_OK;
        } else if( riid == IID_IUnknown ) {
            *ppvObject = static_cast<IUnknown*>( static_cast<IVMRSurfaceAllocator9*>( this ) );
            AddRef();
            hr = S_OK;    
        }

        return hr;
    }

    HRESULT _stdcall InitializeDevice(DWORD_PTR dwUserID, VMR9AllocationInfo *lpAllocInfo, DWORD *lpNumBuffers) {
        HRESULT hr;
        //Set the device
        IDirect3D9* d3d;
        RealDevice->GetDirect3D(&d3d);
        hr = pAllocNotify->SetD3DDevice(RealDevice, d3d->GetAdapterMonitor(0));
        d3d->Release();
        //if(hr!=S_OK) return hr;

        lpAllocInfo->dwFlags |= VMR9AllocFlag_TextureSurface;
        lpAllocInfo->Pool = D3DPOOL_SYSTEMMEM;
        // Ask the VMR-9 to allocate the surfaces for us.
        hr = pAllocNotify->AllocateSurfaceHelper(lpAllocInfo, lpNumBuffers, &surface);
        if(hr!=S_OK) return hr;

        hr=surface->GetContainer(IID_IDirect3DTexture9,(void**)&ptex);
        if(hr!=S_OK) {
            TerminateDevice(0);
            return hr;
        }

        RealDevice->CreateTexture(lpAllocInfo->dwWidth,lpAllocInfo->dwHeight,1,0,lpAllocInfo->Format,
            D3DPOOL_DEFAULT,&tex,NULL);

        return S_OK;
    }

    HRESULT _stdcall TerminateDevice(DWORD_PTR dwID) {
        if(ptex) {
            ptex->Release();
            ptex=NULL;
        }
        if(surface) {
            surface->Release();
            surface=NULL;
        }
        if(tex) {
            tex->Release();
            tex=NULL;
        }
        return S_OK;
    }

    HRESULT _stdcall GetSurface(DWORD_PTR dwUserID, DWORD SurfaceIndex, DWORD SurfaceFlags, IDirect3DSurface9 **lplpSurface) {
        if(SurfaceIndex!=0) return E_FAIL;
        *lplpSurface=surface;
        surface->AddRef();
        return S_OK;
    }

    HRESULT _stdcall AdviseNotify(IVMRSurfaceAllocatorNotify9 *lpIVMRSurfAllocNotify) {
        if(lpIVMRSurfAllocNotify) {
            pAllocNotify=lpIVMRSurfAllocNotify;
            pAllocNotify->AddRef();
            return S_OK;
        } else {
            return E_FAIL;
        }
    }

    HRESULT _stdcall StartPresenting(DWORD_PTR dwUserID) {
        return S_OK;
    }
    HRESULT _stdcall StopPresenting(DWORD_PTR dwUserID) {
        return S_OK;
    }
    HRESULT _stdcall PresentImage(DWORD_PTR dwUserID,VMR9PresentationInfo *lpPresInfo) {
        RealDevice->UpdateTexture(ptex,tex);
        return S_OK;
    }
};


struct sDSTexture {
    IGraphBuilder *pGraph;
    ICaptureGraphBuilder2 *pBuild;
    IBaseFilter *pVmr;
    IVMRFilterConfig9 *pConfig;
    IVMRSurfaceAllocatorNotify9 *pAlloc;
    IMediaControl *pControl;
    CAllocator *pMyAlloc;
    IMediaSeeking *pSeek;
};
void ResumeMovie(sDSTexture* movie) {
    movie->pControl->Run();
}
void PauseMovie(sDSTexture* movie) {
    movie->pControl->Pause();
}
void StopMovie(sDSTexture* movie) {
    movie->pControl->Stop();
}
void RewindMovie(sDSTexture* movie) {
    LONGLONG time=0;
    movie->pSeek->SetPositions(&time,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
}
void SeekMovie(sDSTexture* movie,DWORD shortTime) {
    LONGLONG time=shortTime*10000;
    movie->pSeek->SetPositions(&time,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
}
sDSTexture* FreeMovie(sDSTexture* info) {
    LoadedVideoCount--;
    if(info->pControl) info->pControl->Release();
    if(info->pSeek) info->pSeek->Release();
    if(info->pAlloc) info->pAlloc->Release();
    if(info->pMyAlloc) info->pMyAlloc->Release();
    if(info->pConfig) info->pConfig->Release();
    if(info->pVmr) info->pVmr->Release();
    if(info->pGraph) info->pGraph->Release();
    if(info->pBuild) info->pBuild->Release();
    ZeroPointer(info);
    return NULL;
}
sDSTexture* CreateDSGraph(char* path,IDirect3DTexture9** tex) {
    g_userid++;
    HRESULT hr;
    size_t unused;
    wchar_t wpath[256];
    mbstowcs_s(&unused,wpath,256,path,256);
    *tex=NULL;

    sDSTexture* result=new sDSTexture();
    ZeroPointer(result);

    LoadedVideoCount++;

    // Create the Capture Graph Builder.
    hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, 0, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&result->pBuild );
    if(hr!=S_OK) return FreeMovie(result);

    // Create the Filter Graph Manager.
    hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&result->pGraph);
    if(hr!=S_OK) return FreeMovie(result);

    // Initialize the Capture Graph Builder.
    hr = result->pBuild->SetFiltergraph(result->pGraph);
    if(hr!=S_OK) return FreeMovie(result);

    hr = CoCreateInstance(CLSID_VideoMixingRenderer9, 0, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&result->pVmr);
    if(hr!=S_OK) return FreeMovie(result);

    hr = result->pGraph->AddFilter(result->pVmr, L"VMR9");
    if(hr!=S_OK) return FreeMovie(result);

    result->pVmr->QueryInterface(IID_IVMRFilterConfig9, (void**)&result->pConfig);
    result->pConfig->SetRenderingMode(VMR9Mode_Renderless);
    result->pVmr->QueryInterface(IID_IVMRSurfaceAllocatorNotify9,(void**)&result->pAlloc);

    result->pMyAlloc=new CAllocator();

    result->pMyAlloc->AdviseNotify(result->pAlloc);
    result->pAlloc->AdviseSurfaceAllocator(g_userid, (IVMRSurfaceAllocator9*)result->pMyAlloc);

    hr = result->pGraph->QueryInterface(IID_IMediaControl, (void**)&result->pControl);
    hr |= result->pGraph->QueryInterface(IID_IMediaSeeking, (void**)&result->pSeek);
    if(hr!=S_OK) return FreeMovie(result);
    
    //hr = pGraph->RenderFile(L"C:\\Games\\Morrowind\\Data Files\\MGE videos\\test.avi", NULL);
    result->pGraph->RenderFile(wpath, NULL);
    result->pControl->Run();

    *tex=result->pMyAlloc->tex;
    return result;
}
