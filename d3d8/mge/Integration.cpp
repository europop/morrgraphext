#include "DistantLand.h"
#include "Configuration.h"
#include "MME function defs.h"
#include "ModuleInformation.h"
#include "Patch.h"
#include "MWECompiler.h"
#include "Integration.h"
#include "assert.h"

//-----------------------------------------------------------------------------

bool IntegrateMWECompiler(HINSTANCE hInstance, DWORD dwVersionLS) {
	if(dwVersionLS != DEFAULTFILEVER) return false;

	DWORD instance = (DWORD)hInstance;	
	DWORD address1 = 0x0002CC03;
	BYTE original1[] = {
						0x8B, 0x47, 0x4C,						// mov     eax, [edi+4Ch]
						0x8B, 0x4F, 0x58						// mov     ecx, [edi+58h]
						};

	int size = sizeof(original1) + (14 * sizeof(BYTE) + 3 * sizeof(DWORD));
	LPBYTE code = new BYTE[size];

	LPBYTE lpByte = code;
	LPWORD& lpWord = (LPWORD&)lpByte;
	LPDWORD& lpDword = (LPDWORD&)lpByte;

	memcpy_s(lpByte, size, original1, sizeof(original1));
	lpByte += sizeof(original1);

	// Save registers before calling:
	*(lpByte++) = 0x50;											// push    eax
	*(lpByte++) = 0x51;											// push    ecx
	*(lpByte++) = 0x52;											// push    edx
	*(lpByte++) = 0x53;											// push    ebx

	*(lpDword++) = 0x903877FF;									// push    dword ptr [edi+38h]
	*(lpByte++) = 0x51;											// push    ecx
	*(lpByte++) = 0xE8;											// call    MWECompiler
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, MWECompiler);

	// Restore the registers:
	*(lpByte++) = 0x5B;											// pop     ebx
	*(lpByte++) = 0x5A;											// pop     edx
	*(lpByte++) = 0x59;											// pop     ecx
	*(lpByte++) = 0x58;											// pop     eax

	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp    (address1 + sizeof(original1))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + address1 + sizeof(original1)));

	// Aligning:
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	
	assert(lpByte <= code + size);
	return MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address1), original1, sizeof(original1), code, size);
}

//-----------------------------------------------------------------------------

bool IntegrateDialogFix(HINSTANCE hInstance, DWORD dwVersionLS) {
	if(dwVersionLS != DEFAULTFILEVER) return false;

	DWORD instance = (DWORD)hInstance;	
	DWORD address1 = 0x000F276F;
	BYTE original1[] = {
						0x74, 0x61								//
						};
	if(MachineCode::CompareCode(LPVOID(address1 + instance), original1, sizeof(original1))) {
		BYTE jmpshort = 0xEB;
		return MachineCode::SetCode(LPVOID(address1 + instance), &jmpshort, sizeof(BYTE));
	}
	return false;
}

//-----------------------------------------------------------------------------

bool IntegrateDLProgress(HINSTANCE hInstance, DWORD dwVersionLS) {
	if(dwVersionLS != DEFAULTFILEVER) return false;

	DWORD instance = (DWORD)hInstance;
	DWORD address1 = 0x0005D2AC;
	BYTE original1[] = {
						0x8B, 0x8A, 0x90, 0x00, 0x00, 0x00		// mov     ecx, [edx+90h]
						};

	int size = (18 * sizeof(BYTE) + 0 * sizeof(WORD) + 2 * sizeof(DWORD)) + sizeof(original1);
	LPBYTE code = new BYTE[size];

	LPBYTE lpByte = code;
	LPWORD& lpWord = (LPWORD&)lpByte;
	LPDWORD& lpDword = (LPDWORD&)lpByte;

	// Save registers before calling:
	*(lpByte++) = 0x50;											// push    eax
	*(lpByte++) = 0x51;											// push    ecx
	*(lpByte++) = 0x52;											// push    edx
	*(lpByte++) = 0x53;											// push    ebx

	*(lpByte++) = 0xE8;											// call    DL_Initialize
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, DL_Initialize);

	// Restore the registers:
	*(lpByte++) = 0x5B;											// pop     ebx
	*(lpByte++) = 0x5A;											// pop     edx
	*(lpByte++) = 0x59;											// pop     ecx
	*(lpByte++) = 0x58;											// pop     eax

	memcpy_s(lpByte, size, original1, sizeof(original1));
	lpByte += sizeof(original1);

	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp    (address1 + sizeof(original1))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + address1 + sizeof(original1)));

	// Aligning:
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop

	assert(lpByte <= code + size);
	return MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address1), original1, sizeof(original1), code, size);
}

//-----------------------------------------------------------------------------

bool IntegrateDLButton(HINSTANCE hInstance, DWORD dwVersionLS) {
	if(dwVersionLS != DEFAULTFILEVER) return false;

	LPSTR lpUseDLlabel = "Distant Land";

	DWORD instance = (DWORD)hInstance;
	DWORD address1 = 0x00237DE0;
	BYTE original1[] = {
						0x6A, 0x10,								// push    10h
						0xB8, 0xCC, 0x80, 0xFF, 0xFF			// mov     eax, 0FFFF80CCh
						};

	int size = (70 * sizeof(BYTE) + 36 * sizeof(WORD) + 53 * sizeof(DWORD)) + sizeof(original1);
	LPBYTE code = new BYTE[size];

	LPBYTE lpByte = code;
	LPWORD& lpWord = (LPWORD&)lpByte;
	LPDWORD& lpDword = (LPDWORD&)lpByte;

	*(lpByte++) = 0x68;											// push    offset MenuPrefs_DistantLandbutton
	*(lpDword++) = (DWORD)"MenuPrefs_DistantLandbutton";		//
	*(lpByte++) = 0xE8;											// call    sub_58DF10
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0018DF10));
	*(lpDword++) = 0x9004C483;									// add     esp, 4
	*(lpByte++) = 0x25;											// and     eax, 0FFFFh
	*(lpDword++) = 0x0000FFFF;									//
	*(lpWord++) = 0x006A;										// push    0
	*(lpByte++) = 0x68;											// push    offset sub_639420
	*(lpDword++) = instance + 0x00239420;						//
	*(lpWord++) = 0xCE8B;										// mov     ecx, esi
	*(lpByte++) = 0x50;											// push    eax
	*(lpByte++) = 0xE8;											// call    sub_588140
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x00188140));
	*(lpWord++) = 0x206A;										// push    20h
	*(lpWord++) = 0xF88B;										// mov     edi, eax
	*(lpByte++) = 0x68;											// push    offset bDLhandler
	LPDWORD bDLhandler = lpDword++;	// Will be set below		//
	*(lpByte++) = 0x68;											// push    0FFFF8035h
	*(lpDword++) = 0xFFFF8035;									//
	*(lpWord++) = 0xCF8B;										// mov     ecx, edi
	*(lpByte++) = 0xE8;											// call    sub_581F30
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x00181F30));
	*(lpByte++) = 0xA1;											// mov     eax, dword ptr [&MGEFlags]
	*(lpDword++) = DWORD(&MGEFlags);							//
	*(lpByte++) = 0x25;											// and     eax, USE_DISTANT_LAND
	*(lpDword++) = USE_DISTANT_LAND;							//
	*(lpWord++) = 0xE8C1;										// shr     eax, USE_DISTANT_LAND_BIT
	*(lpByte++) = USE_DISTANT_LAND_BIT;							//
	*(lpDword++) = 0x9001F083;									// xor     eax, 1
	*(lpDword++) = 0x9046C083;									// add     eax, 46h
	*(lpByte++) = 0x50;											// push    eax
	*(lpByte++) = 0xE8;											// call    sub_40F930
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0000F930));
	*(lpByte++) = 0x50;											// push    eax
	*(lpWord++) = 0xCF8B;										// mov     ecx, edi
	*(lpByte++) = 0xE8;											// call    sub_58AD30
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0018AD30));
	*(lpWord++) = 0x006A;										// push    0
	*(lpWord++) = 0x006A;										// push    0
	*(lpWord++) = 0x87C7;										// mov     dword ptr [edi+0B4h], 2
	*(lpDword++) = 0x000000B4;									//
	*(lpDword++) = 0x00000002;									//
	*(lpWord++) = 0x87C7;										// mov     dword ptr [edi+0C4h], 5
	*(lpDword++) = 0x000000C4;									//
	*(lpDword++) = 0x00000005;									//
	*(lpWord++) = 0x87C7;										// mov     dword ptr [edi+0BCh], 8
	*(lpDword++) = 0x000000BC;									//
	*(lpDword++) = 0x00000008;									//
	*(lpWord++) = 0x0D8B;										// mov     ecx, dword_7C67DC
	*(lpDword++) = instance + 0x003C67DC;						//
	*(lpByte++) = 0xB8;											// mov     eax, lpUseDLlabel
	*(lpDword++) = (DWORD)lpUseDLlabel;
	*(lpByte++) = 0x50;											// push    eax
	*(lpByte++) = 0x68;											// push    0FFFF80B4h
	*(lpDword++) = 0xFFFF80B4;									//
	*(lpWord++) = 0xCE8B;										// mov     ecx, esi
	*(lpByte++) = 0xE8;											// call    sub_588BE0
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x00188BE0));
	*(lpWord++) = 0x006A;										// push    0
 	*(lpByte++) = 0xBF;											// mov     edi, 2
	*(lpDword++) = 0x00000002;									//
	*(lpByte++) = 0x68;											// push    0FFFF80B4h
	*(lpDword++) = 0xFFFF80B4;									//
	*(lpWord++) = 0xCE8B;										// mov     ecx, esi
	*(lpWord++) = 0x80C7;										// mov     dword ptr [eax+0C4h], 5
	*(lpDword++) = 0x000000C4;									//
	*(lpDword++) = 0x00000005;									//
	*(lpWord++) = 0xB889;										// mov     [eax+0B4h], edi
	*(lpDword++) = 0x000000B4;									//
	*(lpByte++) = 0xE8;											// call    sub_588980
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x00188980));
	*(lpWord++) = 0xF08B;										// mov     esi, eax
	*(lpWord++) = 0x016A;										// push    1
	*(lpWord++) = 0xCE8B;										// mov     ecx, esi
	*(lpByte++) = 0xBF;											// mov     edi, 3F800000h
	*(lpDword++) = 0x3F800000;									//
	*(lpWord++) = 0xBE89;										// mov     [esi+118h], edi
	*(lpDword++) = 0x00000118;									//
	*(lpByte++) = 0xE8;											// call    sub_581400
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x00181400));

	memcpy_s(lpByte, size, original1, sizeof(original1));
	lpByte += sizeof(original1);

	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp    (address1 + sizeof(original1))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + address1 + sizeof(original1)));

	// Aligning:
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop

	//////////////////////////////////////////////////////////////
	// DistantLand menu button click:
	*bDLhandler = (DWORD)lpByte;
	
	*(lpByte++) = 0x56;											// push    esi
	*(lpDword++) = 0x0824748B;									// mov     esi, [esp+8]
	*(lpWord++) = 0xF685;										// test    esi, esi
	*(lpWord++) = 0x0475;										// jnz     short loc_+4
	*(lpWord++) = 0x01B0;										// mov     al, 1
	*(lpByte++) = 0x5E;											// pop     esi
	*(lpByte++) = 0xC3;											// retn
	*(lpByte++) = 0x57;											// push    edi
	*(lpWord++) = 0xCE8B;										// mov     ecx, esi
	*(lpByte++) = 0xE8;											// call    sub_582EF0
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x00182EF0));
	*(lpWord++) = 0xF88B;										// mov     edi, eax
	*(lpWord++) = 0xCF8B;										// mov     ecx, edi
	*(lpByte++) = 0xE8;											// call    sub_583B60
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x00183B60));
	*(lpByte++) = 0xA1;											// mov     eax, dword ptr [&MGEFlags]
	*(lpDword++) = DWORD(&MGEFlags);							//
	*(lpByte++) = 0x25;											// and     eax, USE_DISTANT_LAND
	*(lpDword++) = USE_DISTANT_LAND;							//
	*(lpWord++) = 0xE8C1;										// shr     eax, USE_DISTANT_LAND_BIT
	*(lpByte++) = USE_DISTANT_LAND_BIT;							//
	
	// Save registers before calling:
	*(lpByte++) = 0x50;											// push    eax
	*(lpByte++) = 0x51;											// push    ecx
	*(lpByte++) = 0x52;											// push    edx
	*(lpByte++) = 0x53;											// push    ebx

	*(lpWord++) = 0x05C6;										// mov     byte ptr [&InvertUsingDL], 1
	*(lpDword++) = DWORD(&InvertUsingDL);						//
	*(lpByte++) = 1;											//

	//	Set Pixelshader to 1:
	*(lpByte++) = 0xA1;											// mov     eax, dword_7C6CDC
	*(lpDword++) = instance + 0x003C6CDC;						//
	*(lpDword++) = 0x012040C6;									// mov     byte ptr [eax+20h], 1

	//	Recreating:
	*(lpByte++) = 0xA1;											// mov     eax, dword_7C67DC
	*(lpDword++) = instance + 0x003C67DC;						//
	*(lpWord++) = 0x888B;										// mov     ecx, [eax+80h]
	*(lpDword++) = 0x00000080;									//
	*(lpDword++) = 0x907C508B;									// mov     edx, [eax+7Ch]
	*(lpDword++) = 0x9078408B;									// mov     eax, [eax+78h]
	*(lpByte++) = 0x51;											// push    ecx
	*(lpWord++) = 0x0D8B;										// mov     ecx, dword_7C6CDC
	*(lpDword++) = instance + 0x003C6CDC;						//
	*(lpByte++) = 0x52;											// push    edx
	*(lpByte++) = 0x50;											// push    eax
	*(lpByte++) = 0xE8;											// call    sub_41A700
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0001A700));

	// Restore the registers:
	*(lpByte++) = 0x5B;											// pop     ebx
	*(lpByte++) = 0x5A;											// pop     edx
	*(lpByte++) = 0x59;											// pop     ecx
	*(lpByte++) = 0x58;											// pop     eax

	*(lpDword++) = 0x9046C083;									// add     eax, 46h
	*(lpByte++) = 0x50;											// push    eax
	*(lpByte++) = 0xE8;											// call    sub_40F930
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0000F930));
	*(lpByte++) = 0x50;											// push    eax
	*(lpWord++) = 0xCE8B;										// mov     ecx, esi
	*(lpByte++) = 0xE8;											// call    sub_58AD30
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0018AD30));
	*(lpWord++) = 0x016A;										// push    1
	*(lpWord++) = 0xCF8B;										// mov     ecx, edi
	*(lpByte++) = 0xE8;											// call    sub_583B70
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x00183B70));
	*(lpByte++) = 0x5F;											// pop     edi
	*(lpWord++) = 0x01B0;										// mov     al, 1
	*(lpByte++) = 0x5E;											// pop     esi
	*(lpByte++) = 0xC3;											// retn

	// Aligning:
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop

	assert(lpByte <= code + size);
	return MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address1), original1, sizeof(original1), code, size);
}

//-----------------------------------------------------------------------------

bool IntegrateScreenshot(HINSTANCE hInstance, DWORD dwVersionLS) {
	if(dwVersionLS != DEFAULTFILEVER) return false;

	DWORD instance = (DWORD)hInstance;
	DWORD address1 = 0x0001B098;
	BYTE original1[] = {
						0xE8, 0x83, 0x35, 0x01, 0x00			// call    sub_42E620
						};
	// Calling TakeScreenshot instead of sub_42E620
	if(MachineCode::CompareCode(LPVOID(address1 += instance), original1, sizeof(original1))) {
		instance = MachineCode::GetLabelOffset(LPVOID(address1 + 1), TakeScreenshot);
		return MachineCode::SetCode(LPVOID(address1 + 1), (LPBYTE)&instance, sizeof(DWORD));
	}
	return false;
}

//-----------------------------------------------------------------------------

bool IntegrateNativeResolution(HINSTANCE hInstance, DWORD dwVersionLS) {
	if(dwVersionLS != DEFAULTFILEVER) return false;

	POINT resolution = GetNativeResolution();
	DWORD instance = (DWORD)hInstance;
	DWORD address1 = 0x0002A0A4;
	BYTE original1[] = {
						0x0F, 0x84, 0x10, 0x01, 0x00, 0x00		// jz      loc_42A1BA
						};

	int size = (6 * sizeof(BYTE) + 5 * sizeof(WORD) + 6 * sizeof(DWORD));
	LPBYTE code = new BYTE[size];

	LPBYTE lpByte = code;
	LPWORD& lpWord = (LPWORD&)lpByte;
	LPDWORD& lpDword = (LPDWORD&)lpByte;

	*(lpWord++) = 0x850F;										// jnz     (address1 + sizeof(original1))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + address1 + sizeof(original1)));
	*(lpWord++) = 0xF981;										// cmp     ecx, resolution.y
	*(lpDword++) = resolution.y;								//
	*(lpWord++) = 0x850F;										// jnz     loc_42A1BA
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0002A1BA));
	*(lpWord++) = 0xFA81;										// cmp     edx, resolution.x
	*(lpDword++) = resolution.x;								//
	*(lpWord++) = 0x850F;										// jnz     loc_42A1BA
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0002A1BA));

	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp    (address1 + sizeof(original1))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + address1 + sizeof(original1)));

	// Aligning:
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop

	assert(lpByte <= code + size);
	return MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address1), original1, sizeof(original1), code, size);
}

//-----------------------------------------------------------------------------

bool IntegrateNativeResolution(HINSTANCE hInstance) {

	POINT resolution = GetNativeResolution();

	DWORD instance = (DWORD)hInstance;
	DWORD address1 = 0x0000C9DB;
	BYTE original1[] = {
						0x25, 0x00, 0x41, 0x00, 0x00			// and     eax, 4100h
						};

	int size = sizeof(original1) + (3 * sizeof(BYTE) + 4 * sizeof(WORD) + 4 * sizeof(DWORD));
	LPBYTE code = new BYTE[size];

	LPBYTE lpByte = code;
	LPWORD& lpWord = (LPWORD&)lpByte;
	LPDWORD& lpDword = (LPDWORD&)lpByte;

	*(lpWord++) = 0xF981;										// cmp     ecx, resolution.x
	*(lpDword++) = resolution.x;								//
	*(lpWord++) = 0x0D75;										// jnz     short loc_+0D
	*(lpWord++) = 0xFA81;										// cmp     edx, resolution.y
	*(lpDword++) = resolution.y;								//
	*(lpWord++) = 0x0575;										// jnz     short loc_+05
	*(lpByte++) = 0xE9;											// jmp     loc_40CA2F
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0000CA2F));

	memcpy_s(lpByte, size, original1, sizeof(original1));
	lpByte += sizeof(original1);

	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp    (address1 + sizeof(original1))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + address1 + sizeof(original1)));

	// Aligning:
	*(lpByte++) = 0x90;											// nop

	assert(lpByte <= code + size);
	return MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address1), original1, sizeof(original1), code, size);
}

//-----------------------------------------------------------------------------

bool IntegrateLauncherOptionsMenu(HINSTANCE hInstance) {

	LPSTR lpMGEgui = "MGEgui.exe";
	LPSTR lpUseDLlabel = "Distant Land";
	LPSTR lpLaunchMGEgui = "Launch MGEgui";
	LPDWORD lpdwLaunchMGEgui = new DWORD;
	*lpdwLaunchMGEgui = 0;

	DWORD instance = (DWORD)hInstance;
	DWORD address0 = 0x0000D5CA;
	DWORD address1 = 0x0000D80B;
	DWORD address2 = 0x0000D840;
	DWORD address3 = 0x0000D87E;
	DWORD address4 = 0x0001DE2B;
	DWORD address5 = 0x0000CC36;
	BYTE original0[] = {
						0x66, 0xC7, 0x43, 0x08, 0x0D, 0x00		// mov     word ptr [ebx+8], 0Dh
						};
	BYTE original1[] = {
						0x8D, 0x54, 0x24, 0x0C,	                // lea     edx, [esp+74h+var_68]
						0x51									// push    ecx
						};
	BYTE original2[] = {
						0x6A, 0x7D								// push    7Dh
						};
	BYTE original3[] = {
						0x6A, 0x7C								// push    7Ch
						};
	BYTE original4[] = {
						0x8D, 0x46, 0x20,	 	                // lea     eax, [esi+20h]
						0x6A, 0x01								// push    1
						};
	BYTE original5[] = {
						0x0F, 0x85, 0x63, 0x04, 0x00, 0x00		// jnz     loc_40D09F
						};

	if(MachineCode::CompareCode(LPVOID(address0 += instance), original0, sizeof(original0))) {
		original0[4]++;	//	We want to add one more control (button).
		MachineCode::SetCode(LPVOID(address0), (LPBYTE)&original0, sizeof(original0));
	}

	int size1 = 2 * sizeof(original1) + (16 * sizeof(BYTE) + 5 * sizeof(WORD) + 7 * sizeof(DWORD));
	LPBYTE code1 = new BYTE[size1];

	LPBYTE lpByte = code1;
	LPWORD& lpWord = (LPWORD&)lpByte;
	LPDWORD& lpDword = (LPDWORD&)lpByte;

	*(lpByte++) = 0xB9;											// mov     ecx, lpLaunchMGEgui
	*(lpDword++) = (DWORD)lpLaunchMGEgui;

	memcpy_s(lpByte, size1, original1, sizeof(original1));
	lpByte += sizeof(original1);

	*(lpByte++) = 0x68;											// push    offset aButton  ; "BUTTON"
	*(lpDword++) = DWORD(instance + 0x001306C0);				//
	*(lpWord++) = 0x036A;										// push    3               ; Unique ID
	*(lpWord++) = 0x0E6A;										// push    0Eh
	*(lpWord++) = 0x606A;										// push    60h
	*(lpWord++) = 0x596A;										// push    59h
	*(lpByte++) = 0x68;											// push    07h
	*(lpDword++) = DWORD(0x00000007);							//
	*(lpByte++) = 0x68;											// push    10000h
	*(lpDword++) = DWORD(0x00010000);							//
	*(lpByte++) = 0x52;											// push    edx
	*(lpByte++) = 0xE8;											// call    sub_4012E9
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x000012E9));
	*(lpWord++) = 0xC483;										// add     esp, 24h
	*(lpByte++) = 0x24;											//

	*(lpByte++) = 0xB9;											// mov     ecx, lpUseDLlabel
	*(lpDword++) = (DWORD)lpUseDLlabel;

	memcpy_s(lpByte, size1, original1, sizeof(original1));
	lpByte += sizeof(original1);

	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp    (address1 + sizeof(original1))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + address1 + sizeof(original1)));

	// Aligning:
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop

	assert(lpByte <= code1 + size1);
	if(!MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address1), original1, sizeof(original1), code1, size1)) return false;

	int size4 = sizeof(original4) + (17 * sizeof(BYTE) + 12 * sizeof(WORD) + 5 * sizeof(DWORD));
	LPBYTE code4 = new BYTE[size4];

	lpByte = code4;

	memcpy_s(lpByte, size4, original4, sizeof(original4));
	lpByte += sizeof(original4);

	// Save registers before calling:
	*(lpByte++) = 0x50;											// push    eax
	*(lpByte++) = 0x51;											// push    ecx
	*(lpByte++) = 0x52;											// push    edx
	*(lpByte++) = 0x53;											// push    ebx

	*(lpWord++) = 0xC933;										// xor     ecx, ecx
	*(lpWord++) = 0x088A;										// mov     cl, byte ptr [eax]
	*(lpWord++) = 0xF183;										// xor     ecx, 0x01
	*(lpByte++) = 0x01;											//
	*(lpByte++) = 0x51;											// push    ecx
	*(lpByte++) = 0xE8;											// call    SetDistantLand
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, SetDistantLand);

	*(lpByte++) = 0xB8;											// mov     eax, lpdwLaunchMGEgui
	*(lpDword++) = DWORD(lpdwLaunchMGEgui);						//
	*(lpWord++) = 0x3880;										// cmp     byte ptr [eax], 1
	*(lpByte++) = 0x01;											//
	*(lpWord++) = 0x1875;										// jnz     short loc_+18

	*(lpWord++) = 0x00C6;										// mov     byte ptr [eax], 0
	*(lpByte++) = 0x00;											//

	*(lpWord++) = 0x056A;										// push    5               ; nShowCmd
	*(lpWord++) = 0x006A;										// push    0               ; lpDirectory
	*(lpWord++) = 0x006A;										// push    0               ; lpParameters
	*(lpByte++) = 0x68;											// push    offset "MGEgui.exe"
	*(lpDword++) = DWORD(lpMGEgui);								//
	*(lpWord++) = 0x006A;										// push    0               ; lpOperation
	*(lpWord++) = 0x006A;										// push    0               ; hwnd
	*(lpWord++) = 0x15FF;										// call    ds:ShellExecuteA
	*(lpDword++) = DWORD(instance + 0x0013998C);				//

	// Restore the registers:
	*(lpByte++) = 0x5B;											// pop     ebx
	*(lpByte++) = 0x5A;											// pop     edx
	*(lpByte++) = 0x59;											// pop     ecx
	*(lpByte++) = 0x58;											// pop     eax

	// Append the last 5 bytes of jump instruction:
	*(lpByte++) = 0xE9;											// jmp    (address4 + sizeof(original4))
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + address4 + sizeof(original4)));

	// Aligning:
	*(lpByte++) = 0x90;											// nop

	assert(lpByte <= code4 + size4);

	int size5 = (10 * sizeof(BYTE) + 3 * sizeof(WORD) + 4 * sizeof(DWORD));
	LPBYTE code5 = new BYTE[size5];

	lpByte = code5;

	*(lpWord++) = 0x840F;										// jz      loc_40CC3C
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0000CC3C));
	*(lpByte++) = 0x48;											// dec     eax
	*(lpWord++) = 0x850F;										// jnz     loc_40D09F
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0000D09F));
	*(lpByte++) = 0xB8;											// mov     eax, lpdwLaunchMGEgui
	*(lpDword++) = DWORD(lpdwLaunchMGEgui);						//
	*(lpWord++) = 0x00C6;										// mov     byte ptr [eax], 1
	*(lpByte++) = 0x01;											//
	*(lpByte++) = 0xE9;											// jmp     loc_40CC68
	*(lpDword++) = MachineCode::GetLabelOffset(lpDword, LPDWORD(instance + 0x0000CC68));

	// Aligning:
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop
	*(lpByte++) = 0x90;											// nop

	assert(lpByte <= code5 + size5);
	MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address5), original5, sizeof(original5), code5, size5);

	if(MachineCode::CompareCode(LPVOID(address2 += instance), original2, sizeof(original2)) &&
		MachineCode::CompareCode(LPVOID(address3 += instance), original3, sizeof(original3))) {
		
		return	MachineCode::SetCode(LPVOID(address2), (LPBYTE)&original3, sizeof(original2)) &&
				MachineCode::SetCode(LPVOID(address3), (LPBYTE)&original2, sizeof(original3)) &&
				MachineCode::CompareAndPatchByJumpToAttachment(LPVOID(instance + address4), original4, sizeof(original4), code4, size4);
	}
	return false;
}

//-----------------------------------------------------------------------------

void Integration(HINSTANCE hInstance, LPSTR origFilename, DWORD dwVersionLS, bool isMorrowind) {
	if(isMorrowind) {
		if (!(MGEFlags & MGE_DISABLED)) {
			IntegrateDLProgress(hInstance, dwVersionLS);
			IntegrateDLButton(hInstance, dwVersionLS);
			IntegrateScreenshot(hInstance, dwVersionLS);
		}
		IntegrateNativeResolution(hInstance, dwVersionLS);
	} else if(_stricmp(origFilename, "TES Construction Set.exe") == 0) {
		IntegrateMWECompiler(hInstance, dwVersionLS);
		IntegrateDialogFix(hInstance, dwVersionLS);
	} else if(_stricmp(origFilename, "Morrowind Launcher.exe") == 0) {
		if(dwVersionLS != 0x01) return;
		IntegrateNativeResolution(hInstance);
		IntegrateLauncherOptionsMenu(hInstance);
	}
	return;
}
