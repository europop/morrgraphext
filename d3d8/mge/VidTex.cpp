//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d9.h>
#include <dshow.h>
#include <Vmr9.h>

extern IDirect3D9* RealObject;
extern IDirect3DDevice9* RealDevice;

#define SAFERELEASE(a) { if(a) { a->Release(); a=0; } }

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
        hr = pAllocNotify->SetD3DDevice(RealDevice, RealObject->GetAdapterMonitor(0));
        //if(hr!=S_OK) return hr;

        lpAllocInfo->dwFlags |= VMR9AllocFlag_TextureSurface;
        lpAllocInfo->Pool = D3DPOOL_SYSTEMMEM;
        // Ask the VMR-9 to allocate the surfaces for us.
        hr = pAllocNotify->AllocateSurfaceHelper(lpAllocInfo, lpNumBuffers, &surface);
        if(FAILED(hr)) return hr;

        hr=surface->GetContainer(IID_IDirect3DTexture9,(void**)&ptex);
        if(FAILED(hr)) {
            TerminateDevice(0);
            return hr;
        }

        RealDevice->CreateTexture(lpAllocInfo->dwWidth,lpAllocInfo->dwHeight,1,0,lpAllocInfo->Format, D3DPOOL_DEFAULT,&tex,NULL);

        return S_OK;
    }

    HRESULT _stdcall TerminateDevice(DWORD_PTR dwID) {
		SAFERELEASE(ptex);
		SAFERELEASE(surface);
		SAFERELEASE(tex);
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

static DWORD _FreeMovie(sDSTexture* info) {
    if(info->pControl) info->pControl->Release();
    if(info->pSeek) info->pSeek->Release();
    if(info->pAlloc) info->pAlloc->Release();
    if(info->pMyAlloc) info->pMyAlloc->Release();
    if(info->pConfig) info->pConfig->Release();
    if(info->pVmr) info->pVmr->Release();
    if(info->pGraph) info->pGraph->Release();
    if(info->pBuild) info->pBuild->Release();
	return 0;
}
void FreeMovie(void* movie) { _FreeMovie((sDSTexture*)movie); }
static DWORD CreateDSGraph(wchar_t* path,IDirect3DTexture9** tex,sDSTexture* result) {
	memset(result, 0, sizeof(sDSTexture));

    // Create the Capture Graph Builder.
    HRESULT hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, 0, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&result->pBuild );
    if(hr!=S_OK) return _FreeMovie(result);

    // Create the Filter Graph Manager.
    hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&result->pGraph);
    if(hr!=S_OK) return _FreeMovie(result);

    // Initialize the Capture Graph Builder.
    hr = result->pBuild->SetFiltergraph(result->pGraph);
    if(hr!=S_OK) return _FreeMovie(result);

    hr = CoCreateInstance(CLSID_VideoMixingRenderer9, 0, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&result->pVmr);
    if(hr!=S_OK) return _FreeMovie(result);

    hr = result->pGraph->AddFilter(result->pVmr, L"VMR9");
    if(hr!=S_OK) return _FreeMovie(result);

    result->pVmr->QueryInterface(IID_IVMRFilterConfig9, (void**)&result->pConfig);
    result->pConfig->SetRenderingMode(VMR9Mode_Renderless);
    result->pVmr->QueryInterface(IID_IVMRSurfaceAllocatorNotify9,(void**)&result->pAlloc);

    result->pMyAlloc=new CAllocator();

    result->pMyAlloc->AdviseNotify(result->pAlloc);
    result->pAlloc->AdviseSurfaceAllocator(0, (IVMRSurfaceAllocator9*)result->pMyAlloc);

    hr = result->pGraph->QueryInterface(IID_IMediaControl, (void**)&result->pControl);
    hr |= result->pGraph->QueryInterface(IID_IMediaSeeking, (void**)&result->pSeek);
    if(hr!=S_OK) return _FreeMovie(result);

    result->pGraph->RenderFile(path, NULL);
    result->pControl->Run();

    *tex=result->pMyAlloc->tex;
    return 1;
}

void Restart(void* ptr) {
	IMediaSeeking* pSeek=((sDSTexture*)ptr)->pSeek;
	if(pSeek) {
		__int64 pos, end;
		pSeek->GetCurrentPosition(&pos);
		pSeek->GetStopPosition(&end);
		__int64 time=0;
		if(end==pos) pSeek->SetPositions(&time,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
	}
}

bool LoadMovie(const char* path, IDirect3DTexture9** tex, void** ptr) {
	//Get file path in unicode
	wchar_t wpath[MAX_PATH];
	wcscpy_s(wpath, MAX_PATH, L"Data files\\MGE videos\\");
	DWORD offset=wcslen(wpath);
	DWORD len=0;
	while(path[len]) { wpath[len+offset]=path[len]; len++; }
	wpath[len+offset]=0;

	//Check for existance of file
	HANDLE h=CreateFileW(wpath,GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if(h==INVALID_HANDLE_VALUE) return false;
	CloseHandle(h);

	//Create texture and dshow info
	sDSTexture* info=new sDSTexture();
	if(!CreateDSGraph(wpath, tex, info)) {
		delete info;
		return false;
	} else {
		*ptr=info;
		return true;
	}
}
