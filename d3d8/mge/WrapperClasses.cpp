#define _WrapperClasses

#include "Configuration.h"
#include "WrapperClasses.h"
#include "VidTex.h"
#include "FogFormulas.h"
#include "MWBridge.h"
#include "MWSEFunctions.h"
#include "MorrowindBSA.h"
#include "DistantLand.h"
#include "DepthManager.h"
#include "Patch.h"
#include "MWE.h"

static int clientWND_width, clientWND_height;
static HWND client_hWND;
static bool WindowedMode;
static bool TriedOnce = false;

//#define NV_PERF_HUD

static bool RenderingToTexture = false;
static bool RenderedLand = false;
static bool ShadowPlaneDrawn = false;
static bool RenderedGrass = false;
//True when the main menu or game loading screens are showing
static bool LoadingScreen = true;

static float MatrixHUDidentifier;
static bool HUDRenderingStarted = false;
static bool ShadersApplied = false;
static bool BackBufferWasCleared = false;
static bool zWriteEnabled = 0;
static DWORD CurrentSrcBlend = 0;
static DWORD CurrentDestBlend = 0;

static IDirect3D8 *fakeDirect3D;
static IDirect3DDevice8 *fakeDevice;
static D3DCAPS8 d3d8Caps;

EX IDirect3DTexture9 *FakeWaterTexture;

static IDirect3DSurface9 *OldDepthSurface;

//#include <fstream>
//using namespace std;
//ofstream wrapper_log_file ("C:\\mge_wrapper_log.txt");

static int tcount = 0;

//=============================================================================


class FakeSurface : public IDirect3DSurface8 {

public:
	IDirect3DSurface9 *RealObject;


	//-----------------------------------------------------------------------------
	/*** Constructor ***/
	//-----------------------------------------------------------------------------

	FakeSurface (IDirect3DSurface9 *Real) {
		RealObject = Real;
		void *This = this;
		Real->SetPrivateData (guid, (void *)&This, 4, 0);
	}


	//-----------------------------------------------------------------------------
	/*** IUnknown methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall QueryInterface (REFIID riid, void **ppvObj) {
		return RealObject->QueryInterface (riid, ppvObj);
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall AddRef () {
		return RealObject->AddRef ();
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall Release () {
		ULONG Refs = RealObject->Release ();
		if (!Refs) {
			delete this;
			return 0;
		}
		return Refs;
	}


	//-----------------------------------------------------------------------------
	/*** IDirect3DSurface8 methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDevice (IDirect3DDevice8 **ppDevice) {
		*ppDevice = fakeDevice;
		fakeDevice->AddRef ();
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPrivateData (REFGUID refguid, CONST void *pData, DWORD SizeOfData, DWORD Flags) {
		return RealObject->SetPrivateData (refguid, pData, SizeOfData, Flags);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPrivateData (REFGUID refguid, void *pData, DWORD *pSizeOfData) {
		return RealObject->GetPrivateData (refguid, pData, pSizeOfData);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall FreePrivateData (REFGUID refguid) {
		return RealObject->FreePrivateData (refguid);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetContainer (REFIID riid, void **ppContainer) {
		return RealObject->GetContainer (riid, ppContainer);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDesc (D3DSURFACE_DESC8 *pDesc) {
		D3DSURFACE_DESC a2;
		HRESULT hr = RealObject->GetDesc (&a2);
		if (hr == D3D_OK) {
			pDesc->Format = a2.Format;
			pDesc->Height = a2.Height;
			pDesc->MultiSampleType = a2.MultiSampleType;
			pDesc->Pool = a2.Pool;
			pDesc->Size = 0; //TODO: Fix;
			pDesc->Type = a2.Type;
			pDesc->Usage = a2.Usage;
			pDesc->Width = a2.Width;
		}
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall LockRect (D3DLOCKED_RECT *pLockedRect, CONST RECT *pRect, DWORD Flags) {
		return RealObject->LockRect (pLockedRect, pRect, Flags);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall UnlockRect () {
		return RealObject->UnlockRect ();
	}

	//-----------------------------------------------------------------------------
};


//=============================================================================


class FakeTexture : public IDirect3DTexture8 {

private:
	int ltcount;


public:
	IDirect3DTexture9 *RealObject;


	//-----------------------------------------------------------------------------
	/*** Constructor ***/
	//-----------------------------------------------------------------------------

	FakeTexture (IDirect3DTexture9 *Real) {
		ltcount = tcount++;
		if (ltcount == 76) {
			int iii = 0;	// What is this for? /krzymark/
		}
		RealObject = Real;
		void *This = this;
		Real->SetPrivateData (guid, (void *)&This, 4, 0);
	}

	//-----------------------------------------------------------------------------
	/*** IUnknown methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall QueryInterface (REFIID riid, void **ppvObj) {
		return RealObject->QueryInterface (riid, ppvObj);
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall AddRef () {
		return RealObject->AddRef ();
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall Release () {
		if (ltcount == 76) {
			int iii = 0;	// What is this for? /krzymark/
		}
		ULONG Refs = RealObject->Release ();
		if (!Refs) {
			delete this;
			return 0;
		}
		return Refs;
	}


	//-----------------------------------------------------------------------------
	/*** IDirect3DBaseTexture8 methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDevice (IDirect3DDevice8 **ppDevice) {
		*ppDevice = fakeDevice;
		fakeDevice->AddRef ();
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPrivateData (REFGUID refguid, CONST void *pData, DWORD SizeOfData, DWORD Flags) {
		return RealObject->SetPrivateData (refguid, pData, SizeOfData, Flags);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPrivateData (REFGUID refguid, void *pData, DWORD *pSizeOfData) {
		return RealObject->GetPrivateData (refguid, pData, pSizeOfData);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall FreePrivateData (REFGUID refguid) {
		return RealObject->FreePrivateData (refguid);
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall SetPriority (DWORD PriorityNew) {
		return RealObject->SetPriority (PriorityNew);
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall GetPriority () {
		return RealObject->GetPriority ();
	}

	//-----------------------------------------------------------------------------

	void _stdcall PreLoad () {
		return RealObject->PreLoad ();
	}

	//-----------------------------------------------------------------------------

	D3DRESOURCETYPE _stdcall GetType () {
		return RealObject->GetType ();
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall SetLOD (DWORD LODNew) {
		return RealObject->SetLOD (LODNew);
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall GetLOD () {
		return RealObject->GetLOD ();
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall GetLevelCount () {
		return RealObject->GetLevelCount ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetLevelDesc (UINT Level, D3DSURFACE_DESC8 *pDesc) {
		D3DSURFACE_DESC b2;
		HRESULT hr = RealObject->GetLevelDesc (Level, &b2);
		if (hr == D3D_OK) {
			pDesc->Format = b2.Format;
			pDesc->Height = b2.Height;
			pDesc->MultiSampleType = b2.MultiSampleType;
			pDesc->Pool = b2.Pool;
			pDesc->Size = 0; //TODO: Fix;
			pDesc->Type = b2.Type;
			pDesc->Usage = b2.Usage;
			pDesc->Width = b2.Width;
		}
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetSurfaceLevel (UINT Level, IDirect3DSurface8 **ppSurfaceLevel) {
		IDirect3DSurface9 *b2;
		*ppSurfaceLevel = NULL;
		DWORD unused = 4;
		HRESULT hr = RealObject->GetSurfaceLevel (Level, &b2);
		if (hr != D3D_OK || b2 == NULL) return hr;
		hr = b2->GetPrivateData (guid, (void *)ppSurfaceLevel, &unused);
		if (hr != D3D_OK) *ppSurfaceLevel = new FakeSurface (b2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall LockRect (UINT Level, D3DLOCKED_RECT *pLockedRect, CONST RECT *pRect, DWORD Flags) {
		return RealObject->LockRect (Level, pLockedRect, pRect, Flags);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall UnlockRect (UINT Level) {
		return RealObject->UnlockRect (Level);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall AddDirtyRect (CONST RECT *pDirtyRect) {
		return RealObject->AddDirtyRect (pDirtyRect);
	}

	//-----------------------------------------------------------------------------
};


//=============================================================================


class TextureHook : public IDirect3DTexture8 {

public:
	DWORD refs;
	BYTE HookIndex;
	DWORD w;
	DWORD h;
	BYTE *locked;


	//-----------------------------------------------------------------------------
	/*** Misc methods ***/
	//-----------------------------------------------------------------------------

	TextureHook (BYTE index, DWORD W, DWORD H) {
		HookIndex = index;
		w = W;
		h = H;
		refs = 2;
		locked = NULL;
	}


	//-----------------------------------------------------------------------------
	/*** IUnknown methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall QueryInterface (REFIID a, LPVOID *b) {
		return E_NOINTERFACE;
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall AddRef (void) {
		return ++refs;
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall Release (void) {
		if (--refs == 0) {
			if (HookedTextures [HookIndex].AutoClean) {
				if (HookedTextures [HookIndex].avi) FreeMovie (HookedTextures [HookIndex].avi);
				else MWSE::FreeTexture (HookedTextures [HookIndex].TextureIndex);
				MWSE::FreeEffect (HookedTextures [HookIndex].ShaderIndex);
				ZeroStruct (HookedTextures [HookIndex]);
			}
			HookedTextures [HookIndex].tHook = 0;
			delete this;
			return 0;
		}
		return refs;
	}


	//-----------------------------------------------------------------------------
	/*** IDirect3DResource8 methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDevice (IDirect3DDevice8 **a) {
		return D3DERR_INVALIDCALL;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPrivateData (REFGUID a, const void *b, DWORD c, DWORD d) {
		return D3DERR_INVALIDCALL;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPrivateData (REFGUID a, void *b, DWORD *c) {
		return D3DERR_INVALIDCALL;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall FreePrivateData (REFGUID a) {
		return D3DERR_INVALIDCALL;
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall SetPriority (DWORD a) {
		return a;
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall GetPriority (void) {
		return HookIndex;
	}

	//-----------------------------------------------------------------------------

	void _stdcall PreLoad () {
	}

	//-----------------------------------------------------------------------------

	D3DRESOURCETYPE _stdcall GetType () {
		return D3DRTYPE_TEXTURE;
	}


	//-----------------------------------------------------------------------------
	/*** IDirect3DBaseTexture8 methods ***/
	//-----------------------------------------------------------------------------

	DWORD _stdcall SetLOD (DWORD a) {
		return a;
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall GetLOD () {
		return HOOKEDTEXTUREID;
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall GetLevelCount () {
		return 1;
	}


	//-----------------------------------------------------------------------------
	/*** IDirect3DTexture8 methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetLevelDesc (UINT a, D3DSURFACE_DESC8 *b) {
		return D3DERR_INVALIDCALL;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetSurfaceLevel (UINT a, IDirect3DSurface8 **b) {
		return D3DERR_INVALIDCALL;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall LockRect (UINT a, D3DLOCKED_RECT *b, CONST RECT *c, DWORD d) {
		if (locked) return D3DERR_INVALIDCALL;
		b->Pitch = w * 4;
		b->pBits = new BYTE [w * h * 4];
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall UnlockRect (UINT a) {
		if (locked) {
			delete [w * h * 4] locked;
			locked = NULL;
		}
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall AddDirtyRect (const RECT *a) {
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------
};


//=============================================================================


class FakeDevice : public IDirect3DDevice8 {

public:

	//-----------------------------------------------------------------------------
	/*** Fields ***/
	//-----------------------------------------------------------------------------

	IDirect3D8 *Object;
	DWORD Refs;
	UINT BaseVertexIndex;

	bool RecordRenders;
	D3DXMATRIX CurrentWorldTrans;
	const D3DMATERIAL8 *CurrentMaterial;
	IDirect3DTexture9 *CurrentTexture;
//	IDirect3DTexture9 *SecondaryTexture; //phal
	IDirect3DVertexBuffer9 *CurrentVBuffer;
	IDirect3DIndexBuffer9 *CurrentIndices;
	DWORD CurrentVertexBlendState;
	D3DXMATRIX CurrentBlendTrans1;
	D3DXMATRIX CurrentBlendTrans2;
	D3DXMATRIX CurrentBlendTrans3;
	DWORD CurrentCullModeState;

	DWORD CurrentFVF;
	UINT CurrentStreamSpan;
	DWORD CurrentPShader;
	DWORD CurrentVShader;
	IDirect3DVertexShader9 *NewVShaders [4];
	IDirect3DPixelShader9 *NewPShaders [9];
	DWORD pShaderCount;
	DWORD vShaderCount;
	IDirect3DVertexDeclaration9 *decl [2];

	struct RenderState {
		DWORD FVF;
		DWORD AlphaBlendEnable;
		DWORD AlphaTestEnable;
		DWORD SrcBlend;
		DWORD DestBlend;
		DWORD CullMode;
		DWORD VertexBlend;
		DWORD StencilEnable;
		DWORD ZEnable;
		DWORD ZWriteEnable;
		D3DMATRIX View;
		D3DMATRIX Projection;
		D3DMATRIX World;

		RenderState() {
			//Set default values
			FVF = 0;
			AlphaBlendEnable = 0;
			AlphaTestEnable = 0;
			SrcBlend = 0;
			DestBlend = 0;
			CullMode = D3DCULL_CCW;
			VertexBlend = 0;
			ZEnable = 1;
			ZWriteEnable = 1;
			
		}

		void RestoreState() {
			RealDevice->SetFVF(FVF);
			RealDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, AlphaBlendEnable );
			RealDevice->SetRenderState( D3DRS_ALPHATESTENABLE, AlphaTestEnable );
			RealDevice->SetRenderState( D3DRS_SRCBLEND, SrcBlend );
			RealDevice->SetRenderState( D3DRS_DESTBLEND, DestBlend );
			RealDevice->SetRenderState( D3DRS_CULLMODE, CullMode );
			RealDevice->SetRenderState( D3DRS_VERTEXBLEND, VertexBlend );
			RealDevice->SetRenderState( D3DRS_STENCILENABLE, StencilEnable );
			RealDevice->SetRenderState( D3DRS_ZENABLE, ZEnable );
			RealDevice->SetRenderState( D3DRS_ZWRITEENABLE, ZWriteEnable );
			RealDevice->SetTransform( D3DTS_VIEW, &View );
			RealDevice->SetTransform( D3DTS_PROJECTION, &Projection );
			RealDevice->SetTransform( D3DTS_WORLD, &World );
		}
	};

	RenderState m_MWRenderState;

	//-----------------------------------------------------------------------------
	/*** Misc methods ***/
	//-----------------------------------------------------------------------------

	FakeDevice (IDirect3D8 *Ob) {
		Object = Ob;
		Refs = 1;
		//TempBool = false;
		BaseVertexIndex = 0;
		CurrentPShader = 0;
		CurrentVShader = 0;
		pShaderCount = 0;
		vShaderCount = 1;
		RecordRenders = false;
		ZeroStruct (NewVShaders);
		ZeroStruct (NewPShaders);
		D3DVERTEXELEMENT9 elements [6] = {
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION},
			{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD},
			D3DDECL_END (),
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION},
			{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR},
			D3DDECL_END ()
		};
		RealDevice->CreateVertexDeclaration (elements, &decl [0]);
		RealDevice->CreateVertexDeclaration (&elements [3], &decl [1]);
	}


	//-----------------------------------------------------------------------------
	/*** IUnknown methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall QueryInterface (REFIID a, LPVOID *b) {
		return RealDevice->QueryInterface (a, b);
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall AddRef (void) {
		return ++Refs;
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall Release (void) {
		if (--Refs == 0) {
			DEBUGSTRING ("MGE: Graphics device lost. Wiping all loaded MWSE objects.\n");
			for (UINT i = 0; i < MAXMWSEOBJECTS; ++i) if (Objects [i].Created) MWSE::FreeModel (i);
			for (UINT i = 0; i < MAXMWSESHADERS; ++i) if (ShaderStruct.MWSEeffects [i].effect != NULL) MWSE::FreeEffect (i);
			for (UINT i = 0; i < MAXMWSESHADERS; ++i) if (LoadedTextures [i].texture != NULL) MWSE::FreeTexture (i);
			for (UINT i = 0; i < MAXMWSEOBJECTS; ++i) if (HUDbits [i].texture) MWSE::FreeHUD (i);
			for (UINT i = 0; i < MAXHOOKEDTEXTURES; ++i) {
				if (HookedTextures [i].tHook) {
					delete HookedTextures [i].tHook;
					HookedTextures [i].tHook = 0;
				}
			}
			if (SunTexture != NULL) SunTexture->Release ();
			if (HUDsprite != NULL) HUDsprite->Release ();
			DEBUGSTRING ("MGE: Graphics device lost. Releasing all created resources.\n");
			if (ShaderStruct.thisframeSurface != NULL) ShaderStruct.thisframeSurface->Release ();
			if (ShaderStruct.lastframeSurface != NULL) ShaderStruct.lastframeSurface->Release ();
			if (ShaderStruct.lastpassSurface != NULL) ShaderStruct.lastpassSurface->Release ();
			if (ShaderStruct.lastshaderSurface != NULL) ShaderStruct.lastshaderSurface->Release ();
			if (ShaderStruct.thisframe != NULL) ShaderStruct.thisframe->Release ();
			if (ShaderStruct.lastframe != NULL) ShaderStruct.lastframe->Release ();
			if (ShaderStruct.lastpass != NULL) ShaderStruct.lastpass->Release ();
			if (ShaderStruct.lastshader != NULL) ShaderStruct.lastshader->Release ();
			if (ShaderStruct.SoftwareSurface != NULL) ShaderStruct.SoftwareSurface->Release ();
			if (ShaderStruct.buffer != NULL) ShaderStruct.buffer->Release ();
			for (int i = 0; i < ShaderStruct.DefEffectCount; ++i) {
				if (ShaderStruct.DefaultEffects [i].effect) {
					ShaderStruct.DefaultEffects [i].effect->Release ();
					ShaderStruct.DefaultEffects [i].effect = 0;
				}
				ShaderStruct.DefEffectCount = 0; //mendres: Not resetting causing crash after DL Reset.
			}
			if (BackBuffer != NULL) while (BackBuffer->Release () > 0);
			if (DepthTexture != NULL) DepthTexture->Release ();
			if (DepthBuffer != NULL) while (DepthBuffer->Release () > 0);
			if (tempShaderTexture != NULL) tempShaderTexture->Release ();
			if (Font != NULL) Font->Release ();
			delete this;

			//Release depth manager
			if ( g_DepthManager.IsInitialized() ) {
				g_DepthManager.Shutdown();
			}

			//Release distant land resources
			DL_Shutdown ();
			ClearBSATextureCache ();
			RealDevice->Release ();
			HookNextTexture = false;
			NextTexIsWater = false;
			NextHookIndex = 0;
			//ZeroStruct (HookedTextures);	//mendres
			HookedShaderActive = 0;
			ActiveHookedShader = 0;
			
			return 0;
		}
		return Refs;
	}


	//-----------------------------------------------------------------------------
	/*** IDirect3DDevice8 methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall TestCooperativeLevel (void) {
		return RealDevice->TestCooperativeLevel ();
	}

	//-----------------------------------------------------------------------------

	UINT _stdcall GetAvailableTextureMem (void) {
		return RealDevice->GetAvailableTextureMem ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall ResourceManagerDiscardBytes (DWORD a) {
		return D3D_OK;
		//return RealDevice->EvictManagedResources ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDirect3D (IDirect3D8 **a) {
		*a = Object;
		Object->AddRef ();
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDeviceCaps (D3DCAPS8 *a) {
		*a = d3d8Caps;
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDisplayMode (D3DDISPLAYMODE *a) {
		return RealDevice->GetDisplayMode (0, a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetCreationParameters (D3DDEVICE_CREATION_PARAMETERS *a) {
		return RealDevice->GetCreationParameters (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetCursorProperties (UINT a, UINT b, IDirect3DSurface8 *c) {
		//return RealDevice->SetCursorProperties (a, b, ((FakeSurface *)c)->RealObject);
		return 1;
	}

	//-----------------------------------------------------------------------------

	void _stdcall SetCursorPosition (int X, int Y, DWORD c) {
		//RealDevice->SetCursorPosition (X, Y, c);
	}

	//-----------------------------------------------------------------------------

	BOOL _stdcall ShowCursor (BOOL a) {
		//return RealDevice->ShowCursor (a);
		return 1;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateAdditionalSwapChain (D3DPRESENT_PARAMETERS8 *a, IDirect3DSwapChain8 **b) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall Reset (D3DPRESENT_PARAMETERS8 *a) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall Present (const RECT *a, const RECT *b, HWND c, const RGNDATA *d) {
		DECLARE_MWBRIDGE
		//Load Morrowind's dynamic memory pointers
		if (!mwBridge->IsLoaded () && mwBridge->CanLoad ()) {
			mwBridge->Load ();
		}

		//Ensure view distance is not being set higher than Morrowind's default max if distant land is turned on
		if ((MGEFlags & USE_DISTANT_LAND) && DistantLandActive && DL_HasStatics ()) {
			float viewDistance = mwBridge->GetViewDistance ();

			if (viewDistance > 7168.0f) {
				//View distance is too high, so cut it down
				viewDistance = 7168.0f;

				mwBridge->SetViewDistance (viewDistance);
			}
		}

		if (mwBridge->Is3rdPerson ()) {
			D3DXVECTOR3 *mwCam3Offset = mwBridge->PCam3Offset ();
			if (mwCam3Offset) {
				if (ForceCam3Offset) {
					mwCam3Offset->x = Cam3Offset.x;
					mwCam3Offset->y = Cam3Offset.y;
					mwCam3Offset->z = Cam3Offset.z;
				} else {
					Cam3Offset.x = mwCam3Offset->x;
					Cam3Offset.y = mwCam3Offset->y;
					Cam3Offset.z = mwCam3Offset->z;
				}
			}
		}

		CurrentDestBlend = D3DBLEND_INVSRCALPHA;
		RealDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//wraper_log_file << "=== Present ===" << std::endl;
		HRESULT hr;
		DWORD time = GetTickCount ();
		if (MGEFlags & USE_HDR) HDR_Update (time);
		RenderedLand = false;
		RenderedGrass = false;
		ShadowPlaneDrawn = false;
		//Update SinVar and LinVar variables
		if ((MGEFlags & SET_SHADER_VARS) || (UsingDistantLand && DistantLandActive)) {
			float TimePassed = ((float)(time % 1000)) / 1000.0f;
			LinVar.x = TimePassed;
			LinVar.y = TimePassed;
			LinVar.z = TimePassed;
			LinVar.w = 1 - TimePassed;
			//need to mod to stop it loosing resolution after a few hours. Makes it jerk a bit every minute or so.
			TimePassed = sin ((float)(time % 60001) * (0.00025f * PI));
			SinVar.x = TimePassed;
			SinVar.y = TimePassed;
			SinVar.z = TimePassed;
			SinVar.w = cos ((float)(time % 60001) * (0.00025f * PI));
			//moved to ApplyShaders () //phal
			/* for (int i = 0; i < MAXMWSESHADERS; ++i) {
				if (ShaderStruct.MWSEeffects [i].effect) {
					SetShaderVars (ShaderStruct.MWSEeffects [i].effect, time);
				}
			} */
		}
		//HUD
		if (UsedHUDbits) {
			RealDevice->BeginScene ();
			HUDsprite->Begin (D3DXSPRITE_ALPHABLEND);
			for (DWORD d = 0; d < MAXMWSEOBJECTS; ++d) {
				if (HUDbits [d].texture && HUDbits [d].Enabled) {
					if (HUDbits [d].effect) {
						UINT passes;
						HUDbits [d].effect->Begin (&passes, 0);
						for (UINT j = 0; j < passes; ++j) {
							HUDbits [d].effect->BeginPass (j);
							D3DXMATRIX m;
							D3DXVECTOR3 v;
							v.x = HUDbits [d].position.x;
							v.y = HUDbits [d].position.y;
							v.z = 0;
							HUDsprite->SetTransform (::D3DXMatrixScaling (&m, HUDbits [d].scale.x, HUDbits [d].scale.y, 1));
							HUDsprite->Draw (HUDbits [d].texture, 0, 0, &v, 0xFFFFFFFF);
							HUDbits [d].effect->EndPass ();
						}
						HUDbits [d].effect->End ();
					} else {
						D3DXMATRIX m;
						D3DXVECTOR3 v;
						v.x = HUDbits [d].position.x;
						v.y = HUDbits [d].position.y;
						v.z = 0;
						HUDsprite->SetTransform (::D3DXMatrixScaling (&m, HUDbits [d].scale.x, HUDbits [d].scale.y, 1));
						HRESULT hrr = HUDsprite->Draw (HUDbits [d].texture, 0, 0, &v, 0xFFFFFFFF);
					}
				}
			}
			HUDsprite->End ();
			RealDevice->EndScene ();
		}
		//Calculate frame time
		FrameTime = 0.0f;
		if (time > lastFrameTime) FrameTime = float (time - lastFrameTime) / 1000.0f;
		lastFrameTime = time;

		//Calculate fps
		++fpsFrames;
		if (time - fpsLastTime >= 1000) {
			fps = (float)fpsFrames / ((float)(time - fpsLastTime) / 1000.0f);
			fpsFrames = 0;
			fpsLastTime = time;
			if (MGEFlags & FPS_COUNTER) {
				if (fps > 0.1) _gcvt_s (fpsString, 10, fps, 4);
			}
		}

		if(InvertUsingDL >= 3) {
			mwBridge->SetViewDistance((InvertUsingDL & 0x01 ? +1.0f : -1.0f) + mwBridge->GetViewDistance());
			if(InvertUsingDL >= 4) InvertUsingDL = 0; else InvertUsingDL++;
		}

		if (MGEFlags & FPS_HOLD && mwBridge->IsLoaded ()) {
			if (!fpsAdjusting && (fps > FPSDesired + FPSTolerance)
				|| fpsAdjusting && (fps > FPSDesired + FPSTolerance / 2.0f)
			) {
				fpsAdjusting = true;

				float f = mwBridge->GetViewDistance ();
				SatUp (&f, MaxViewRange, fps - FPSDesired);

				//If we're using distant land, make sure it doesn't get too high
				if (DistantLandActive && DL_HasStatics () && (MGEFlags & USE_DISTANT_LAND) && f > 7168.0f) f = 7168.0f;

				mwBridge->SetViewDistance (f);

				if (MGEFlags & BIND_AI_TO_VIEW) {
					float f2 = f / STDMAXVIEWDISTANCE;
					mwBridge->SetAIDistance (f2);
				}
			} else if (!fpsAdjusting && (fps < FPSDesired - FPSTolerance)
				|| fpsAdjusting && (fps < FPSDesired - FPSTolerance / 2.0f)
			) {
				fpsAdjusting = true;
				float f = mwBridge->GetViewDistance ();

				SatDown (&f, MinViewRange, FPSDesired - fps);

				//If we're using distant land, make sure it doesn't get too high
				if (DistantLandActive && DL_HasStatics () && (MGEFlags & USE_DISTANT_LAND) && f > 7168.0f) f = 7168.0f;

				mwBridge->SetViewDistance (f);

				if (MGEFlags & BIND_AI_TO_VIEW) {
					float f2 = f / STDMAXVIEWDISTANCE;
					mwBridge->SetAIDistance (f2);
				}
			} else if (fpsAdjusting) fpsAdjusting = false;
		}

		//shaders moved to separate function //phal
		ApplyShaders ();
		m_MWRenderState.RestoreState();

		//sprintf (fpsString, "IFE: %f  SFE: %f  fogend: %f", InteriorFogEnd(), StandardFogEnd(), FogEnd);
		/* if (LoadedMemMap) {
			sprintf (fpsString, "%f, %f, %f",
				PlayerPositionX (),
				PlayerPositionY (),
				PlayerPositionZ ()
			);
		} */

		//fps counter
		if ((MGEFlags & DISPLAY_MESSAGES) && ((MGEFlags & FPS_COUNTER) || StatusStringTime)) {
			RealDevice->BeginScene ();
			if (MGEFlags & FPS_COUNTER) Font->DrawTextA (0, fpsString, -1, &fpsRect, 0, fpsColor);
			if (StatusStringTime) {
				if (StatusStringTime > time) Font->DrawTextA (0, StatusString, -1, &StatusRect, 0, fpsColor);
				else StatusStringTime = 0;
			}
			RealDevice->EndScene ();
		}
		hr = RealDevice->Present (a, b, c, d);
		//cpu idle
		if (MGEFlags & CPU_IDLE) Sleep (IdleTime);
		//Antialiasing colour fix
/* 		if (MGEFlags & AA_COLOUR_FIX) {
			RealDevice->StretchRect (ShaderStruct.thisframeSurface, 0, BackBuffer, 0, D3DTEXF_NONE);
		}
 */
		//Handles model movement, fog zooming and zoom zooming
		if (TimerFrequency) {
			__int64 TimerNow;
			if (QueryPerformanceCounter ((LARGE_INTEGER *)&TimerNow)) {
				if (TimerNow > LastTimer) {
					JointInfo.FrameTime = ((float)(TimerNow - LastTimer)) / TimerFrequency;
					LastTimer = TimerNow;
					for (int i = 0; i < MAXMWSEOBJECTS; ++i) {
						if ((!Objects [i].Enabled) || (!Objects [i].SpeedFlags)) continue;
						if (Objects [i].SpeedFlags & 0x01) {
							Objects [i].pos += (Objects [i].speed * JointInfo.FrameTime);
							MWSE::UpdateObjectMatrix (i, 0);
						}
						if (Objects [i].SpeedFlags & 0x02) {
							Objects [i].rot += (Objects [i].AngSpeed * JointInfo.FrameTime);
							MWSE::UpdateObjectMatrix (i, 1);
						}
						if (Objects [i].SpeedFlags & 0x04) {
							Objects [i].scale += (Objects [i].growth * JointInfo.FrameTime);
							MWSE::UpdateObjectMatrix (i, 2);
						}
					}
					if (FogZooming) {
						FogRange = CLAMP (FogRange + (FogZoomRate * JointInfo.FrameTime), MINFOG, MAXFOG);
						UpdateFogRange ();
					}
					if (FOVZooming) {
						FOV = CLAMP (FOV + (FOVZoomRate * JointInfo.FrameTime), MINZOOM, MAXZOOM);
					}
					if (RotSpinning) {
						Rotation += RotSpinRate * JointInfo.FrameTime;
						UpdateRotation ();
					}
					if (CameraShakeChange) {
						CameraShakeAmp = CLAMP (CameraShakeAmp + (CameraShakeChangeRate * JointInfo.FrameTime), MINSHAKE, MAXSHAKE);
					}
				}
			}
			if (CameraShake) UpdateScreenShake ();
			MWSE::Update ();
			ActiveLights = 0;
		}

		if (SceneCount <= 2) {
			UpdateScene = 1;
			HUDScene = 1;
			//AlphaScene = -1;
			SolarScene = -1;
		} else if (UpdateScene == 1 || LastSceneCount != SceneCount) {
			UpdateSceneNums ();
			UpdateScene = 0;
		}

		/*
		//Determine whether we are recording renders in the next frame
		//if (UsingDistantLand && LoadedMemMap && IsExterior () && !LoadingScreen && MyBackbuffer) {
		if (UsingDistantLand && LoadedMemMap && !LoadingScreen && MyBackbuffer) { //d4w
			wrapper_log_file << "Begining recording." << endl;
			RecordRenders = true;
			//Flush renders from last frame
			DL_ClearRecordedRenders ();
		} else {
			DL_ClearRecordedRenders ();
			RecordRenders = false;
		}
		*/

		LastSceneCount = SceneCount;
		SceneCount = 0;
		AlphaScene = 2;
		ShadowsDrawn = 0;
		WaterDrawn = false;
		FogAlreadySet = false;
		RenderDLand = true;
		SunEnabled = false;
		LightsSet = false;
		RecordRenders = false;
		DepthDrawRecorder.ClearRecordedDrawCalls();

		/////////phal
		HUDRenderingStarted = false;
		BackBufferWasCleared = false;
		ShadersApplied = false;
		//////////////

		if ( WindowedMode ) {
			// Maximize client graphics area, if window was resized
			HWND win = GetParent(client_hWND);
			RECT r;
			GetClientRect (win, &r);
			if ( clientWND_width - (r.right - r.left) != 0 || clientWND_height - (r.bottom - r.top) != 0 ) {
				clientWND_width = r.right - r.left;
				clientWND_height = r.bottom - r.top;

				// if window is maximized, make it fullscreen
				if( IsZoomed (win) ) {
					WindowedMode = false;
					ShowWindow(win, SW_HIDE);
					SetWindowLong(win, GWL_STYLE, WS_CLIPSIBLINGS|WS_VISIBLE );
					SetWindowPos(win,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
					ShowWindow(win, SW_SHOWMAXIMIZED);
				}
				SetWindowPos(client_hWND,0,0,0,clientWND_width,clientWND_height,SWP_NOZORDER|SWP_NOMOVE);
				ShowWindow(client_hWND, SW_SHOWMAXIMIZED);
			}
		}

		//if (DepthTexture) RealDevice->SetDepthStencilSurface (DepthBuffer);
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetBackBuffer (UINT a, D3DBACKBUFFER_TYPE b, IDirect3DSurface8 **c) {
		IDirect3DSurface9 *c2 = NULL;
		*c = NULL;
		DWORD unused = 4;
		HRESULT hr = RealDevice->GetBackBuffer (0, a, b, &c2);
		if (hr != D3D_OK || c2 == NULL) return hr;
		hr = c2->GetPrivateData (guid, (void *)c, &unused);
		if (hr != D3D_OK) *c = new FakeSurface (c2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetRasterStatus (D3DRASTER_STATUS *a) {
		return RealDevice->GetRasterStatus (0, a);
	}

	//-----------------------------------------------------------------------------

	void _stdcall SetGammaRamp (DWORD a, const D3DGAMMARAMP *b) {
		RealDevice->SetGammaRamp (0, a, b);
	}

	//-----------------------------------------------------------------------------

	void _stdcall GetGammaRamp (D3DGAMMARAMP *a) {
		RealDevice->GetGammaRamp (0, a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateTexture (UINT a, UINT b, UINT c, DWORD d, D3DFORMAT e, D3DPOOL f, IDirect3DTexture8 **g) {
		if (HookNextTexture) {
			HookNextTexture = false;
			HookedTextures [NextHookIndex].tHook = new TextureHook (NextHookIndex, a, b);
			*g = (IDirect3DTexture8 *)HookedTextures [NextHookIndex].tHook;
			return D3D_OK;
		}
		IDirect3DTexture9 *g2 = NULL;
		HRESULT hr = RealDevice->CreateTexture (a, b, c, d, e, f, &g2, NULL);
		if (hr != D3D_OK || g2 == NULL) return hr;
		*g = new FakeTexture (g2);
		if (a == 128 &&
			b == 128 &&
			c == 5 &&
			d == 0 &&
			e == 894720068 &&
			f == 1 &&
			g2->GetLevelCount() == 5 &&
			g2->GetLOD() == 0 &&
			g2->GetPriority() == 0 &&
			g2->GetType() == 3 ) {
				HookWaterLoad (&((FakeTexture *)*g)->RealObject);
		}	//mendres: Seems to be Water Texture. Works only with modified texture files!
		//if (NextTexIsWater) {
		//	HookWaterLoad (&((FakeTexture *)*g)->RealObject);
		//	NextTexIsWater = false;
		//}	//mendres: Using generic method.
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateVolumeTexture (UINT a, UINT b, UINT c, UINT d, DWORD e, D3DFORMAT f, D3DPOOL g, IDirect3DVolumeTexture8 **h) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateCubeTexture (UINT a, UINT b, DWORD c, D3DFORMAT d, D3DPOOL e, IDirect3DCubeTexture8 **f) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateVertexBuffer (UINT a, DWORD b, DWORD c, D3DPOOL d, IDirect3DVertexBuffer8 **e) {
		return RealDevice->CreateVertexBuffer (a, b, c, d, (IDirect3DVertexBuffer9 **)e, NULL);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateIndexBuffer (UINT a, DWORD b, D3DFORMAT c, D3DPOOL d, IDirect3DIndexBuffer8 **e) {
		return RealDevice->CreateIndexBuffer (a, b, c, d, (IDirect3DIndexBuffer9 **)e, NULL);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateRenderTarget (UINT a, UINT b, D3DFORMAT c, D3DMULTISAMPLE_TYPE d, BOOL e, IDirect3DSurface8 **f) {
		IDirect3DSurface9 *f2 = NULL;
		HRESULT hr = RealDevice->CreateRenderTarget (a, b, c, d, 0, e, &f2, NULL);
		if (hr != D3D_OK || f2 == NULL) return hr;
		*f = new FakeSurface (f2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateDepthStencilSurface (UINT a, UINT b, D3DFORMAT c, D3DMULTISAMPLE_TYPE d, IDirect3DSurface8 **e) {
		//Not sure if Discard should be true or false
		IDirect3DSurface9 *e2 = NULL;
		HRESULT hr = RealDevice->CreateDepthStencilSurface (a, b, c, d, 0, false, &e2, NULL);
		if (hr != D3D_OK) return hr;
		*e = new FakeSurface (e2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateImageSurface (UINT a, UINT b, D3DFORMAT c, IDirect3DSurface8 **d) {
		IDirect3DSurface9 *d2 = NULL;
		//Use default to get stretchrect to work, scratch otherwise
		HRESULT hr = RealDevice->CreateOffscreenPlainSurface (a, b, c, D3DPOOL_SYSTEMMEM, &d2, NULL);
		//HRESULT hr = RealDevice->CreateOffscreenPlainSurface (a, b, c, D3DPOOL_DEFAULT, &d2, NULL);
		if (hr != D3D_OK || d2 == NULL) return hr;
		*d = new FakeSurface (d2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CopyRects (IDirect3DSurface8 *a, const RECT *b, UINT c, IDirect3DSurface8 *d, const POINT *e) {
		IDirect3DSurface9 *a2 = ((FakeSurface *)a)->RealObject;
		IDirect3DSurface9 *d2 = ((FakeSurface *)d)->RealObject;
		if (b == NULL && e == NULL) {
			D3DSURFACE_DESC9 source;
			D3DSURFACE_DESC9 dest;
			if (a2->GetDesc (&source) != D3D_OK || d2->GetDesc (&dest) != D3D_OK) {
				return UnusedFunction ();
			} else if (source.Usage == 1 && dest.Usage == 0) {
				return RealDevice->GetRenderTargetData (a2, d2);
			} else if (source.Usage == 0 && dest.Usage == 1) {
				return RealDevice->UpdateSurface (a2, NULL, d2, NULL);
			} else {
				return RealDevice->StretchRect (a2, NULL, d2, NULL, D3DTEXF_NONE);
			}
		} else {
			//Really cant be bothered to get this working unless absolutely neccessery
			return UnusedFunction ();
			/* HRESULT hr;
			RECT r;
			r.top = e->y;
			r.left = e->x;
			for (UINT i = 0; i < c; ++i) {
			r.bottom = r.top + b [i].bottom - b [i].top;
			r.right = r.left + b [i].right - b [i].left;
			hr = RealDevice->StretchRect (a2, &b [i], d2, &r, D3DTEXF_LINEAR);
			}
			return hr; */
		}
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall UpdateTexture (IDirect3DBaseTexture8 *a, IDirect3DBaseTexture8 *b) {
		IDirect3DTexture9 *a2 = ((FakeTexture *)a)->RealObject;
		IDirect3DTexture9 *b2 = ((FakeTexture *)b)->RealObject;
		return RealDevice->UpdateTexture (a2, b2);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetFrontBuffer (IDirect3DSurface8 *a) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetRenderTarget (IDirect3DSurface8 *a, IDirect3DSurface8 *b) {
		DECLARE_MWBRIDGE
		//wrapper_log_file << "Setting Render target to " << (unsigned int)a << " at time " << GetTickCount () << endl;
		IDirect3DSurface9 *a2 = NULL;
		IDirect3DSurface9 *b2 = NULL;
		if (a != NULL) a2 = ((FakeSurface *)a)->RealObject;
		if (b != NULL) b2 = ((FakeSurface *)b)->RealObject;
		MyBackbuffer = (a2 == BackBuffer);
		if (MyBackbuffer && SceneCount == 1) SceneCount = 0;
		if (a2 == BackBuffer || a2 == NULL) {
			RenderingToTexture = false;
			//if (!DistantLandActive && UsingDistantLand && LoadedMemMap && IsExterior ()) {
			if (!LoadingScreen && (!(MGEFlags & NO_INTERIOR_DL) || mwBridge->IsExterior ())
				&& !DistantLandActive && UsingDistantLand && !mwBridge->IsMenu ()
			) DL_Activate ();
			RealDevice->SetRenderState (D3DRS_RANGEFOGENABLE, RangedFog);
		} else {
			RenderingToTexture = true;
			/*
			if (DistantLandActive) {
				DL_Deactivate ();
			} */
			//DistantLandWasActive = DistantLandActive;
			RealDevice->SetRenderState (D3DRS_RANGEFOGENABLE, 0);
		}
		HRESULT hr = D3D_OK;
		if (a2 != NULL) hr |= RealDevice->SetRenderTarget (0, a2);
		hr |= RealDevice->SetDepthStencilSurface (b2);
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetRenderTarget (IDirect3DSurface8 **a) {
		IDirect3DSurface9 *a2 = NULL;
		*a = NULL;
		DWORD unused = 4;
		HRESULT hr = RealDevice->GetRenderTarget (0, &a2);
		if (hr != D3D_OK || a2 == NULL) return hr;
		hr = a2->GetPrivateData (guid, (void *)a, &unused);
		if (hr != D3D_OK) *a = new FakeSurface (a2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDepthStencilSurface (IDirect3DSurface8 **a) {
		IDirect3DSurface9 *a2 = NULL;
		*a = NULL;
		DWORD unused = 4;
		HRESULT hr = RealDevice->GetDepthStencilSurface (&a2);
		if (hr != D3D_OK || a2 == NULL) return hr;
		hr = a2->GetPrivateData (guid, (void *)a, &unused);
		if (hr != D3D_OK) *a = new FakeSurface (a2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall BeginScene () {
		//wraper_log_file << "Begin Scene. Count = " << (int)SceneCount << " AlphaScene = " << (int)AlphaScene << std::endl;
		////Start stencil tracking over
		//StencilEnabled = false;
		if(SceneCount == 0 && (MGEFlags & EXP_FOG) && DL_ShaderModel >= 3) {
			UpdateFogMethod();
			UpdateFogRange();
		}
		if (SunManual) {
			if (HUDScene > 1) {
				if (SceneCount == HUDScene - 1) RealDevice->SetRenderState (D3DRS_AMBIENT, 0xFFFFFFFF);
				else if (SceneCount == HUDScene) {
					if (HUDManual) RealDevice->SetRenderState (D3DRS_AMBIENT, HUDColour);
				}
				if (SceneCount == 0) {
					if (SkyManual) RealDevice->SetRenderState (D3DRS_AMBIENT, SkyColour);
					else RealDevice->SetRenderState (D3DRS_AMBIENT, 0xFFFFFFFF);
				}
			} else {
				RealDevice->SetRenderState (D3DRS_AMBIENT, 0xFFFFFFFF);
			}
		}
		DECLARE_MWBRIDGE
		if(!LoadingScreen && SceneCount == 0 && !RenderedLand && MyBackbuffer) {
			if ( (!(MGEFlags & NO_INTERIOR_DL) || mwBridge->IsExterior ()) && !RenderingToTexture) {
				if ( g_DepthManager.IsInitialized() ) {
					//Clear the depth buffer and start recording renders
					g_DepthManager.ClearDepthBuffer(!mwBridge->IsMenu() || InvertUsingDL);
					RecordRenders = true;
					
					//Flush renders from last frame
					DepthDrawRecorder.ClearRecordedDrawCalls();
				}
			}
		}
		return RealDevice->BeginScene ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall EndScene (void) {
		if (MyBackbuffer) {
			++SceneCount;
			if (StencilEnabled) ++ShadowsDrawn;
			//if (StencilEnabled == true) {
			//	++AlphaScene;
			//}
		}

		//wraper_log_file << "End Scene. SceneCount = " << (int)SceneCount << " AlphaScene = " << (int)AlphaScene << " StencilEnabled = " << (int)StencilEnabled << " SunEnabled = " << SunEnabled << " NumDrawCalls = " << NumDrawCalls << " AlphaBlendEnabled = " << AlphaBlendEnabled << " AlphaTestEnabled = " << AlphaTestEnabled << std::endl;

		DECLARE_MWBRIDGE

		if (mwBridge->IsLoaded () && (mwBridge->IsExterior () || mwBridge->IntLikeExterior ())
			&& !LoadingScreen && MyBackbuffer && !RenderingToTexture
		) {
			if ( mwBridge->IsExterior () && SceneCount == 1 && !RenderedLand ) {
				if ( g_DepthManager.IsInitialized() ) {
					//Clear the depth buffer and start recording renders.
					g_DepthManager.ClearDepthBuffer(!mwBridge->IsMenu() || InvertUsingDL);
					RecordRenders = true;
				}

				if (UsingDistantLand) {
					DL_RenderLand ();
				}

				m_MWRenderState.RestoreState();

				RenderedLand = true;
			}

			if (DL_HasStatics () && !RenderedGrass && g_DepthManager.IsInitialized() ) {
				if (SceneCount == 1 && !RenderedGrass) {
					DrawRecordedDrawCallDepth (&TransView, &TransProj );
					if ( DistantLandActive && !(MGEFlags & NO_MW_MGE_BLEND) ) {
						DL_BlendNearAndFarView();
					}

					//Flush previous renders
					DepthDrawRecorder.ClearRecordedDrawCalls();

					if (DistantLandActive && (mwBridge->IsExterior () || mwBridge->IntLikeExterior ()) ) {
						//VisibleSet visible_near_grass;
						//DL_CullGrass (visible_near_grass, &TransView, &TransProj, false, false, false);
						DL_RenderGrass (&TransView, &TransProj, false, false, false);

						// render grass into depth texture //phal
						if ((MGEFlags & SHADER_DEPTH) && ((MGEFlags & USE_HW_SHADER) || UsedMWSEShaders)) {
							DL_RenderGrass (&TransView, &TransProj, false, false, true);
						}

						//Start culling the next frame
						DL_SubmitNearGrassCullingTask ();
						RenderedGrass = true;
					}
				}

				m_MWRenderState.RestoreState();
			}


			//if (WaterDrawn == true && StencilEnabled == false && ShadowsDrawn > 0) {
			//	DL_RenderWater (&TransProj);
			//	wraper_log_file << "* Water Drawn" << std::endl;
			//	WaterDrawn = false;
			//}
		}
		//Draw any new models loaded by mods
		if (UsedMWSEobjects && SceneCount == AlphaScene) {
			StateBlock->Capture ();
			RealDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, 0);
			RealDevice->SetRenderState (D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
			RealDevice->SetRenderState (D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
			RealDevice->SetRenderState (D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
			RealDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_ONE);
			RealDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_ZERO);
			RealDevice->SetRenderState (D3DRS_ALPHATESTENABLE, false);
			for (DWORD i = 0; i < ActiveLights; ++i) {
				RealDevice->LightEnable (LightIDs [i], false);
			}
			for (int i = 0; i < MAXMWSEOBJECTS; ++i) {
				if (!Objects [i].Enabled) continue;
				RealDevice->SetTransform (D3DTS_WORLDMATRIX (0), &Objects [i].WorldMatrix);
				for (DWORD j = 0; j < Objects [i].NumLights; ++j) {
					RealDevice->LightEnable (Objects [i].lights [j], true);
				}
				if (Objects [i].IsMesh) {
					for (int j = 1; j < 8; ++j) RealDevice->SetTexture (j, NULL); //Clear unused textures
					if (Objects [i].effect != NULL) {
						UINT passes;
						D3DXMATRIX worldviewproj = Objects [i].WorldMatrix * TransView * TransProj;
						Objects [i].effect->SetMatrix ("worldviewproj", &worldviewproj);
						Objects [i].effect->Begin (&passes, 0);
						for (UINT j = 0; j < passes; ++j) {
							Objects [i].effect->BeginPass (j);
							for (UINT k = 0; k < Objects [i].subsets; ++k) {
								RealDevice->SetTexture (0, Objects [i].textures [k]);
								Objects [i].mesh->DrawSubset (k);
							}
							Objects [i].effect->EndPass ();
						}
						Objects [i].effect->End ();
					} else {
						for (UINT k = 0; k < Objects [i].subsets; ++k) {
							RealDevice->SetTexture (0, Objects [i].textures [k]);
							Objects [i].mesh->DrawSubset (k);
						}
					}
				} else {
					for (int j = 0; j < 8; ++j) RealDevice->SetTexture (j, Objects [i].textures [j]);
					RealDevice->SetStreamSource (0, Objects [i].vBuffer, 0, Objects [i].stride);
					for (DWORD j = 1; j < d3d8Caps.MaxStreams; ++j) RealDevice->SetStreamSource (j, NULL, 0, 0);
					RealDevice->SetIndices (Objects [i].iBuffer);
					RealDevice->SetFVF (Objects [i].FVF);
					if (Objects [i].effect != NULL) {
						UINT passes;
						D3DXMATRIX worldviewproj = Objects [i].WorldMatrix * TransView * TransProj;
						Objects [i].effect->SetMatrix ("worldviewproj", &worldviewproj);
						Objects [i].effect->Begin (&passes, 0);
						for (UINT j = 0; j < passes; ++j) {
							Objects [i].effect->BeginPass (j);
							RealDevice->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, 0, Objects [i].vertices, 0, 0,
								Objects [i].faces
							);
							Objects [i].effect->EndPass ();
						}
						Objects [i].effect->End ();
					} else {
						RealDevice->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, 0, Objects [i].vertices, 0, 0,
							Objects [i].faces
						);
					}
				}
				for (DWORD j = 0; j < Objects [i].NumLights; ++j) {
					RealDevice->LightEnable (Objects [i].lights [j], false);
				}
			}
			StateBlock->Apply ();
		}
		if (MGEFlags & INPUT_LAG_FIX) {
			Font->DrawTextA (0, " ", -1, &StatusRect, 0, fpsColor);
		}

		//Reset num draw calls at the end of each scene
		NumDrawCalls = 0;
		return RealDevice->EndScene ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall Clear (DWORD a, const D3DRECT *b, DWORD c, D3DCOLOR d, float e, DWORD f) {
		DECLARE_MWBRIDGE
		if (DepthTexture && (c & D3DCLEAR_ZBUFFER) && !(c & D3DCLEAR_TARGET)) RealDevice->SetDepthStencilSurface (OldDepthSurface);
		if (SunManual) {
			if (ClsManual) d = ClsColour;
			else ClsColour = d;
		}

		if (!FogManual) {	//	europop: Without additional check of FogManual it seems that function SetFogColour doesn't work.
			FogColour = d;
			FogAlreadySet = true;
		}

		if (c & D3DCLEAR_TARGET) BackBufferWasCleared = true; //phal

		//Set any custom FOV
		if (SceneCount == 0 && ScreenFOV > 0 && mwBridge->IsLoaded () && mwBridge->GetShadowFov ()) {
			mwBridge->SetFOV (ScreenFOV);
		}

		return RealDevice->Clear (a, b, c, d, e, f);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetTransform (D3DTRANSFORMSTATETYPE a, const D3DMATRIX *b) {
		DECLARE_MWBRIDGE

		if (a == D3DTS_WORLD || a == D3DTS_WORLDMATRIX (0)) {
			m_MWRenderState.World = *b;
			CurrentWorldTrans = *b;
		} else if (a == D3DTS_WORLDMATRIX (1)) {
			CurrentBlendTrans1 = *b;
		} else if (a == D3DTS_WORLDMATRIX (2)) {
			CurrentBlendTrans2 = *b;
		} else if (a == D3DTS_WORLDMATRIX (3)) {
			CurrentBlendTrans3 = *b;
		}

		if (a == D3DTS_VIEW || a == D3DTS_PROJECTION) {
			D3DXMATRIX matrix = *b;

			//Ensure that Morrowind never draws beyond 7168 if using distant land
			if ((MGEFlags & USE_DISTANT_LAND) && DistantLandActive && !RenderingToTexture
				&& !((MGEFlags & NO_INTERIOR_DL) && !mwBridge->IsExterior ()) && DL_HasStatics ()
			) {
				//Check that Morrowind's projection matrix does not use a far clipping plane that's too far away
				//float zn = viewDistance - viewDistance / matrix._33;
				float zn = 1.0f;
				float zf = - (matrix._33 * zn) / (1 - matrix._33);

				if (zf > 7168.0f) {
					//Adjust the projection matrix Morrowind is trying to set to keep it in line
					zf = 7168.0f;

					matrix._33 = zf / (zf - zn);
					matrix._43 = - zn * zf / (zf - zn);
				}
			}

			if (!(MGEFlags & ZOOM_ROT_HUD) && (SceneCount < HUDScene)) {
				if (Zoom && a == D3DTS_PROJECTION) {
					if (MGEFlags & ZOOM_ASPECT) {
						D3DXMatrixPerspectiveFovLH (&matrix, D3DX_PI / FOV, JointInfo.aspect, 1, 100);
					} else {
						D3DXMatrixPerspectiveFovLH (&matrix, D3DX_PI / FOV, 4.0f / 3.0f, 1, 100);
					}
					matrix._33 = b->_33;
					matrix._43 = b->_43;
				}
				if (Rotation && a == D3DTS_VIEW) {
					matrix *= RotateMatrix;
				}
				if (CameraShake && a == D3DTS_VIEW) {
					matrix *= CameraShakeMatrix;
				}
			}
			if (SceneCount == 0 && MyBackbuffer) {
				D3DXVECTOR4 blank;
				D3DXMATRIX invmat;
				DECLARE_MWBRIDGE
				switch (a) {
				case D3DTS_VIEW:
					if ((matrix._11 == 0 || matrix._11 == 1)
						&& matrix._12 == 0
						&& (matrix._13 == 1 || matrix._13 == 0)
						&& matrix._14 == 0
						&& matrix._21 == 0
						&& (matrix._22 == 1 || matrix._22 == 0)
						&& (matrix._23 == 0 || matrix._23 == 1)
						&& matrix._24 == 0
						&& (matrix._31 == 1 || matrix._31 == 0)
						&& (matrix._32 == 0 || matrix._32 == 1)
						&& matrix._33 == 0
						&& matrix._34 == 0
						&& matrix._41 == 0
						&& (matrix._42 == 0 || matrix._42 == -600)
						&& matrix._43 == 0
						&& matrix._44 == 1
					) {
						LoadingScreen = true;
					} else {
						TransView = matrix;
						LoadingScreen = false;
					}
					EyeVec.x = b->_13;
					EyeVec.y = b->_23;
					EyeVec.z = b->_33;
					ZeroStruct (blank);
					blank.w = 1;
					D3DXMatrixInverse (&invmat, 0, &matrix);
					D3DXVec4Transform (&EyePos, &blank, &invmat);

					if (!mwBridge->IsLoaded ()) break;

					if (UsingDistantLand && DistantLandActive && !LoadingScreen && LastSceneCount > 1
						&& (!(MGEFlags & NO_INTERIOR_DL) || mwBridge->IsExterior ()) && !mwBridge->IsMenu ()
					) {
						DL_PlayerPos (EyePos.x, EyePos.y);
					}

					//set real sun direction //phal
					if (!LoadingScreen) {
						if (mwBridge->IsExterior () || mwBridge->IntLikeExterior ()) {
							mwBridge->GetSunDir (SunPos.x, SunPos.y, SunPos.z);
							SunPos -= EyePos;
							SunPos.w = 1;
							SunPos /= sqrt (SunPos.x * SunPos.x + SunPos.y * SunPos.y + SunPos.z * SunPos.z);
							SunVis = (float)(mwBridge->GetSunVis ()) / 255.0f;
							if (SunVis == 0) SunPos.z *= -1;
						} else {
							SunPos.x = 0;
							SunPos.y = 0;
							SunPos.z = -1;
							SunPos.w = 1;
							SunVis = 0;
						}
					}

					//Set player position //phal
					PlayerPos.x = mwBridge->PlayerPositionX ();
					PlayerPos.y = mwBridge->PlayerPositionY ();
					PlayerPos.z = mwBridge->PlayerPositionZ ();
					PlayerPos.w = 1;
					break;
				case D3DTS_PROJECTION:
					TransProj = matrix;
					break;
				}
			}

			//Remember the final Matrix that Morrowind will be rendering with
			if (a == D3DTS_PROJECTION ) {
				m_MWRenderState.Projection = matrix;
			} else if (a == D3DTS_VIEW ) {
				m_MWRenderState.View = matrix;
			}

			return RealDevice->SetTransform (a, &matrix);
		}


		return RealDevice->SetTransform (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetTransform (D3DTRANSFORMSTATETYPE a, D3DMATRIX *b) {
		return RealDevice->GetTransform (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall MultiplyTransform (D3DTRANSFORMSTATETYPE a, const D3DMATRIX *b) {
		return RealDevice->MultiplyTransform (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetViewport (const D3DVIEWPORT8 *a) {
		return RealDevice->SetViewport (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetViewport (D3DVIEWPORT8 *a) {
		return RealDevice->GetViewport (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetMaterial (const D3DMATERIAL8 *a) {
		CurrentMaterial = a;
		return RealDevice->SetMaterial (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetMaterial (D3DMATERIAL8 *a) {
		return RealDevice->GetMaterial (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetLight (DWORD a, const D3DLIGHT8 *b) {
		if (a != 6) {
			const_cast <D3DLIGHT9 *> (b)->Ambient.r = 0; //Required so that distant land lighting matches with standard lighting
			const_cast <D3DLIGHT9 *> (b)->Ambient.g = 0;
			const_cast <D3DLIGHT9 *> (b)->Ambient.b = 0;
		}
		if (ActiveLights < MAXACTIVELIGHTS) {
			LightIDs [ActiveLights++] = a;
		}

		if (a == 6) {
			if (LightsSet == false) {
				LightsSet = true;

				D3DLIGHT8 light = *b;
				if (SunManual) {
					if (SunManDir) light.Direction = SunDirection;
					else SunDirection = light.Direction;
					if (SunManColour) light.Diffuse = SunColour;
					else SunColour = light.Diffuse;
					if (SunAmbManual) light.Ambient = SunAmbColour;
					else SunAmbColour = light.Ambient;
				}
				DECLARE_MWBRIDGE
				if (!LoadingScreen && ((MGEFlags & SET_SHADER_VARS)
					|| (UsingDistantLand && (!(MGEFlags & NO_INTERIOR_DL) && (mwBridge->IsExterior() || mwBridge->IntLikeExterior())) && DistantLandActive))	//	europop: IntLikeExterior() is checked too.
				) {
					if (!mwBridge->CellHasWeather ()) {
						SunVec.x = 0.0f;
						SunVec.y = 0.0f;
						SunVec.z = 1.0f;
					} else {
						SunVec.x = light.Direction.x;
						SunVec.y = light.Direction.y;
						SunVec.z = light.Direction.z;
					}
					D3DXVec3Normalize ((D3DXVECTOR3 *)&SunVec, (D3DXVECTOR3 *)&SunVec);
					SunCol = *(D3DXVECTOR4 *)&light.Diffuse;
					SunAmb = *(D3DXVECTOR4 *)&light.Ambient;
					SunAmbD = D3DXCOLOR (light.Ambient);
				}
				return RealDevice->SetLight (6, &light);
			}
		}
		return RealDevice->SetLight (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetLight (DWORD a, D3DLIGHT8 *b) {
		return RealDevice->GetLight (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall LightEnable (DWORD a, BOOL b) {
		if (a == 6) {
			if (b == TRUE) SunEnabled = true;
			else SunEnabled = false;
		}
		return RealDevice->LightEnable (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetLightEnable (DWORD a, BOOL *b) {
		return RealDevice->GetLightEnable (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetClipPlane (DWORD a, const float *b) {
		return RealDevice->SetClipPlane (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetClipPlane (DWORD a, float *b) {
		return RealDevice->GetClipPlane (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetRenderState (D3DRENDERSTATETYPE a, DWORD b) {
		//phal
		if (a == D3DRS_ZWRITEENABLE) {
			zWriteEnabled = (b == 1);
		} else if (a == D3DRS_DESTBLEND) {
			CurrentDestBlend = b;
		} else if (a == D3DRS_SRCBLEND) {
			CurrentSrcBlend = b;
		}

		DECLARE_MWBRIDGE

		if (a == D3DRS_ZWRITEENABLE) {
			//Remember the state that MW set
			m_MWRenderState.ZWriteEnable = b;
			
			if (b == 1 && SceneCount == 0 && !RenderedLand && MyBackbuffer) {
				//if (IsExterior () && !RenderingToTexture) {
				if ( (!(MGEFlags & NO_INTERIOR_DL) || mwBridge->IsExterior ()) && !RenderingToTexture) {
					// moved to BeginScene() to prevent glitches with depth texture when in interiors
					/*if ( g_DepthManager.IsInitialized() ) {
						//Clear the depth buffer and start recording renders
						g_DepthManager.ClearDepthBuffer(!mwBridge->IsMenu());
						RecordRenders = true;
						
						//Flush renders from last frame
						DepthDrawRecorder.ClearRecordedDrawCalls();
					}*/
					
					if ( UsingDistantLand ) {
						DL_RenderLand ();
						m_MWRenderState.RestoreState();
					}

					//RenderedLand = true;
				} else if (DistantLandActive) {
					DL_Deactivate ();
				}
				RenderedLand = true;
			}
		}
		//Skip this render state if this is an option MGE controls
		if (a == D3DRS_FOGVERTEXMODE || a == D3DRS_FOGTABLEMODE) return D3D_OK;
		//Fogging is special. Setting it with the other render states causes a black screen
		if (a == D3DRS_FOGENABLE && !(MGEFlags & FOG_ENABLED)) return D3D_OK;
		if (a == D3DRS_FOGSTART) {
			FogStart = DW2F (b);
			if (DistantLandActive && DL_HasStatics ()) {
				if (mwBridge->IsAboveWater ()) {
					if (mwBridge->CellHasWeather ()) b = F2DW (DistantFogStart ());
					else b = F2DW (InteriorFogStart ());
				} else b = F2DW (UnderwaterFogStart ());
			} else b = F2DW (StandardFogStart ());
		} else if (a == D3DRS_FOGEND) {
			FogEnd = DW2F (b);
			if (DistantLandActive && DL_HasStatics ()) {
				if (mwBridge->IsAboveWater ()) {
					if (mwBridge->CellHasWeather ()) b = F2DW (DistantFogEnd ());
					else b = F2DW (InteriorFogEnd ());
				} else b = F2DW (UnderwaterFogEnd ());
			} else b = F2DW (StandardFogEnd ());
		}
		if (a == D3DRS_STENCILENABLE) {
			//Remember the state that MW set
			m_MWRenderState.StencilEnable = b;

			if (b == TRUE) {
				StencilEnabled = true;
			} else {
				StencilEnabled = false;
			}
		}
		if (a == D3DRS_ALPHABLENDENABLE) {
			//Remember the state that MW set
			m_MWRenderState.AlphaBlendEnable = b;

			if (b == TRUE) {
				AlphaBlendEnabled = true;
			} else {
				AlphaBlendEnabled = false;
			}
		}
		if (a == D3DRS_ALPHATESTENABLE) {
			//Remember the state that MW set
			m_MWRenderState.AlphaTestEnable = b;

			if (b == TRUE) {
				AlphaTestEnabled = true;
			} else {
				AlphaTestEnabled = false;
			}
		}
		if (a == D3DRS_VERTEXBLEND) {
			//Remember the state that MW set
			m_MWRenderState.VertexBlend = b;

			CurrentVertexBlendState = b;
		}
		if (a == D3DRS_CULLMODE) {
			//Remember the state that MW set
			m_MWRenderState.CullMode = b;

			CurrentCullModeState = b;
		}
		if ( a == D3DRS_SRCBLEND ) {
			//Remember the state that MW set
			m_MWRenderState.SrcBlend = b;
		}
		if ( a == D3DRS_DESTBLEND) {
			//Remember the state that MW set
			m_MWRenderState.DestBlend = b;
		}
		if (SceneCount < AlphaScene) {
			if (SunManual) {
				if (SceneCount < AlphaScene) {
					if (a == D3DRS_AMBIENT) {
						if (AmbientManual) {
							b = AmbientColour;

							AmbCol = (D3DXVECTOR4)D3DXCOLOR (b);
						} else {
							AmbientColour = b;
							AmbCol = (D3DXVECTOR4)D3DXCOLOR (b);
						}
						if (SceneCount == -1) {
							SkyColour = b;
						}
					}
					if (a == D3DRS_FOGCOLOR) {
						if (FogManual) {
							b = FogColour;
						} else {
							D3DXCOLOR cv = (D3DXCOLOR)b;
							if (cv.r != 0 && cv.g != 0 && cv.b != 0) {
								if (FogAlreadySet == false) {
									FogColour = 0xFF9F6F;
									FogAlreadySet = true;
								}
							}
						}
					}
				}
			} else {
				if (a == D3DRS_AMBIENT) {
					AmbientColour = b;
				}
				if (a == D3DRS_FOGCOLOR) {
					D3DXCOLOR cv = (D3DXCOLOR)b;
					if (cv.r != 0 && cv.g != 0 && cv.b != 0) {
						if (FogAlreadySet == false) {
							FogColour = b;
							FogAlreadySet = true;
						}
					}
				}
			}
		}
		if (SceneCount == HUDScene) {
			if (a == D3DRS_AMBIENT) {
				if (HUDManual) b = HUDColour;
				else HUDColour = b;
			}
		}
		return RealDevice->SetRenderState (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetRenderState (D3DRENDERSTATETYPE a, DWORD *b) {
		return RealDevice->GetRenderState (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall BeginStateBlock (void) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall EndStateBlock (DWORD *a) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall ApplyStateBlock (DWORD a) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CaptureStateBlock (DWORD a) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DeleteStateBlock (DWORD a) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateStateBlock (D3DSTATEBLOCKTYPE a, DWORD *b) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetClipStatus (const D3DCLIPSTATUS8 *a) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetClipStatus (D3DCLIPSTATUS8 *a) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetTexture (DWORD a, IDirect3DBaseTexture8 **b) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetTexture (DWORD a, IDirect3DBaseTexture8 *b) {

		CurrentTexture = NULL;
		if (b != NULL) CurrentTexture = ((FakeTexture *)b)->RealObject;
		/* if (b != NULL) {
			if (a == 0) CurrentTexture = ((FakeTexture *)b)->RealObject;
			else SecondaryTexture = ((FakeTexture *)b)->RealObject;
		} */

		if (MGEFlags & USE_TEX_HOOKS) {
			if (SetHookedTextures [a] != 255) {
				HookedTextures [SetHookedTextures [a]].tHook->Release ();
				SetHookedTextures [a] = 255;
			}
			if (HookedShaderActive && a == 0) {
				ShaderStruct.MWSEeffects [ActiveHookedShader].effect->EndPass ();
				ShaderStruct.MWSEeffects [ActiveHookedShader].effect->End ();
				HookedShaderActive = 0;
			}
			if (b != NULL && b->GetLOD () == HOOKEDTEXTUREID) {
				BYTE d = ((TextureHook *)b)->HookIndex;
				ActiveHookedShader = HookedTextures [d].ShaderIndex;
				SetHookedTextures [a] = d;
				HookedTextures [d].tHook->AddRef ();

				if (a == 0 && HookedTextures [d].ShaderIndex != 255) {
					ShaderStruct.MWSEeffects [ActiveHookedShader].effect->Begin (&HookedShaderActive, 0);
					if (HookedShaderActive == 1) {
						ShaderStruct.MWSEeffects [ActiveHookedShader].effect->BeginPass (0);
					}
				}
				if (HookedTextures [d].avi) {
					Restart (HookedTextures [d].avi);
					return RealDevice->SetTexture (a, HookedTextures [d].tex);
				} else return RealDevice->SetTexture (a, LoadedTextures [HookedTextures [d].TextureIndex].texture);
			}
		}
		if (SunManual && SunTexture && !Indoors && (SceneCount == SolarScene)) return RealDevice->SetTexture (a, SunTexture);

		if ( IsFakeWaterTex (CurrentTexture) && WaterDrawn == false && !RenderingToTexture ) {
			WaterDrawn = true;
			//wraper_log_file << "* Fake Water Texture activated" << std::endl;
			//if (ShadowsDrawn == 0) {
				//Draw recorded renders
				//RealDevice->SetRenderState (D3DRS_ZENABLE, 0);
				//RealDevice->SetRenderState (D3DRS_ZWRITEENABLE, 0);

				//Draw any remaining solid objects to the depth buffer
				if (MGEFlags & (USE_DISTANT_LAND | SHADER_DEPTH)) DrawRecordedDrawCallDepth( &TransView, &TransProj );
				DepthDrawRecorder.ClearRecordedDrawCalls();

				//Stop recording renders
				//only if there is no need to record it further //phal
				if (!((MGEFlags & SHADER_DEPTH) && (MGEFlags & USE_HW_SHADER) || UsedMWSEShaders)) {
					RecordRenders = false;
				}

				if ( DistantLandActive && (MGEFlags & USE_DISTANT_LAND) ) {
					DL_RenderWater (&TransProj, false);
				}

				m_MWRenderState.RestoreState();
				//DrawRecordedDrawCallDepth (&TransView, &TransProj);
				////Stop recording renders
				//RecordRenders = false;
				//wraper_log_file << "* Water Drawn" << std::endl;
			//}

		}

		return RealDevice->SetTexture (a, CurrentTexture);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetTextureStageState (DWORD a, D3DTEXTURESTAGESTATETYPE b, DWORD *c) {
		//Some bugger decided to split this into two functions : (
		D3DSAMPLERSTATETYPE sampler;
		switch (b) {
		case D3DTSS_ADDRESSU:
			sampler = D3DSAMP_ADDRESSU;
			break;
		case D3DTSS_ADDRESSV:
			sampler = D3DSAMP_ADDRESSV;
			break;
		case D3DTSS_BORDERCOLOR:
			sampler = D3DSAMP_BORDERCOLOR;
			break;
		case D3DTSS_MAGFILTER:
			sampler = D3DSAMP_MAGFILTER;
			break;
		case D3DTSS_MINFILTER:
			sampler = D3DSAMP_MINFILTER;
			break;
		case D3DTSS_MIPFILTER:
			sampler = D3DSAMP_MIPFILTER;
			break;
		case D3DTSS_MIPMAPLODBIAS:
			sampler = D3DSAMP_MIPMAPLODBIAS;
			break;
		case D3DTSS_MAXMIPLEVEL:
			sampler = D3DSAMP_MAXMIPLEVEL;
			break;
		case D3DTSS_MAXANISOTROPY:
			sampler = D3DSAMP_MAXANISOTROPY;
			break;
		case D3DTSS_ADDRESSW:
			sampler = D3DSAMP_ADDRESSW;
			break;
		default:
			return RealDevice->GetTextureStageState (a, b, c);
		}
		return RealDevice->GetSamplerState (a, sampler, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetTextureStageState (DWORD a, D3DTEXTURESTAGESTATETYPE b, DWORD c) {
		//if (b == 11) return D3DERR_INVALIDCALL;
		//If it's something MGE controls then override it
		if (a == 0) {
			if ((b == D3DTSS_MINFILTER /*|| b == D3DTSS_MAGFILTER*/) && c == 2) {
				D3DSAMP_MINFILTER;
				return RealDevice->SetSamplerState (0, (D3DSAMPLERSTATETYPE) (b - 11), ScaleFilter);
			}
			if (b == D3DTSS_MIPFILTER && c == 2) {
				return RealDevice->SetSamplerState (0, D3DSAMP_MIPFILTER, MipFilter);
			}
			if (b == D3DTSS_MIPMAPLODBIAS && LODBias) {
				return RealDevice->SetSamplerState (0, D3DSAMP_MIPMAPLODBIAS, *(DWORD *)&LODBias);
			}
		}
		//Some bugger decided to split this into two functions : (
		D3DSAMPLERSTATETYPE sampler;
		switch (b) {
		case D3DTSS_ADDRESSU:
			sampler = D3DSAMP_ADDRESSU;
			break;
		case D3DTSS_ADDRESSV:
			sampler = D3DSAMP_ADDRESSV;
			break;
		case D3DTSS_BORDERCOLOR:
			sampler = D3DSAMP_BORDERCOLOR;
			break;
		case D3DTSS_MAGFILTER:
			sampler = D3DSAMP_MAGFILTER;
			break;
		case D3DTSS_MINFILTER:
			sampler = D3DSAMP_MINFILTER;
			break;
		case D3DTSS_MIPFILTER:
			sampler = D3DSAMP_MIPFILTER;
			break;
		case D3DTSS_MIPMAPLODBIAS:
			sampler = D3DSAMP_MIPMAPLODBIAS;
			break;
		case D3DTSS_MAXMIPLEVEL:
			sampler = D3DSAMP_MAXMIPLEVEL;
			break;
		case D3DTSS_MAXANISOTROPY:
			sampler = D3DSAMP_MAXANISOTROPY;
			break;
		case D3DTSS_ADDRESSW:
			sampler = D3DSAMP_ADDRESSW;
			break;
		default:
			return RealDevice->SetTextureStageState (a, b, c);
		}
		return RealDevice->SetSamplerState (a, sampler, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall ValidateDevice (DWORD *a) {
		return RealDevice->ValidateDevice (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetInfo (DWORD a, void *b, DWORD c) {
		//Doesn't seem to have a DX9 equivilent
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPaletteEntries (UINT a, const PALETTEENTRY *b) {
		return RealDevice->SetPaletteEntries (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPaletteEntries (UINT a, PALETTEENTRY *b) {
		return RealDevice->GetPaletteEntries (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetCurrentTexturePalette (UINT a) {
		return RealDevice->SetCurrentTexturePalette (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetCurrentTexturePalette (UINT *a) {
		return RealDevice->GetCurrentTexturePalette (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawPrimitive (D3DPRIMITIVETYPE a, UINT b, UINT c) {
		++NumDrawCalls;

		if (HookedShaderActive > 1) {
			HRESULT hr = 0;
			for (DWORD i = 0; i < HookedShaderActive; ++i) {
				ShaderStruct.MWSEeffects [ActiveHookedShader].effect->BeginPass (i);
				hr |= RealDevice->DrawPrimitive (a, b, c);
			}
			return hr;
		}

		//Record the render if recording is turned on
		if (!(CurrentDestBlend == D3DBLEND_ONE && AlphaBlendEnabled) && zWriteEnabled && !HUDRenderingStarted
			&& RecordRenders && !StencilEnabled && !RenderingToTexture
		) {
			DrawRecorder::RecordedDrawCall rdc;
			rdc.material = CurrentMaterial;
			rdc.texture = CurrentTexture;
			rdc.vBuffer = CurrentVBuffer;
			rdc.streamSpan = CurrentStreamSpan;
			rdc.indices = CurrentIndices;
			rdc.fvf = CurrentFVF;
			rdc.vertexBlendState = CurrentVertexBlendState;
			rdc.worldTrans [0] = CurrentWorldTrans;
			rdc.worldTrans [1] = CurrentBlendTrans1;
			rdc.worldTrans [2] = CurrentBlendTrans2;
			rdc.worldTrans [3] = CurrentBlendTrans3;
			rdc.cullModeState = CurrentCullModeState;
			rdc.alphaBlendEnabled = AlphaBlendEnabled;
			rdc.alphaTestEnabled = AlphaTestEnabled;
			rdc.alphaBlendFunc = CurrentSrcBlend;

			rdc.renderType = DrawRecorder::DRAW_PRIM;
			rdc.primType = a;
			rdc.startIndex = b;
			rdc.primitiveCount = c;

			DepthDrawRecorder.RecordDrawCall(rdc);
		}
		return RealDevice->DrawPrimitive (a, b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawIndexedPrimitive (D3DPRIMITIVETYPE a, UINT b, UINT c, UINT d, UINT e) {
		++NumDrawCalls;
		//check if HUD is being rendered //phal
		if (!RenderingToTexture && !HUDRenderingStarted && !ShadersApplied) {
			if (zWriteEnabled == FALSE
			    && (CurrentWorldTrans._11 == 1 && CurrentWorldTrans._12 == 0 && CurrentWorldTrans._13 == 0 && CurrentWorldTrans._14 == 0
			    && CurrentWorldTrans._22 == 1 && CurrentWorldTrans._21 == 0 && CurrentWorldTrans._23 == 0 && CurrentWorldTrans._24 == 0
			    && CurrentWorldTrans._33 == 1 && CurrentWorldTrans._31 == 0 && CurrentWorldTrans._32 == 0 && CurrentWorldTrans._34 == 0)
			    && abs (CurrentWorldTrans._42 - MatrixHUDidentifier) < 0.02f
			) {
				HUDRenderingStarted = true;

				//Hardware shaders
				if ((MGEFlags & USE_HW_SHADER) || UsedMWSEShaders) {
					RealDevice->EndScene ();
					ApplyShaders ();
					m_MWRenderState.RestoreState();
					RealDevice->BeginScene ();
				}

			}

		}

		// record sky renders
		DECLARE_MWBRIDGE
		if (!RenderingToTexture && (MGEFlags & REFLECT_SKY) && !HUDRenderingStarted && DistantLandActive && c != 4 && SceneCount == 0
			&& mwBridge->IsAboveWater () && (mwBridge->IsExterior () || mwBridge->IntLikeExterior ()) && (!mwBridge->IsMenu () || InvertUsingDL)
		) {
			if (zWriteEnabled == FALSE) {

				//clear last frame's recorded sky
				if (NumDrawCalls == 1) {
					SkyDrawRecorder.ClearRecordedDrawCalls();
				}
				DrawRecorder::RecordedDrawCall rdc;
				rdc.material = CurrentMaterial;
				rdc.texture = CurrentTexture;
				rdc.vBuffer = CurrentVBuffer;
				rdc.streamSpan = CurrentStreamSpan;
				rdc.indices = CurrentIndices;
				rdc.fvf = CurrentFVF;
				rdc.vertexBlendState = CurrentVertexBlendState;
				rdc.worldTrans [0] = CurrentWorldTrans;
				rdc.worldTrans [1] = CurrentBlendTrans1;
				rdc.worldTrans [2] = CurrentBlendTrans2;
				rdc.worldTrans [3] = CurrentBlendTrans3;
				rdc.cullModeState = CurrentCullModeState;
				rdc.alphaBlendEnabled = AlphaBlendEnabled;
				rdc.alphaTestEnabled = AlphaTestEnabled;
				rdc.alphaBlendFunc = CurrentSrcBlend;
				RealDevice->GetTextureStageState (0, D3DTSS_COLOROP, &rdc.colorOp);
				RealDevice->GetTextureStageState (0, D3DTSS_ALPHAOP, &rdc.alphaOp);

				rdc.renderType = DrawRecorder::DRAW_INDEXED;
				rdc.primType = a;
				rdc.baseVertexIndex = BaseVertexIndex;
				rdc.minIndex = b;
				rdc.numVertices = c;
				rdc.startIndex = d;
				rdc.primitiveCount = e;

				SkyDrawRecorder.RecordDrawCall(rdc);
			}
		}

		////Detect drawing of Shadow plane
		//if (StencilEnabled && AlphaBlendEnabled && a == D3DPT_TRIANGLELIST && b == 0 && c == 4 && d == 0 && e == 2) {
		//	ShadowPlaneDrawn = true;
		//	wrapper_log_file << "Shadow Plane Drawn during scene " << int (SceneCount) << " after " << int (ShadowsDrawn) << " shadows drawn." << endl;
		//}

		//Record the render if recording is turned on
		if (!(CurrentDestBlend == D3DBLEND_ONE && AlphaBlendEnabled) && zWriteEnabled && !HUDRenderingStarted
			&& RecordRenders && !StencilEnabled && !RenderingToTexture
		) {
			DrawRecorder::RecordedDrawCall rdc;
			rdc.material = CurrentMaterial;
			rdc.texture = CurrentTexture;
			rdc.vBuffer = CurrentVBuffer;
			rdc.streamSpan = CurrentStreamSpan;
			rdc.indices = CurrentIndices;
			rdc.fvf = CurrentFVF;
			rdc.vertexBlendState = CurrentVertexBlendState;
			rdc.worldTrans [0] = CurrentWorldTrans;
			rdc.worldTrans [1] = CurrentBlendTrans1;
			rdc.worldTrans [2] = CurrentBlendTrans2;
			rdc.worldTrans [3] = CurrentBlendTrans3;
			rdc.cullModeState = CurrentCullModeState;
			rdc.alphaBlendEnabled = AlphaBlendEnabled;
			rdc.alphaTestEnabled = AlphaTestEnabled;
			rdc.alphaBlendFunc = CurrentSrcBlend;

			rdc.renderType = DrawRecorder::DRAW_INDEXED;
			rdc.primType = a;
			rdc.baseVertexIndex = BaseVertexIndex;
			rdc.minIndex = b;
			rdc.numVertices = c;
			rdc.startIndex = d;
			rdc.primitiveCount = e;

			DepthDrawRecorder.RecordDrawCall(rdc);

			//D3DXMatrixIdentity (&CurrentBlendTrans1);
			//D3DXMatrixIdentity (&CurrentBlendTrans2);
			//D3DXMatrixIdentity (&CurrentBlendTrans3);
		}

		if (HookedShaderActive > 1) {
			HRESULT hr = 0;
			for (DWORD i = 0; i < HookedShaderActive; ++i) {
				ShaderStruct.MWSEeffects [ActiveHookedShader].effect->BeginPass (i);
				hr |= RealDevice->DrawIndexedPrimitive (a, BaseVertexIndex, b, c, d, e);
			}
			return hr;
		}
		return RealDevice->DrawIndexedPrimitive (a, BaseVertexIndex, b, c, d, e);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawPrimitiveUP (D3DPRIMITIVETYPE a, UINT b, const void *c, UINT d) {
		++NumDrawCalls;
		if (HookedShaderActive > 1) {
			HRESULT hr = 0;
			for (DWORD i = 0; i < HookedShaderActive; ++i) {
				ShaderStruct.MWSEeffects [ActiveHookedShader].effect->BeginPass (i);
				hr |= RealDevice->DrawPrimitiveUP (a, b, c, d);
			}
			return hr;
		}
		//Record the render if recording is turned on
		if (!(CurrentDestBlend == D3DBLEND_ONE && AlphaBlendEnabled) && zWriteEnabled && !HUDRenderingStarted
			&& RecordRenders && !StencilEnabled && !RenderingToTexture
		) {
			DrawRecorder::RecordedDrawCall rdc;
			rdc.material = CurrentMaterial;
			rdc.texture = CurrentTexture;
			rdc.vBuffer = CurrentVBuffer;
			rdc.streamSpan = CurrentStreamSpan;
			rdc.indices = CurrentIndices;
			rdc.fvf = CurrentFVF;
			rdc.vertexBlendState = CurrentVertexBlendState;
			rdc.worldTrans [0] = CurrentWorldTrans;
			rdc.worldTrans [1] = CurrentBlendTrans1;
			rdc.worldTrans [2] = CurrentBlendTrans2;
			rdc.worldTrans [3] = CurrentBlendTrans3;
			rdc.cullModeState = CurrentCullModeState;
			rdc.alphaBlendEnabled = AlphaBlendEnabled;
			rdc.alphaTestEnabled = AlphaTestEnabled;
			rdc.alphaBlendFunc = CurrentSrcBlend;

			rdc.renderType = DrawRecorder::DRAW_PRIM_UP;
			rdc.primType = a;
			rdc.primitiveCount = b;
			rdc.vertexStreamZeroData = c;
			rdc.vertexStreamZeroSride = d;

			DepthDrawRecorder.RecordDrawCall(rdc);
		}
		return RealDevice->DrawPrimitiveUP (a, b, c, d);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawIndexedPrimitiveUP (D3DPRIMITIVETYPE a, UINT b, UINT c, UINT d, const void *e, D3DFORMAT f, const void *g, UINT h) {
		++NumDrawCalls;
		if (HookedShaderActive > 1) {
			HRESULT hr = 0;
			for (DWORD i = 0; i < HookedShaderActive; ++i) {
				ShaderStruct.MWSEeffects [ActiveHookedShader].effect->BeginPass (i);
				hr |= RealDevice->DrawIndexedPrimitiveUP (a, b, c, d, e, f, g, h);
			}
			return hr;
		}

		//Record the render if recording is turned on
		if (!(CurrentDestBlend == D3DBLEND_ONE && AlphaBlendEnabled) && zWriteEnabled && !HUDRenderingStarted
			&& RecordRenders && !StencilEnabled && !RenderingToTexture
		) {
			DrawRecorder::RecordedDrawCall rdc;

			rdc.material = CurrentMaterial;
			rdc.texture = CurrentTexture;
			rdc.vBuffer = CurrentVBuffer;
			rdc.streamSpan = CurrentStreamSpan;
			rdc.indices = CurrentIndices;
			rdc.fvf = CurrentFVF;
			rdc.vertexBlendState = CurrentVertexBlendState;
			rdc.worldTrans [0] = CurrentWorldTrans;
			rdc.worldTrans [1] = CurrentBlendTrans1;
			rdc.worldTrans [2] = CurrentBlendTrans2;
			rdc.worldTrans [3] = CurrentBlendTrans3;
			rdc.cullModeState = CurrentCullModeState;
			rdc.alphaBlendEnabled = AlphaBlendEnabled;
			rdc.alphaTestEnabled = AlphaTestEnabled;
			rdc.alphaBlendFunc = CurrentSrcBlend;

			rdc.renderType = DrawRecorder::DRAW_INDEXED_UP;
			rdc.primType = a;
			rdc.minIndex = b;
			rdc.numVertices = c;
			rdc.primitiveCount = d;
			rdc.indexData = e;
			rdc.indexDataFormat = f;
			rdc.vertexStreamZeroData = g;
			rdc.vertexStreamZeroSride = h;

			DepthDrawRecorder.RecordDrawCall(rdc);
		}
		return RealDevice->DrawIndexedPrimitiveUP (a, b, c, d, e, f, g, h);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall ProcessVertices (UINT a, UINT b, UINT c, IDirect3DVertexBuffer8 *d, DWORD e) {
		return RealDevice->ProcessVertices (a, b, c, (IDirect3DVertexBuffer9 *)d, NULL, e);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateVertexShader (const DWORD *a, const DWORD *b, DWORD *c, DWORD d) {
		*c = (vShaderCount += 2);
		ID3DXBuffer *ShaderData = NULL;
		int i = 0;
		switch (*c) {
		case 9: ++i;
		case 7: ++i;
		case 5: ++i;
		case 3: ++i;
			char shaderfile [MAX_PATH];
			sprintf_s (shaderfile, sizeof (shaderfile), "Data Files\\shaders\\water\\vs%i.txt", i);
			D3DXAssembleShaderFromFileA (shaderfile, NULL, NULL, 0, &ShaderData, NULL);
			break;
		default:
			ERRORMSG ("Morrowind attempted to create a vertex shader which MGE did not expect");
			/* ID3DXBuffer *output;
			HANDLE h;
			DWORD unused;
			D3DXDisassembleShader (b, false, NULL, &output);
			h = CreateFileA ("vout.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			WriteFile (h, output->GetBufferPointer (), output->GetBufferSize () - 1, &unused, NULL);
			CloseHandle (h);
			D3DXAssembleShaderFromFileA ("vout.txt", NULL, NULL, 0, &ShaderData, &Errors); break;
			break; */
		}
		HRESULT hr = RealDevice->CreateVertexShader ((DWORD *)ShaderData->GetBufferPointer (), &NewVShaders [(*c - 3) / 2]);
		if (ShaderData) ShaderData->Release ();
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetVertexShader (DWORD a) {
		CurrentVShader = a;
		switch (a) {
		case 0:
			RealDevice->SetVertexShader (NULL);
		case 3:
		case 5:
		case 9:
			RealDevice->SetVertexDeclaration (decl [0]);
			break;
		case 7:
			RealDevice->SetVertexDeclaration (decl [1]);
			break;
		default:
			//Save FVF setting
			m_MWRenderState.FVF = a;

			CurrentVShader = 0;
			RealDevice->SetVertexShader (NULL);
			CurrentFVF = a;
			return RealDevice->SetFVF (a);
		}
		return RealDevice->SetVertexShader (NewVShaders [(a - 3) / 2]);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetVertexShader (DWORD *a) {
		if (CurrentVShader != 0) {
			*a = CurrentVShader;
			return D3D_OK;
		} else {
			return RealDevice->GetFVF (a);
		}
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DeleteVertexShader (DWORD a) {
		if (NewVShaders [(a - 3) / 2] != NULL) {
			while (NewVShaders [(a - 3) / 2]->Release ());
			NewVShaders [(a - 3) / 2] = NULL;
		}
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetVertexShaderConstant (DWORD a, const void *b, DWORD c) {
		return RealDevice->SetVertexShaderConstantF (a, (float *)b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetVertexShaderConstant (DWORD a, void *b, DWORD c) {
		return RealDevice->GetVertexShaderConstantF (a, (float *)b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetVertexShaderDeclaration (DWORD a, void *b, DWORD *c) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetVertexShaderFunction (DWORD a, void *b, DWORD *c) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetStreamSource (UINT a, IDirect3DVertexBuffer8 *b, UINT c) {
		CurrentVBuffer = (IDirect3DVertexBuffer9 *)b;
		CurrentStreamSpan = c;
		return RealDevice->SetStreamSource (a, (IDirect3DVertexBuffer9 *)b, 0, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetStreamSource (UINT a, IDirect3DVertexBuffer8 **b, UINT *c) {
		UINT unused;
		return RealDevice->GetStreamSource (a, (IDirect3DVertexBuffer9 **)b, &unused, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetIndices (IDirect3DIndexBuffer8 *a, UINT b) {
		BaseVertexIndex = b; //BaseVertexIndex has been moved to ProcessVerticies ()
		CurrentIndices = (IDirect3DIndexBuffer9 *)a;
		return RealDevice->SetIndices ((IDirect3DIndexBuffer9 *)a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetIndices (IDirect3DIndexBuffer8 **a, UINT *b) {
		*b = BaseVertexIndex;
		return RealDevice->GetIndices ((IDirect3DIndexBuffer9 **)a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreatePixelShader (const DWORD *a, DWORD *b) {
		PixelShading = true;
		*b = ++pShaderCount;
		ID3DXBuffer *ShaderData = NULL;
		char shaderfile [MAX_PATH];
		sprintf_s (shaderfile, sizeof (shaderfile), "Data Files\\shaders\\water\\ps%i.txt", *b);
		if (*b <= 9 && *b > 0) D3DXAssembleShaderFromFileA (shaderfile, NULL, NULL, 0, &ShaderData, NULL);
		else ERRORMSG ("Morrowind tried to create an unexpected pixel shader");
		HRESULT hr = RealDevice->CreatePixelShader ((DWORD *)ShaderData->GetBufferPointer (), &NewPShaders [*b - 1]);
		if (hr != D3D_OK) {
			DebugBreak ();
		}
		if (ShaderData != NULL) ShaderData->Release ();
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPixelShader (DWORD a) {
		CurrentPShader = a;
		if (a == 0) return RealDevice->SetPixelShader (NULL);
		else return RealDevice->SetPixelShader (NewPShaders [a - 1]);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPixelShader (DWORD *a) {
		*a = CurrentPShader;
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DeletePixelShader (DWORD a) {
		if (NewPShaders [a - 1] != NULL) {
			while (NewPShaders [a - 1]->Release ());
			NewPShaders [a - 1] = NULL;
		}
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPixelShaderConstant (DWORD a, const void *b, DWORD c) {
		return RealDevice->SetPixelShaderConstantF (a, (float *)b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPixelShaderConstant (DWORD a, void *b, DWORD c) {
		return RealDevice->GetPixelShaderConstantF (a, (float *)b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPixelShaderFunction (DWORD a, void * b, DWORD * c) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawRectPatch (UINT a, const float *b, const D3DRECTPATCH_INFO *c) {
		return RealDevice->DrawRectPatch (a, b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawTriPatch (UINT a, const float *b, const D3DTRIPATCH_INFO *c) {
		return RealDevice->DrawTriPatch (a, b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DeletePatch (UINT a) {
		return RealDevice->DeletePatch (a);
	}

	//-----------------------------------------------------------------------------

};


//=============================================================================


class FakeObject : public IDirect3D8 {

public:

	//-----------------------------------------------------------------------------
	/*** Fields ***/
	//-----------------------------------------------------------------------------

	DWORD Refs;


	//-----------------------------------------------------------------------------
	/*** Misc methods ***/
	//-----------------------------------------------------------------------------

	FakeObject (IDirect3D *Real) {
		RealObject = Real;
		Refs = 1;
	}


	//-----------------------------------------------------------------------------
	/*** IUnknown methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall QueryInterface (REFIID a, LPVOID *b) {
		return RealObject->QueryInterface (a, b);
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall AddRef (void) {
		return ++Refs;
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall Release (void) {
		if (--Refs == 0) {
			RealObject->Release ();
			delete this;
			return 0;
		}
		return Refs;
	}


	//-----------------------------------------------------------------------------
	/*** IDirect3D8 methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall RegisterSoftwareDevice (void *a) {
		return RealObject->RegisterSoftwareDevice (a);
	}

	//-----------------------------------------------------------------------------

	UINT _stdcall GetAdapterCount (void) {
		return RealObject->GetAdapterCount ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetAdapterIdentifier (UINT a, DWORD b, D3DADAPTER_IDENTIFIER8 *c) {
		D3DADAPTER_IDENTIFIER9 c2;
		HRESULT hr = RealObject->GetAdapterIdentifier (a, 0, &c2);
		if (hr == D3D_OK) {
			//Strings, so use copymemory instead of assignment
			::CopyMemory (c, &c2, 1024);
			c->DeviceId = c2.DeviceId;
			c->DeviceIdentifier = c2.DeviceIdentifier;
			c->DriverVersion = c2.DriverVersion;
			c->Revision = c2.Revision;
			c->SubSysId = c2.SubSysId;
			c->VendorId = c2.VendorId;
			c->WHQLLevel = c2.WHQLLevel;
		}
		return hr;
	}

	//-----------------------------------------------------------------------------

	UINT _stdcall GetAdapterModeCount (UINT a) {
		return RealObject->GetAdapterModeCount (a, D3DFMT_X8R8G8B8);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall EnumAdapterModes (UINT a, UINT b, D3DDISPLAYMODE *c) {
		return RealObject->EnumAdapterModes (a, D3DFMT_X8R8G8B8, b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetAdapterDisplayMode (UINT a, D3DDISPLAYMODE *b) {
		//Hide the fact that windows is using a 16bit colour depth
		HRESULT hr = RealObject->GetAdapterDisplayMode (a, b);
		b->Format = D3DFMT_X8R8G8B8;
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CheckDeviceType (UINT a, D3DDEVTYPE b, D3DFORMAT c, D3DFORMAT d, BOOL e) {
		return RealObject->CheckDeviceType (a, b, c, d, e);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CheckDeviceFormat (UINT a, D3DDEVTYPE b, D3DFORMAT c, DWORD d, D3DRESOURCETYPE e, D3DFORMAT f) {
		return RealObject->CheckDeviceFormat (a, b, c, d, e, f);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CheckDeviceMultiSampleType (UINT a, D3DDEVTYPE b, D3DFORMAT c, BOOL d, D3DMULTISAMPLE_TYPE e) {
		return RealObject->CheckDeviceMultiSampleType (a, b, c, d, e, NULL);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CheckDepthStencilMatch (UINT a, D3DDEVTYPE b, D3DFORMAT c, D3DFORMAT d, D3DFORMAT e) {
		return RealObject->CheckDepthStencilMatch (a, b, c, d, e);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDeviceCaps (UINT a, D3DDEVTYPE b, D3DCAPS8 *c) {
		*c = d3d8Caps;
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HMONITOR _stdcall GetAdapterMonitor (UINT a) {
		return RealObject->GetAdapterMonitor (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateDevice (UINT a, D3DDEVTYPE b, HWND c, DWORD d, D3DPRESENT_PARAMETERS8 *e, IDirect3DDevice8 **f) {
	
		if ( WindowedMode = (e->Windowed == TRUE) ) {
			// make MW window resizable
			HWND win = GetParent(c);
			client_hWND = c;

			RECT r;
			GetWindowRect (GetDesktopWindow(), &r);

			RECT s;
			s.left=s.top=0;
			s.right=e->BackBufferWidth;
			s.bottom=e->BackBufferHeight;

			long WNDstyle = WS_CLIPSIBLINGS|WS_VISIBLE|WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX|WS_BORDER|WS_SYSMENU|WS_CAPTION;
			AdjustWindowRect(&s, WNDstyle, false);
			ShowWindow(win, SW_HIDE);
			SetWindowLong(win, GWL_STYLE, WNDstyle );
			// center on desktop
			SetWindowPos(win,0,
				(r.left+r.right)/2 - (s.right-s.left)/2, 
				(r.top+r.bottom)/2 - (s.bottom-s.top)/2,
				s.right-s.left,//e->BackBufferWidth,
				s.bottom-s.top,//e->BackBufferHeight,
				SWP_NOZORDER|SWP_FRAMECHANGED);
			ShowWindow(c, SW_SHOWMAXIMIZED);
			GetClientRect (win, &r);
			clientWND_width = r.right - r.left;
			clientWND_height = r.bottom - r.top;
		}

		if (!TriedOnce) {
			if (MGEFlags & MWE_DISABLED) LOG::log ("MWE: Disabled\r\n");
			else InjectMWE(c);
			if (!PatchDisabled) { delete lpPatchTree; lpPatchTree = NULL; }
			TriedOnce = true;
		}

		D3DPRESENT_PARAMETERS9 e2;
		D3DSURFACE_DESC BackBufferDesc;
		JointInfo.GraphicsWidth = (WORD)e->BackBufferWidth;
		JointInfo.GraphicsHeight = (WORD)e->BackBufferHeight;
		JointInfo.aspect = (float)e->BackBufferWidth / (float)e->BackBufferHeight;
		RcpRes.x = (1 / (float)e->BackBufferWidth);
		RcpRes.y = (1 / (float)e->BackBufferHeight);
		RcpRes.z = 0;
		RcpRes.w = 1;
		e2.Flags = e->Flags;
		e2.Windowed = e->Windowed;
		e2.BackBufferCount = e->BackBufferCount;
		e2.BackBufferFormat = e->BackBufferFormat;
		e2.BackBufferHeight = e->BackBufferHeight;
		e2.BackBufferWidth = e->BackBufferWidth;
		e2.hDeviceWindow = e->hDeviceWindow;
		e2.EnableAutoDepthStencil = e->EnableAutoDepthStencil;
		e2.SwapEffect = e->SwapEffect;
		e2.MultiSampleQuality = 0;
		//e2.MultiSampleType = (D3DMULTISAMPLE_TYPE)AALevel;
		e2.MultiSampleType = e->MultiSampleType = (D3DMULTISAMPLE_TYPE)AALevel;


		if (e->Windowed) JointInfo.WNDwindowed = true;
		else JointInfo.WNDwindowed = false;

		if (e2.Flags & D3DPRESENTFLAG_LOCKABLE_BACKBUFFER) e2.Flags ^= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		if ((MGEFlags & USE_DISTANT_LAND) || (ZBufFormat > 0 && ZBufFormat < 5)) e2.AutoDepthStencilFormat = D3DFMT_D24S8;
		else e2.AutoDepthStencilFormat = (D3DFORMAT)ZBufFormat;

		if (!e->Windowed) e2.FullScreen_RefreshRateInHz = RefreshRate;
		else e2.FullScreen_RefreshRateInHz = 0;

		if (VWait == 3) e2.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		else {
			if (VWait > 1 && e->Windowed) e2.PresentationInterval = 0;
			else e2.PresentationInterval = VWait;
		}

#ifdef NV_PERF_HUD
		for (UINT Adapter = 0; Adapter < RealObject->GetAdapterCount (); ++Adapter) {
			D3DADAPTER_IDENTIFIER9 Identifier;
			HRESULT Res;
			Res = RealObject->GetAdapterIdentifier (Adapter, 0, &Identifier);
			if (strstr (Identifier.Description, "PerfHUD") != 0) {
				a = Adapter;
				b = D3DDEVTYPE_REF;
				break;
			}
		}
#endif

		HRESULT hr = RealObject->CreateDevice (a, b, c, d, &e2, &RealDevice);

		if (hr != D3D_OK || RealObject == NULL) return hr;

		RealDevice->SetRenderState (D3DRS_MULTISAMPLEANTIALIAS, (AALevel > 0));

		//Texture based depth buffers
		if (ZBufFormat > 0 && ZBufFormat < 5) {
			RealDevice->GetDepthStencilSurface (&OldDepthSurface);
			D3DFORMAT fmt;
			switch (ZBufFormat) {
			case 1:
				fmt = (D3DFORMAT)MAKEFOURCC ('I', 'N', 'T', 'Z');
				break;
			case 2:
				fmt = (D3DFORMAT)MAKEFOURCC ('R', 'A', 'W', 'Z');
				break;
			case 3:
				fmt = (D3DFORMAT)MAKEFOURCC ('D', 'F', '1', '6');
				break;
			case 4:
				fmt = (D3DFORMAT)MAKEFOURCC ('D', 'F', '2', '4');
				break;
			}
			hr = RealDevice->CreateTexture (e->BackBufferWidth, e->BackBufferHeight, 1, D3DUSAGE_DEPTHSTENCIL, fmt, D3DPOOL_DEFAULT, &DepthTexture, 0);
			if (FAILED (hr)) FATALERROR ("Unsupported z-buffer format");
			DepthTexture->GetSurfaceLevel (0, &DepthBuffer);
			RealDevice->SetDepthStencilSurface (DepthBuffer);
			RealDevice->SetRenderState (D3DRS_ZENABLE, 1);
			RealDevice->SetRenderState (D3DRS_ZWRITEENABLE, 1);
			RealDevice->SetRenderState (D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			MyBackbuffer = true; //Morrowind never calls SetRenderTarget with a RAWZ depth buffer
		} else {
			hr = RealDevice->GetDepthStencilSurface (&DepthBuffer);
		}

		//Setup any render options we need (Do _NOT_ enable fog here!)
		//FOV = Options.InitialZoom;
		//Setup render states
		RealDevice->SetSamplerState (0, D3DSAMP_MAXANISOTROPY, AnisoLevel);
		RealDevice->SetSamplerState (0, D3DSAMP_MINFILTER, ScaleFilter);
		//RealDevice->SetSamplerState (0, D3DSAMP_MAGFILTER, ScaleFilter); //Only min filter needs to be changed
		RealDevice->SetSamplerState (0, D3DSAMP_MIPMAPLODBIAS, *(DWORD *)&LODBias);
		RealDevice->SetRenderState (D3DRS_FOGTABLEMODE, (D3DTEXTURESTAGESTATETYPE)FogPixelMode);
		RealDevice->SetRenderState (D3DRS_FOGVERTEXMODE, (D3DTEXTURESTAGESTATETYPE)FogVertexMode);
		RealDevice->SetRenderState (D3DRS_RANGEFOGENABLE, (D3DTEXTURESTAGESTATETYPE)RangedFog);
		UpdateFogMethod();

		//RealDevice->SetRenderState (D3DRS_ZENABLE, D3DZB_USEW);
		//RealDevice->SetRenderState (D3DRS_CLIPPING, (D3DTEXTURESTAGESTATETYPE)0);
		
		//Get a pointer to the default back buffer and set up stuff for fullscreen shaders
		hr = RealDevice->GetBackBuffer (0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuffer);
		BackBuffer->GetDesc (&BackBufferDesc);

		// Initialize the depth manager
		if ( UsingDistantLand || (MGEFlags & SHADER_DEPTH) ) {
			g_DepthManager.Initialize( RealDevice, BackBufferDesc.Width, BackBufferDesc.Height );
		}

		//Create texture surfaces for fullscreen hardware shaders
		RealDevice->CreateTexture (BackBufferDesc.Width, BackBufferDesc.Height, 1, D3DUSAGE_RENDERTARGET, BackBufferDesc.Format,
			D3DPOOL_DEFAULT, &ShaderStruct.thisframe, NULL
		);
		ShaderStruct.thisframe->GetSurfaceLevel (0, &ShaderStruct.thisframeSurface);
		RealDevice->CreateTexture (BackBufferDesc.Width, BackBufferDesc.Height, 1, D3DUSAGE_RENDERTARGET, BackBufferDesc.Format,
			D3DPOOL_DEFAULT, &ShaderStruct.lastframe, NULL
		);
		ShaderStruct.lastframe->GetSurfaceLevel (0, &ShaderStruct.lastframeSurface);
		/* RealDevice->CreateTexture (BackBufferDesc.Width, BackBufferDesc.Height, 1, D3DUSAGE_RENDERTARGET, BackBufferDesc.Format,
			D3DPOOL_DEFAULT, &ShaderStruct.lastpass, NULL
		); *///phal
		RealDevice->CreateTexture (BackBufferDesc.Width, BackBufferDesc.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, &ShaderStruct.lastpass, NULL
		);
		ShaderStruct.lastpass->GetSurfaceLevel (0, &ShaderStruct.lastpassSurface);
		RealDevice->CreateTexture (BackBufferDesc.Width, BackBufferDesc.Height, 1, D3DUSAGE_RENDERTARGET, BackBufferDesc.Format,
			D3DPOOL_DEFAULT, &ShaderStruct.lastshader, NULL
		);
		ShaderStruct.lastshader->GetSurfaceLevel (0, &ShaderStruct.lastshaderSurface);
		
		RealDevice->CreateTexture (BackBufferDesc.Width, BackBufferDesc.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, &tempShaderTexture, NULL
		);
		tempShaderTexture->GetSurfaceLevel (0, &tempShaderSurface);
		ShaderVertices [3].x = ShaderVertices [2].x = (float) (BackBufferDesc.Width)-.5f;
		ShaderVertices [3].y = ShaderVertices [1].y = (float) (BackBufferDesc.Height)-.5f;
		RealDevice->CreateVertexBuffer (4 * sizeof (MyVertex), D3DUSAGE_WRITEONLY, MYVERTEXFORMAT, D3DPOOL_DEFAULT,
			&ShaderStruct.buffer, 0
		);
		byte *VertexPointer;
		ShaderStruct.buffer->Lock (0, 0, (void **)&VertexPointer, 0);
		CopyMemory (VertexPointer, ShaderVertices, sizeof (ShaderVertices));
		ShaderStruct.buffer->Unlock ();
		//Hardware shaders
		HANDLE InFile = CreateFileA ("Data Files\\shaders\\default\\active.sav", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (InFile != INVALID_HANDLE_VALUE) {
			BYTE b;
			DWORD read;
			DWORD count = 0;
			char TexPath [256];
			while (true) {
				ReadFile (InFile, &b, 1, &read, NULL);
				if (!read) break;
				if (count > 255) { ERRORMSG ("Shader name is greater than 256 characters long"); }
				TexPath [count++] = b;
				if (!b) {
					hr = D3DXCreateEffectFromFile (RealDevice, TexPath, 0, 0, 0, 0,
						&ShaderStruct.DefaultEffects [ShaderStruct.DefEffectCount].effect, NULL);
					if (hr == D3D_OK) {
						ShaderStruct.DefaultEffects [ShaderStruct.DefEffectCount].effect->SetTechnique ("T0");
						LoadShaderTextures (ShaderStruct.DefaultEffects [ShaderStruct.DefEffectCount].effect);
						InitShaderFlags (&ShaderStruct.DefaultEffects [ShaderStruct.DefEffectCount]);
						++ShaderStruct.DefEffectCount;
					}
					count = 0;
				}
			}
			CloseHandle (InFile);
		} else {
			BitReset (&MGEFlags, USE_HW_SHADER_BIT);
		}
		RealDevice->CreateStateBlock (D3DSBT_ALL, &StateBlock);
		//Create a font object for scribbling over the screen
		if (D3DXCreateFontA (RealDevice, 12, 0, 400, 1, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "arial", &Font)
		) {
			DEBUGSTRING ("MGE: Could not create font."
				"If MGE is set to display messages, it will probably CTD shortly.\n"
			);
		}
		//Create a sprite object for MGE HUD elements
		if (D3DXCreateSprite (RealDevice, &HUDsprite) != D3D_OK) {
			DEBUGSTRING ("MGE: Could not create HUD sprite."
				"Any mod that tries to add HUD elements will cause morrowind to CTD.\n"
			);
		}

		//if (Options.UseDistantLand) DL_Init ();
		//Create and return the fake device
		fpsLastTime = GetTickCount ();
		SETSTATUS (VERSION)6;
		*f = new FakeDevice (this);

		//Calculate view matrix element that identifies HUD rendering:
		MatrixHUDidentifier = (float)((int)(1.0f + BackBufferDesc.Width * 0.651608f));

		return D3D_OK;
	}

	//-----------------------------------------------------------------------------
};


//=============================================================================


void * CreateD3DWrapper (UINT version) {

	if(InvertUsingDL == 0x01) {
		MGEFlags ^= USE_DISTANT_LAND;
		SetDistantLand((MGEFlags & USE_DISTANT_LAND) != 0);
		UsingDistantLand = false;
		WaterLoadedCount = 0;
		InvertUsingDL++;
	}

	static bool hooked = false;

	//Init texture hooks if using texture hooking
	if (hooked == false && (MGEFlags & (USE_TEX_HOOKS | USE_DISTANT_LAND))) {
		InitHooks ();
		hooked = true;
	}

	//Local handle variables
	HMODULE d3ddll = NULL;
	D3DProc8 func = NULL;
	D3DProc9 func9 = NULL;

	//Load the real d3d8 dll and get device caps
	char Path [MAX_PATH];
	GetSystemDirectoryA (Path, MAX_PATH);
	strcat_s (Path, MAX_PATH, "\\d3d8.dll");
	d3ddll = LoadLibraryA (Path);
	func = (D3DProc8)GetProcAddress (d3ddll, "Direct3DCreate8");
	if (func == NULL) {
		ERRORMSG ("MGE: Unable to load real direct 3d dll");
		return 0;
	}
	IDirect3D8 *TempObject = (func)(version);
	HRESULT hr = TempObject->GetDeviceCaps (0, D3DDEVTYPE_HAL, &d3d8Caps);
	if (hr != D3D_OK) { ERRORMSG ("Unable to get DX8 device caps") }
	TempObject->Release ();
	FreeLibrary (d3ddll);

	//If using distant land, make sure that standard pixel shaders aren't in use
	if (MGEFlags & USE_DISTANT_LAND) {
		d3d8Caps.PixelShaderVersion = 0;
		d3d8Caps.VertexShaderVersion = 0;
		UsingDistantLand = !(MGEFlags & NOT_USING_DL);
	}

	//Load the dx9 dll and return
	d3ddll = LoadLibrary ("d3d9.dll");
	func9 = (D3DProc9)GetProcAddress (d3ddll, "Direct3DCreate9");
	DEBUGSTRING ("MGE: The Direct3D8 device has been wrapped\n");
	return new FakeObject ((func9)(D3D_SDK_VERSION));
}


//=============================================================================


void ApplyShaders () {		//added by phal
	if (ShadersApplied) return;
	ShadersApplied = true;

	//check if shaders are activated and if backbuffer was cleared before this frame
	if (!(((MGEFlags & USE_HW_SHADER) && ShaderStruct.DefEffectCount > 0) || UsedMWSEShaders)
		|| (!BackBufferWasCleared && (MGEFlags & (AA_COLOUR_FIX)))
	) return;

	DECLARE_MWBRIDGE
	if ((MGEFlags & NO_SHADER_IN_MENU) && (LoadingScreen || (mwBridge->IsLoaded() && mwBridge->IsMenu()))){
		return;
	}	//mendres: Should fix framerate issues in menus.

	DWORD time = GetTickCount ();

	D3DXMATRIX old_view, old_proj, old_world;
	RealDevice->GetTransform (D3DTS_VIEW, &old_view);
	RealDevice->GetTransform (D3DTS_PROJECTION, &old_proj);
	RealDevice->GetTransform (D3DTS_WORLD, &old_world);

	DWORD alphaTest;
	DWORD zenable;
	DWORD zwriteenable;
	DWORD alphablend;
	DWORD srcblend;
	DWORD destblend;

	RealDevice->GetRenderState (D3DRS_ALPHATESTENABLE, &alphaTest);
	RealDevice->GetRenderState (D3DRS_ZENABLE, &zenable);
	RealDevice->GetRenderState (D3DRS_ZWRITEENABLE, &zwriteenable);
	RealDevice->GetRenderState (D3DRS_ALPHABLENDENABLE, &alphablend);
	RealDevice->GetRenderState (D3DRS_SRCBLEND, &srcblend);
	RealDevice->GetRenderState (D3DRS_DESTBLEND, &destblend);

	//IDirect3DBaseTexture9 *oldTexture; RealDevice->GetTexture (0, &oldTexture);
	//IDirect3DBaseTexture9 *oldTexture1; RealDevice->GetTexture (1, &oldTexture1);

	DWORD FVF; RealDevice->GetFVF (&FVF);
	DWORD CullMode; RealDevice->GetRenderState (D3DRS_CULLMODE, &CullMode);

	IDirect3DVertexBuffer9 *old_stream_data;
	unsigned int old_offset;
	unsigned int old_stride;
	RealDevice->GetStreamSource (0, &old_stream_data, &old_offset, &old_stride);
	IDirect3DIndexBuffer9 *old_index_data;
	RealDevice->GetIndices (&old_index_data);

	RealDevice->SetRenderState (D3DRS_ALPHATESTENABLE, 0);
	RealDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, 0);

	RealDevice->SetRenderState (D3DRS_ZENABLE, 0);
	RealDevice->SetRenderState (D3DRS_ZWRITEENABLE, 0);


	RealDevice->StretchRect (BackBuffer, 0, ShaderStruct.thisframeSurface, 0, D3DTEXF_NONE);
	if ( g_DepthManager.IsInitialized() ) {
		//if recorded renders have not yet been written into depth map, do it now.
		DrawRecordedDrawCallDepth( &TransView, &TransProj);

		//Flush renders from last frame
		DepthDrawRecorder.ClearRecordedDrawCalls();

		if ( DistantLandActive) {
			//Blend the near and far views
			DL_BlendNearAndFarView();
		}
	}

	if (DepthTexture) RealDevice->SetDepthStencilSurface (0);
	UINT passes = 0;
	IDirect3DBaseTexture9 *textures [6];
	RealDevice->BeginScene ();
	for (int j = 0; j < 6; ++j) {
		RealDevice->GetTexture (j, &textures [j]);
		RealDevice->SetTexture (j, ShaderStruct.lastshader);
	}


	RealDevice->SetStreamSource (0, ShaderStruct.buffer, 0, sizeof (MyVertex)); 
	RealDevice->SetFVF (MYVERTEXFORMAT);

	RealDevice->SetRenderState (D3DRS_CULLMODE, 2);

	RealDevice->StretchRect (ShaderStruct.thisframeSurface, 0, BackBuffer, 0, D3DTEXF_NONE);

	// phal -- render passes onto temporary surface, then copy it onto backbuffer afterwards
	// this enables support for alpha channel between passes
	RealDevice->SetRenderTarget (0, tempShaderSurface);

	// DECLARE_MWBRIDGE	//mendres

	int i;
	if (MGEFlags & USE_HW_SHADER) {
		int ShaderCount = ShaderStruct.DefEffectCount;
		for (i = 0; i < ShaderCount; ++i) {
			LoadedShader *shader = &ShaderStruct.DefaultEffects [i];
			DWORD sflags = shader->flags;
			if (((sflags & SFLAG_NOTINTERIOR) && !mwBridge->IsExterior () && !mwBridge->IntLikeExterior ())
				|| ((sflags & SFLAG_NOTEXTERIOR) && mwBridge->IsExterior ())
				|| ((sflags & SFLAG_NOTINTEXT) && mwBridge->IntLikeExterior ())
				|| ((sflags & SFLAG_NOTUNDERWATER) && !mwBridge->IsAboveWater ())
				|| ((sflags & SFLAG_NOTABOVEWATER) && mwBridge->IsAboveWater ())
				|| ((sflags & SFLAG_SUNVIS) && (SunVis >= 0.001))
				|| ((sflags & SFLAG_NOTSUNVIS) && (SunVis < 0.001))
			) continue;
			ID3DXEffect *seff = shader->effect;
			if (!seff) continue;

			RealDevice->StretchRect (BackBuffer, 0, ShaderStruct.lastshaderSurface, 0, D3DTEXF_NONE);

			//LoadShaderTextures(seff);
			SetShaderVars (shader, time);
			seff->Begin (&passes, 0);
			for (UINT pass = 0; pass < passes; ++pass) {
				seff->BeginPass (pass);
				RealDevice->DrawPrimitive (D3DPT_TRIANGLESTRIP, 0, 2);
				seff->EndPass ();
				//RealDevice->StretchRect (BackBuffer, 0, ShaderStruct.lastpassSurface, 0, D3DTEXF_NONE);
				RealDevice->StretchRect (tempShaderSurface, 0, ShaderStruct.lastpassSurface, 0, D3DTEXF_NONE);
			}
			//Now copy the result of the last pass onto back buffer
			RealDevice->StretchRect (ShaderStruct.lastpassSurface, 0, BackBuffer, 0, D3DTEXF_NONE);
			seff->End ();
		}
	}
	if (UsedMWSEShaders) {
		for (i = 0; i < MAXMWSESHADERS; ++i) {
			LoadedShader *shader = &ShaderStruct.MWSEeffects [i];
			if (!shader->Active) continue;
			DWORD sflags = shader->flags;
			if (((sflags  & SFLAG_NOTINTERIOR) && !mwBridge->IsExterior () && !mwBridge->IntLikeExterior ())
				|| ((sflags  & SFLAG_NOTEXTERIOR) && mwBridge->IsExterior ())
				|| ((sflags  & SFLAG_NOTINTEXT) && mwBridge->IntLikeExterior ())
				|| ((sflags  & SFLAG_NOTUNDERWATER) && !mwBridge->IsAboveWater ())
				|| ((sflags  & SFLAG_NOTABOVEWATER) && mwBridge->IsAboveWater ())
				|| ((sflags  & SFLAG_SUNVIS) && (SunVis >= 0.001))
				|| ((sflags  & SFLAG_NOTSUNVIS) && (SunVis < 0.001))
			) continue;
			ID3DXEffect *seff = shader->effect;
			if (!seff) continue;

			RealDevice->StretchRect (BackBuffer, 0, ShaderStruct.lastshaderSurface, 0, D3DTEXF_NONE);

			SetShaderVars (shader, time);
			seff->Begin (&passes, 0);
			for (UINT pass = 0; pass < passes; pass++) {
				seff->BeginPass (pass);
				RealDevice->DrawPrimitive (D3DPT_TRIANGLESTRIP, 0, 2);
				seff->EndPass ();
				//RealDevice->StretchRect (BackBuffer, 0, ShaderStruct.lastpassSurface, 0, D3DTEXF_NONE);
				RealDevice->StretchRect (tempShaderSurface, 0, ShaderStruct.lastpassSurface, 0, D3DTEXF_NONE);
			}
			//Now copy the result of the last pass onto back buffer
			RealDevice->StretchRect (ShaderStruct.lastpassSurface, 0, BackBuffer, 0, D3DTEXF_NONE);
			seff->End ();
		}
	}

	//clean up
	for (int i = 0; i < 6; ++i) {
		RealDevice->SetTexture (i, textures [i]);
		if (textures [i] != NULL) textures [i]->Release ();
	}
	RealDevice->EndScene ();

	RealDevice->SetRenderTarget (0, BackBuffer);
	RealDevice->SetRenderState (D3DRS_CULLMODE, CullMode);
	RealDevice->SetFVF (FVF);
	RealDevice->StretchRect (ShaderStruct.thisframeSurface, 0, ShaderStruct.lastframeSurface, 0, D3DTEXF_NONE);
	if (DepthTexture) RealDevice->SetDepthStencilSurface (DepthBuffer);

	RealDevice->SetRenderState (D3DRS_ALPHATESTENABLE, alphaTest);
	RealDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, alphablend);
	RealDevice->SetRenderState (D3DRS_SRCBLEND, srcblend);
	RealDevice->SetRenderState (D3DRS_DESTBLEND, destblend);
	RealDevice->SetRenderState (D3DRS_ZENABLE, zenable);
	RealDevice->SetRenderState (D3DRS_ZWRITEENABLE, zwriteenable);
	//RealDevice->SetTexture (0, oldTexture);
	//RealDevice->SetTexture (1, oldTexture1);

	RealDevice->SetTransform (D3DTS_VIEW, &old_view);
	RealDevice->SetTransform (D3DTS_PROJECTION, &old_proj);
	RealDevice->SetTransform (D3DTS_WORLD, &old_world);

	RealDevice->SetStreamSource (0, old_stream_data, old_offset, old_stride);
	RealDevice->SetIndices (old_index_data);
}

//=============================================================================

void DrawRecordedDrawCallDepth ( D3DXMATRIX* view, D3DXMATRIX* proj ) {
	D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"Near Depth Buffer");

	//Set the render target to the depth texture
	g_DepthManager.BeginDepthRenderTarget();

	DepthDrawRecorder.RenderRecordedDrawCallDepth(
		RealDevice,
		view,
		proj,
		&g_DepthManager
	);

	//Restore render target
	g_DepthManager.EndDepthRenderTarget();
	RealDevice->SetRenderTarget(0,BackBuffer);
	RealDevice->SetDepthStencilSurface(DepthBuffer);

	D3DPERF_EndEvent(); // Near depth buffer
}
