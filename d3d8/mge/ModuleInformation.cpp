#include "ModuleInformation.h"
#include <stdio.h>

DWORD GetModuleNameAndVersion (HINSTANCE hInstance, LPSTR lpOriginalFilename, DWORD dwBufferSize) {

	DWORD dwVersionLS = NULL;
	HRSRC hVersionInfoRsrc = NULL;
	if(lpOriginalFilename && dwBufferSize) {
		strcpy_s(lpOriginalFilename, dwBufferSize, "");
		hVersionInfoRsrc = FindResourceEx(hInstance, RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO), MAKELANGID (LANG_NEUTRAL, SUBLANG_NEUTRAL));
	}
	if (hVersionInfoRsrc) {
		UINT size = *((LPUINT) hVersionInfoRsrc + 1);
		HGLOBAL hVersionInfoData = LoadResource (hInstance, hVersionInfoRsrc);
		if (hVersionInfoData) {
			LPVOID pVersionInfo = LockResource (hVersionInfoData);
			if (pVersionInfo) {
				UINT len;
				LPVOID lpBuffer;
				DWORD oldProtect;
				VirtualProtect (pVersionInfo, size, PAGE_READWRITE, &oldProtect);
				if (VerQueryValueA (pVersionInfo, vvfiTranslation, &lpBuffer, &len)) {
					char vsfiOrigFname [sizeof (vsfiOriginalFilename)];
					sprintf_s (vsfiOrigFname, sizeof (vsfiOrigFname), vsfiOriginalFilename,
						((VS_LANGANDCODEPAGE *) lpBuffer)->wLanguage,
						((VS_LANGANDCODEPAGE *) lpBuffer)->wCodePage
					);
					if (VerQueryValueA (pVersionInfo, vsfiOrigFname, &lpBuffer, &len)) {
						if(lpOriginalFilename && dwBufferSize) {
							strcpy_s(lpOriginalFilename, dwBufferSize, (char *) lpBuffer);
						}
						if (VerQueryValueA (pVersionInfo, "\\", &lpBuffer, &len)) {
							dwVersionLS = ((VS_FIXEDFILEINFO *) lpBuffer)->dwFileVersionLS;
						}
					}
				}
				VirtualProtect (pVersionInfo, size, oldProtect, &oldProtect);
			}
		}
	}
	return dwVersionLS;
}

DllEntryPoint GetEntryPoint(HINSTANCE hModule) {
	if (hModule) {
		PIMAGE_DOS_HEADER pDOSHead = (PIMAGE_DOS_HEADER)hModule;
		if(pDOSHead->e_magic == IMAGE_DOS_SIGNATURE ) {
			PIMAGE_NT_HEADERS pPEHeader = PIMAGE_NT_HEADERS(LPBYTE(pDOSHead) + PIMAGE_DOS_HEADER(pDOSHead)->e_lfanew);
			if(pPEHeader->Signature == IMAGE_NT_SIGNATURE) {
				return DllEntryPoint(pPEHeader->OptionalHeader.AddressOfEntryPoint + (DWORD)hModule);
			}
		}
	}
	return NULL;
}

POINT GetNativeResolution() {
	POINT resolution;
	DISPLAY_DEVICE lpDisplayDevice = {0};
	lpDisplayDevice.cb = sizeof(DISPLAY_DEVICE);
	EnumDisplayDevices(NULL, /**/0/**/, &lpDisplayDevice, NULL);	/*	Get Adapter from Morrowind registry */
	HDC hdc = CreateDC(/*"DISPLAY"*/lpDisplayDevice.DeviceName, NULL, NULL, NULL);
	resolution.x = GetDeviceCaps(hdc, HORZRES);
	resolution.y = GetDeviceCaps(hdc, VERTRES);
	DeleteDC(hdc);
	return resolution;
}
