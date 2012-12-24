#include "ModuleInformation.h"
#include "Configuration.h"
#include "MWEImproving.h"
#include "assert.h"
#include "Patch.h"

LPSTR lpJournalText = NULL;

void PatchMWE(HINSTANCE hMWEdll) {
	if(!PatchDisabled) {
		PatchModuleMemory(hMWEdll, MWE_DLL_NAME, true);
	}
}

//-----------------------------------------------------------------------------

DWORD _stdcall GetJournalEntrySize(LPDWORD lpStack) {
	if(lpJournalText) return strlen(lpJournalText);
	return *(lpStack + 0x93);
}

//-----------------------------------------------------------------------------

bool ConnectWithTextDll(HINSTANCE hMWEdll) {
	LPVOID translator_address = LPVOID(0x0064B31A);
	DWORD translator_function = NULL;
	if(!MachineCode::GetCode(translator_address, LPBYTE(&translator_function), sizeof(DWORD))) return false;
	if(translator_function == 0xC033FFC9) return false;			// EnglishInput=1

	DWORD instance = (DWORD)hMWEdll;
	DWORD address1 = 0x00008EBA;
	BYTE original1[] = {
						0x88, 0x04, 0x2E,						// mov     [esi+ebp], al
						0x46,									// inc     esi
						0xEB, 0x0C								// jmp     short loc_10008ECC
						};

	int size = (3 * sizeof(BYTE) + 2 * sizeof(WORD) + 3 * sizeof(DWORD)) + (3 + 1);
	int align = MachineCode::GetAlignLength(size, 0x10);

	LPBYTE code = new BYTE[size += align];
	LPBYTE lpByte = code;
	LPWORD& lpWord = (LPWORD&)lpByte;
	LPDWORD& lpDword = (LPDWORD&)lpByte;

	*(lpByte++) = 0x25;											// and     eax, 0FFh
	*(lpDword++) = 0x000000FF;									//
	*(lpWord++) = 0xD88B;										// mov     ebx, eax
	*(lpByte++) = 0xE8;											// call    translator
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(0 - MachineCode::GetLabelOffset(translator_address, LPDWORD(0 - translator_function))));
	*(lpWord++) = 0xC38B;										// mov     eax, ebx

	memcpy_s(lpByte, size, original1, (3 + 1));
	lpByte += (3 + 1);
	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp     loc_10008ECC
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance  +  0x00008ECC));
	// Aligning:
	for(int i = 0; i < align; i++) *(lpByte++) = 0x90;			// align 10h
	assert(lpByte <= code + size);
	return MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address1), original1, sizeof(original1), code, size);
}

//-----------------------------------------------------------------------------

bool ImproveJournalUpdate(HINSTANCE hMWEdll) {
	DWORD instance = (DWORD)hMWEdll;
	DWORD address0 = 0x00009F9D;								// Morrowind Enhanced.dll
	BYTE original0[] = {
						0x8B, 0x73, 0x3C,						// mov     esi, [ebx+3Ch]
						0x3B, 0xF7								// cmp     esi, edi
						};
	int size0 = (1 * sizeof(BYTE) + 1 * sizeof(WORD) + 2 * sizeof(DWORD));
	int align0 = MachineCode::GetAlignLength(size0, 0x10);
	LPBYTE code0 = new BYTE[size0 += align0];
	LPBYTE lpByte = code0;
	LPWORD& lpWord = (LPWORD&)lpByte;
	LPDWORD& lpDword = (LPDWORD&)lpByte;

	*(lpWord++) = 0x3D89;										// mov     lpJournalText, edi
	*(lpDword++) = DWORD(&lpJournalText);						//
	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp     loc_10009F6D)
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword,  LPDWORD(instance  +  0x0009F6D));
	// Aligning:
	for(int i = 0; i < align0; i++) *(lpByte++) = 0x90;			// align 10h
	assert(lpByte <= code0 + size0);
	//---------------------------------
	DWORD address1 = 0x0000A02F;								// Morrowind Enhanced.dll
	BYTE original1[] = {
						0x8B, 0x44, 0x24, 0x20,					// mov     eax, [esp+20h]
						0x8B, 0xCB								// mov     ecx, ebx
						};
	
	int size1 = (3 * sizeof(BYTE) + 2 * sizeof(WORD) + 3 * sizeof(DWORD)) + (500);
	int align1 = MachineCode::GetAlignLength(size1, 0x10);

	LPBYTE code1 = new BYTE[size1 += align1];
	lpByte = code1;

	*(lpDword++) = 0x903C738B;									// mov     esi, [ebx+3Ch]
	*(lpWord++) = 0x368B; 										// mov     esi, [esi]
	*(lpWord++) = 0x3589;										// mov     lpJournalText, esi
	*(lpDword++) = DWORD(&lpJournalText);						//

	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp     loc_1000A095
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword,  LPDWORD(instance  +  0x000A095));
	// Aligning:
	for(int i = 0; i < align1; i++) *(lpByte++) = 0x90;			// align 10h
	assert(lpByte <= code1 + size1);
	//----------------------------------
	DWORD address2 = 0x000B1D47;								// Morrowind.exe
	BYTE original2[] = {
						0x8B,0x84,0x24,0x4C,0x02,0x00,0x00		// mov     eax, [esp+24Ch]
						};
	LPBYTE code2 = new BYTE[sizeof(original2)];
	lpByte = code2;
	*(lpByte++) = 0x54;											// push    esp
	*(lpByte++) = 0xE8;											// call    GetJournalEntrySize
	*(lpDword++) = MachineCode::GetLabelOffset(LPBYTE(0x400000 + address2 + lpByte - code2), GetJournalEntrySize);
	*(lpByte++) = 0x90;											// nop
	assert(lpByte == code2 + sizeof(original2));
	//---------------------------------
	DWORD address3 = 0x000B1D70;								// Morrowind.exe
	BYTE original3[] = {
						0xE8, 0x0B, 0x4B, 0x00, 0x00			// call    sub_4B6880
						};
	int size3 = (3 * sizeof(BYTE) + 2 * sizeof(WORD) + 3 * sizeof(DWORD)) + (500);
	int align3 = MachineCode::GetAlignLength(size3, 0x10);
	LPBYTE code3 = new BYTE[size3 += align3];
	lpByte = code3;

	*(lpByte++) = 0xA1;											// mov     eax, lpJournalText
	*(lpDword++) = DWORD(&lpJournalText);						//
	*(lpWord++) = 0xC085;										// test    eax, eax
	*(lpWord++) = 0x0A75;										// jnz     short 0A
	*(lpByte++) = 0xE8;											// call    sub_4B6880
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(0x400000  + 0x00B6880));
	*(lpByte++) = 0xE9;											// jmp    (address3 + sizeof(original3))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(0x400000 + address3 + sizeof(original3)));
	///////////////////////////
	*(lpDword++) = 0x2847C790;									// mov     dword ptr [edi+28h], 0
	*(lpDword++) = 0x00000000;									// 
	*(lpDword++) = 0x04244489;									// mov     [esp+4], eax
	*(lpWord++) = 0x15FF;										// call    ds:lstrcpyA
	*(lpDword++) = (0x400000 + 0x00346054);						// 
	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp    (address3 + sizeof(original3))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(0x400000 + address3 + sizeof(original3)));
	// Aligning:
	for(int i = 0; i < align3; i++) *(lpByte++) = 0x90;			// align 10h
	assert(lpByte <= code3 + size3);
	//---------------------------------
	DWORD address5 = 0x00009B71;								// Morrowind Enhanced.dll
	BYTE original5[] = {
						0xC6, 0x44, 0x01, 0xFF, 0x20			// mov     byte ptr [ecx+eax-1], 20h
						};
	//---------------------------------
	DWORD address4 = 0x00008EF7;								// Morrowind Enhanced.dll
	BYTE original4[] = {
						0x8D, 0x44, 0x0E, 0x64,					// lea     eax, [esi+ecx+64h]
						0x5F									// pop     edi
						};
	int size4 = (3 * sizeof(BYTE) + 2 * sizeof(WORD) + 3 * sizeof(DWORD)) + (500);
	int align4 = MachineCode::GetAlignLength(size4, 0x10);
	LPBYTE code4 = new BYTE[size4 += align4];
	lpByte = code4;

	*(lpDword++) = 0x002E04C6;									// mov     byte ptr [esi+ebp], 0
	
	memcpy_s(lpByte, size4, original4, sizeof(original4));
	lpByte += sizeof(original4);

	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp    (address4 + sizeof(original4))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + address4 + sizeof(original4)));

	// Aligning:
	for(int i = 0; i < align4; i++) *(lpByte++) = 0x90;			// align 10h
	assert(lpByte <= code4 + size4);
	//---------------------------------
	if( MachineCode::CompareCode (LPVOID(0x400000 + address2), original2, sizeof(original2)) &&
		MachineCode::CompareCode (LPVOID(instance + address5), original5, sizeof(original5))) {
		original5[4] = 0;
		return	MachineCode::SetCode(LPVOID(0x400000 + address2), code2, sizeof(original2)) &&
//				MachineCode::SetCode(LPVOID(instance + address5), original5, sizeof(original5)) &&
				MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address4), original4, sizeof(original4), code4, size4) &&
				MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(0x400000 + address3), original3, sizeof(original3), code3, size3) &&
				MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address1), original1, sizeof(original1), code1, size1) &&
				MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address0), original0, sizeof(original0), code0, size0);
	}
	return false;
}

//-----------------------------------------------------------------------------

void ImproveMWE(HINSTANCE hMWEdll) {
	if(GetModuleHandle(TEXT_DLL_NAME)) ConnectWithTextDll(hMWEdll);	// Provides cyrillic input together with MWE.
	ImproveJournalUpdate(hMWEdll);
}
