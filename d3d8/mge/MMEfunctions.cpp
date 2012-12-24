#define _MMEfunctions
#include "Configuration.h"
#include "d3d8header.h"
#include <sys/stat.h>
#include "MWBridge.h"
#include "DistantLand.h"

int DebugTesterThing=0;
void TakeScreenshot() {
    if(RealDevice==NULL) return;
    HRESULT hr;
    IDirect3DSurface* surface;
	RECT r;
	RECT* pRect=0;
//#ifndef RELEASE
//	hr=RealDevice->CreateOffscreenPlainSurface(1280,1024,
//        D3DFMT_A8R8G8B8,D3DPOOL_SCRATCH,&surface,NULL);
//#else
	DWORD width, height;
	if(JointInfo.WNDwindowed) {
		D3DDISPLAYMODE mode;
		RealDevice->GetDisplayMode(0, &mode);
		hr=RealDevice->CreateOffscreenPlainSurface(mode.Width,mode.Height,D3DFMT_A8R8G8B8,D3DPOOL_SCRATCH,&surface,NULL);
		HWND win=FindWindow("morrowind", "morrowind");
		//GetWindowRect(win, &r);
		POINT p; p.x = 0; p.y = 0;
		ClientToScreen(win, &p);
		GetClientRect(win, &r);
		r.left += p.x;
		r.top += p.y;
		r.right += p.x;
		r.bottom += p.y;
		if(r.left < 0) r.left = 0;
		if(r.top < 0) r.top = 0;
		if((UINT)r.right >= mode.Width) r.right = (LONG)mode.Width - 1;
		if((UINT)r.bottom >= mode.Height) r.bottom = (LONG)mode.Height - 1;
		pRect=&r;
		width=mode.Width;
		height=mode.Height;
	} else {
		width=JointInfo.GraphicsWidth;
		height=JointInfo.GraphicsHeight;
		hr=RealDevice->CreateOffscreenPlainSurface(JointInfo.GraphicsWidth,JointInfo.GraphicsHeight, D3DFMT_A8R8G8B8,D3DPOOL_SCRATCH,&surface,NULL);
	}
//#endif
	if(hr!=D3D_OK) return;
	hr=RealDevice->GetFrontBufferData(0,surface);
	if(hr==D3D_OK) {
		D3DLOCKED_RECT rect2;
		hr=surface->LockRect(&rect2, 0, 0);
		if(hr==D3D_OK&&!(rect2.Pitch%4)) {
			DWORD *ptr=(DWORD*)rect2.pBits;
			rect2.Pitch/=4;
			for(DWORD y=0;y<height;y++) {
				DWORD offset=y*rect2.Pitch;
				for(DWORD x=0;x<width;x++) {
					ptr[offset+x]|=0xff000000;
				}
			}

			surface->UnlockRect();
		}
		char FileNameEnds[40]=".bmp\0\0\0\0.jpg\0\0\0\0.dds\0\0\0\0.png\0\0\0\0.tga\0\0\0";
		char FileNameEnd[8];
		char FileNameStart[240];
		char FileName[256];
		char Number[6];
		FileNameStart[0] = '\0';
		struct _stat64i32 unused;
		if (strlen(SSDir) > 0) {
			bool usedir = true;
			if (_stat64i32(SSDir, &unused) == -1 && !CreateDirectory(SSDir, NULL)) usedir = false;
			if (usedir) {
				strcat_s(FileNameStart, 240, SSDir);
				strcat_s(FileNameStart, 240, "\\");
			}
		}
		strcat_s(FileNameStart, 240, SSName);
		for (int i = 0; i < 8; ++i) FileNameEnd[i] = FileNameEnds[i + SSFormat * 8];
		for (unsigned long ui = 1; ui <= 99999; ++ui) {
			FileName[0] = '\0';
			strcat_s(FileName, 256, FileNameStart);
			char* FNamEnd = FileName + strlen(FileName);
			_ultoa_s(ui, Number, 6, 10);
			for (int i = strlen(Number); i < SSMinNumChars; ++i) *(FNamEnd++) = '0';
			*FNamEnd = '\0';
			strcat_s(FileName, 256, Number);
			strcat_s(FileName, FileNameEnd);
			if (_stat64i32(FileName, &unused) == -1) {
				switch (SSFormat) {
					case 0:
						hr = D3DXSaveSurfaceToFile(FileName, D3DXIFF_BMP, surface, NULL, pRect);
						break;
					case 1:
						hr = D3DXSaveSurfaceToFile(FileName, D3DXIFF_JPG, surface, NULL, pRect);
						break;
					case 2:
						hr = D3DXSaveSurfaceToFile(FileName, D3DXIFF_DDS, surface, NULL, pRect);
						break;
					case 3:
						hr = D3DXSaveSurfaceToFile(FileName, D3DXIFF_PNG, surface, NULL, pRect);
						break;
					case 4:
						hr = D3DXSaveSurfaceToFile(FileName, D3DXIFF_TGA, surface, NULL, pRect);
						break;
					default:
						hr = -1;                    
				}
			break;
			}
		}
	}
	if(hr == D3D_OK) {
		SETSTATUS("Screen shot saved");
	} else {
		SETSTATUS("Screen shot failed");
	}
	surface->Release();
}

void ToggleFog() {
    if(RealDevice==NULL) return;
    RealDevice->SetRenderState(D3DRS_FOGENABLE, !BitCompliment(&MGEFlags, FOG_ENABLED_BIT));
    if (MGEFlags & FOG_ENABLED) {
        SETSTATUS("Fogging enabled");
    } else {
        SETSTATUS("Fogging disabled");
    }

}
void CycleScaleFilter() {
    if(RealDevice==NULL) return;
    if (++ScaleFilter == 6) ScaleFilter = 1;
    //RealDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,Options.MagMinFilter);
    RealDevice->SetSamplerState(0, D3DSAMP_MINFILTER, ScaleFilter);
    switch(ScaleFilter) {
        case 1:SETSTATUS("Using point mag and min filters");break;
        case 2:SETSTATUS("Using linear mag and min filters");break;
        case 3:SETSTATUS("Using anisotropic mag and min filters");break;
        case 4:SETSTATUS("Using flat cubic mag and min filters");break;
        case 5:SETSTATUS("Using gaussian cubic mag and min filters");break;
    }
}
void CycleMipFilter() {
    if(RealDevice==NULL) return;
    if(++MipFilter == 6) MipFilter = 1;
    switch(MipFilter) {
        case 1:SETSTATUS("Using a point mip filter");break;
        case 2:SETSTATUS("Using a linear mip filter");break;
        case 3:SETSTATUS("Using a anisotropic mip filter");break;
        case 4:SETSTATUS("Using a flat cubic mip filter");break;
        case 5:SETSTATUS("Using a gaussian cubic mip filter");break;
    }
}

void CycleFogMode() {
    if(RealDevice==NULL) return;
    if(++FogMode == 3) FogMode = 0;
    switch(FogMode) {
        case 0:
			FogPixelMode = 3;
            FogVertexMode = 0;
            RangedFog = 0;
            /*RealDevice->SetRenderState(D3DRS_FOGVERTEXMODE,0);
            RealDevice->SetRenderState(D3DRS_FOGTABLEMODE,3);
            RealDevice->SetRenderState(D3DRS_RANGEFOGENABLE,0);*/
            SETSTATUS("Using depth based per pixel fog");
            break;
        case 1:
            FogPixelMode = 0;
            FogVertexMode = 3;
            RangedFog = 0;
            /*RealDevice->SetRenderState(D3DRS_FOGVERTEXMODE,3);
            RealDevice->SetRenderState(D3DRS_FOGTABLEMODE,0);
            RealDevice->SetRenderState(D3DRS_RANGEFOGENABLE,0);*/
            SETSTATUS("Using depth based per vertex fog");
            break;
        case 2:
            FogPixelMode = 0;
            FogVertexMode = 3;
            RangedFog = 1;
            /*RealDevice->SetRenderState(D3DRS_FOGVERTEXMODE,3);
            RealDevice->SetRenderState(D3DRS_FOGTABLEMODE,0);
            RealDevice->SetRenderState(D3DRS_RANGEFOGENABLE,1);*/
            SETSTATUS("Using range based per vertex fog");
            break;
    }
	// change fogging accordingly
	UpdateFogMethod( true );
}
void ToggleAntialias() {
    if(RealDevice==NULL) return;
    RealDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, (AALevel = !AALevel)); // ? A BUG ?
    if (AALevel) {
        SETSTATUS("Fullscreen antialiasing enabled");
    } else {
        SETSTATUS("Fullscreen antialiasing disabled.");
    }
}
void CycleAnisoLevel() {
    if(RealDevice==NULL) return;
    if ((AnisoLevel *= 2) == 32) AnisoLevel = 1;
    RealDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, AnisoLevel);
    switch (AnisoLevel) {
        case 1:SETSTATUS("Anisotropic filtering off");break;
        case 2:SETSTATUS("Using 2x anisotropic filtering");break;
        case 4:SETSTATUS("Using 4x anisotropic filtering");break;
        case 8:SETSTATUS("Using 8x anisotropic filtering");break;
        case 16:SETSTATUS("Using 16x anisotropic filtering");break;
    }
}
void ToggleZoom() {
    if(RealDevice==NULL) return;
    Zoom=!Zoom;
    if(Zoom) {
        SETSTATUS("Zoom enabled");
    } else {
        SETSTATUS("Zoom disabled");
    }
}
void IncreaseZoom() {
    SatUp(&FOV,MAXZOOM,ZOOMSTEP);
}
void DecreaseZoom() {
    SatDown(&FOV,MINZOOM,ZOOMSTEP);
}
void RotateScreen() {
    if(RealDevice==NULL) return;
    Rotation+=PI/2.0f;
    UpdateRotation();
}
void ToggleStatusText() {
    BitCompliment(&MGEFlags, DISPLAY_MESSAGES_BIT);
}
void ToggleFpsCounter() {
    BitCompliment(&MGEFlags, FPS_COUNTER_BIT);
}
void ShowLastMessage() {
    StatusStringTime = GetTickCount() + MsgTime;
}
void ToggleHardwareShader() {
    if(RealDevice==NULL) return;
    if(ShaderStruct.DefaultEffects==0) {
        SETSTATUS("Cannot enable hardware shaders because there are none active");
        return;
    }
    if (!BitCompliment(&MGEFlags, USE_HW_SHADER_BIT)) {
        SETSTATUS("Hardware shader on");
    } else {
        SETSTATUS("Hardware shader off");
    }
}
void IncreaseFog() {
    if(RealDevice==NULL) return;
    SatDown(&FogRange,MINFOG,FOGSTEP);
    UpdateFogRange();
}
void DecreaseFog() {
    if(RealDevice==NULL) return;
    SatUp(&FogRange,MAXFOG,FOGSTEP);
    UpdateFogRange();
}
void ToggleAAFix() {
    if(RealDevice==NULL) return;
    if (!BitCompliment(&MGEFlags, AA_COLOUR_FIX_BIT)) {
        SETSTATUS("AA colour fix on");
    } else {
        SETSTATUS("AA colour fix off");
    }
}
void IncreaseView() {
	DECLARE_MWBRIDGE
    float f = mwBridge->GetViewDistance();
    SatUp(&f,MAXVIEWDISTANCE,50);
	mwBridge->SetViewDistance(f);

    if (MGEFlags & BIND_AI_TO_VIEW) {
        float f2=f/STDMAXVIEWDISTANCE;
		mwBridge->SetAIDistance(f2);
    }
    if ((MGEFlags & DISPLAY_MESSAGES) && RealDevice != NULL) {
        char str[11];
        char str2[27]; str2[0]='\0';
        strcat(str2,"View distance: ");
        _gcvt_s(str,10,f,8);
        strcat_s(str2,26,str);
        SETSTATUS(str2);
    }
}
void DecreaseView() {
	DECLARE_MWBRIDGE
    float f = mwBridge->GetViewDistance();
    SatDown(&f,MINVIEWDISTANCE,50);
	mwBridge->SetViewDistance(f);

    if (MGEFlags & BIND_AI_TO_VIEW) {
        float f2=f/STDMAXVIEWDISTANCE;
		mwBridge->SetAIDistance(f2);
    }
    if ((MGEFlags & DISPLAY_MESSAGES) && RealDevice != NULL) {
        char str[11];
        char str2[27]; str2[0]='\0';
        strcat(str2,"View distance: ");
        _gcvt_s(str,10,f,8);
        strcat_s(str2,26,str);
        SETSTATUS(str2);
    }
}
void IncreaseAI() {
	DECLARE_MWBRIDGE
    float f = mwBridge->GetAIDistance();
    SatUp(&f,1,0.015625f);
	mwBridge->SetAIDistance(f);
    if ((MGEFlags & DISPLAY_MESSAGES) && RealDevice != NULL) {
        char str[15];
        char str2[31]; str2[0]='\0';
        strcat(str2,"AI distance: ");
        _gcvt(f,8,str);
        strcat_s(str2,30,str);
        SETSTATUS(str2);
    }
}
void DecreaseAI() {
    DECLARE_MWBRIDGE
    float f = mwBridge->GetAIDistance();
    SatDown(&f,0,0.015625f);
	mwBridge->SetAIDistance(f);
    if ((MGEFlags & DISPLAY_MESSAGES) && RealDevice != NULL) {
        char str[15];
        char str2[31]; str2[0]='\0';
        strcat(str2,"AI distance: ");
        _gcvt(f,8,str);
        strcat_s(str2,30,str);
        SETSTATUS(str2);
    }
}
void IncreaseLOD() {
    if(RealDevice==NULL) return;
    if (LODBias > -12) LODBias--;
    RealDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *(DWORD*)&LODBias);
    if (MGEFlags & DISPLAY_MESSAGES) {
        char str[5];
        char str2[21]; str2[0]='\0';
        strcat(str2,"LOD bias: ");
        _itoa((int)LODBias, str, 10);
        strcat_s(str2,20,str);
        SETSTATUS(str2);
    }
}
void DecreaseLOD() {
    if(RealDevice==NULL) return;
    if (LODBias < 12) LODBias++;
    RealDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *(DWORD*)&LODBias);
    if (MGEFlags & DISPLAY_MESSAGES) {
        char str[5];
        char str2[21]; str2[0]='\0';
        strcat(str2,"LOD bias: ");
        _itoa((int)LODBias, str, 10);
        strcat_s(str2,20,str);
        SETSTATUS(str2);
    }
}
void ToggleCrosshair() {
	DECLARE_MWBRIDGE
	mwBridge->ToggleCrosshair();
}
void NextTrack() {
    DECLARE_MWBRIDGE
	mwBridge->SkipToNextTrack();
}
void DisableMusic() {
    DECLARE_MWBRIDGE
	mwBridge->DisableMusic();
}

void HaggleMore1() {
	DECLARE_MWBRIDGE
	mwBridge->HaggleMore(1);
}
void HaggleMore10() {
	DECLARE_MWBRIDGE
	mwBridge->HaggleMore(10);
}
void HaggleMore100() {
	DECLARE_MWBRIDGE
	mwBridge->HaggleMore(100);
}
void HaggleMore1000() {
	DECLARE_MWBRIDGE
	mwBridge->HaggleMore(1000);
}
void HaggleMore10000() {
	DECLARE_MWBRIDGE
	mwBridge->HaggleMore(10000);
}
void HaggleLess1() {
	DECLARE_MWBRIDGE
	mwBridge->HaggleLess(1);
}
void HaggleLess10() {
	DECLARE_MWBRIDGE
	mwBridge->HaggleLess(10);
}
void HaggleLess100() {
	DECLARE_MWBRIDGE
	mwBridge->HaggleLess(100);
}
void HaggleLess1000() {
   	DECLARE_MWBRIDGE
	mwBridge->HaggleLess(1000);
}
void HaggleLess10000() {
	DECLARE_MWBRIDGE
	mwBridge->HaggleLess(10000);
}
void ToggleDistantLand() {
	if (!(MGEFlags & USE_DISTANT_LAND)) {
		SETSTATUS("Distant land is not enabled in the GUI");
		return;
	}
	UsingDistantLand = !UsingDistantLand;
	if (!UsingDistantLand) {
		DL_Deactivate();
		if (MGEFlags & DYNAMIC_RIPPLES) {
			DECLARE_MWBRIDGE
			mwBridge->TurnOldRipples (true);
		}
		SETSTATUS("Distant land disabled");
	} else {
		DL_Activate();
		if (MGEFlags & DYNAMIC_RIPPLES) {
			DECLARE_MWBRIDGE
			mwBridge->TurnOldRipples (false);
		}
		SETSTATUS("Distant land enabled");
	}
}
void ToggleDistantStatics() {
	if (!(MGEFlags & USE_DISTANT_STATICS)) {
		SETSTATUS("Distant statics are not enabled in the GUI");
		return;
	}
	UseDistantStatics = !UseDistantStatics;
	if (UseDistantStatics) {
		SETSTATUS("Distant statics enabled");
	} else {
		SETSTATUS("Distant statics disabled");
	}
}
void ToggleGrass() {
	if (!(MGEFlags & USE_DISTANT_STATICS)) {
		SETSTATUS("Distant statics are not enabled in the GUI");
		return;
	}
	UseGrass = !UseGrass;
	if (UseGrass) {
		SETSTATUS("Grass enabled");
	} else {
		SETSTATUS("Grass disabled");
	}
}
void ToggleMwMgeBlending() {
	if (!(MGEFlags & USE_DISTANT_LAND)) {
		SETSTATUS("Distant land is not enabled in the GUI");
		return;
	}
	if (BitCompliment(&MGEFlags, NO_MW_MGE_BLEND_BIT)) {
		SETSTATUS("Morrowind / MGE blending enabled");
	} else {
		SETSTATUS("Morrowind / MGE blending disabled");
	}
}

void CycleWater(){
	if (!(MGEFlags & USE_DISTANT_LAND)) {
		SETSTATUS("Distant land is not enabled in the GUI");
		return;
	}
	if ((++DL_WaterReflect > 1 && !(MGEFlags & USE_DISTANT_STATICS)) || DL_WaterReflect > 3) DL_WaterReflect = 0;
    switch (DL_WaterReflect) {
        case 0: SETSTATUS("Water is non reflective"); break;
        case 1: SETSTATUS("Water reflects landscape"); break;
        case 2: SETSTATUS("Water reflects near statics"); break;
        case 3: SETSTATUS("Water reflects far statics"); break;
    }
}

void ToggleSkyReflection(){
	if (!(MGEFlags & USE_DISTANT_LAND)) {
		SETSTATUS("Distant land is not enabled in the GUI");
		return;
	}
	MGEFlags ^= REFLECT_SKY;
	if(MGEFlags & REFLECT_SKY) {
		SETSTATUS("Water reflects the sky");
	} else {
		SETSTATUS("Water does not reflect the sky");
	}
}
void IncreaseFOV() {
    SatUp(&ScreenFOV,150,1);
    if ((MGEFlags & DISPLAY_MESSAGES) && RealDevice != NULL) {
        char str[11];
        char str2[27]; str2[0]='\0';
        strcat(str2,"Field of view: ");
        _gcvt_s(str,10,ScreenFOV,8);
        strcat_s(str2,26,str);
        SETSTATUS(str2);
    }
}
void DecreaseFOV() {
    SatDown(&ScreenFOV,5,1);
    if ((MGEFlags & DISPLAY_MESSAGES) && RealDevice != NULL) {
        char str[11];
        char str2[27]; str2[0]='\0';
        strcat(str2,"Field of view: ");
        _gcvt_s(str,10,ScreenFOV,8);
        strcat_s(str2,26,str);
        SETSTATUS(str2);
    }
}

void Make3PCamForced () {
	DECLARE_MWBRIDGE
	D3DXVECTOR3 *mwCam3Offset = mwBridge->PCam3Offset ();
	if (mwCam3Offset) {
		Cam3Offset.x = mwCam3Offset->x;
		Cam3Offset.y = mwCam3Offset->y;
		Cam3Offset.z = mwCam3Offset->z;
		ForceCam3Offset = true;
	}
}

void Display3PCamPos (DWORD coord) {
	if (RealDevice != NULL) {
		char str [256];
		float value;
		char ch;
		switch (coord) {
			case 1: value = Cam3Offset.x; ch = 'X'; break;
			case 2: value = Cam3Offset.y; ch = 'Y'; break;
			case 3: value = Cam3Offset.z; ch = 'Z'; break;
			default: value = 0; ch = '?';
		}
		sprintf_s (str, sizeof (str), "Camera %c offset: %.1f", ch, value);
		SETSTATUS(str);
    }
}

void MoveForward3PC () {
	DECLARE_MWBRIDGE
	if (mwBridge->IsMenu () || !(mwBridge->Is3rdPerson () && !mwBridge->IsLookingAround ())) return;
	if (!ForceCam3Offset) Make3PCamForced ();
	SatUp (&Cam3Offset.y, -25, fabs (Cam3Offset.y / 25.0f));
	if (MGEFlags & DISPLAY_MESSAGES) Display3PCamPos (2);
}

void MoveBack3PC () {
	DECLARE_MWBRIDGE
	if (mwBridge->IsMenu () || !(mwBridge->Is3rdPerson () && !mwBridge->IsLookingAround ())) return;
	if (!ForceCam3Offset) Make3PCamForced ();
	SatDown (&Cam3Offset.y, -2500, fabs (Cam3Offset.y / 25.0f));
	if (MGEFlags & DISPLAY_MESSAGES) Display3PCamPos (2);
}

void MoveLeft3PC () {
	DECLARE_MWBRIDGE
	if (mwBridge->IsMenu () || !(mwBridge->Is3rdPerson () && !mwBridge->IsLookingAround ())) return;
	if (!ForceCam3Offset) Make3PCamForced ();
	SatDown (&Cam3Offset.x, -125, 1);
	if (MGEFlags & DISPLAY_MESSAGES) Display3PCamPos (1);
}

void MoveRight3PC () {
	DECLARE_MWBRIDGE
	if (mwBridge->IsMenu () || !(mwBridge->Is3rdPerson () && !mwBridge->IsLookingAround ())) return;
	if (!ForceCam3Offset) Make3PCamForced ();
	SatUp (&Cam3Offset.x, 125, 1);
	if (MGEFlags & DISPLAY_MESSAGES) Display3PCamPos (1);
}

void MoveDown3PC () {
	DECLARE_MWBRIDGE
	if (mwBridge->IsMenu () || !(mwBridge->Is3rdPerson () && !mwBridge->IsLookingAround ())) return;
	if (!ForceCam3Offset) Make3PCamForced ();
	SatDown (&Cam3Offset.z, -125, 1);
	if (MGEFlags & DISPLAY_MESSAGES) Display3PCamPos (3);
}

void MoveUp3PC () {
	DECLARE_MWBRIDGE
	if (mwBridge->IsMenu () || !(mwBridge->Is3rdPerson () && !mwBridge->IsLookingAround ())) return;
	if (!ForceCam3Offset) Make3PCamForced ();
	SatUp (&Cam3Offset.z, 125, 1);
	if (MGEFlags & DISPLAY_MESSAGES) Display3PCamPos (3);
}
