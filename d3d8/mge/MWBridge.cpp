//This file contains code from fps optimizer
//Memory layout originally worked out by Alexander Stasenko

#include "MWBridge.h"
#include "assert.h"

MWBridge* MWBridge::m_instance = 0;
DWORD MWBridge::m_version = 0;

typedef void (*mmVolumeProc)(float);
typedef void (*mmHaggleProc)();

//-----------------------------------------------------------------------------

MWBridge::MWBridge() {

	//Set all values to zero
	memset(this, 0, sizeof(*this));
	m_loaded = false;
	InitStaticMemory ();
}


//-----------------------------------------------------------------------------

MWBridge::~MWBridge() {

}

//-----------------------------------------------------------------------------

void MWBridge::InitLayout722 () {
	// v1.2.0722
	eNoMusicBreak = 0x00403639;
	eMusicVolFunc = 0x00403A00;
	eHaggleMore = 0x005990C8;
	eTruform = 0x006CFA0C;
	eGetMouseState = 0x004066D1;
	eNoWorldFOV = 0x004049ee;
	eXRotSpeed = 0x0055c298;
	eYRotSpeed = eXRotSpeed + 0x11C;
	eScrollScale = 0x00602316;
	eBookScale = 0x0059de0b;
	eRipplesSwitch = 0x00511032;

	eXMenuHudIn = 0x005e2d84;
	eXMenuNoMouse = 0x004086d0;
	eXMenuNoFOV = 0x00404b28;
	eXMenuWnds = 0x00574b52;
	eXMenuPopups = 0x00587F2C;
	eXMenuLoWnds = 0x00578485;
	eXMenuSubtitles = 0x005e7fcf;
	eXMenuFPS = 0x0041b653;

	dwTruRenderWidthOff = 0x48;
	dwHRotScaleOff = 0x4C;
	dwWthrStructOff = 0x20;
	dwWthrStructPntOff = 0xC;
	dwSunVisOff = 0x60;
	dwPCViewOff = 0x23C;
	dwAlwaysRunOffset = 0x5A4;
	bMenuPopupOff1 = 0xF0;
	bMenuPopupOff2 = 0xE8;
	IsRotSpeedRegular = true;

	eMaster = 0x007B679C;
	eFOpen = 0x007382D8;
	ePCView = 0x0040FEF0;
	eEnviro = eMaster + 0x4;
	eLoad = eMaster + 0x1DE4;

	eHaggleLess  = eHaggleMore + 0xF0;
	eXMenuHudOut = eXMenuHudIn + 0x448;
	eMenuMouseMove = eXMenuNoMouse + 0xE0;
	eJournalScale = eBookScale + 0xF5;

	RipplesSwitchable = (read_dword (eRipplesSwitch) == 0x33504D8B);
}

//-----------------------------------------------------------------------------

void MWBridge::InitLayout1029() { // v1.3.1029 (Tribunal)
	eNoMusicBreak = 0x004036a9;
	eMusicVolFunc = 0x00403A70;
	eHaggleMore = 0x0059ED08;
	eTruform = 0x006DC32C;
	eGetMouseState = 0x00406791;
	eNoWorldFOV = 0x00404a5e;
	eXRotSpeed = 0x00561fa8;
	eYRotSpeed = eXRotSpeed + 0x11C;
	eScrollScale = 0x0060af56;
	eBookScale = 0x005a3a5b;
	eRipplesSwitch = 0x00516964;

	eXMenuHudIn = 0x005ebae4;
	eXMenuNoMouse = 0x004087b0;
	eXMenuNoFOV = 0x00404b98;
	eXMenuWnds = 0x0057a852;
	eXMenuPopups = 0x0058d8fc;
	eXMenuLoWnds = 0x0057e165;
	eXMenuSubtitles = 0x005f0d2f;
	eXMenuFPS = 0x0041b6a3;

	dwTruRenderWidthOff = 0x48;
	dwHRotScaleOff = 0x4C;
	dwWthrStructOff = 0x20;
	dwWthrStructPntOff = 0xC;
	dwSunVisOff = 0x64;
	dwPCViewOff = 0x244;	
	dwAlwaysRunOffset = 0x5B0;
	bMenuPopupOff1 = 0xEC;
	bMenuPopupOff2 = 0xF0;
	IsRotSpeedRegular = true;

	eMaster = 0x007BCE1C;
	eFOpen = 0x0073E2E0;
	ePCView = 0x0040FF00;
	eEnviro = eMaster + 0x4;
	eLoad = eMaster + 0x1DE8;
	
	eHaggleLess  = eHaggleMore + 0xF0;
	eXMenuHudOut = eXMenuHudIn + 0x448;
	eMenuMouseMove = eXMenuNoMouse + 0xE0;
	eJournalScale = eBookScale + 0xF5;

	RipplesSwitchable = (read_dword (eRipplesSwitch) == 0x33504D8B);
}

//-----------------------------------------------------------------------------

void MWBridge::InitLayout1820 () {
	// Bloodmoon v. 1080
	eNoMusicBreak = 0x00403659;
	eMusicVolFunc = 0x00403a10;
	eHaggleMore = 0x005A73D0;
	eTruform = 0x006e4ffc;
	eGetMouseState = 0x00406721;
	eNoWorldFOV = 0x004049fe;
	eXRotSpeed = 0x005692b1;
	eYRotSpeed = eXRotSpeed + 0x11C;
	eScrollScale = 0x006139b6;
	eBookScale = 0x005ac47b;
	eRipplesSwitch = 0x0051C2D4;

	eXMenuHudIn = 0x005f43c4;
	eXMenuNoMouse = 0x00408740;
	eXMenuNoFOV = 0x00404b38;
	eXMenuWnds = 0x00583072;
	eXMenuPopups = 0x005961AC;
	eXMenuLoWnds = 0x00586985;
	eXMenuSubtitles = 0x005f980f;
	eXMenuFPS = 0x0041bc5e;
						
	dwTruRenderWidthOff = 0x48;
	dwHRotScaleOff = 0x50;
	dwWthrStructOff = 0x28;
	dwWthrStructPntOff = 0x0;
	dwSunVisOff = 0x74;
	dwPCViewOff = 0x244;
	dwAlwaysRunOffset = 0x5B4;
	bMenuPopupOff1 = 0xEC;
	bMenuPopupOff2 = 0xF0;
	IsRotSpeedRegular = true;

	eMaster = 0x007C67DC;
	eFOpen = 0x007462E0;
	ePCView = 0x0040FF20;
	eEnviro = eMaster + 0x4;
	eLoad = eMaster + 0x1DDC;

	eHaggleLess  = eHaggleMore + 0xF0;
	eXMenuHudOut = eXMenuHudIn + 0x448;
	eMenuMouseMove = eXMenuNoMouse + 0xE0;
	eJournalScale = eBookScale + 0xF5;

	//MaxNrRipplesRead = false;	// not used (yet)
	RipplesSwitchable = (read_dword (eRipplesSwitch) == 0x33504D8B);
}

//-----------------------------------------------------------------------------

void MWBridge::Load() {
	eMaster1 = read_dword(eMaster); //0x007C67DC
	eMaster2 = read_dword(eMaster + 0x500);  //0x007C6CDC

	eFPS = eMaster1 + 0x14;  //0x14
	eTimer = eFPS + 0xC;       //0x20
	eD3D = eTimer + 0x10;    //0x30
	ePCRef = eD3D + 0x2C;			//0x5C
	eTruRenderWidth = eD3D + dwTruRenderWidthOff;   //0x78
	eShadowSlider = eTruRenderWidth + 0xC;        //0x84
	eCrosshair1 = eShadowSlider + 0x10;         //0x94
	eAI = eCrosshair1 + 0x4;            //0x98
	eView0 = eAI + dwHRotScaleOff + 0x5C;  //0xF4
	eRenderWidth = eView0 + 0x4;                 //0xF8
	eView1 = eRenderWidth + 0x188;         //0x280  //0x007C6A5C
	eCombat = eView1 + 0x80;                //0x300 

	ePixelshader = eMaster2 + 0x20;
	eGamma = eMaster2 + 0x3C;  //0x3C  //00D1E090
	eView4 = eGamma + 0x10;   //0x4C  //00D1E0A0
	eLookMenu = eView4 + 0x9C;    //0xE8  //00D1E090

	eX = read_dword(eView0 - 0x10) + 0x1C;
	eCos = eX + 0xD4;
	eWorldFOV = eCos + 0x10;
	eView2 = eWorldFOV + 0x14;

	eSkyFOV = read_dword(eRenderWidth + 0x18) + 0x100;
	eMenuFOV = read_dword(eRenderWidth + 0x44) + 0x100;

	eView3 = read_dword(eView1 - 0x10) + 0x114;
	eExt = read_dword(eView1 + 0x10) + 0x10;
	eMenu = read_dword(read_dword(eView1 + 0x14) + 0x10) + 0x58;
	eMouseLim = read_dword(eD3D + 0x20) + 0x24;

	eGammaFunc = read_dword(read_dword(eMaster2) + 0x50);

	eWthrArray = read_dword(eMaster1 + 0x58) + 0x14;
	eCurWthrStruct = eWthrArray + dwWthrStructOff;  // 0x3c
	eNextWthrStruct = eCurWthrStruct + 0x04;  // 0x40
	eWthrStructPnt = eNextWthrStruct - dwWthrStructPntOff;
	eCurSkyCol = eWthrStructPnt + 0x50;  // 0x90
	eCurFogCol = eWthrStructPnt + 0x5c;  // 0x9c
	eWindVector = eWthrStructPnt + 0x78;  // 0xb8
	eSunriseHour = eWthrStructPnt + 0x9c;  // 0xdc
	eSunsetHour = eWthrStructPnt + 0xa0;  // 0xe0
	eSunriseDuration = eWthrStructPnt + 0xa4;  // 0xe4
	eSunsetDuration = eWthrStructPnt + 0xa8;  // 0xe8
	eWeatherRatio = eWthrStructPnt + 0x130;  // 0x170

	eSunDir = read_dword(eWthrArray - 0x14) + 0x1C; //phal
	eSunVis = read_dword(read_dword(read_dword(eWthrArray + dwSunVisOff)+0x98)+0x24)+0x3; //phal

	m_loaded = true;

	GetShadowFov();
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetMorrowVersion() {
	if(m_version==1875) return 1820;
	return m_version;
}

//-----------------------------------------------------------------------------

void MWBridge::InitStaticMemory() {
	switch (MWVersion) {
		case 722: InitLayout722 (); break;
		case 1029: InitLayout1029 (); break;
		case 1875:
		case 1820: InitLayout1820 (); break;
		default: ERRORMSG("Unsupported morrowind version.\n"
						  "A fully patched copy of bloodmoon is required to use MGE.");
	}
}

//-----------------------------------------------------------------------------

DWORD MWBridge::read_dword(const DWORD dwAddress) {
	return *reinterpret_cast<DWORD*>(dwAddress);
}

//-----------------------------------------------------------------------------

WORD MWBridge::read_word(const DWORD dwAddress) {
	return *reinterpret_cast<WORD*>(dwAddress);
}

//-----------------------------------------------------------------------------

BYTE MWBridge::read_byte(const DWORD dwAddress) {
	return *reinterpret_cast<BYTE*>(dwAddress);
}

//-----------------------------------------------------------------------------

float MWBridge::read_float(const DWORD dwAddress) {
	return *reinterpret_cast<float*>(dwAddress);
}

//-----------------------------------------------------------------------------

void MWBridge::write_dword(const DWORD dwAddress, DWORD dword) {
	*reinterpret_cast<DWORD*>(dwAddress) = dword;
}

//-----------------------------------------------------------------------------

void MWBridge::write_word(const DWORD dwAddress, WORD word) {
	*reinterpret_cast<WORD*>(dwAddress) = word;
}

//-----------------------------------------------------------------------------

void MWBridge::write_byte(const DWORD dwAddress, BYTE byte) {
	*reinterpret_cast<BYTE*>(dwAddress) = byte;
}

//-----------------------------------------------------------------------------

void MWBridge::write_float(const DWORD dwAddress, float f) {
	*reinterpret_cast<float*>(dwAddress) = f;
}

//-----------------------------------------------------------------------------

bool MWBridge::CanLoad () {
	// reads static address, so game doesn't need to be loaded
	return (read_dword (eEnviro) != 0);
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetAlwaysRun() {
	assert(m_loaded);
	return read_dword(read_dword(read_dword(eD3D + 0x2C) + 0x24)) + dwAlwaysRunOffset;
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetAutoRun() {
	assert(m_loaded);
	return is_auto_run_available ? GetAlwaysRun() + 4 : 0;
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetShadowToggle() {
	assert(m_loaded);
	return read_dword(eView1 + 0xC) + 0xC;
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetShadowReal() {
	assert(m_loaded);
	return GetShadowToggle() + 0x8;
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetShadowFov() {
	assert(m_loaded);
	//if (eShadowFOV == 0) {
	eShadowFOV = read_dword (eView1 + 0xC);
	//assert(eShadowFOV != 0);
	if (eShadowFOV) {
		eShadowFOV = read_dword (eShadowFOV + 0x8);
		if (eShadowFOV) eShadowFOV += 0x100;
	}
	return eShadowFOV;
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetCrosshair2() {
	assert(m_loaded);
	return read_dword(eView0 - 0x50) + 0x14;
}

//-----------------------------------------------------------------------------

void MWBridge::SetCrosshairEnabled(bool enabled) {
	assert(m_loaded);
	if (enabled) {
		eCrosshair2 = GetCrosshair2();
		write_byte(eCrosshair2, read_byte(eCrosshair2) & 0xfe);
		write_byte(eCrosshair1, read_byte(eCrosshair1) & 0xfe);
	} else {
		eCrosshair2 = GetCrosshair2();
		write_byte(eCrosshair2, read_byte(eCrosshair2) | 0x01);
		write_byte(eCrosshair1, read_byte(eCrosshair1) | 0x01);
	}
}

//-----------------------------------------------------------------------------

void MWBridge::ToggleCrosshair() {
	assert(m_loaded);
	eCrosshair2 = GetCrosshair2();
	BYTE b = read_byte(eCrosshair2);
	BYTE b2 = read_byte(eCrosshair1);
	write_byte(eCrosshair2, b ^ 0x01);
	write_byte(eCrosshair1, b2 ^ 0x01);
}

//-----------------------------------------------------------------------------

bool MWBridge::IsExterior() {
	assert(m_loaded);
	DWORD addr = read_dword(eEnviro);
	if ( addr !=0 ) {
		return read_dword(addr + 0xAC) == 0;
	} else {
		return 0;
	}
}

//-----------------------------------------------------------------------------

bool MWBridge::IsMenu() {
	assert(m_loaded);
	return (read_dword(eMenu) & 0x10000) != 0;
}

//-----------------------------------------------------------------------------

bool MWBridge::IsLoading() {
	return (read_byte(eLoad) != 0);
}

//-----------------------------------------------------------------------------

bool MWBridge::IsCombat() {
	assert(m_loaded);
	return (read_dword(eCombat) & 1) != 0;
}

//-----------------------------------------------------------------------------

bool MWBridge::IsCrosshair() {
	assert(m_loaded);
	return (read_dword(eCrosshair1) & 1) == 0;
}

//-----------------------------------------------------------------------------

bool MWBridge::IsAlwaysRun() {
	assert(m_loaded);
	return  read_byte(GetAlwaysRun()+3) != 0;
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetNextTrack() {
	assert(m_loaded);
	return read_dword(eD3D + 0x4) + 0x8;
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetMusicVol() {
	assert(m_loaded);
	return GetNextTrack() + 0x28C;
}

//-----------------------------------------------------------------------------

void MWBridge::SkipToNextTrack() {
	assert(m_loaded);
	eMusicVol = GetMusicVol();
	float f = read_float(eMusicVol);
	mmVolumeProc mvp=(mmVolumeProc)eMusicVolFunc;
	__asm {
		mov ecx, eMusicVol;
		sub ecx, 0x294;
	}
	mvp(f);
}
	
//-----------------------------------------------------------------------------

void MWBridge::DisableMusic() {
	assert(m_loaded);
	eMusicVol = GetMusicVol();
	write_float(eMusicVol, 0.05f);
	write_float(eMusicVol - 0x08, 0.05f);
	mmVolumeProc mvp = (mmVolumeProc)eMusicVolFunc;
	__asm {
	    mov ecx, eMusicVol;
	    sub ecx, 0x294;
	}
	mvp(0.05f);
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetCurrentWeather() {
	assert(m_loaded);
	DWORD weather = read_dword(eCurWthrStruct);
	if (weather == 0) return 0;
	return read_byte(weather + 4);
}

//-----------------------------------------------------------------------------

DWORD MWBridge::GetNextWeather() {
	assert(m_loaded);
	DWORD weather = read_dword(eNextWthrStruct);
	if (weather == 0) return GetCurrentWeather();
	return read_byte(weather + 4);
}

//-----------------------------------------------------------------------------

float MWBridge::GetWeatherRatio() {
	assert(m_loaded);
	return read_float(eWeatherRatio);
}

//-----------------------------------------------------------------------------

RGBVECTOR* MWBridge::CurSkyColVector() {
	assert(m_loaded);
	return (RGBVECTOR*)eCurSkyCol;
}

//-----------------------------------------------------------------------------

RGBVECTOR* MWBridge::CurFogColVector() {
	assert(m_loaded);
	return (RGBVECTOR*)eCurFogCol;
}

//-----------------------------------------------------------------------------

bool MWBridge::CellHasWeather() {
	assert(m_loaded);
	DWORD addr = read_dword(eEnviro);
	if (addr == 0) return true;
	addr = read_dword(addr + 0xAC);
	if (addr != 0) return ((read_byte(addr + 0x18) & 0xF3) == 0x93);
	return true;
}

//-----------------------------------------------------------------------------

float* MWBridge::GetWindVector() {
	assert(m_loaded);
	return (float*)eWindVector;
}

//-----------------------------------------------------------------------------

DWORD MWBridge::WthrStruc(int wthr) {
	assert(m_loaded);
	if (wthr >= 0 && wthr <= 9) {
		DWORD addr = read_dword(eWthrArray + (wthr << 2));
		if (addr != 0) return addr;
	}
	return 0;
}

//-----------------------------------------------------------------------------

int MWBridge::GetWthrString(int wthr, int offset, char str[]) {
	assert(m_loaded);
	int i = 0;
	DWORD addr = WthrStruc(wthr);
	if (addr != 0) {
		addr += offset;
		while ((str[i] = read_byte(addr)) != 0) {
			++addr; ++i;
		}
	}
	str[i++] = 0;
	return i;
}

//-----------------------------------------------------------------------------

void MWBridge::SetWthrString(int wthr, int offset, char str[]) {
	assert(m_loaded);
	int i = 0;
	DWORD addr = WthrStruc(wthr);
	if (addr != 0) {
		addr += offset;
		char c;
		do {
			write_byte(addr++, c = str[i++]);
		} while (c != 0);
	}
}

//-----------------------------------------------------------------------------

bool MWBridge::CellHasWater() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return ((read_byte(addr + 0x18) & 0x73) == 0x13);
	return true;
}

//-----------------------------------------------------------------------------

bool MWBridge::IsAboveWater() {
	assert(m_loaded);
	return ((EyePos.z > WaterLevel() - 4.0f) || !CellHasWater());
}

//-----------------------------------------------------------------------------

bool MWBridge::WaterReflects() {
	assert(m_loaded);
	return (CellHasWater() && (EyePos.z > WaterLevel()));
}

//-----------------------------------------------------------------------------

BYTE MWBridge::GetPixelshader() {
	assert(m_loaded);
	return read_byte(ePixelshader);
}

//-----------------------------------------------------------------------------

void MWBridge::SetPixelshader(BYTE val) {
	assert(m_loaded);
	write_byte(ePixelshader, val);
}

//-----------------------------------------------------------------------------

float MWBridge::GetViewDistance() {
	assert(m_loaded);
	return read_float(eView0);
}

//-----------------------------------------------------------------------------

void MWBridge::SetViewDistance(float dist) {
	assert(m_loaded);
	write_float(eView0, dist);
	write_float(eView1, dist);
	write_float(eView2, dist);
	write_float(eView3, dist);
	write_float(eView4, dist);
}

//-----------------------------------------------------------------------------

float MWBridge::GetAIDistance() {
	assert(m_loaded);
	return read_float(eAI);
}

//-----------------------------------------------------------------------------

void MWBridge::SetAIDistance(float dist) {
	assert(m_loaded);
	write_float(eAI, dist);
}

//-----------------------------------------------------------------------------

void MWBridge::SetFOV(float screenFOV) {
	assert(m_loaded);

	//Recalculate FOV values
	float fovtan = tanf(screenFOV*D3DX_PI/360.0f);

	if( abs(read_float(eWorldFOV)+fovtan) > 0.001f ) {
		float fovtanaspect = fovtan/JointInfo.aspect;
		write_float(eWorldFOV,-fovtan);
		write_float(eWorldFOV+1*sizeof(float),fovtan);
		write_float(eWorldFOV+2*sizeof(float),fovtanaspect);
		write_float(eWorldFOV+3*sizeof(float),-fovtanaspect);

		write_float(eSkyFOV ,-fovtan);
		write_float(eSkyFOV+1*sizeof(float),fovtan);
		write_float(eSkyFOV+2*sizeof(float),fovtanaspect);
		write_float(eSkyFOV+3*sizeof(float),-fovtanaspect);
		if (!eShadowFOV) GetShadowFov();
		if (eShadowFOV) {
			write_float(eShadowFOV ,-fovtan);
			write_float(eShadowFOV+1*sizeof(float),fovtan);
			write_float(eShadowFOV+2*sizeof(float),fovtanaspect);
			write_float(eShadowFOV+3*sizeof(float),-fovtanaspect);
		}
	}
}

//-----------------------------------------------------------------------------

void MWBridge::GetSunDir(float& x, float& y, float& z) {
	assert(m_loaded);
	x = read_float(eSunDir);
	y = read_float(eSunDir + 0x4);
	z = read_float(eSunDir + 0x8);
}

//-----------------------------------------------------------------------------

BYTE MWBridge::GetSunVis() {
	assert(m_loaded);
	return read_byte(eSunVis);
}

//-----------------------------------------------------------------------------

float MWBridge::GetSunriseHour() {
	assert(m_loaded);
	return read_float(eSunriseHour);
}

//-----------------------------------------------------------------------------

void MWBridge::SetSunriseHour(float hour) {
	assert(m_loaded);
	write_float(eSunriseHour, hour);
}

//-----------------------------------------------------------------------------

float MWBridge::GetSunriseDuration() {
	assert(m_loaded);
	return read_float(eSunriseDuration);
}

//-----------------------------------------------------------------------------

void MWBridge::SetSunriseDuration(float duration) {
	assert(m_loaded);
	write_float(eSunriseDuration, duration);
}

//-----------------------------------------------------------------------------

float MWBridge::GetSunsetHour() {
	assert(m_loaded);
	return read_float(eSunsetHour);
}

//-----------------------------------------------------------------------------

void MWBridge::SetSunsetHour(float hour) {
	assert(m_loaded);
	write_float(eSunsetHour, hour);
}

//-----------------------------------------------------------------------------

float MWBridge::GetSunsetDuration() {
	assert(m_loaded);
	return read_float(eSunsetDuration);
}

//-----------------------------------------------------------------------------

void MWBridge::SetSunsetDuration(float duration) {
	assert(m_loaded);
	write_float(eSunsetDuration, duration);
}

//-----------------------------------------------------------------------------

//d4w Interior parameters - optimized by krzymar
DWORD MWBridge::IntCurCelladdr() {
	assert(m_loaded);

	DWORD addr = read_dword(eEnviro);
	if (addr != 0) return read_dword(addr + 0xAC);
	return 0;
}

//-----------------------------------------------------------------------------

void MWBridge::IntName (char IntNameArray[]){
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	int i = 0;
	if (addr != 0) {
		addr = read_dword(addr + 0x10);
		if (addr != 0) while ((IntNameArray[i] = read_byte(addr)) != 0) {
			++i;
			++addr;
		}
	}
	IntNameArray[i] = 0;
}

//-----------------------------------------------------------------------------

bool MWBridge::IntLikeExterior() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return ((read_byte(addr + 0x18) & 0xF3) == 0x93);
	return false;
}

//-----------------------------------------------------------------------------

bool MWBridge::IntIllegSleep() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return ((read_byte(addr + 0x18) & 0x75) == 0x15);
	return false;
}

//-----------------------------------------------------------------------------

bool MWBridge::IntHasWater() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return ((read_byte(addr + 0x18) & 0xF3) == 0x13);
	return false;
}

//-----------------------------------------------------------------------------

float MWBridge::WaterLevel() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0 && ((read_byte(addr + 0x18) & 0xF3) == 0x13)) return read_float(addr + 0x90);
	return 0.0f;
}

//-----------------------------------------------------------------------------

void MWBridge::HaggleMore(DWORD num) {
	assert(m_loaded);
	mmHaggleProc proc=(mmHaggleProc)eHaggleMore;
	num -= 1;
	if (num != 0) {
		long d = (long)read_dword(0x7D287C);
		if (d <= 0) {
			d-=num;
		} else {
			d+=num;
		}
		write_dword(0x7D287C, d);
	}
	proc();
}

//-----------------------------------------------------------------------------

void MWBridge::HaggleLess(DWORD num) {
	assert(m_loaded);
	mmHaggleProc proc = (mmHaggleProc)eHaggleLess;
	long d =( long)read_dword(0x7D287C);
	num -=1;
	if (num != 0) {
		if (d <= 0) {
			d+=num;
		} else {
			d-=num;
		}
		write_dword(0x7D287C,d);
	}
	proc();
}

//-----------------------------------------------------------------------------

BYTE MWBridge::IntAmbR() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return read_byte(addr + 0x1C);
	return 0;
}

//-----------------------------------------------------------------------------

BYTE MWBridge::IntAmbG() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return read_byte(addr + 0x1D);
	return 0;
}

//-----------------------------------------------------------------------------

BYTE MWBridge::IntAmbB() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return read_byte(addr + 0x1E);
	return 0;
}

//-----------------------------------------------------------------------------

BYTE MWBridge::IntSunR() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return read_byte(addr + 0x20);
	return 0;
}

//-----------------------------------------------------------------------------

BYTE MWBridge::IntSunG() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return read_byte(addr + 0x21);
	return 0;
}

//-----------------------------------------------------------------------------

BYTE MWBridge::IntSunB() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return read_byte(addr + 0x22);
	return 0;
}

//-----------------------------------------------------------------------------

BYTE MWBridge::IntFogR() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return read_byte(addr + 0x24);
	return 0;
}

//-----------------------------------------------------------------------------

BYTE MWBridge::IntFogG() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return read_byte(addr + 0x25);
	return 0;
}

//-----------------------------------------------------------------------------

BYTE MWBridge::IntFogB() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return read_byte(addr + 0x26);
	return 0;
}

//-----------------------------------------------------------------------------

float MWBridge::IntFogDens() {
	assert(m_loaded);
	DWORD addr = IntCurCelladdr();
	if (addr != 0) return read_float(addr + 0x28);
	return 0;
}

//-----------------------------------------------------------------------------

DWORD MWBridge::PlayerPositionPointer() {
	DWORD addr = eMaster1;
	if (addr != 0) {
		addr = read_dword(addr + 0x34);
		if (addr != 0) return addr;
	}
	return 0;
}

//-----------------------------------------------------------------------------

float MWBridge::PlayerPositionX() {
	DWORD addr = PlayerPositionPointer();
	if (addr != 0) return read_float(addr + 0x2AC);
	return 0;
}

//-----------------------------------------------------------------------------

float MWBridge::PlayerPositionY() {
	DWORD addr = PlayerPositionPointer();
	if (addr != 0) return read_float(addr + 0x2B0);
	return 0;
}

//-----------------------------------------------------------------------------

float MWBridge::PlayerPositionZ() {
	DWORD addr = PlayerPositionPointer();
	if (addr != 0) return read_float(addr + 0x2B4);
	return 0;
}

//-----------------------------------------------------------------------------

float MWBridge::PlayerHeight() { //player eyes heigh, in CS
	float height = read_float(0x007D39F0); //like "Master", only read, in game PlayerHeight*125.0f
	return (height == 0 ? 1.0f : height);
}

//-----------------------------------------------------------------------------

bool MWBridge::IsPlayerWaiting() { //wait\sleep menu
	DWORD addr = eMaster1;
	if (addr != 0) {
		addr+=0x354;
		if (addr != 0) {
			addr=read_dword(addr);
			if (addr != 0) return (read_byte(addr)==0x01); 
		}
	}
	return false;
}

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4035)
DWORD MWBridge::GetPCView (DWORD dwMaster, DWORD dwPCView, DWORD PCVOff) {
	__asm {
		mov ecx,dwMaster
		mov ecx,[ecx]
		mov eax,dwPCView
		call eax
		or eax,eax
		jz NoPCView
		mov ecx,PCVOff
		mov eax,[eax+ecx]
	NoPCView:
	}
}
#pragma warning(pop)

//-----------------------------------------------------------------------------

D3DXVECTOR3 *MWBridge::PCam3Offset () {
	DWORD addr = GetPCView (eMaster, ePCView, dwPCViewOff);
	if (addr) addr += 0xD8;
	return (D3DXVECTOR3 *)addr;
}

//-----------------------------------------------------------------------------

bool MWBridge::Is3rdPerson () {
	DWORD addr = GetPCView (eMaster, ePCView, dwPCViewOff);
	if (addr) return (read_byte (addr + 0xE8) != 0);
	return false;
}

//-----------------------------------------------------------------------------

bool MWBridge::IsVanityMode () {
	DWORD addr = GetPCView (eMaster, ePCView, dwPCViewOff);
	if (addr) return (read_byte (addr + 0xEC) != 0);
	return false;
}

//-----------------------------------------------------------------------------

bool MWBridge::IsLookingAround () {
	DWORD addr = GetPCView (eMaster, ePCView, dwPCViewOff);
	if (addr) return (read_dword (addr + 0xFC) != 0);
	return false;
}

//-----------------------------------------------------------------------------
//
// Gets Morrowind's maximum number of ripples (not MGE)
// Function is disabled due to fact it's unused in MGE (yet)
//
/*DWORD MWBridge::GetMaxNrRipples () {
	DWORD addr = read_dword (eEnviro);
	if (addr) {
		addr = read_dword (addr + 0xB4EC);
		if (addr) return (read_dword (addr + 0x50));
	}
	return 0;
}/**/

//-----------------------------------------------------------------------------
//
// Sets Morrowind's maximum number of ripples (not MGE)
// Function is disabled due to fact it's unused in MGE (yet)
//
/*void MWBridge::SetMaxNrRipples (DWORD ripples) {
	DWORD addr = read_dword (eEnviro);
	if (addr) {
		addr = read_dword (addr + 0xB4EC);
		if (addr) {
			addr += 0x50;
			if (!MaxNrRipplesRead) MaxNrRipples =  read_dword (addr);
			else write_dword (addr, ripples);
		}
	}
}/**/

//-----------------------------------------------------------------------------
//
// Resets Morrowind's maximum number of ripples (not MGE)
// Function is disabled due to fact it's unused in MGE (yet)
//
/*void MWBridge::ResetMaxNrRipples () {
	if (MaxNrRipplesRead) {
		DWORD addr = read_dword (eEnviro);
		if (addr) {
			addr = read_dword (addr + 0xB4EC);
			if (addr) {
				addr += 0x50;
				if (!MaxNrRipplesRead) MaxNrRipples =  read_dword (addr);
				else write_dword (addr, MaxNrRipples);
			}
		}
	}
}/**/

//-----------------------------------------------------------------------------
//
// Toggles old Morrowind's ripples
// Function is disabled due to fact it's unused in MGE (yet)
//
/*void MWBridge::ToggleOldRipples () {
	if (RipplesSwitchable) {
		DWORD addr = eRipplesSwitch;
		DWORD code = read_dword (addr);
		if (code == 0x33504D8B) code = 0x3390C931;
		else code = 0x33504D8B;
		DWORD oldProtect;
		VirtualProtect ((LPVOID) addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
		write_dword (addr, code);
		VirtualProtect ((LPVOID) addr, 4, oldProtect, &oldProtect);
	}
}/**/

//-----------------------------------------------------------------------------

void MWBridge::TurnOldRipples (BOOL enabled) {
	if (RipplesSwitchable) {
		DWORD addr = eRipplesSwitch;
		DWORD code = read_dword (addr);
		if (enabled && code == 0x33504D8B || !enabled && code == 0x3390C931) return;
		code = (enabled ? 0x33504D8B : 0x3390C931);
		DWORD oldProtect;
		VirtualProtect ((LPVOID) addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
		write_dword (addr, code);
		VirtualProtect ((LPVOID) addr, 4, oldProtect, &oldProtect);
	}
}

//-----------------------------------------------------------------------------

void MWBridge::SetProgressBarName(char *name) {
	__asm{
		mov     eax, name
		push    eax
		mov     eax, 0x005DED20
		call    eax
		add     esp, 4
	}
}

//-----------------------------------------------------------------------------

void MWBridge::ShowProgressBar(float value) {
	__asm{
		mov     eax, value
		push    eax
		mov     eax, 0x004C7D90
		call    eax
		add     esp, 4
	}
}

//-----------------------------------------------------------------------------

void MWBridge::HideProgressBar() {
	__asm{
		mov     eax, 0x005DEEA0
		call    eax
	}
}

//-----------------------------------------------------------------------------

void MWBridge::SetScreen(float value) {
	DWORD address = eMaster1;
	if (address != 0) {
		__asm{
			mov     edx, address
			mov     ecx, [edx+0x354]
			push    value
			mov     eax, 0x004092F0
			call    eax
		}
	}
}

//-----------------------------------------------------------------------------

void MWBridge::UnsetScreen(float value) {
	DWORD address = eMaster1;
	if (address != 0) {
		__asm{
			mov     edx, address
			mov     ecx, [edx+0x354]
			push    value
			mov     eax, 0x00409440
			call    eax
		}
	}
}

//-----------------------------------------------------------------------------
