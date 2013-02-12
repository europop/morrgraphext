#ifndef _MW_BRIDGE_H_
#define _MW_BRIDGE_H_

#include "d3d8header.h"

#define DECLARE_MWBRIDGE MWBridge* mwBridge = MWBridge::Inst();

//-----------------------------------------------------------------------------

class MWBridge {
public:
	/// Destructor
	~MWBridge();

	/// Singleton access
	static inline MWBridge* Inst();

	/// Connect to Morrowind memory
	void Load();

	/// Used to determine whether we have connected to Morrowind's dynamic memory yet.
	inline bool IsLoaded();

	/// fopen function pointer
	inline void* GetFOpenPnt();

	/// Used to determine whether dynamic memory is allocated.
	bool CanLoad ();

	DWORD GetMorrowVersion();
	DWORD GetAlwaysRun();
	DWORD GetAutoRun();
	DWORD GetShadowToggle();
	DWORD GetShadowReal();
	DWORD GetShadowFov();
	DWORD GetCrosshair2();
	void SetCrosshairEnabled(bool enabled);
	void ToggleCrosshair();
	bool IsExterior();
	bool IsMenu();
	bool IsLoading();
	bool IsCombat();
	bool IsCrosshair();
	bool IsAlwaysRun();

	DWORD GetNextTrack();
	DWORD GetMusicVol();
	void SkipToNextTrack();
	void DisableMusic();

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

	BYTE GetPixelshader();
	void SetPixelshader(BYTE val);
	float GetViewDistance();
	void SetViewDistance(float dist);
	float GetAIDistance();
	void SetAIDistance(float dist);

	void SetFOV(float screenFOV);

	void GetSunDir(float& x, float& y, float& z);
	BYTE GetSunVis();
	float GetSunriseHour();
	void SetSunriseHour(float hour);
	float GetSunriseDuration();
	void SetSunriseDuration(float duration);
	float GetSunsetHour();
	void SetSunsetHour(float hour);
	float GetSunsetDuration();
	void SetSunsetDuration(float duration);

	DWORD IntCurCelladdr(); //d4w
	bool IntLikeExterior(); //d4w
	bool IntIllegSleep(); //d4w
	bool IntHasWater(); //d4w
	float WaterLevel(); //d4w

	void HaggleMore(DWORD num);
	void HaggleLess(DWORD num);

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

	DWORD PlayerPositionPointer();
	float PlayerPositionX();
	float PlayerPositionY();
	float PlayerPositionZ();
	float PlayerHeight();
	bool IsPlayerWaiting();

	DWORD GetPCView (DWORD dwMaster, DWORD dwPCView, DWORD PCVOff);
	D3DXVECTOR3 *PCam3Offset ();
	bool Is3rdPerson ();
	bool IsVanityMode ();
	bool IsLookingAround ();

	//DWORD GetMaxNrRipples ();	// unused (yet)
	//void SetMaxNrRipples (DWORD ripples);	// unused (yet)
	//void ResetMaxNrRipples ();	// unused (yet)
	//void ToggleOldRipples ();	// unused (yet)
	void TurnOldRipples (BOOL enabled);
	void SetProgressBarName(char *name);
	void ShowProgressBar(float value);
	void HideProgressBar();
	void SetScreen(float value);
	void UnsetScreen(float value);

protected:
	static MWBridge* m_instance;
	static DWORD m_version;
	bool m_loaded;

	/// Constructor - hidden to prevent direct use.  Use Inst function instead.
	MWBridge();

	/// Used to check if the user has a compatible version of Morrowind
//	static void CheckMWVersion();

	/// Sets pointers to static memory of Morrowind
	void InitStaticMemory ();
	void InitLayout722 ();
	void InitLayout1029 ();
	void InitLayout1820 ();

	/// Functions for reading and writing data at locations in Morrowind's memory
	DWORD read_dword(const DWORD dwAddress);
	WORD read_word(const DWORD dwAddress);
	BYTE read_byte(const DWORD dwAddress);
	float read_float(const DWORD dwAddress);
	void write_dword(const DWORD dwAddress, DWORD dword);
	void write_word(const DWORD dwAddress, WORD word);
	void write_byte(const DWORD dwAddress, BYTE byte);
	void write_float(const DWORD dwAddress, float f);

	//Pointers to Morrowind Memory
	DWORD
		dwTruRenderWidthOff, dwHRotScaleOff, dwSunVisOff, 
		dwPCViewOff, dwWthrStructOff, dwWthrStructPntOff,
		eMaster, eEnviro, eMaster1, eMaster2,
		eFPS, eTimer, eD3D, eTruRenderWidth, eShadowSlider,
		eCrosshair1, eAI, eView0, eRenderWidth,
		eView1, eCombat, ePCRef,

		ePixelshader, eGamma, eView4, eLookMenu,

		eX, eCos, eWorldFOV, eView2,

		eSkyFOV, eMenuFOV, eView3, eExt, eMenu, eMouseLim,

		eLoad,

		eWthrArray, eCurWthrStruct, eNextWthrStruct,
		eWthrStructPnt, eCurSkyCol, eCurFogCol,
		eWindVector,
		eSunriseHour, eSunsetHour, eSunriseDuration, eSunsetDuration,
		eSunDir, eSunVis,//real sun direction, sun(glare) alpha value //phal
		eWeatherRatio;

	// floating point variables
	DWORD eNextTrack, eMusicVol,
		eAlwaysRun, eAutoRun,
		eShadowToggle, eShadowReal, eShadowFOV,
		eCrosshair2;

	// Pointers to Morrowind code
	DWORD eNoMusicBreak,
		eGammaFunc,
		eMusicVolFunc,
		eHaggleMore, eHaggleLess,
		eMenuMouseMove,
		eTruform, eGetMouseState,
		eXMenuHudIn, eXMenuHudOut, eXMenuNoMouse, eXMenuNoFOV,
		eXMenuWnds, eXMenuPopups, eXMenuLoWnds, eXMenuSubtitles, eXMenuFPS,
		eNoWorldFOV, eXRotSpeed, eYRotSpeed,
		eScrollScale, eBookScale, eJournalScale, eRipplesSwitch,
		eFOpen, ePCView;

	// Other values
	DWORD dwAlwaysRunOffset;
	BYTE bMenuPopupOff1;
	BYTE bMenuPopupOff2;
	bool IsRotSpeedRegular;
	//bool MaxNrRipplesRead;		// unused (yet)
	bool RipplesSwitchable;
	//DWORD MaxNrRipples;	// unused (yet)

	static const bool is_auto_run_available = true;
};

//-----------------------------------------------------------------------------
// Inline Functions
//-----------------------------------------------------------------------------

inline MWBridge* MWBridge::Inst() {
	if ( m_instance == 0 ) {
		m_instance = new MWBridge();
	}

	return m_instance;
}

//-----------------------------------------------------------------------------

inline bool MWBridge::IsLoaded() {
	return m_loaded;
}

//-----------------------------------------------------------------------------

inline void* MWBridge::GetFOpenPnt() {
	return (void*)eFOpen;
}

//-----------------------------------------------------------------------------


#endif