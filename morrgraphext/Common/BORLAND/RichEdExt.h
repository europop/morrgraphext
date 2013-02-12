//---------------------------------------------------------------------------

#ifndef RichEdExtH
#define RichEdExtH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>

#include "Ole2.h"
#include "Richole.h"
//---------------------------------------------------------------------------
class TRichEditOleCallback : public IRichEditOleCallback
{
 public:
 ULONG RefCount;
 LPSTR RtfDocPtr;
 long  RtfDocLen;

 HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
 ULONG   __stdcall AddRef(void);
 ULONG   __stdcall Release(void);

 HRESULT __stdcall ContextSensitiveHelp(BOOL fEnterMode);
 HRESULT __stdcall DeleteObject(LPOLEOBJECT lpoleobj);
 HRESULT __stdcall GetClipboardData(CHARRANGE *lpchrg,DWORD reco,LPDATAOBJECT *lplpdataobj);
 HRESULT __stdcall GetContextMenu(WORD seltype,LPOLEOBJECT lpoleobj,CHARRANGE *lpchrg,HMENU *lphmenu);
 HRESULT __stdcall GetDragDropEffect(BOOL fDrag,DWORD grfKeyState,LPDWORD pdwEffect);
 HRESULT __stdcall GetInPlaceContext(LPOLEINPLACEFRAME *lplpFrame,LPOLEINPLACEUIWINDOW *lplpDoc,LPOLEINPLACEFRAMEINFO lpFrameInfo);
 HRESULT __stdcall GetNewStorage(LPSTORAGE *lplpstg);
 HRESULT __stdcall QueryAcceptData(LPDATAOBJECT lpdataobj,CLIPFORMAT *lpcfFormat,DWORD reco,BOOL fReally,HGLOBAL hMetaPict);
 HRESULT __stdcall QueryInsertObject(LPCLSID lpclsid,LPSTORAGE lpstg,LONG cp);
 HRESULT __stdcall ShowContainerUI(BOOL fShow);
};
//---------------------------------------------------------------------------
DWORD LoadOleRtf(HWND hwRichEdit, LPSTR RtfDocument);

//---------------------------------------------------------------------------
#endif
