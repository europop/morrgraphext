#define _D3D8
#include "Configuration.h"
#include "d3d8header.h"
#include "FogFormulas.h"
#include "MWBridge.h"
#include "DistantLand.h"

void MGEOnProcessStart(HANDLE pipe) {
	// Set variables dependent on configuration
	switch (FogMode) {
		case 2:
			FogPixelMode = 0;
			FogVertexMode = 3;
			RangedFog = 1;
			break;
		case 1:
			FogPixelMode = 0;
			FogVertexMode = 3;
			RangedFog = 0;
			break;
		default:
			FogPixelMode = 3;
			FogVertexMode = 0;
			RangedFog = 0;
			break;
	}
	if (!(MGEFlags & REFLECTIVE_WATER)) {
		BitReset(&MGEFlags, REFLECT_NEAR_BIT);
		BitReset(&MGEFlags, REFLECT_FAR_BIT);
	}
	if (!(MGEFlags & REFLECT_NEAR)) {
		BitReset(&MGEFlags, REFLECT_FAR_BIT);
	}
	DL_WaterReflect = (BYTE)BitTst(&MGEFlags, REFLECTIVE_WATER_BIT) + (BYTE)BitTst(&MGEFlags, REFLECT_NEAR_BIT) + (BYTE)BitTst(&MGEFlags, REFLECT_FAR_BIT);
	Rotation *= PI / 180;
	ReactionSpeed = 1.0f / ReactionSpeed;
	DL_DrawDist *= 8192.0f;
	DL_NearStaticEnd *= 8192.0f;
	DL_FarStaticEnd *= 8192.0f;
	DL_VeryFarStaticEnd *= 8192.0f;
	DL_AboveWaterFogStart *= 8192.0f;
	DL_AboveWaterFogEnd *= 8192.0f;
	DL_BelowWaterFogStart *= 8192.0f;
	DL_BelowWaterFogEnd *= 8192.0f;
	DL_InteriorFogStart *= 8192.0f;
	DL_InteriorFogEnd *= 8192.0f;
	for (int fog = 0; fog <= 9; ++fog) DL_FgOD[fog] /= 100.0f;
	//zero all the structs
	//ZeroStruct(Cam3Offset);
	ZeroStruct(JointInfo);
	//ZeroStruct(Options);
	ZeroStruct(fpsString);
	ZeroStruct(ShaderStruct);
	ZeroStruct(RcpRes);
	ZeroStruct(SinVar);
	ZeroStruct(LinVar);
	ZeroStruct(EyeVec);
	ZeroStruct(EyePos);
	ZeroStruct(SunVec);
	ZeroStruct(SunCol);
	ZeroStruct(AmbCol);
	EyeVec.w=1;
	//Set some values for some structs
	fpsRect.bottom=35;
	fpsRect.right=600;
	fpsRect.left=5;
	fpsRect.top=20;
	StatusRect.bottom=50;
	StatusRect.right=635;
	StatusRect.left=5;
	StatusRect.top=5;
	fpsColor.a=255;
	fpsColor.r=255;
	fpsColor.b=255;
	fpsColor.g=255;
	fps=0;
	//Setup any global variables
	UsingDistantLand=false;
	UseDistantStatics=true;
	UseGrass=true;
	RealDevice=NULL;
	BackBuffer=NULL;
	DepthTexture=NULL;
	DepthBuffer=NULL;
	StateBlock=0;
	Font=NULL;
	StatusStringTime=0;
	//Scenecount
	SceneCount=0;
	HUDScene=-1;
	AlphaScene=2;
	SolarScene=-1;
	Indoors=false;
	UpdateScene=false;
	LastSceneCount=0;
	PixelShading=false;
	StencilEnabled=false;
	AlphaBlendEnabled=false;
	AlphaTestEnabled=false;
	SunEnabled=false;
	LightsSet=false;
	ShadowsDrawn=0;
	NumDrawCalls=0;
	WaterDrawn=false;
	FogAlreadySet=false;
	DlDoneHooking=false;
	//fps variables (Could make an fps structure I suppose)
	fpsLastTime=0;
	fpsFrames=0;
	fpsAdjusting=false;
	//These are variables used for graphics features which cant be set by MGEgui
	Zoom=false;
	//FOV=4.0f;
	FogRange=1.0f;
	FogStart=0;
	FogEnd=0;
	//Rotation=(PI*Options.Rotation)/2;
	D3DXMatrixRotationZ(&RotateMatrix,Rotation);
	//MaxViewRange=STDMAXVIEWDISTANCE;
	//MinViewRange=STDMINVIEWDISTANCE;
	//MWSE stuff
	//Lighting
	ZeroStruct(LightIDs);
	ActiveLights=0;
	//STANDARD stuff
	ZeroStruct(Objects);
	UsedMWSEobjects=0;
	ZeroStruct(HUDbits);
	UsedHUDbits=0;
	HUDsprite=NULL;
	ZeroStruct(LoadedTextures);
	LastTimer=0;
	UsedMWSEShaders=0;
	HookNextTexture=false;
	NextTexIsWater=false;
	NextHookIndex=0;
	ZeroStruct(HookedTextures);
	HookedShaderActive=0;
	ActiveHookedShader=0;
	for(int i=0;i<8;i++) SetHookedTextures[i]=255;
	//Camera shake
	CameraShake=false;
	CameraShakeAmp=0;
	CameraShakeChange=false;
	CameraShakeChangeRate=0;
	D3DXMatrixTranslation(&CameraShakeMatrix,0,0,0);
	//fps independent stuff
	FOVZooming=false;
	FOVZoomRate=0;
	FogZooming=false;
	FogZoomRate=0;
	RotSpinning=false;
	RotSpinRate=0;
	//Exterior lighting control
	SunManual=false;
	SunManColour=false;
	SunManDir=false;
	AmbientManual=false;
	FogManual=false;
	ClsManual=false;
	SkyManual=false;
	HUDManual=false;
	ZeroStruct(SunDirection);
	SunColour=D3DXCOLOR(0,0,0,0);
	AmbientColour=0;
	FogColour=0;
	ClsColour=0;
	SkyColour=0;
	HUDColour=0xFFFFFFFF;
	SunTexture=NULL;
	//Texture spamming
	TexRenderTarget=NULL;
	TexCopyTo=NULL;
	SufRenderTarget=NULL;
	SufCopyTo=NULL;
	ZeroStruct(TexDrawOrigin);
	ZeroStruct(TexDrawScale);
	//video stuff
	LoadedVideoCount=0;
	DistantLandActive=false; //phal
        //Now init some stuff
	//MWSE timer
	TimerFrequency=0;
	if(!QueryPerformanceFrequency((LARGE_INTEGER*)&TimerFrequency)) {
		TimerFrequency=0;
		DEBUGSTRING("Your computer has no high resolution performance counter available.\n"
			"Some MGE features or plugins will not function correctly.\n");
	}
	MWSEpipe=pipe;
}

void UpdateFogRange() {
	DECLARE_MWBRIDGE

	if (UsingDistantLand && DL_HasStatics()) {
		//Set fog end based on water height
		if (mwBridge->IsAboveWater()) {
			if (mwBridge->CellHasWeather()) {
				RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(DistantFogStart()));
				RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(DistantFogEnd()));
				RealDevice->SetRenderState (D3DRS_FOGDENSITY, F2DW(1/DistantFogEnd ()));
			} else {
				RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(InteriorFogStart()));
				RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(InteriorFogEnd()));
				RealDevice->SetRenderState (D3DRS_FOGDENSITY, F2DW(1/InteriorFogEnd ()));
			}
		} else {
			RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(UnderwaterFogStart()));
			RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(UnderwaterFogEnd()));
			RealDevice->SetRenderState (D3DRS_FOGDENSITY, F2DW(1/UnderwaterFogEnd ()));
		}
	} else {
		RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(StandardFogStart()));
		RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(StandardFogEnd()));
	}
}
void UpdateFogMethod( bool force_update ) {
	static bool expfog = false;
	
	// check for need of changing fog method
	if(force_update || (expfog != ((MGEFlags & USE_DISTANT_LAND) && DistantLandActive && 
						DL_HasStatics() && (MGEFlags & EXP_FOG) && DL_ShaderModel >= 3))) {
		expfog = ((MGEFlags & USE_DISTANT_LAND) && DistantLandActive && 
					DL_HasStatics() && (MGEFlags & EXP_FOG) && DL_ShaderModel >= 3);
		
		if(expfog) {
			//set exponential fog method
			RealDevice->SetRenderState (D3DRS_FOGTABLEMODE, (D3DTEXTURESTAGESTATETYPE)FogPixelMode/3);
			RealDevice->SetRenderState (D3DRS_FOGVERTEXMODE, (D3DTEXTURESTAGESTATETYPE)FogVertexMode/3);
			//SETSTATUS("exponential!");
		} else {
			//set linear fog method
			RealDevice->SetRenderState (D3DRS_FOGTABLEMODE, (D3DTEXTURESTAGESTATETYPE)FogPixelMode);
			RealDevice->SetRenderState (D3DRS_FOGVERTEXMODE, (D3DTEXTURESTAGESTATETYPE)FogVertexMode);
			//SETSTATUS("linear!");
		}
		RealDevice->SetRenderState(D3DRS_RANGEFOGENABLE,RangedFog);
	}
}
void UpdateRotation() {
    while(Rotation<0) Rotation+=PI*2.0f;
    if(Rotation>PI*2.0f) Rotation=fmod(Rotation,PI*2.0f);
    D3DXMatrixRotationZ(&RotateMatrix,Rotation);
}
void LoadShaderTextures(ID3DXEffect* effect) {
    effect->SetTexture("thisframe",ShaderStruct.thisframe);
    effect->SetTexture("lastframe",ShaderStruct.lastframe);
    effect->SetTexture("lastpass",ShaderStruct.lastpass);
    effect->SetTexture("lastshader",ShaderStruct.lastshader);
	if( g_DepthManager.IsInitialized() && (MGEFlags & SHADER_DEPTH) ) {
		effect->SetTexture( "depthframe", g_DepthManager.GetDepthTexture() );
	}
    effect->SetVector("rcpres",&RcpRes);
	HRESULT hr;

	DWORD count=1;
	IDirect3DTexture* tex;
	char* TexLastPart;
	char TexFirstPart[22]="Data Files\\Textures\\";
	char TexPath[65];
	char FirstPart[9]="texname";
	char FirstPart2[5]="tex";
	char LastPart[17];
	char Finished[25];
	while(true) {
		_itoa_s(count++,LastPart,16,10);
		Finished[0]='\0';
		strcat_s(Finished,24,FirstPart);
		strcat_s(Finished,24,LastPart);
		hr=effect->GetString(Finished,(LPCSTR*)&TexLastPart);
		if(hr==D3D_OK) {
			TexPath[0]='\0';
			strcat_s(TexPath,64,TexFirstPart);
			strcat_s(TexPath,64,TexLastPart);
			hr=D3DXCreateTextureFromFileA(RealDevice,TexPath,&tex);
			if(hr==D3D_OK) {
				Finished[0]='\0';
				strcat_s(Finished,24,FirstPart2);
				strcat_s(Finished,24,LastPart);
				effect->SetTexture(Finished,tex);
			}
		} else break;
	}
}


void SetShaderVars (LoadedShader* shader, DWORD time) {
	DECLARE_MWBRIDGE

	if (MGEFlags & SET_SHADER_VARS) {
		D3DXHANDLE *svar = shader->shadervar;
		ID3DXEffect *seff = shader->effect;
		if (!seff) return;
		if (svar [SVAR_SINVAR]) seff->SetVector (svar [SVAR_SINVAR], &SinVar);
		if (svar [SVAR_LINVAR]) seff->SetVector (svar [SVAR_LINVAR], &LinVar);
		if (svar [SVAR_EYEVEC]) seff->SetVector (svar [SVAR_EYEVEC], &EyeVec);
		if (svar [SVAR_EYEPOS]) seff->SetVector (svar [SVAR_EYEPOS], &EyePos);
		if (svar [SVAR_SUNVEC]) seff->SetVector (svar [SVAR_SUNVEC], &SunVec);
		if (svar [SVAR_SUNCOL]) seff->SetVector (svar [SVAR_SUNCOL], &SunCol);
		if (svar [SVAR_SUNAMB]) seff->SetVector (svar [SVAR_SUNAMB], &SunAmb);
		if (svar [SVAR_SUNPOS]) seff->SetVector (svar [SVAR_SUNPOS], &SunPos);//'real' sun position //phal
		if (svar [SVAR_SUNVIS]) seff->SetFloat (svar [SVAR_SUNVIS], SunVis);//sun alpha //phal
		if (svar [SVAR_TICKCOUNT]) seff->SetInt (svar [SVAR_TICKCOUNT], time);
		if (MGEFlags & USE_HDR) {
			if(svar [SVAR_HDR]) seff->SetVector (svar [SVAR_HDR], &HDR);
		}

		if (svar [SVAR_MVIEW]) seff->SetMatrix (svar [SVAR_MVIEW], &TransView);
		if (svar [SVAR_MPROJ]) seff->SetMatrix (svar [SVAR_MPROJ], &TransProj);

		bool bInterior;
		bool bMenu;
		bool bUnderwater;
		bool bIswater;
		if (mwBridge->IsLoaded ()) {
			bInterior = !mwBridge->IsExterior () && !mwBridge->IntLikeExterior ();
			bMenu = mwBridge->IsMenu () || mwBridge->IsLoading ();
			bUnderwater = !mwBridge->IsAboveWater ();
			bIswater = mwBridge->CellHasWater ();
		} else {
			bInterior = true;
			bMenu = true;
			bUnderwater = false;
			bIswater = false;
		}

		if (svar [SVAR_ISINTERIOR]) seff->SetBool (svar [SVAR_ISINTERIOR], bInterior);
		if (svar [SVAR_ISMENU]) seff->SetBool (svar [SVAR_ISMENU], bMenu);
		if (svar [SVAR_ISUNDERWATER]) seff->SetBool (svar [SVAR_ISUNDERWATER], bUnderwater);
		if (svar [SVAR_HASWATER]) seff->SetBool (svar [SVAR_HASWATER], bIswater);

		//legacy support for older shaders
		if (svar [SVAR_PKINSIDE]) seff->SetBool (svar [SVAR_PKINSIDE], bInterior);
		if (svar [SVAR_PKMENU]) seff->SetBool (svar [SVAR_PKMENU], bMenu);
		if (svar [SVAR_PKUWATER]) seff->SetBool (svar [SVAR_PKUWATER], bUnderwater);

		if (svar [SVAR_WATERLEVEL]) seff->SetFloat (svar [SVAR_WATERLEVEL], bIswater ? mwBridge->WaterLevel () : -1e6f);
		if (svar [SVAR_FOGCOL]) seff->SetFloatArray (svar [SVAR_FOGCOL],(float *)&RGBVECTOR (FogColour), 3);
		if (DistantLandActive && DL_HasStatics ()) {
			if (!bUnderwater) {
				if (mwBridge->CellHasWeather()) {
					if (svar [SVAR_FOGSTART]) seff->SetFloat (svar [SVAR_FOGSTART], DistantFogStart ());
					if (svar [SVAR_FOGRANGE]) seff->SetFloat (svar [SVAR_FOGRANGE], DistantFogEnd ());
				} else {
					if (svar [SVAR_FOGSTART]) seff->SetFloat (svar [SVAR_FOGSTART], InteriorFogStart ());
					if (svar [SVAR_FOGRANGE]) seff->SetFloat (svar [SVAR_FOGRANGE], InteriorFogEnd ());
				}
			} else {
				if (svar [SVAR_FOGSTART]) seff->SetFloat (svar [SVAR_FOGSTART], UnderwaterFogStart ());
				if (svar [SVAR_FOGRANGE]) seff->SetFloat (svar [SVAR_FOGRANGE], UnderwaterFogEnd ());
			}
		} else {
			if(svar[SVAR_FOGSTART]) shader->effect->SetFloat(svar[SVAR_FOGSTART], StandardFogStart());
			if(svar[SVAR_FOGRANGE]) shader->effect->SetFloat(svar[SVAR_FOGRANGE], StandardFogEnd());
		}

		if(svar [SVAR_FOV]) seff->SetFloat (svar [SVAR_FOV], ScreenFOV);
		if(svar [SVAR_PLAYERPOS]) seff->SetFloatArray (svar [SVAR_PLAYERPOS], (float *)&PlayerPos, 3);
		// end
		
	}
}


void InitShaderFlags(LoadedShader* shader) {
	if (!shader) return;

	shader->flags = 0;

	D3DXHANDLE *svar = shader->shadervar;
	ID3DXEffect *seff = shader->effect;

	if (!seff) return;

	// Determine used variables
	svar [SVAR_SINVAR]		 = seff->GetParameterByName (0, "sinvar");
	svar [SVAR_LINVAR]		 = seff->GetParameterByName (0, "linvar");
	svar [SVAR_EYEVEC]		 = seff->GetParameterByName (0, "eyevec");
	svar [SVAR_EYEPOS]		 = seff->GetParameterByName (0, "eyepos");
	svar [SVAR_SUNVEC]		 = seff->GetParameterByName (0, "sunvec");
	svar [SVAR_SUNCOL]		 = seff->GetParameterByName (0, "suncol");
	svar [SVAR_SUNAMB]		 = seff->GetParameterByName (0, "sunamb");
	svar [SVAR_SUNPOS]		 = seff->GetParameterByName (0, "sunpos");
	svar [SVAR_SUNVIS]		 = seff->GetParameterByName (0, "sunvis");
	svar [SVAR_TICKCOUNT]	 = seff->GetParameterByName (0, "tickcount");
	svar [SVAR_HDR]			 = seff->GetParameterByName (0, "HDR");
	svar [SVAR_MVIEW]		 = seff->GetParameterByName (0, "mview");
	svar [SVAR_MPROJ]		 = seff->GetParameterByName (0, "mproj");
	svar [SVAR_ISINTERIOR]	 = seff->GetParameterByName (0, "isinterior");
	svar [SVAR_ISMENU]		 = seff->GetParameterByName (0, "ismenu");
	svar [SVAR_ISUNDERWATER] = seff->GetParameterByName (0, "isunderwater");
	svar [SVAR_HASWATER]	 = seff->GetParameterByName (0, "haswater");
	svar [SVAR_PKINSIDE]	 = seff->GetParameterByName (0, "pk_inside");
	svar [SVAR_PKMENU]		 = seff->GetParameterByName (0, "pk_menu");
	svar [SVAR_PKUWATER]	 = seff->GetParameterByName (0, "pk_uwater");
	svar [SVAR_WATERLEVEL]	 = seff->GetParameterByName (0, "waterlevel");
	svar [SVAR_FOGCOL]		 = seff->GetParameterByName (0, "fogcol");
	svar [SVAR_FOGSTART]	 = seff->GetParameterByName (0, "fogstart");
	svar [SVAR_FOGRANGE]	 = seff->GetParameterByName (0, "fogrange");
	svar [SVAR_FOV]			 = seff->GetParameterByName (0, "fov");
	svar [SVAR_PLAYERPOS]	 = seff->GetParameterByName (0, "playerpos");

	D3DXHANDLE flags_handle;
	if (flags_handle = seff->GetParameterByName (0, "mgeflags")) {
		seff->GetInt (flags_handle, (int *)&shader->flags);		// Isn't something missing here? /krzymark/
	} else {
		shader->flags = 0;
	}
}

void UpdateLighting(int index) {
    D3DLIGHT8 TempLight;
    BYTE upto=0;
    for(DWORD i=0;i<ActiveLights;i++) {
        RealDevice->GetLight(LightIDs[i],&TempLight);
        if(TempLight.Type==D3DLIGHT_DIRECTIONAL) {
            Objects[index].lights[upto++]=LightIDs[i];
        } else {
            D3DXVECTOR3 vec;
            float f;
            f=abs(D3DXVec3Length(D3DXVec3Subtract(&vec,&Objects[index].pos,(D3DXVECTOR3*)&TempLight.Position)));
            if(f<TempLight.Range) Objects[index].lights[upto++]=LightIDs[i];
        }
        if(upto==8) break;
    }
    Objects[index].NumLights=upto;
}
void UpdateScreenShake() {
    if(CameraShakeAmp<0) return;
    float f=GetTickCount()/1000.0f;
    f=sin(f);
    D3DXMatrixTranslation(&CameraShakeMatrix,f*CameraShakeAmp,0,0);
}
void UpdateSceneNums() {
    DECLARE_MWBRIDGE

    //Indoors=!IsExterior();
    Indoors = !mwBridge->IsExterior() && !mwBridge->IntLikeExterior(); //d4w
    HUDScene=SceneCount-1;
    if(Indoors) { SolarScene=-1; } else { SolarScene=SceneCount-2; }
    AlphaScene = 2;
	//AlphaScene=SceneCount-2;
    if(!Indoors) {
		
        //AlphaScene--;
        //if(PixelShading) AlphaScene--;
    }
}

