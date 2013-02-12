#define _MAIN
#include "mge/Configuration.h"
#include "mwse/MWSEmain.h"
#include "mge/Integration.h"
#include "mge/Patch.h"
#include "main.h"
#include <stdio.h>


/****** TODO: Check whether below notes are still valid

//TODO: Check FUNCHASEQUIPEDPART2 works - uses dodgy breakpoint
//TODO: Get rid of the pipe between mwse/mge

//cell water size is incorrect
//multiple water types
//fix shadows

//Render higher quality distant land by only using part of the index buffer in each drawprimitive call
//Stuttering water
//Reflect statics

******/


//-----------------------------------------------------------------------------

HRESULT _stdcall FakeDirectInputCreate (HINSTANCE a, DWORD b, REFIID c, void ** d, void * e) {
	void * ret = 0;

	HMODULE dinput8dll = NULL;	//Handle to the real dinput dll
	DInputProc func = NULL;		//Handle to the real create dinput8 function

	char Path [MAX_PATH];
	GetSystemDirectoryA (Path, MAX_PATH);
	strcat_s (Path, 256, "\\dinput8.dll");
	dinput8dll = ::LoadLibraryA (Path);
	func = (DInputProc) GetProcAddress (dinput8dll, "DirectInput8Create");
	HRESULT hr = (func) (a, b, c, &ret, e);
	if (hr != S_OK) return hr;

	if (!Initialized) *d = ret;
	else *d = CreateInputWrapper (ret);
	return S_OK;
}

//-----------------------------------------------------------------------------

void * _stdcall FakeDirect3DCreate (UINT version) {
	void * ret = 0;
	if (Initialized) ret = CreateD3DWrapper (version);
	if (ret) return ret;

	//not morrowind or hook failed, so load the real d3d8 dll
	HMODULE d3ddll = 0;
	D3DProc func = 0;
	char Path [MAX_PATH];
	GetSystemDirectoryA (Path, MAX_PATH);
	strcat_s (Path, MAX_PATH, "\\d3d8.dll");
	d3ddll = LoadLibraryA (Path);
	func = (D3DProc) GetProcAddress (d3ddll, "Direct3DCreate8");
	return (func) (version);
}

//-----------------------------------------------------------------------------

BOOL _stdcall DllMain (HANDLE hModule, DWORD reason, void * unused) {
	if (reason != DLL_PROCESS_ATTACH) return true;

	MGEFlags = NULL;
	bool iniLoaded = LoadSettings ();
	if (!IsMorrowind ()) return true;

	if (FindWindow (0, GUI_VERSION)) {
		ERRORMSG ("Error: MGEgui is running. Please shut it down before launching morrowind.");
	}

	if (!iniLoaded) {
		ERRORMSG ("Error: MGE is not configured. Please run MGEgui to configure it before launching Morrowind.");
	}
	Initialized = !BitTst (&MGEFlags, MGE_DISABLED_BIT);

	HANDLE MWSEpipe;
	HANDLE MGEpipe;

	if (Initialized) {
		MWSEpipe = CreateNamedPipeA ("\\\\.\\pipe\\MWSEMGEpipe", PIPE_ACCESS_DUPLEX, 
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT,
			PIPE_UNLIMITED_INSTANCES, 4096, 4096, 20000, NULL
		);
		MWSEpipe2 = CreateNamedPipeA ("\\\\.\\pipe\\MWSEMGEpipe2", PIPE_ACCESS_DUPLEX, 
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT,
			PIPE_UNLIMITED_INSTANCES, 4096, 4096, 20000, NULL
		);
		if (MWSEpipe == INVALID_HANDLE_VALUE || MWSEpipe2 == INVALID_HANDLE_VALUE) {
			LOG::log ("Failed to create mge-mwse pipe\r\n");
			return false;
		}
		MGEpipe = CreateFile ("\\\\.\\pipe\\MWSEMGEpipe", GENERIC_WRITE|GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
		MGEpipe2 = CreateFile ("\\\\.\\pipe\\MWSEMGEpipe2", GENERIC_WRITE|GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
		if (MGEpipe == INVALID_HANDLE_VALUE || MGEpipe2 == INVALID_HANDLE_VALUE) {
			LOG::log ("Failed to open mge-mwse pipe\r\n");
			return false;
		}
	} else MWSEpipe = INVALID_HANDLE_VALUE;

	if (MGEFlags & MWSE_DISABLED) LOG::log ("MWSE: Disabled\r\n");
	else MWSEOnProcessStart (MWSEpipe);
	if (Initialized) {
		MGEOnProcessStart (MGEpipe);
		DInputOnProcessStart ();
	} else LOG::log ("MGE: Disabled\r\n");

	return true;
}

//-----------------------------------------------------------------------------

BOOL IsMorrowind () {
	if (ItsMorrowind) return false;
	BOOL status = false;
	HINSTANCE hInstance = GetModuleHandleA (NULL);

	char *origFilename = new char[MAX_PATH];
	MWVersion = GetModuleNameAndVersion(hInstance, origFilename, MAX_PATH);
	if (!_stricmp (origFilename, "Morrowind.exe")) {
		LOG::open ("mwse-mge log.txt");
		LOG::log (WelcomeMessage);
		LOG::log ("DLL injected\r\n");
		if (!MWVersion) {
			ERRORMSG ("Unable to get Morrowind's version number.");
		}
		ItsMorrowind = status = true;
	}
	if (!PatchDisabled) lpPatchTree = PatchTree::BuildPatchTree();
	if (ItsMorrowind) ForceTextDll(GetModuleHandle(TEXT_DLL_NAME));
	if (strstr(origFilename, ".") == NULL) strcat_s(origFilename, MAX_PATH, ".exe");
	if (!IntegrationDisabled) Integration(hInstance, origFilename, MWVersion, ItsMorrowind);
	if (!PatchDisabled) PatchProcessMemory(hInstance, origFilename, MWVersion, ItsMorrowind);
	delete [] origFilename;

	return status;
}

//-----------------------------------------------------------------------------
