#pragma once

#include "../mwse/WinHeader.h"

#define TEXT_DLL_NAME	"Text.dll"
#define MWE_DLL_NAME	"Morrowind Enhanced.dll"

struct VS_LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
};

static const char vvfiTranslation [] = "\\VarFileInfo\\Translation";

static const char vsfiOriginalFilename [] = "\\StringFileInfo\\%04x%04x\\OriginalFilename";

typedef BOOL (_stdcall *DllEntryPoint)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

DWORD GetModuleNameAndVersion (HINSTANCE hInstance, LPSTR lpOriginalFilename, DWORD dwBufferSize);

DllEntryPoint GetEntryPoint(HINSTANCE hModule);

POINT GetNativeResolution();
