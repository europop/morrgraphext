//---------------------------------------------------------------------------
#pragma hdrstop

#include "RichEdExt.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TRichEditOleCallback RichOleCallback;
//---------------------------------------------------------------------------
ULONG   __stdcall TRichEditOleCallback::Release(void)
{
 return --RefCount;
}
//---------------------------------------------------------------------------
ULONG   __stdcall TRichEditOleCallback::AddRef(void)
{
 return ++RefCount;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::QueryInterface(REFIID iid, void **ppvObject)
{
 (*ppvObject) = NULL;
 return E_NOINTERFACE;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::ContextSensitiveHelp(BOOL fEnterMode)
{
 return 0;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::DeleteObject(LPOLEOBJECT lpoleobj)
{
 return 0;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::GetClipboardData(CHARRANGE *lpchrg,DWORD reco,LPDATAOBJECT *lplpdataobj)
{
 return 0;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::GetContextMenu(WORD seltype,LPOLEOBJECT lpoleobj,CHARRANGE *lpchrg,HMENU *lphmenu)
{
 return 0;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::GetDragDropEffect(BOOL fDrag,DWORD grfKeyState,LPDWORD pdwEffect)
{
 return 0;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::GetInPlaceContext(LPOLEINPLACEFRAME *lplpFrame,LPOLEINPLACEUIWINDOW *lplpDoc,LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
 return E_INVALIDARG;   // Open object in associated application
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::GetNewStorage(LPSTORAGE *lplpstg)
{
 ILockBytes* pLockBytes;

 if(CreateILockBytesOnHGlobal(0, true, &pLockBytes))return E_OUTOFMEMORY;
 StgCreateDocfileOnILockBytes(pLockBytes, STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_CREATE, 0, lplpstg);
 pLockBytes->Release();
 return 0;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::QueryAcceptData(LPDATAOBJECT lpdataobj,CLIPFORMAT *lpcfFormat,DWORD reco,BOOL fReally,HGLOBAL hMetaPict)
{
 return 0;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::QueryInsertObject(LPCLSID lpclsid,LPSTORAGE lpstg,LONG cp)
{
 return 0;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TRichEditOleCallback::ShowContainerUI(BOOL fShow)
{
 return 0;
}
//===========================================================================
DWORD __stdcall EditStreamCallback(DWORD_PTR dwCookie,LPBYTE pbBuff,LONG cb,LONG *pcb)
{
 long CopyLen;

 CopyLen = ((TRichEditOleCallback*)dwCookie)->RtfDocLen;
 if(CopyLen <= 0)return 1;
 if(CopyLen > cb)CopyLen = cb;
 CopyMemory(pbBuff,((TRichEditOleCallback*)dwCookie)->RtfDocPtr,CopyLen);
 ((TRichEditOleCallback*)dwCookie)->RtfDocPtr += CopyLen;
 ((TRichEditOleCallback*)dwCookie)->RtfDocLen -= CopyLen;
 (*pcb)=CopyLen;
 return 0;
}
//===========================================================================
DWORD LoadOleRtf(HWND hwRichEdit, LPSTR RtfDocument)
{
 EDITSTREAM es;

 if(!RtfDocument)return 1;
 if(!LoadLibrary("riched20.dll"))return 2;
 if(!SendMessage(hwRichEdit,EM_SETOLECALLBACK,NULL,(LPARAM)&RichOleCallback))return 3;
 RichOleCallback.RtfDocLen = lstrlen(RtfDocument)+1;
 RichOleCallback.RtfDocPtr = RtfDocument;
 es.dwCookie=(DWORD)&RichOleCallback;
 es.dwError=0;
 es.pfnCallback=&EditStreamCallback;
 SendMessage(hwRichEdit,EM_STREAMIN,SF_RTF,(LPARAM)&es);
 return 0;
}
//---------------------------------------------------------------------------
