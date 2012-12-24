#pragma once
#ifdef _MAIN

#include "mge/ModuleInformation.h"

static const char WelcomeMessage [] =
	"Morrowind Script Extender v0.9.3\r\n"
	VERSION "\r\n"
	"MWSE Copyright (C) 2003-2007 FreshFish copyleft GPL\r\n"
	"\r\nThis program is free software, licensed according to the GNU Public License,\r\n"
	"and comes with ABSOLUTELY NO WARRANTY.  Visit http://www.gnu.org/copyleft/gpl.html for details.\r\n"
	"MWSE contributers: cdcooley, Fliggerty, Tp21, Timeslip and probably others\r\n"
	"MGE contributers: Timeslip, LizTail, Scanti, krzymar, phal, peachykeen, d4w\r\n"
	"\r\n";


//Tempory, until updated versions of old MGE mods have been released
HANDLE MGEpipe2;
HANDLE MWSEpipe2;

static bool Initialized = false;
static bool ItsMorrowind = false;
DWORD MWVersion = 0;

BOOL IsMorrowind ();
extern void * CreateD3DWrapper (UINT);
extern void MGEOnProcessStart (HANDLE pipe);
extern void * CreateInputWrapper (void *);
extern void DInputOnProcessStart();

typedef void * (_stdcall * D3DProc) (UINT version);
typedef HRESULT (_stdcall * DInputProc) (HINSTANCE a, DWORD b, REFIID c, void ** d, void * e);

#endif
