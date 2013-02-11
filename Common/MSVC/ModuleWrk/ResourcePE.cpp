//---------------------------------------------------------------------------

#include "ResourcePE.h"

//---------------------------------------------------------------------------
HANDLE DEFCALL GetBitmapFromResource(LPSTR BmpResName)
{
 return LoadImage(GetModuleHandle(NULL),BmpResName,IMAGE_BITMAP,NULL,NULL,NULL);
}
//---------------------------------------------------------------------------
PVOID DEFCALL GetDataFromResource(LPSTR BinResName)
{
 return LoadResource(NULL,FindResource(NULL,BinResName,RT_RCDATA));
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
