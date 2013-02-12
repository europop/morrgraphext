#pragma once
//Windows header defines. Use strict pointers, exclude rare stuff, and compile for windows 2000
#define _CRT_SECURE_NO_DEPRECATE
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINDOWS 0x0500
#define _WIN32_WINNT 0x500
#define WINVER 0x0500

#include <windows.h>
#include "../log.h"
#include "MGEversion.h"

#define DIRECT3D_VERSION 0x0900
#define DIRECTINPUT_VERSION 0x0800
#define DIRECTSOUND_VERSION 0x0800

#define PI 3.141592653589793238f

#ifdef RELEASE
#define DEBUGSTRING(a)
#else
#define DEBUGSTRING(a) { OutputDebugString(a); LOG::logline(a); }
#endif

#ifdef _D3D8
#define EX
#else
#define EX extern
#endif

//Some info about the morrowind window
struct sJointInfo {
	//HWND mwwin;
	long WNDtop;
	long WNDleft;
	long WNDheight;
	long WNDwidth;
	bool WNDwindowed;
	WORD GraphicsWidth;
	WORD GraphicsHeight;
	float FrameTime;  //Stores how many seconds have passed since the last frame
	float aspect;
};
EX sJointInfo JointInfo;

#undef EX

extern DWORD MWVersion;

#define ERRORMSG(msg)  { LOG::logline(msg); MessageBoxA(0,msg,"MGE Error",0); ExitProcess(-1); }
#define ZeroStruct(a) { ZeroMemory(&a,sizeof(a)); }
#define ZeroPointer(a) { ZeroMemory(a,sizeof(*a)); }
#define InitPointer(a,b) { a=new b(); ZeroPointer(a); }
