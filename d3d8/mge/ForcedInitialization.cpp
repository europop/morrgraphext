#include "Patch.h"
#include "Configuration.h"
#include "ModuleInformation.h"
#include "ForcedInitialization.h"

//-----------------------------------------------------------------------------

static HINSTANCE TextDll = NULL;
static HINSTANCE Msvcp60 = NULL;

static UINT64 InitialTextDll = 0;
static UINT64 InitialMsvcp60 = 0;

static const UINT64 EmptyDLLMain = 0x90000CC201B0C033;          // xor     eax, eax
                                                                // mov     al, 1
                                                                // retn    0Ch

//-----------------------------------------------------------------------------

bool ModifyDLLEntryPoint(HINSTANCE hModule, LPBYTE lpNewCode, LPBYTE outInitialCode, DWORD dwLength) {
	bool success = false;
	LPVOID address;
	if(hModule && lpNewCode && outInitialCode && dwLength && (address = GetEntryPoint(hModule))) {
		if(MachineCode::GetCode(address, outInitialCode, dwLength))
			success = MachineCode::SetCode(address, lpNewCode, dwLength);
	}
	return success;
}

bool RestoreDLLEntryPoint(HINSTANCE hModule, LPBYTE lpInitialCode, DWORD dwLength) {
	bool success = false;
	LPVOID address;
	if(hModule && lpInitialCode && dwLength && (address = GetEntryPoint(hModule))) {
		success = MachineCode::SetCode(address, lpInitialCode, dwLength);
	}
	return success;
}

//-----------------------------------------------------------------------------

bool _stdcall RestoreMsvcpAndTextDll() {
	bool success = false;
	if (TextDll && Msvcp60 && InitialTextDll && InitialMsvcp60) {
		success = RestoreDLLEntryPoint(Msvcp60, LPBYTE(&InitialMsvcp60), sizeof(UINT64)) &&
				  RestoreDLLEntryPoint(TextDll, LPBYTE(&InitialTextDll), sizeof(UINT64));
		InitialMsvcp60 = 0;
		InitialTextDll = 0;
	}
	return success;
}

//-----------------------------------------------------------------------------

bool ForceTextDll(HINSTANCE hTextDll) {
	if(hTextDll) TextDll = hTextDll; else return true;
	// Provides compatibility with Russian dialog engine.
	Msvcp60 = GetModuleHandle("msvcp60.dll");
	DllEntryPoint Msvcp60Main = GetEntryPoint(Msvcp60);
	DllEntryPoint TextDllMain = GetEntryPoint(TextDll);
	if(TextDll && Msvcp60 && Msvcp60Main && TextDllMain) {
		if(!PatchDisabled) PatchModuleMemory(TextDll, TEXT_DLL_NAME, true);
		if(!Msvcp60Main(Msvcp60, DLL_PROCESS_ATTACH, (LPVOID)1)) return false;
		if(!ModifyDLLEntryPoint(Msvcp60, LPBYTE(&EmptyDLLMain), LPBYTE(&InitialMsvcp60), sizeof(UINT64))) return false;
		if(!TextDllMain(TextDll, DLL_PROCESS_ATTACH, (LPVOID)1)) return false;
		if(!ModifyDLLEntryPoint(TextDll, LPBYTE(&EmptyDLLMain), LPBYTE(&InitialTextDll), sizeof(UINT64))) return false;
		int size = (8 * sizeof(BYTE) + 2 * sizeof(DWORD));
		LPBYTE code = new BYTE[size];
		LPBYTE lpByte = code;
		LPWORD& lpWord = (LPWORD&)lpByte;
		LPDWORD& lpDword = (LPDWORD&)lpByte;
		// Save registers before calling:
		*(lpByte++) = 0x51;										// push    ecx
		*(lpByte++) = 0x52;										// push    edx
		*(lpByte++) = 0x53;										// push    ebx
		// Calling function:
		*(lpByte++) = 0xE8;										// call    RestoreMsvcpAndTextDll
		*(lpDword++) = MachineCode::GetLabelOffset(lpDword, RestoreMsvcpAndTextDll);
		// Restore the registers:
		*(lpByte++) = 0x5B;										// pop     ebx
		*(lpByte++) = 0x5A;										// pop     edx
		*(lpByte++) = 0x59;										// pop     ecx
		*(lpDword++) = 0x90000CC2;								// retn    0Ch
		// Aligning:
		*(lpByte++) = 0x90;										// nop
		return MachineCode::CompareAndPatchByJumpToAttachment(TextDllMain, LPBYTE(&EmptyDLLMain), sizeof(UINT64), code, size);
	}
	return false;
}
