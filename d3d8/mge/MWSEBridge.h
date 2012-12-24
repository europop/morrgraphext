#ifndef _MWSE_BRIDGE_H_
#define _MWSE_BRIDGE_H_

#include "FakeCommon.h"
//#include "d3d8header.inl"


#define MAXMWSEOBJECTS 64
#define MAXMWSESHADERS 32
#define MAXMWSESUBSETS 8
#define MAXFILEPATHLENGTH 64
#define MAXACTIVELIGHTS 1024
#define MAXHOOKEDTEXTURES 64

struct MWSEobject {
    //Administutive
    bool Created;
    bool Enabled;
    bool IsMesh;
    BYTE LightingMode; //0 - none, 1 - single directional, 2 - morrowind manual, 3 - morrowind dynamic
    //bool LightUsed[128];
    //D3DXVECTOR4 LightPos;
    char Tag[33];
    //Mesh data
    ID3DXMesh* mesh;
    DWORD subsets;
    //Manual data
    IDirect3DVertexBuffer* vBuffer;
    IDirect3DIndexBuffer* iBuffer;
    DWORD FVF;
    DWORD stride;
    DWORD vertices;
    DWORD faces;
    //Both
    D3DMATERIAL8 materials[MAXMWSESUBSETS];
    IDirect3DTexture* textures[MAXMWSESUBSETS];
    D3DXVECTOR3 pos;
    D3DXVECTOR3 rot;
    D3DXVECTOR3 scale;
    D3DXVECTOR3 speed;
    D3DXVECTOR3 AngSpeed;
    D3DXVECTOR3 growth;
    DWORD SpeedFlags;   //1 - speed, 2 - angspeed, 4 - growth
    D3DXMATRIX PosMatrix;
    D3DXMATRIX RotMatrix;
    D3DXMATRIX ScaleMatrix;
    D3DXMATRIX WorldMatrix;
    ID3DXEffect* effect;
    //Lighting
    DWORD lights[8];
    DWORD NumLights;
};

struct MWSE_HUDobject {
    char Tag[33];
    bool Enabled;
    D3DXVECTOR2 position;
    D3DXVECTOR2 scale;
    IDirect3DTexture* texture;
    ID3DXEffect* effect;
};

struct sDSTexture {
    IUnknown *pGraph;
    IUnknown *pBuild;
    IUnknown *pVmr;
    IUnknown *pConfig;
    IUnknown *pAlloc;
    IUnknown *pControl;
    IUnknown *pMyAlloc;
    IUnknown *pSeek;
};

//Shader variables
#define SVAR_SINVAR			0
#define SVAR_LINVAR			1
#define SVAR_EYEVEC			2
#define SVAR_EYEPOS			3
#define SVAR_SUNVEC			4
#define SVAR_SUNCOL			5
#define SVAR_SUNAMB			6
#define SVAR_SUNPOS			7
#define SVAR_SUNVIS			8
#define SVAR_TICKCOUNT		9
#define SVAR_HDR			10
#define SVAR_MVIEW			11
#define SVAR_MPROJ			12
#define SVAR_ISINTERIOR		13
#define SVAR_ISMENU			14
#define SVAR_ISUNDERWATER	15
#define SVAR_HASWATER		16
#define SVAR_PKINSIDE		17
#define SVAR_PKMENU			18
#define SVAR_PKUWATER		19
#define SVAR_WATERLEVEL		20
#define SVAR_FOGCOL			21
#define SVAR_FOGSTART		22
#define SVAR_FOGRANGE		23
#define SVAR_FOV			24
#define MAXSVARS			25
//Shader flags
#define SFLAG_NOTINTERIOR		1
#define SFLAG_NOTEXTERIOR		2
#define SFLAG_NOTINTEXT			4
#define SFLAG_NOTUNDERWATER		8
#define SFLAG_NOTABOVEWATER		16
#define SFLAG_SUNVIS			32
#define SFLAG_NOTSUNVIS			64

struct LoadedShader {
    char FilePath[MAXFILEPATHLENGTH+1];
    ID3DXEffect* effect;
    bool Active;
	DWORD flags;
	D3DXHANDLE shadervar[MAXSVARS];
};
struct LoadedTexture {
    char FilePath[MAXFILEPATHLENGTH+1];
    IDirect3DTexture* texture;
    sDSTexture* dsInfo;
};

struct sHookedTexture {
    IUnknown* tHook;
	void* avi;
	IDirect3DTexture9* tex;
    bool AutoClean;
    BYTE TextureIndex;
    BYTE ShaderIndex;
    char path[65];
};

typedef void (_stdcall *SoftwareShaderProc)(int,BYTE*);

struct sShaderStruct {
    HMODULE dll;
    IDirect3D* object;
    //ID3DXEffect* DefaultEffects[32];
    BYTE DefEffectCount;
    IDirect3DVertexBuffer* buffer;
    IDirect3DTexture* thisframe;
    IDirect3DSurface* thisframeSurface;
    IDirect3DTexture* lastframe;
    IDirect3DSurface* lastframeSurface;
    IDirect3DTexture* lastpass;
    IDirect3DSurface* lastpassSurface;
    IDirect3DTexture* lastshader;
    IDirect3DSurface* lastshaderSurface;
    bool released;
    bool SoftwareShaderLoaded;
    IDirect3DSurface* SoftwareSurface;
    SoftwareShaderProc SoftwareProc;
    LoadedShader MWSEeffects[MAXMWSESHADERS];
	LoadedShader DefaultEffects[32];
};

sShaderStruct ShaderStruct;

HANDLE MWSEpipe;
MWSEobject Objects[MAXMWSEOBJECTS];
DWORD UsedMWSEobjects;
MWSE_HUDobject HUDbits[MAXMWSEOBJECTS];
DWORD UsedHUDbits;
ID3DXSprite* HUDsprite;
LoadedTexture LoadedTextures[MAXMWSESHADERS];
DWORD UsedMWSEShaders;
bool HookNextTexture;
bool NextTexIsWater;
bool DlDoneHooking;
BYTE NextHookIndex;
sHookedTexture HookedTextures[MAXHOOKEDTEXTURES];
UINT HookedShaderActive;
BYTE ActiveHookedShader;
BYTE SetHookedTextures[8];

//MWSE
int MWSE_HUDIndex(char* p,char* file);
int MWSE_ObjectIndex(char* p,char* file);
void MWSE_UpdateObjectMatrix(int index,BYTE type);
void MWSE_FreeModel(int index);
int MWSE_ShaderIndex(char* p,bool LoadIfNeeded);
int MWSE_TextureIndex(char* p,bool LoadIfNeeded);
bool MWSE_IsSafeString(char* str);
void MWSE_GetString(char* buffer,DWORD* pPointer);
void MWSE_GetStringC(char* buffer,DWORD* pPointer);
void MWSE_SetStringC(DWORD* pPointer,char* buffer);
void MWSE_Update();
void MWSE_FreeModel(int index);
void MWSE_FreeTexture(int index);
void MWSE_FreeEffect(int index);
void MWSE_FreeHUD(int index);
//MME MWSE
void MWSE_TapKey(WORD keycode);
void MWSE_HoldKey(WORD keycode);
void MWSE_UnholdKey(WORD keycode);
void MWSE_HammerKey(WORD keycode);
void MWSE_UnhammerKey(WORD keycode);
void MWSE_AHammerKey(WORD keycode);
void MWSE_AUnhammerKey(WORD keycode);
void MWSE_DisallowKey(WORD keycode);
void MWSE_AllowKey(WORD keycode);
void MWSE_MoveMouseX(WORD pixels);
void MWSE_MoveMouseY(WORD pixels);
void MWSE_MouseSpeedX(float speed);
void MWSE_MouseSpeedY(float speed);
void MWSE_DisableMouse();
void MWSE_EnableMouse();
void MWSE_BufferedDown(WORD keycode);
void MWSE_BufferedUp(WORD keycode);
//MWSE debugging
//VideoPlayback
sDSTexture* FreeMovie(sDSTexture* info);
sDSTexture* CreateDSGraph(char* path,IDirect3DTexture9** tex);
void ResumeMovie(sDSTexture* movie);
void PauseMovie(sDSTexture* movie);
void StopMovie(sDSTexture* movie);
void RewindMovie(sDSTexture* movie);
void SeekMovie(sDSTexture* movie,DWORD shortTime);

//Morrowind memory layout
enum eMW_ADDR {
  eMaster, eMaster1, eMaster2,
  eFPS, eTimer, eD3D, eTruRenderWidth, eShadowSlider,
  eCrosshair1, eAI, eView0, eRenderWidth,
  eView1, eCombat,

  eGamma, eView4, eLookMenu,

  eX, eCos, eWorldFOV, eView2,

  eSkyFOV, eMenuFOV, eView3, eExt, eMenu, eMouseLim,

  eLoad,

  eWthrArray, eCurWthrStruct, eNextWthrStruct,
  eCurSkyCol, eCurFogCol,
  eWindVector,
  eSunriseHour, eSunsetHour, eSunriseDuration, eSunsetDuration,
  eSunDir, eSunVis,//real sun direction, sun(glare) alpha value //phal
  eWeatherRatio,

  // pointers to code
  eNoMusicBreak,
  eGammaFunc,
  eMusicVolFunc,
  eHaggleMore, eHaggleLess,
  eMenuMouseMove,
  eTruform, eGetMouseState,
  eXMenuHudIn, eXMenuHudOut, eXMenuNoMouse, eXMenuNoFOV,
  eXMenuWnds, eXMenuPopups, eXMenuLoWnds, eXMenuSubtitles, eXMenuFPS,
  eNoWorldFOV, eXRotSpeed, eYRotSpeed,
  eScrollScale, eBookScale, eJournalScale, 
  // floating pointers
  eNextTrack, eMusicVol,
  eAlwaysRun, eAutoRun,
  eShadowToggle, eShadowReal, eShadowFOV,
  eCrosshair2,

  eMW_ADDR_COUNT
};
DWORD MemMap[eMW_ADDR_COUNT];
bool LoadedMemMap;

typedef void (*mmVolumeProc)(float);
typedef void (*mmHaggleProc)();

DWORD GetMorrowVersion();

DWORD read_dword(const DWORD dwAddress);
WORD read_word(const DWORD dwAddress);
BYTE read_byte(const DWORD dwAddress);
float read_float(const DWORD dwAddress);
void write_dword(const DWORD dwAddress, DWORD dword);
void write_word(const DWORD dwAddress, WORD word);
void write_byte(const DWORD dwAddress, BYTE byte);
void write_float(const DWORD dwAddress, float f);
DWORD GetNextTrack();
DWORD GetMusicVol();
DWORD GetAlwaysRun();
DWORD GetAutoRun();
DWORD GetShadowToggle();
DWORD GetShadowReal();
DWORD GetShadowFov();
DWORD GetCrosshair2();
bool IsExterior();
bool IsMenu();
bool IsCombat();
bool IsCrosshair();
bool IsAlwaysRun();

DWORD GetCurrentWeather();
DWORD GetNextWeather();
float GetWeatherRatio();
RGBVECTOR* CurSkyColVector();
RGBVECTOR* CurFogColVector();
bool CellHasWeather();
float* GetWindVector();
DWORD WthrStruc(int wthr);
int GetWthrString(int wthr, int offset, char str[]);
void SetWthrString(int wthr, int offset, char str[]);
bool IsAboveWater();
bool CellHasWater();
bool WaterReflects();

DWORD IntCurCelladdr(); //d4w
DWORD IntCurCelladdr_old; //d4w
bool IntLikeExterior(); //d4w
bool IntIllegSleep(); //d4w
bool IntHasWater(); //d4w
float WaterLevel(); //d4w
float WaterLevel_old; //d4w
void IntName(char IntNameArray[]); //d4w
BYTE IntAmbR(); //d4w
BYTE IntAmbG(); //d4w
BYTE IntAmbB(); //d4w
BYTE IntSunR(); //d4w
BYTE IntSunG(); //d4w
BYTE IntSunB(); //d4w
BYTE IntFogR(); //d4w
BYTE IntFogG(); //d4w
BYTE IntFogB(); //d4w
float IntFogDens(); //d4w

void LoadMemMap();
void CheckMWVersion();

//Camera shake
D3DXMATRIX CameraShakeMatrix;
bool CameraShake;
float CameraShakeAmp;
bool CameraShakeChange;
float CameraShakeChangeRate;

bool DistantLandActive;

#endif