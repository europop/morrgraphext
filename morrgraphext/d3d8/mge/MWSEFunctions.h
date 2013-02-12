#ifndef _MWSE_FUNCTIONS_H_
#define _MWSE_FUNCTIONS_H_

//-----------------------------------------------------------------------------

namespace MWSE {

	//MGE
	/*BYTE _stdcall MGECompileShader(HWND window,BYTE Render,char* path1,char* path2,char* compilepath);
	BYTE _stdcall MGEPreviewShaderChain(HWND window,char* path1,char* path2);
	void _stdcall MGERenderFrame(bool UseShader);
	void _stdcall MGEEndShader();
	BYTE _stdcall MGEMaxAA(HWND window);
	BYTE _stdcall MGEMaxAF(HWND window);
	void _stdcall MGERefreshRates(HWND window,DWORD adapter,DWORD Width,DWORD Height,bool TrueColour);
	void _stdcall MGEResolutions(HWND window,DWORD adapter,bool TrueColour);*/
	//MWSE
	int HUDIndex(char* p,char* file);
	int ObjectIndex(char* p,char* file);
	void UpdateObjectMatrix(int index,BYTE type);
	void FreeModel(int index);
	int ShaderIndex(char* p,bool LoadIfNeeded);
	int TextureIndex(char* p,bool LoadIfNeeded);
	bool IsSafeString(char* str);
	void GetString(char* buffer,DWORD* pPointer);
	void GetStringC(char* buffer,DWORD* pPointer);
	void SetStringC(DWORD* pPointer,char* buffer);
	void Update();
	void FreeModel(int index);
	void FreeTexture(int index);
	void FreeEffect(int index);
	void FreeHUD(int index);
	//MME MWSE
	void TapKey(WORD keycode);
	void HoldKey(WORD keycode);
	void UnholdKey(WORD keycode);
	void HammerKey(WORD keycode);
	void UnhammerKey(WORD keycode);
	void AHammerKey(WORD keycode);
	void AUnhammerKey(WORD keycode);
	void DisallowKey(WORD keycode);
	void AllowKey(WORD keycode);
	void MoveMouseX(WORD pixels);
	void MoveMouseY(WORD pixels);
	void MouseSpeedX(float speed);
	void MouseSpeedY(float speed);
	void DisableMouse();
	void EnableMouse();
	void BufferedDown(WORD keycode);
	void BufferedUp(WORD keycode);
	//MWSE debugging

	// Input

	//MWSE function calls made to MME
	void TapKey(WORD keycode);
	void HoldKey(WORD keycode);
	void UnholdKey(WORD keycode);
	void HammerKey(WORD keycode);
	void UnhammerKey(WORD keycode);
	void AHammerKey(WORD keycode);
	void AUnhammerKey(WORD keycode);
	void DisallowKey(WORD keycode);
	void AllowKey(WORD keycode);
	void MoveMouseX(WORD pixels);
	void MoveMouseY(WORD pixels);
	void MouseSpeedX(float speed);
	void MouseSpeedY(float speed);
	void DisableMouse();
	void EnableMouse();
	void BufferedDown(WORD keycode);
	void BufferedUp(WORD keycode);

}; // end namespace

//-----------------------------------------------------------------------------

#endif