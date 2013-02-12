//---------------------------------------------------------------------------
#ifndef StrWorkH
#define StrWorkH

#include "Windows.h"
#include "MemWork.h"

#define INLINESPEC __inline
#define CALLSPEC   __stdcall    // Only for OVERLOADED, and not ASSEMBLER
//============================================================================
// Always make code compatible with BCB and MSVC !!!
// Do not overload exact functions (assembler cannot use overloaded funcs),
//   instead overload INLINE REDIRECTORS
// Do Not Let to Compiler GUESS wich function exactly use, always cpecify right one
// Do not use in separate namespaces - some function have a names already defined in standart IMPORT headers
// Use 'NAKED' specificator only in definitions, not in declarations !!!
// Always use '__asm' for assembler code !!!
//---------------------------------------------------------------------------
namespace STRWORK
{
 INLINESPEC DWORD _stdcall swBytesInStringA(LPSTR String);
 INLINESPEC DWORD _stdcall swBytesInStringW(PWSTR String);
 INLINESPEC DWORD _stdcall swCopyStringADS(LPSTR DstString, LPSTR SrcString);
 INLINESPEC DWORD _stdcall swCopyStringADSC(LPSTR DstString, LPSTR SrcString, DWORD CharsToCopy);
 INLINESPEC DWORD _stdcall swCopyStringWDS(PWSTR DstString, PWSTR SrcString);
 INLINESPEC DWORD _stdcall swCopyStringWDSC(PWSTR DstString, PWSTR SrcString, DWORD CharsToCopy);

 DWORD _stdcall swCharsInStringA(LPSTR String);
 DWORD _stdcall swCharsInStringW(PWSTR String);

 DWORD _stdcall swMergeStringADS(LPSTR DstString, LPSTR SrcString);
 DWORD _stdcall swMergeStringADSC(LPSTR DstString, LPSTR SrcString, DWORD CharsToCopy);
 DWORD _stdcall swMergeStringWDS(PWSTR DstString, PWSTR SrcString);
 DWORD _stdcall swMergeStringWDSC(PWSTR DstString, PWSTR SrcString, DWORD CharsToCopy);
 BOOL  _stdcall swIsWildcardStringsMatchA(LPSTR StringA, LPSTR StringB);
 BOOL  _stdcall swIsWildcardStringsMatchW(PWSTR StringA, PWSTR StringB);

 long  _stdcall swCharPosFromLeftAS(LPSTR String, BYTE CharCode);
 long  _stdcall swCharPosFromLeftASL(LPSTR String, BYTE CharCode, DWORD StringLen);
 long  _stdcall swCharPosFromLeftASLIO(LPSTR String, BYTE CharCode, DWORD Index, DWORD Offset, DWORD StringLen);
 long  _stdcall swCharPosFromLeftWS(PWSTR String, WORD CharCode);
 long  _stdcall swCharPosFromLeftWSL(PWSTR String, WORD CharCode, DWORD StringLen);
 long  _stdcall swCharPosFromLeftWSLIO(PWSTR String, WORD CharCode, DWORD Index, DWORD Offset, DWORD StringLen);

 long  _stdcall swCharPosFromRightAS(LPSTR String, BYTE CharCode);
 long  _stdcall swCharPosFromRightASL(LPSTR String, BYTE CharCode, DWORD StringLen);
 long  _stdcall swCharPosFromRightASLIO(LPSTR String, BYTE CharCode, DWORD Index, DWORD Offset, DWORD StringLen);
 long  _stdcall swCharPosFromRightWS(PWSTR String, WORD CharCode);
 long  _stdcall swCharPosFromRightWSL(PWSTR String, WORD CharCode, DWORD StringLen);
 long  _stdcall swCharPosFromRightWSLIO(PWSTR String, WORD CharCode, DWORD Index, DWORD Offset, DWORD StringLen);

 //+++++++++++++++++++ OVERLOADED INLINESPEC REDIRECTORS +++++++++++++++++++++++
 // Stupid compiler cannot compile PreDeclared INLINESPEC functions - Code must be right here
 INLINESPEC DWORD CALLSPEC swBytesInString(LPSTR String){return swBytesInStringA(String);}
 INLINESPEC DWORD CALLSPEC swBytesInString(PWSTR String){return swBytesInStringW(String);}

 INLINESPEC DWORD CALLSPEC swCharsInString(LPSTR String){return swCharsInStringA(String);}
 INLINESPEC DWORD CALLSPEC swCharsInString(PWSTR String){return swCharsInStringW(String);}

 INLINESPEC DWORD CALLSPEC swMergeString(LPSTR DstString, LPSTR SrcString){return swMergeStringADS(DstString,SrcString);}
 INLINESPEC DWORD CALLSPEC swMergeString(LPSTR DstString, LPSTR SrcString, DWORD CharsToCopy){return swMergeStringADSC(DstString,SrcString,CharsToCopy);}
 INLINESPEC DWORD CALLSPEC swMergeString(PWSTR DstString, PWSTR SrcString){return swMergeStringWDS(DstString,SrcString);}
 INLINESPEC DWORD CALLSPEC swMergeString(PWSTR DstString, PWSTR SrcString, DWORD CharsToCopy){return swMergeStringWDSC(DstString,SrcString,CharsToCopy);}

 INLINESPEC DWORD CALLSPEC swCopyString(LPSTR DstString, LPSTR SrcString){return swCopyStringADS(DstString,SrcString);}
 INLINESPEC DWORD CALLSPEC swCopyString(LPSTR DstString, LPSTR SrcString, DWORD CharsToCopy){return swCopyStringADSC(DstString,SrcString,CharsToCopy);}
 INLINESPEC DWORD CALLSPEC swCopyString(PWSTR DstString, PWSTR SrcString){return swCopyStringWDS(DstString,SrcString);}
 INLINESPEC DWORD CALLSPEC swCopyString(PWSTR DstString, PWSTR SrcString, DWORD CharsToCopy){return swCopyStringWDSC(DstString,SrcString,CharsToCopy);}

 INLINESPEC BOOL  CALLSPEC swIsWildcardStringsMatch(LPSTR StringA, LPSTR StringB){return swIsWildcardStringsMatchA(StringA,StringB);}
 INLINESPEC BOOL  CALLSPEC swIsWildcardStringsMatch(PWSTR StringA, PWSTR StringB){return swIsWildcardStringsMatchW(StringA,StringB);}

 INLINESPEC LONG  CALLSPEC swCharPosFromLeft(LPSTR String, BYTE CharCode){return swCharPosFromLeftAS(String,CharCode);}
 INLINESPEC LONG  CALLSPEC swCharPosFromLeft(PWSTR String, WORD CharCode){return swCharPosFromLeftWS(String,CharCode);}
 INLINESPEC LONG  CALLSPEC swCharPosFromLeft(LPSTR String, BYTE CharCode, DWORD StringLen){return swCharPosFromLeftASL(String,CharCode,StringLen);}
 INLINESPEC LONG  CALLSPEC swCharPosFromLeft(PWSTR String, WORD CharCode, DWORD StringLen){return swCharPosFromLeftWSL(String,CharCode,StringLen);}
 INLINESPEC LONG  CALLSPEC swCharPosFromLeft(LPSTR String, BYTE CharCode, DWORD Index, DWORD Offset){return swCharPosFromLeftASLIO(String,CharCode,Index,Offset,swCharsInStringA(String));}
 INLINESPEC LONG  CALLSPEC swCharPosFromLeft(PWSTR String, WORD CharCode, DWORD Index, DWORD Offset){return swCharPosFromLeftWSLIO(String,CharCode,Index,Offset,swCharsInStringW(String));}
 INLINESPEC LONG  CALLSPEC swCharPosFromLeft(LPSTR String, BYTE CharCode, DWORD Index, DWORD Offset, DWORD StringLen){return swCharPosFromLeftASLIO(String,CharCode,Index,Offset,StringLen);}
 INLINESPEC LONG  CALLSPEC swCharPosFromLeft(PWSTR String, WORD CharCode, DWORD Index, DWORD Offset, DWORD StringLen){return swCharPosFromLeftWSLIO(String,CharCode,Index,Offset,StringLen);}

 INLINESPEC LONG  CALLSPEC swCharPosFromRight(LPSTR String, BYTE CharCode){return swCharPosFromRightAS(String,CharCode);}
 INLINESPEC LONG  CALLSPEC swCharPosFromRight(PWSTR String, WORD CharCode){return swCharPosFromRightWS(String,CharCode);}
 INLINESPEC LONG  CALLSPEC swCharPosFromRight(LPSTR String, BYTE CharCode, DWORD StringLen){return swCharPosFromRightASL(String,CharCode,StringLen);}
 INLINESPEC LONG  CALLSPEC swCharPosFromRight(PWSTR String, WORD CharCode, DWORD StringLen){return swCharPosFromRightWSL(String,CharCode,StringLen);}
 INLINESPEC LONG  CALLSPEC swCharPosFromRight(LPSTR String, BYTE CharCode, DWORD Index, DWORD Offset){return swCharPosFromRightASLIO(String,CharCode,Index,Offset,swCharsInStringA(String));}
 INLINESPEC LONG  CALLSPEC swCharPosFromRight(PWSTR String, WORD CharCode, DWORD Index, DWORD Offset){return swCharPosFromRightWSLIO(String,CharCode,Index,Offset,swCharsInStringW(String));}
 INLINESPEC LONG  CALLSPEC swCharPosFromRight(LPSTR String, BYTE CharCode, DWORD Index, DWORD Offset, DWORD StringLen){return swCharPosFromRightASLIO(String,CharCode,Index,Offset,StringLen);}
 INLINESPEC LONG  CALLSPEC swCharPosFromRight(PWSTR String, WORD CharCode, DWORD Index, DWORD Offset, DWORD StringLen){return swCharPosFromRightWSLIO(String,CharCode,Index,Offset,StringLen);}

 //+++++++++++++++++++ INLINESPEC FUNCTIONS +++++++++++++++++++++++
 INLINESPEC DWORD _stdcall swBytesInStringA(LPSTR String){return (swCharsInStringA(String)+sizeof(BYTE));}
 INLINESPEC DWORD _stdcall swBytesInStringW(PWSTR String){return ((swCharsInStringW(String)+sizeof(BYTE))*sizeof(WORD));}
 INLINESPEC LONG  _stdcall swCharPosFromLeftASIO(LPSTR String, BYTE CharCode, DWORD Index, DWORD Offset){return swCharPosFromLeftASLIO(String,CharCode,Index,Offset,swCharsInStringA(String));}
 INLINESPEC LONG  _stdcall swCharPosFromLeftWSIO(PWSTR String, WORD CharCode, DWORD Index, DWORD Offset){return swCharPosFromLeftWSLIO(String,CharCode,Index,Offset,swCharsInStringW(String));}
 INLINESPEC LONG  _stdcall swCharPosFromRightASIO(LPSTR String, BYTE CharCode, DWORD Index, DWORD Offset){return swCharPosFromRightASLIO(String,CharCode,Index,Offset,swCharsInStringA(String));}
 INLINESPEC LONG  _stdcall swCharPosFromRightWSIO(PWSTR String, WORD CharCode, DWORD Index, DWORD Offset){return swCharPosFromRightWSLIO(String,CharCode,Index,Offset,swCharsInStringW(String));}
 //---------------------------------------------------------------------------
 // Writes ending zero
 // Returns number of chars copied+EndingZero
 //
 INLINESPEC DWORD _stdcall swCopyStringADS(LPSTR DstString, LPSTR SrcString)
  {
   register DWORD Length;

   Length = swMergeStringADS(DstString, SrcString);
   DstString[Length] = 0;       // Ending BYTE
   return (Length+sizeof(BYTE));
  }
 //---------------------------------------------------------------------------
 // Writes ending zero
 // Returns number of chars copied+EndingZero
 // Requests exact number of chars to copy (Trims from right 'SrcString' if 'CharsToCopy' < sizeof('SrcString'))
 //
 INLINESPEC DWORD _stdcall swCopyStringADSC(LPSTR DstString, LPSTR SrcString, DWORD CharsToCopy)
  {
   register DWORD Length;

   Length = swMergeStringADSC(DstString, SrcString, CharsToCopy);
   DstString[Length] = 0;       // Ending BYTE
   return (Length+sizeof(BYTE));
  }
 //---------------------------------------------------------------------------
 // Faster than Kernel32::lstrcpyW in FOUR times
 // Writes ending zero
 // Returns number of chars copied+EndingZero
 //
 INLINESPEC DWORD _stdcall swCopyStringWDS(PWSTR DstString, PWSTR SrcString)
  {
   register DWORD Length;

   Length = swMergeStringWDS(DstString, SrcString);
   DstString[Length] = 0;       // Ending WORD
   return (Length+sizeof(WORD));
  }
 //---------------------------------------------------------------------------
 // Writes ending zero
 // Returns number of chars copied+EndingZero
 // Requests exact number of chars to copy (Trims from right 'SrcString' if 'CharsToCopy' < sizeof('SrcString'))
 //
 INLINESPEC DWORD _stdcall swCopyStringWDSC(PWSTR DstString, PWSTR SrcString, DWORD CharsToCopy)
  {
   register DWORD Length;

   Length = swMergeStringWDSC(DstString, SrcString, CharsToCopy);
   DstString[Length] = 0;       // Ending WORD
   return (Length+sizeof(WORD));
  }                       
 //---------------------------------------------------------------------------

}
//---------------------------------------------------------------------------
#endif
