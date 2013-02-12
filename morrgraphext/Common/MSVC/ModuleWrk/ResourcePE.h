//---------------------------------------------------------------------------

#ifndef ResourcePEH
#define ResourcePEH

#include <Windows.h>
//---------------------------------------------------------------------------
#ifndef DEFCALL
#define DEFCALL __stdcall
#endif
//---------------------------------------------------------------------------
HANDLE DEFCALL GetBitmapFromResource(LPSTR BmpResName);
PVOID  DEFCALL GetDataFromResource(LPSTR BinResName);
//---------------------------------------------------------------------------
#endif
