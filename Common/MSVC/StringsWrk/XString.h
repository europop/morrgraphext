//---------------------------------------------------------------------------
#ifndef XStringH
#define XStringH

#include "Windows.h"
#include "MemWork.h"
#include "StrWork.h"

#define INLINESPEC __inline
#define CALLSPEC   __stdcall    // Only for OVERLOADED, and not ASSEMBLER
//============================================================================
//                          Format of XSTRING
//---------------------------------------------------------------------------
// DWORD Flags   - XX005358 - Flag of XString
// DWORD Counter - Letters in string (without ending ZERO_WORD)
//   LETTERS     - Pointer to this returned by AllocXString and accepted by other XString functions as PVOID
// ZERO_WORD     - Two zero at end of string - needed to use XSTRING with WinAPI
//---------------------------------------------------------------------------
// Always make code compatible with BCB and MSVC !!!
// Do not overload exact functions (assembler cannot use overloaded funcs),
//   instead overload INLINE REDIRECTORS
// Do Not Let to Compiler GUESS wich function exactly use, always cpecify right one
// Do not use in separate namespaces - some function with names in standart IMPORT headers
// Use 'NAKED' specificator only in definitions, not in declarations !!!
// Always use '__asm' for assembler code !!!
//---------------------------------------------------------------------------
#define XSTRFLAG   0x00005358       // XS
#define XSTRMASK   0x00FFFFFF       // Mask to get XSTRFLAG
#define XSFALMSK   0x03000000       // Mask for AllocationFlags
#define XSFALMEM   0x01000000       // XString was Allocated with 'AllocXString'
#define XSFALTMP   0x02000000       // XString was Allocated with 'AllocXStringTmp'
#define XSFUNICD   0x04000000       // XString is UNICODE
#define XSFINITS   0x88000000       // XStrin was initialized
#define XSOFPFLG   0                // Ofdfset from pointer to BUFFER
#define XSOFPCTR   1                // Ofdfset from pointer to BUFFER
#define XSOFSFLG   -2               // Ofdfset from pointer to STRING
#define XSOFSCTR   -1               // Ofdfset from pointer to STRING
#define XSOFSBEG   XSOFSFLG
#define XSWRKSIZE  8                // (sizeof(DWORD)*2)
#define XSMINSIZEA 9                // (XSWRKSIZE+sizeof(BYTE))
#define XSMINSIZEW 10               // (XSWRKSIZE+sizeof(WORD))

#define _XPTRTOXSTR(XPtr)  ((&((DWORD*)(XPtr))[2]))
#define _XSTRTOXPTR(XStr)  ((&((DWORD*)(XStr))[-2]))

typedef char    *PXSTRA;      // Compatible with 'LPSTR'
typedef wchar_t *PXSTRW;      // Compatible with 'PWSTR'
//===========================================================================
//                        FUNCTION NAMES
//---------------------------------------------------------------------------
namespace XSTRING
{
 INLINESPEC BOOL   _stdcall xsIsXStringInitialized(PVOID String);
 INLINESPEC BOOL   _stdcall xsIsXStringAllocated(PVOID String);
 INLINESPEC BOOL   _stdcall xsIsXStringUnicode(PVOID String);
 INLINESPEC BOOL   _stdcall xsIsStringXString(PVOID String);
 INLINESPEC DWORD  _stdcall xsGetXStringLen(PVOID String);
 INLINESPEC DWORD  _stdcall xsGetXStringBytes(PVOID String);
 INLINESPEC DWORD  _stdcall xsGetXStringMemLen(PVOID String);
 INLINESPEC PVOID  _stdcall xsGetXStringMemPtr(PVOID String);
 INLINESPEC PVOID  _stdcall xsGetXStringMem(PVOID String, PDWORD Length);
 INLINESPEC PXSTRA _stdcall xsAllocXStringAS(LPSTR String);
 INLINESPEC PXSTRW _stdcall xsAllocXStringWS(PWSTR String);
 INLINESPEC DWORD  _stdcall xsInitXStringA(PVOID Buffer, LPSTR String, DWORD BufferSize);
 INLINESPEC DWORD  _stdcall xsInitXStringW(PVOID Buffer, PWSTR String, DWORD BufferSize);

 PXSTRA _stdcall xsAllocXStringASL(LPSTR String, DWORD Length);
 PXSTRW _stdcall xsAllocXStringWSL(PWSTR String, DWORD Length);

 DWORD  _stdcall xsFreeXString(PVOID String);
 PXSTRA _stdcall xsAllocXStringTmpAS(LPSTR String);
 PXSTRW _stdcall xsAllocXStringTmpWS(PWSTR String);
 PXSTRA _stdcall xsAllocXStringTmpASL(LPSTR String, DWORD StringLen);
 PXSTRW _stdcall xsAllocXStringTmpWSL(PWSTR String, DWORD StringLen);
 PXSTRA _stdcall xsAllocXStringTmp(LPSTR String);
 PXSTRW _stdcall xsAllocXStringTmp(PWSTR String);
 PXSTRA _stdcall xsAllocXStringTmp(LPSTR String, DWORD StringLen);
 PXSTRW _stdcall xsAllocXStringTmp(PWSTR String, DWORD StringLen);

 //+++++++++++++++++++ FIX BCC INLINESPEC ASSEMBLER`S BUG +++++++++++++++++++++++
 // STUPID BCC Compiler MESS names of INLINESPEC routines in ASM code - Make for all
 // INLINESPECroutines NORMAL Redirectors
 // STUPID BCC haves a BUG, do not use these functions, just let them be here
 // Else function names in INLINESPEC ASSEMBLER will be MESSED !!!
 // All NotINLINESPEC redirections to INLINESPEC do not expands INLINESPEC their direct child
 // INLINESPEC routines - STUPID COMPILER ?
 #ifdef __BORLANDC__
 BOOL   _stdcall _xsIsXStringInitialized(PVOID String){return xsIsXStringInitialized(String);}
 BOOL   _stdcall _xsIsXStringAllocated(PVOID String){return xsIsXStringAllocated(String);}
 BOOL   _stdcall _xsIsXStringUnicode(PVOID String){return xsIsXStringUnicode(String);}
 BOOL   _stdcall _xsIsStringXString(PVOID String){return xsIsStringXString(String);}
 DWORD  _stdcall _xsGetXStringLen(PVOID String){return xsGetXStringLen(String);}
 DWORD  _stdcall _xsGetXStringBytes(PVOID String){return xsGetXStringBytes(String);}
 DWORD  _stdcall _xsGetXStringMemLen(PVOID String){return xsGetXStringMemLen(String);}
 PVOID  _stdcall _xsGetXStringMemPtr(PVOID String){return xsGetXStringMemPtr(String);}
 PVOID  _stdcall _xsGetXStringMem(PVOID String, PDWORD Length){return xsGetXStringMem(String, Length);}
 PXSTRA _stdcall _xsAllocXStringAS(LPSTR String){return xsAllocXStringAS(String);}
 PXSTRW _stdcall _xsAllocXStringWS(PWSTR String){return xsAllocXStringWS(String);}
 DWORD  _stdcall _xsInitXStringA(PVOID Buffer, LPSTR String, DWORD BufferSize){return xsInitXStringA(Buffer, String, BufferSize);}
 DWORD  _stdcall _xsInitXStringW(PVOID Buffer, PWSTR String, DWORD BufferSize){return xsInitXStringW(Buffer, String, BufferSize);}
 #endif
 
 //+++++++++++++++++++ OVERLOADED INLINESPEC REDIRECTORS +++++++++++++++++++++++
 // Stupid compiler cannot compile PreDeclared INLINESPEC functions - Code must be right here
 INLINESPEC PXSTRA CALLSPEC xsAllocXString(LPSTR String){return xsAllocXStringAS(String);}
 INLINESPEC PXSTRW CALLSPEC xsAllocXString(PWSTR String){return xsAllocXStringWS(String);}
 INLINESPEC PXSTRA CALLSPEC xsAllocXString(LPSTR String, DWORD Length){return xsAllocXStringASL(String, Length);}
 INLINESPEC PXSTRW CALLSPEC xsAllocXString(PWSTR String, DWORD Length){return xsAllocXStringWSL(String, Length);}
 INLINESPEC DWORD  CALLSPEC xsInitXString(PVOID Buffer, LPSTR String, DWORD BufferSize){return xsInitXStringA(Buffer, String, BufferSize);}
 INLINESPEC DWORD  CALLSPEC xsInitXString(PVOID Buffer, PWSTR String, DWORD BufferSize){return xsInitXStringW(Buffer, String, BufferSize);}

 //+++++++++++++++++++ INLINESPEC FUNCTIONS +++++++++++++++++++++++
 INLINESPEC BOOL   _stdcall xsIsXStringInitialized(PVOID String){return ((String)?(((DWORD*)String)[XSOFSFLG] & XSFINITS):NULL);}  // Returns true if String are XString and if was allocated with 'AllocXString' or 'AllocXStringTmp'
 INLINESPEC BOOL   _stdcall xsIsXStringAllocated(PVOID String){return ((String)?(((DWORD*)String)[XSOFSFLG] & XSFALMSK):NULL);}    // Returns true if String are XString and if was allocated with 'AllocXString' or 'AllocXStringTmp'
 INLINESPEC BOOL   _stdcall xsIsXStringUnicode(PVOID String){return ((String)?(((DWORD*)String)[XSOFSFLG] & XSFUNICD):NULL);}      // Returns true if String are XString and it is UNICODE wide string
 INLINESPEC BOOL   _stdcall xsIsStringXString(PVOID String){return ((!IsBadReadPtr(&((DWORD*)String)[XSOFSFLG],XSMINSIZEA)) && ((((DWORD*)String)[XSOFSFLG] & XSTRMASK)==XSTRFLAG) && (((DWORD*)String)[XSOFSFLG] & XSFINITS));} // Returns true if String are XString
 INLINESPEC DWORD  _stdcall xsGetXStringLen(PVOID String){return ((String)?(((DWORD*)String)[XSOFSCTR]):NULL);}                      // Returns length of XString readed from its LENGTH field
 INLINESPEC DWORD  _stdcall xsGetXStringBytes(PVOID String){return ((String)?((((DWORD*)String)[XSOFSCTR]+1) * ((bool)(((DWORD*)String)[XSOFSFLG] & XSFUNICD)+1)):NULL);}
 INLINESPEC DWORD  _stdcall xsGetXStringMemLen(PVOID String){return ((String)?(xsGetXStringBytes(String)+XSWRKSIZE):NULL);}        // Returns length of FLAGS+STRING in bytes
 INLINESPEC PVOID  _stdcall xsGetXStringMemPtr(PVOID String){return ((String)?(&((DWORD*)String)[XSOFSBEG]):NULL);}              // Returns pointer to FLAGS of XSTRING
 INLINESPEC PXSTRA _stdcall xsAllocXStringAS(LPSTR String){if(String)return xsAllocXStringASL(String, STRWORK::swCharsInStringA(String));else return NULL;}
 INLINESPEC PXSTRW _stdcall xsAllocXStringWS(PWSTR String){if(String)return xsAllocXStringWSL(String, STRWORK::swCharsInStringW(String));else return NULL;}

 //---------------------------------------------------------------------------
 INLINESPEC PVOID _stdcall xsGetXStringMem(PVOID String, PDWORD Length)               // Returns pointer to FLAGS of XSTRING and optionally length of FLAGS+STRING in bytes
  {
   if(Length)(*Length) = xsGetXStringMemLen(String);
   return xsGetXStringMemPtr(String);
  }
 //---------------------------------------------------------------------------
 INLINESPEC DWORD _stdcall xsInitXStringA(PVOID Buffer, LPSTR String, DWORD BufferSize) // Returns Num of chars initialized (StringLen, if OK)
  {
   if(Buffer && String && (BufferSize >= XSMINSIZEA))
	{
	 ((DWORD*)Buffer)[XSOFPFLG] = (XSTRFLAG | XSFINITS);
	 return (((DWORD*)Buffer)[XSOFPCTR] = (STRWORK::swCopyStringADSC((LPSTR)_XPTRTOXSTR(Buffer), String, (BufferSize-XSMINSIZEA)) - sizeof(BYTE)));
	}
   return NULL;
  }
 //---------------------------------------------------------------------------
 INLINESPEC DWORD _stdcall xsInitXStringW(PVOID Buffer, PWSTR String, DWORD BufferSize) // Returns Num of chars initialized (StringLen, if OK)
  {
   if(Buffer && String && (BufferSize >= XSMINSIZEW))
	{
	 ((DWORD*)Buffer)[XSOFPFLG] = (XSTRFLAG | XSFINITS | XSFUNICD);
	 return (((DWORD*)Buffer)[XSOFPCTR] = (STRWORK::swCopyStringWDSC((PWSTR)_XPTRTOXSTR(Buffer), String, (BufferSize-XSMINSIZEW)) - sizeof(WORD)));
	}
   return NULL;
  }
 //---------------------------------------------------------------------------

}
//---------------------------------------------------------------------------
#endif
