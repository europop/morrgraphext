#include "../log.h"
#include "MWE.h"
#include "MWEImproving.h"

bool GetValueInReg(HKEY key, LPCTSTR lpSubKey, LPCTSTR lpName, char *cBuffer, LPDWORD lpdwSize)
{
	LONG lResult;
	HKEY hKey;
	lResult = RegOpenKeyEx(key, lpSubKey, NULL, KEY_READ, &hKey);
	if (lResult != ERROR_SUCCESS)
		return false;
	DWORD type;
	lResult=RegQueryValueEx(hKey,lpName,NULL, &type, (LPBYTE)cBuffer, lpdwSize);
	RegCloseKey(hKey);
	if (lResult != ERROR_SUCCESS)
		return false;
	return true;
}

unsigned GetDwRegOrDef(LPCTSTR lpSubKey, LPCTSTR lpName, unsigned dwDefault)
{//	For HKEY_LOCAL_MACHINE.
	unsigned dwReg = 0;
	DWORD dwSize = sizeof(DWORD);
	if(GetValueInReg(HKEY_LOCAL_MACHINE, lpSubKey, lpName, (char*)&dwReg, &dwSize))
	{
		if(dwSize == sizeof(DWORD))
			return dwReg;
	}
	return dwDefault;
}

bool InjectMWE(HWND hMorrowindWindow) {
	HMODULE hMWEdll = LoadLibrary(MWE_DLL_PATH);
	if(hMWEdll == NULL)
	{
		LOG::logline("MWE: failed to load DLL\r\n");
		return false;
	}

	ImproveMWE(hMWEdll);	// At first we try to fix some defects.
	PatchMWE(hMWEdll);		// After that, we try to apply patches (if they exist).

	DWORD dwPID;
	GetWindowThreadProcessId(hMorrowindWindow, &dwPID);

	//	From MWE registry key
	unsigned *lpdwMemAddress = new unsigned[9];
	for(int i=0; i<9; i++)	lpdwMemAddress[i]=0;

	//	From Morrowind registry key
	unsigned dwForward		= GetDwRegOrDef(REGSK_MW, "Forward",	0x01120000);
	unsigned dwBack			= GetDwRegOrDef(REGSK_MW, "Back",		0x01200000);
	unsigned dwSlideleft	= GetDwRegOrDef(REGSK_MW, "SlideLeft",	0x011f0000);
	unsigned dwSlideright	= GetDwRegOrDef(REGSK_MW, "SlideRight",	0x01210000);
	unsigned dwUse			= GetDwRegOrDef(REGSK_MW, "Use",		0x02010302);
	unsigned dwQuickload	= GetDwRegOrDef(REGSK_MW, "QuickLoad",	0x01440000);
	unsigned dwActivate		= GetDwRegOrDef(REGSK_MW, "Activate",	0x013a0301);

	//	From MWE registry key
	unsigned dwBlockKeyMouse= GetDwRegOrDef(REGSK_MWE, "Block Key Mouse",	0x00000000);
	unsigned dwBlockKeyCode	= GetDwRegOrDef(REGSK_MWE, "Block Key Code",	0x000000FF);
	unsigned dwTranceKeyMouse=GetDwRegOrDef(REGSK_MWE, "Trance Key Mouse",	0x00000000);
	unsigned dwTranceKeyCode= GetDwRegOrDef(REGSK_MWE, "Trance Key Code",	0x000000FF);
	unsigned dwRageKeyMouse	= GetDwRegOrDef(REGSK_MWE, "Rage Key Mouse",	0x00000000);
	unsigned dwRageKeyCode	= GetDwRegOrDef(REGSK_MWE, "Rage Key Code",		0x000000FF);

	char *cKey = new char[26];
	cKey[0] = (dwForward & 0x01000000)?0:1;				//	0
	cKey[1] = ((dwForward & 0x00FF0000)>>16)-1;			//	0x11
	cKey[2] = (dwBack & 0x01000000)?0:1;				//	0
	cKey[3] = ((dwBack & 0x00FF0000)>>16)-1;			//	0x1F
	cKey[4] = (dwSlideleft & 0x01000000)?0:1;			//	0
	cKey[5] = ((dwSlideleft & 0x00FF0000)>>16)-1;		//	0x1E
	cKey[6] = (dwSlideright & 0x01000000)?0:1;			//	0
	cKey[7] = ((dwSlideright & 0x00FF0000)>>16)-1;		//	0x20
	cKey[8] = (dwUse & 0x01000000)?0:1;					//	1
	cKey[9] = ((dwUse & 0x00FF0000)>>16)-1;				//	0
	cKey[10] = dwBlockKeyMouse;
	cKey[11] = dwBlockKeyCode;
	cKey[12] = dwTranceKeyMouse;
	cKey[13] = dwTranceKeyCode;
	cKey[14] = dwRageKeyMouse;
	cKey[15] = dwRageKeyCode;
	cKey[16] = 0;
	cKey[17] = 1;
	cKey[18] = (dwQuickload & 0x01000000)?0:1;			//	0
	cKey[19] = ((dwQuickload & 0x00FF0000)>>16)-1;		//	0x43
	cKey[20] = (dwActivate & 0x01000000)?0:1;			//	0
	cKey[21] = ((dwActivate & 0x00FF0000)>>16)-1;		//	0x39
	cKey[22] = 0;
	cKey[23] = 0x38;
	cKey[24] = 0;
	cKey[25] = (char)0xB8;

	//	From MWE registry key
	unsigned *lpdwSlider = new unsigned[8];
	lpdwSlider[0] = GetDwRegOrDef(REGSK_MWE, "Slider 0", 0x00000064);
	lpdwSlider[1] = GetDwRegOrDef(REGSK_MWE, "Slider 1", 0x00000014);
	lpdwSlider[2] = GetDwRegOrDef(REGSK_MWE, "Slider 2", 0x00000014);
	lpdwSlider[3] = GetDwRegOrDef(REGSK_MWE, "Slider 3", 0x00000014);
	lpdwSlider[4] = GetDwRegOrDef(REGSK_MWE, "Slider 4", 0x00000014);
	lpdwSlider[5] = 0;
	lpdwSlider[6] = GetDwRegOrDef(REGSK_MWE, "Slider 6", 0x00000014);
	lpdwSlider[7] = GetDwRegOrDef(REGSK_MWE, "Slider 7", 0x0000003c);

	TRANSFERDATA TransferData =(TRANSFERDATA)GetProcAddress(hMWEdll, "?TransferData@@YAXKKPAUHWND__@@PAKK1H@Z");
	INJECTDLL InjectDll=(INJECTDLL)GetProcAddress(hMWEdll, "?InjectDll@@YA_NPAUHWND__@@@Z");

	if(TransferData && InjectDll)
	{
		TransferData(3,dwPID, hMorrowindWindow,lpdwMemAddress,cKey,lpdwSlider,0);
		if(InjectDll(hMorrowindWindow))
		{
			LOG::logline("MWE contributers: Aerelorn, Tonto, Cid\r\nMWE: DLL injected\r\n");
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		LOG::logline("MWE: incorrect DLL\r\n");
		return false;
	}
}
