//This file includes code from Alexander Stasenko's Morrowind fps optimizer

#ifndef _D3D8_HEADER_H_
#define _D3D8_HEADER_H_

#include "joint.h"
#include "QuadTree.h"
#include "DepthManager.h"

#ifndef RELEASE
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>


static const char define [] = "#define ";

typedef IDirect3DVertexBuffer9 IDirect3DVertexBuffer;
typedef IDirect3DIndexBuffer9 IDirect3DIndexBuffer;
typedef IDirect3DSurface9 IDirect3DSurface;
typedef IDirect3DTexture9 IDirect3DTexture;
typedef IDirect3D9 IDirect3D;
typedef IDirect3DDevice9 IDirect3DDevice;
typedef D3DLIGHT9 D3DLIGHT8;
typedef D3DMATERIAL9 D3DMATERIAL8;
typedef D3DVIEWPORT9 D3DVIEWPORT8;
typedef D3DCLIPSTATUS9 D3DCLIPSTATUS8;

typedef struct RGBVECTOR
{
#ifdef __cplusplus
public:
    RGBVECTOR() {}
    RGBVECTOR( DWORD rgb );
    RGBVECTOR( CONST FLOAT * );
    RGBVECTOR( CONST D3DXFLOAT16 * );
    RGBVECTOR( CONST RGBVECTOR * );
    RGBVECTOR( CONST RGBVECTOR& );
    RGBVECTOR( CONST D3DCOLORVALUE& );
    RGBVECTOR( CONST D3DXCOLOR& );
    RGBVECTOR( FLOAT r, FLOAT g, FLOAT b );

    // casting
    operator DWORD () const;

    operator FLOAT* ();
    operator CONST FLOAT* () const;

    // assignment operators
    RGBVECTOR& operator += ( CONST RGBVECTOR& );
    RGBVECTOR& operator -= ( CONST RGBVECTOR& );
    RGBVECTOR& operator += ( CONST D3DXCOLOR& );
    RGBVECTOR& operator -= ( CONST D3DXCOLOR& );
    RGBVECTOR& operator *= ( FLOAT );
    RGBVECTOR& operator /= ( FLOAT );

    // unary operators
    RGBVECTOR operator + () const;
    RGBVECTOR operator - () const;

    // binary operators
    RGBVECTOR operator + ( CONST RGBVECTOR& ) const;
    RGBVECTOR operator - ( CONST RGBVECTOR& ) const;
    RGBVECTOR operator + ( CONST D3DXCOLOR& ) const;
    RGBVECTOR operator - ( CONST D3DXCOLOR& ) const;
    RGBVECTOR operator * ( FLOAT ) const;
    RGBVECTOR operator / ( FLOAT ) const;

    friend RGBVECTOR operator * ( FLOAT, CONST RGBVECTOR& );

    BOOL operator == ( CONST RGBVECTOR& ) const;
    BOOL operator != ( CONST RGBVECTOR& ) const;
    BOOL operator == ( CONST D3DXCOLOR& ) const;
    BOOL operator != ( CONST D3DXCOLOR& ) const;

#endif //__cplusplus
    FLOAT r, g, b;
} RGBVECTOR, *LPRGBVECTOR;

#include "d3d8header.inl"

typedef struct _D3DCAPS8 {
    /* Device Info */
    D3DDEVTYPE  DeviceType;
    UINT    AdapterOrdinal;

    /* Caps from DX7 Draw */
    DWORD   Caps;
    DWORD   Caps2;
    DWORD   Caps3;
    DWORD   PresentationIntervals;

    /* Cursor Caps */
    DWORD   CursorCaps;

    /* 3D Device Caps */
    DWORD   DevCaps;

    DWORD   PrimitiveMiscCaps;
    DWORD   RasterCaps;
    DWORD   ZCmpCaps;
    DWORD   SrcBlendCaps;
    DWORD   DestBlendCaps;
    DWORD   AlphaCmpCaps;
    DWORD   ShadeCaps;
    DWORD   TextureCaps;
    DWORD   TextureFilterCaps;          // D3DPTFILTERCAPS for IDirect3DTexture8's
    DWORD   CubeTextureFilterCaps;      // D3DPTFILTERCAPS for IDirect3DCubeTexture8's
    DWORD   VolumeTextureFilterCaps;    // D3DPTFILTERCAPS for IDirect3DVolumeTexture8's
    DWORD   TextureAddressCaps;         // D3DPTADDRESSCAPS for IDirect3DTexture8's
    DWORD   VolumeTextureAddressCaps;   // D3DPTADDRESSCAPS for IDirect3DVolumeTexture8's

    DWORD   LineCaps;                   // D3DLINECAPS

    DWORD   MaxTextureWidth, MaxTextureHeight;
    DWORD   MaxVolumeExtent;

    DWORD   MaxTextureRepeat;
    DWORD   MaxTextureAspectRatio;
    DWORD   MaxAnisotropy;
    float   MaxVertexW;

    float   GuardBandLeft;
    float   GuardBandTop;
    float   GuardBandRight;
    float   GuardBandBottom;

    float   ExtentsAdjust;
    DWORD   StencilCaps;

    DWORD   FVFCaps;
    DWORD   TextureOpCaps;
    DWORD   MaxTextureBlendStages;
    DWORD   MaxSimultaneousTextures;

    DWORD   VertexProcessingCaps;
    DWORD   MaxActiveLights;
    DWORD   MaxUserClipPlanes;
    DWORD   MaxVertexBlendMatrices;
    DWORD   MaxVertexBlendMatrixIndex;

    float   MaxPointSize;

    DWORD   MaxPrimitiveCount;          // max number of primitives per DrawPrimitive call
    DWORD   MaxVertexIndex;
    DWORD   MaxStreams;
    DWORD   MaxStreamStride;            // max stride for SetStreamSource

    DWORD   VertexShaderVersion;
    DWORD   MaxVertexShaderConst;       // number of vertex shader constant registers

    DWORD   PixelShaderVersion;
    float   MaxPixelShaderValue;        // max value of pixel shader arithmetic component

} D3DCAPS8;

typedef IDirect3D* (_stdcall *D3DProc)(UINT);
typedef void (_stdcall *SoftwareShaderProc)(int,BYTE*);
typedef void (_stdcall *SetBackbufferProc)(DWORD,DWORD);

#ifdef _D3D8
#define EX
#else
#define EX extern
#endif

inline DWORD F2DW(float f) { return *((DWORD*)&f); }
inline float DW2F(DWORD d) { return *((float*)&d); }
inline float CLAMP(float f,float min,float max) { if(min>f) return min; if(max<f) return max; else return f; }
inline void SatUp(float* f,float max, float add) { if(((*f)+=add)>max) *f=max; }
inline void SatDown(float* f,float min, float sub) { if(((*f)-=sub)<min) *f=min; }

#define SETSTATUS(a) { strcpy_s (StatusString, 1024, a); StatusStringTime = GetTickCount () + MsgTime; }
#define MYVERTEXFORMAT D3DFVF_XYZRHW|D3DFVF_TEX6

#define MAXZOOM 15.0f   //Zoom constants
#define MINZOOM 3.0f
#define ZOOMSTEP 0.1f

#define MAXFOG 2.0f     //Fogging constants
#define MINFOG 0.01f
#define FOGSTEP 0.02f

#define MINSHAKE -1000.0f    //Screen shake constants
#define MAXSHAKE 100.0f

#define MAXMWSEOBJECTS 64
#define MAXMWSESHADERS 32
#define MAXMWSESUBSETS 8
#define MAXFILEPATHLENGTH 64
#define MAXACTIVELIGHTS 1024
#define MAXHOOKEDTEXTURES 64

#define HOOKEDTEXTUREID 87654

#ifdef _DEBUG
#define MAXVIEWDISTANCE 25000
#else
#define MAXVIEWDISTANCE 8192
#endif
#define MINVIEWDISTANCE 50
#define STDMAXVIEWDISTANCE 7168
#define STDMINVIEWDISTANCE 2500

//struct sOptions {
//    BYTE V;                 //Used to make sure the versions of MGEgui.exe and d3d8.dll match
//    BYTE BindAIToView;      //If this is non 0, changing the view distance automatically changes the AI distance
//    //*******************//
//    //* Global settings *//
//    //*******************//
//    BYTE AA;                //Antialiasing
//    BYTE ZBuffer;           //Zbuffer format
//    BYTE VWait;             //Vwait
//    BYTE RefreshRate;       //The refresh rate, or 0 for default
//    BYTE DisplayFPS;        //true to display an fps counter
//    BYTE UseLagFix;         //true to use the input lag fix
//    BYTE UseAAFix;          //true to enable antialiasing fix
//    BYTE DisplayMessages;   //true to display fps counter and status messages
//    BYTE CPUIdle;           //1 to release control of the cpu after each refresh
//    BYTE IdleTime;          //The amount of time to idle for
//    //*******************//
//    //* Device settings *//
//    //*******************//
//    BYTE Fog;           //0 to disable fogging
//    BYTE SaveFormat;    //Format to save screenshots in (0 - bmp, 1 - dds)
//    //Texture filtering
//    BYTE MagMinFilter;  //The filter to use on mag and min
//    BYTE MipFilter;     //The filter to use on mip
//    BYTE AnsiLevel;     //Level of anisotropic filtering to use
//    signed char LodBias;    //Adjust the lod of mipmaps
//    //Fogging
//    BYTE FogMode;       //Type of fog to use (0 - pixel, 1 - depth, 2 - range)
//    BYTE FogPixelMode;  //The mode of per pixel fog to use
//    BYTE FogVertexMode; //The mode of per vertex fog to use
//    BYTE RangedFog;     //True to enable ranged fog
//    //Zoom and rotation
//    BYTE InitialZoom;   //Value between 3 and 10 for initial zoom
//    BYTE Rotation;      //0 for normal, then in 90 degree increments
//    //Fullscreen shaders
//    BYTE HWShader;      //true to enable hardware shaders
//    //Misc stuff
//    BYTE ZoomUI;        //Zoom/shake effects UI
//    //Constant fps settings
//    BYTE FPSHold;       //True to maintain a constant fps
//    BYTE FPSDesired;    //The desired fps
//    BYTE FPSTolerence;  //FPS tolerence
//    BYTE MinViewDistance;
//    BYTE MaxViewDistance;
//    BYTE SetShaderVars; //True to continually update shader variables
//    BYTE HookTextures;  //True to hook texture loading calls
//    BYTE UseDistantLand;//True to use distant land
//	BYTE UseDistantStatics;	//True to use distant statics
//	BYTE DLSM1Mode;			//True to use the sm1 effect file
//	BYTE ReflectiveWater;	//True if water is reflective
//	BYTE NearStaticsReflect;//True if near statics reflect in the water
//	BYTE FarStaticsReflect; //True if far away statics reflect in the water
//	BYTE UseSM3Water;	//True to use the sm3 version of the distant water shader
//    BYTE DL_NearStaticEnd;  //Distance in cells to the end of the range where near statics are drawn
//	BYTE DL_AboveWaterFogEnd;  //Distance in cells to the end of the above water fog
//	BYTE UseHDR;		//true to enable HDR brightness calculations
//	BYTE HDRSpeed;		//Controls the HDR reaction speed
//	BYTE CorrectAspectOnZoom;
//	BYTE DL_AboveWaterFogStart;  //Distance in cells to the start of the fog when above water
//	BYTE DL_BelowWaterFogStart;  //Distance in cells to the start of the fog when under water
//	BYTE DL_BelowWaterFogEnd;  //Distance in cells to the end of the fog when under water
//	BYTE DL_DrawDist;  //Distance in cells to the far clipping plane
//	BYTE DL_FarStaticEnd;  //Distance in cells to the end of the range where far statics are drawn
//	BYTE DL_VeryFarStaticEnd;  //Distance in cells to the end of the range where very far statics are drawn
//	BYTE DL_FarStaticMinSize;  //Minimum size for a static to be classified as "far"
//	BYTE DL_VeryFarStaticMinSize;  //Minimum size for a static to be classified as "very far"
//	BYTE DL_AutoSetDistances; //Whether the user entered distances in manually or let the GUI decide for them based on draw distance
//	BYTE DL_NearStaticMinSize; //Saved by the GUI so it can be displayed, but not used here.
//	BYTE DL_DistantBlurStrength; //The number of blur passes that will be used for the depth of field effect
//};

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
#define SVAR_PLAYERPOS		25
#define MAXSVARS			26
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

struct TexCoord {
    float u,v;
};

struct MyVertex {
    float x,y,z,w;
    TexCoord Tex[6];
};
static MyVertex ShaderVertices[] = {
    {-0.5,-0.5,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {-0.5,+5.0,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
    {+5.0,-0.5,0,1,1,0,1,0,1,0,1,0,1,0,1,0},
    {+5.0,+5.0,0,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

//EX sOptions Options;
//Some device information
EX IDirect3D* RealObject;
EX IDirect3DDevice* RealDevice;

EX IDirect3DSurface* BackBuffer;
EX IDirect3DTexture* DepthTexture;
EX IDirect3DSurface* DepthBuffer;

EX IDirect3DSurface* distantReflectionSurface;
EX IDirect3DTexture* distantReflectionTexture;

EX IDirect3DSurface* distantReflectionSurface1;
EX IDirect3DTexture* distantReflectionTexture1;
EX IDirect3DSurface* distantReflectionSurface2;
EX IDirect3DTexture* distantReflectionTexture2;

EX IDirect3DTexture* tempShaderTexture;
EX IDirect3DSurface* tempShaderSurface;

EX DepthManager g_DepthManager;

EX bool MyBackbuffer;
EX IDirect3DStateBlock9* StateBlock;
EX D3DXMATRIX TransView;
EX D3DXMATRIX TransProj;
EX sShaderStruct ShaderStruct;
//Variables used by shaders
EX D3DXVECTOR4 RcpRes;
EX D3DXVECTOR4 SinVar;
EX D3DXVECTOR4 LinVar;
EX D3DXVECTOR4 EyeVec;
EX D3DXVECTOR4 EyePos;
EX D3DXVECTOR4 SunVec;
EX D3DXVECTOR4 SunCol;
EX D3DXVECTOR4 SunAmb;
EX D3DXVECTOR4 AmbCol;
EX D3DXVECTOR4 HDR;
EX D3DXVECTOR4 SunPos;//phal
EX float SunVis;//phal
EX D3DXVECTOR4 PlayerPos;
//Used for drawing stuff to screen
EX D3DXCOLOR fpsColor;
EX ID3DXFont* Font;
EX char StatusString[1024];
EX DWORD StatusStringTime;
EX RECT StatusRect;
EX BYTE RenderDLand;
//Used for keeping track of which scene morrowind is drawing
EX BYTE SceneCount;
EX BYTE LastSceneCount;
EX BYTE HUDScene;
EX BYTE AlphaScene;
EX BYTE SolarScene;
EX bool Indoors;
EX BYTE UpdateScene;
EX bool PixelShading;
EX bool UsingDistantLand;
EX bool UseDistantStatics;
EX bool UseGrass;
EX bool StencilEnabled;
EX bool AlphaBlendEnabled;
EX bool AlphaTestEnabled;
EX bool WaterDrawn;
EX bool SunEnabled;
EX bool LightsSet;
EX BYTE ShadowsDrawn;
EX int NumDrawCalls;
EX bool FogAlreadySet;
//fps variables (Could make an fps structure I suppose)
EX DWORD fpsLastTime;
EX DWORD fpsFrames;
EX char fpsString[11];
EX RECT fpsRect;
EX float fps;
EX bool fpsAdjusting;
//These are variables used for graphics features which cant be set by MGEgui
EX bool Zoom;
//EX float FOV;
//EX bool OverrideFogRange;
EX float FogRange;
EX float SmoothFogFactor;
EX float SmoothFogOffset;
EX DWORD lastFrameTime;
EX float FrameTime;
EX float FogStart;
EX float FogEnd;
//EX float Rotation;
EX D3DXMATRIX RotateMatrix;
//EX float MaxViewRange;
//EX float MinViewRange;
//Lighting
//MWSE stuff
//High performance timer
EX __int64 TimerFrequency;
EX __int64 LastTimer;
//Lighting
EX DWORD LightIDs[MAXACTIVELIGHTS];
EX DWORD ActiveLights;
//Basc MWSE stuff
EX HANDLE MWSEpipe;
EX MWSEobject Objects[MAXMWSEOBJECTS];
EX DWORD UsedMWSEobjects;
EX MWSE_HUDobject HUDbits[MAXMWSEOBJECTS];
EX DWORD UsedHUDbits;
EX ID3DXSprite* HUDsprite;
EX LoadedTexture LoadedTextures[MAXMWSESHADERS];
EX DWORD UsedMWSEShaders;
EX bool HookNextTexture;
EX bool NextTexIsWater;
EX bool DlDoneHooking;
EX BYTE NextHookIndex;
EX sHookedTexture HookedTextures[MAXHOOKEDTEXTURES];
EX UINT HookedShaderActive;
EX BYTE ActiveHookedShader;
EX BYTE SetHookedTextures[8];
//Camera shake
EX D3DXMATRIX CameraShakeMatrix;
EX bool CameraShake;
EX float CameraShakeAmp;
EX bool CameraShakeChange;
EX float CameraShakeChangeRate;
//FPS independent controls
EX bool FOVZooming;
EX float FOVZoomRate;
EX bool FogZooming;
EX float FogZoomRate;
EX bool RotSpinning;
EX float RotSpinRate;
//Stuff to control exterior lighting
EX bool SunManual;
EX bool SunManColour;
EX bool SunManDir;
EX bool SunAmbManual;
EX bool AmbientManual;
EX bool FogManual;
EX bool ClsManual;
EX bool SkyManual;
EX bool HUDManual;
EX IDirect3DTexture* SunTexture;
EX D3DXVECTOR3 SunDirection;
EX D3DCOLORVALUE SunColour;
EX D3DCOLORVALUE SunAmbColour;
EX DWORD AmbientColour;
EX DWORD ClsColour;
EX DWORD SkyColour;
EX DWORD HUDColour;
EX DWORD SunAmbD;
//For writing stuff to textures
EX IDirect3DTexture* TexRenderTarget;
EX IDirect3DTexture* TexCopyTo;
EX IDirect3DSurface* SufRenderTarget;
EX IDirect3DSurface* SufCopyTo;
EX D3DXVECTOR2 TexDrawOrigin;
EX D3DXVECTOR2 TexDrawScale;
EX DWORD TexDrawColour;
//for supporting videos
EX BYTE LoadedVideoCount;
EX DWORD FogColour;

EX D3DXVECTOR3 Cam3Offset;
EX bool ForceCam3Offset;

//Function definitions
void UpdateFogRange();
void UpdateRotation();
void UpdateSceneNums();
void UpdateScreenShake();
void UpdateLighting(int index);
void LoadShaderTextures(ID3DXEffect* effect);
void SetShaderVars(LoadedShader* shader, DWORD time);
void InitShaderFlags(LoadedShader* shader);
void* _stdcall FakeDirect3DCreate(UINT version);
//DistantLand
void HookWaterLoad(IDirect3DTexture9** ptrAddr);
bool IsFakeWaterTex(IDirect3DTexture9* cmp);

void HDR_Update(DWORD Time);

EX bool DistantLandActive; //phal

//HANDLE _stdcall CreateFileHook(char* path, DWORD b, DWORD c, SECURITY_ATTRIBUTES* d, DWORD creation, DWORD flags, HANDLE g);
//void* _cdecl FileOpenHook(char* file,char* args);
void InitHooks();

void ApplyShaders();
void DrawRecordedDrawCallDepth( D3DXMATRIX* view, D3DXMATRIX* proj );

void UpdateFogMethod( bool force_update = false );

#endif
