#pragma once

#include "../mwse/WinHeader.h"

#define	MWE_DLL_PATH	"mge3/Morrowind Enhanced.dll"
#define	REGSK_MW		"Software\\Bethesda Softworks\\Morrowind"
#define	REGSK_MWE		"Software\\Morrowind Enhanced"

typedef bool (_cdecl *TRANSFERDATA)(unsigned,unsigned,HWND,unsigned*,char*,unsigned*,int);
typedef bool (_cdecl *INJECTDLL)(HWND);

bool InjectMWE(HWND hMorrowindWindow);
