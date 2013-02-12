//---------------------------------------------------------------------------
#pragma hdrstop

#include "StrWork.h"

//#pragma warning( disable: W8070 )  /* Function should return a value   How to disable ?*/
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// Faster than Kernel32::lstrlenA in TWO - THREE times
// Returns number of chars in string
//
__declspec(naked) DWORD _stdcall STRWORK::swCharsInStringA(LPSTR String)
{
 __asm
  {
   mov EAX,[ESP+0x04]     // Value of 'String'
DLoop:
   mov EDX,[EAX]
   test DL,DL
   setz CL
   test DH,DH
   setz CH
   bswap EDX
   bswap ECX
   test DL,DL
   setz CL
   test DH,DH
   setz CH
   add EAX,4
   test ECX,ECX
   jz DLoop               // Jump in one byte is ZERO
   sub EAX,5
BLoop:
   inc EAX
   mov DL,[EAX]
   test DL,DL
   jnz BLoop              // Exit if byte = 0
   sub EAX,[ESP+0x04]
   ret 4
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// Faster than Kernel32::lstrlenW in FOUR times
// Returns number of chars in string
//
__declspec(naked) DWORD _stdcall STRWORK::swCharsInStringW(PWSTR String)
{
 __asm
  {
   mov EAX,[ESP+0x04]     // Value of 'String'
DLoop:
   mov EDX,[EAX]
   test DX,DX
   setz CL
   bswap EDX
   test DX,DX
   setz CH
   add EAX,4
   test CX,CX
   jz DLoop               // Jump in one word is ZERO
   sub EAX,6
WLoop:
   add EAX,2
   mov DX,[EAX]
   test DX,DX
   jnz WLoop              // Exit if word = 0
   sub EAX,[ESP+0x04]
   shr EAX,1              // Bytes to UNICODE chars (bytes/2)
   ret 4
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// No ending zero
// Returns number of chars copied
// A bit faster then 'MergeString' with 'CharsToCopy' parameter
//
__declspec(naked) DWORD _stdcall STRWORK::swMergeStringADS(LPSTR DstString, LPSTR SrcString)
{
 __asm
  {
   push EDI
   push ESI
   mov EDI,[ESP+0x0C]     // Value of 'DstString'
   mov ESI,[ESP+0x10]     // Value of 'SrcString'
   pushfd
   cld
CLoop:
   // Check if one BYTE in DWORD is ZERO
   mov EAX,[ESI]
   test AL,AL
   setz DL
   test AH,AH
   setz DH
   bswap EAX
   bswap EDX
   test AL,AL
   setz DL
   test AH,AH
   setz DH
   test EDX,EDX
   jnz ByteByByte         // Jump in one byte is ZERO
   // Copy DWORD
   movsd
   jmp CLoop
   // Copy byte by byte From 1 to 3 max
ByteByByte:
   mov AL,[ESI]
   test AL,AL
   jz Exit                // Exit if byte = 0
   movsb
   jmp ByteByByte
   // String 'SrcString' was < 'CharsToCopy' by DWORD
Exit:
   popfd
   sub EDI,[ESP+0x0C]
   mov EAX,EDI            // Number of chars copied
   pop ESI
   pop EDI
   ret 8
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// No ending zero
// Returns number of chars copied
// Requests exact number of chars to copy (Trims from right 'SrcString' if 'CharsToCopy' < sizeof('SrcString'))
//
__declspec(naked) DWORD _stdcall STRWORK::swMergeStringADSC(LPSTR DstString, LPSTR SrcString, DWORD CharsToCopy)
{
 __asm
  {
   push EDI
   push ESI
   mov EDI,[ESP+0x0C]     // Value of 'DstString'
   mov ESI,[ESP+0x10]     // Value of 'SrcString'
   mov ECX,[ESP+0x14]     // Value of 'CharsToCopy'
   add ECX,EDI
   pushfd
   cld
CLoop:
   // Check if one BYTE in DWORD is ZERO
   mov EAX,[ESI]
   test AL,AL
   setz DL
   test AH,AH
   setz DH
   bswap EAX
   bswap EDX
   test AL,AL
   setz DL
   test AH,AH
   setz DH
   test EDX,EDX
   jnz ByteByByte         // Jump in one byte is ZERO
   lea EAX,[EDI+4]
   cmp EAX,ECX
   jae ByteByByte         // Counter exeeds
   // Copy DWORD
   movsd
   jmp CLoop
   // Copy byte by byte From 1 to 3 max
ByteByByte:
   mov AL,[ESI]
   test AL,AL
   jz Exit                // Exit if byte = 0
   cmp EDI,ECX
   jae Exit
   movsb
   jmp ByteByByte
   // String 'SrcString' was < 'CharsToCopy' by DWORD
Exit:
   popfd
   sub EDI,[ESP+0x0C]
   mov EAX,EDI            // Number of chars copied
   pop ESI
   pop EDI
   ret 12
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// No ending zero
// Returns number of chars copied
// A bit faster than BYTE string 'MergeString'
// A bit faster then 'MergeString' with 'CharsToCopy' parameter
//
__declspec(naked) DWORD _stdcall STRWORK::swMergeStringWDS(PWSTR DstString, PWSTR SrcString)
{
 __asm
  {
   push EDI
   push ESI
   mov EDI,[ESP+0x0C]     // Value of 'DstString'
   mov ESI,[ESP+0x10]     // Value of 'SrcString'
   pushfd
   cld
CLoop:
   // Check if one WORD in DWORD is ZERO
   mov EAX,[ESI]
   test AX,AX
   setz DL
   bswap EAX
   test AX,AX
   setz DH
   test DX,DX
   jnz WordByWord         // Jump in one word is ZERO
   // Copy DWORD
   movsd
   jmp CLoop
   // Copy byte by byte From 1 to 3 max
WordByWord:
   mov AX,[ESI]
   test AX,AX
   jz Exit                // Exit if word = 0
   movsw
   jmp WordByWord
   // String 'SrcString' was < 'CharsToCopy' by DWORD
Exit:
   popfd
   sub EDI,[ESP+0x0C]
   mov EAX,EDI            // Number of bytes copied
   shr EAX,1              // Bytes to UNICODE chars (bytes/2)
   pop ESI
   pop EDI
   ret 8
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// No ending zero
// Returns number of chars copied
// Requests exact number of chars to copy (Trims from right 'SrcString' if 'CharsToCopy' < sizeof('SrcString'))
// A bit faster than BYTE string 'MergeString'
//
__declspec(naked) DWORD _stdcall STRWORK::swMergeStringWDSC(PWSTR DstString, PWSTR SrcString, DWORD CharsToCopy)
{
 __asm
  {
   push EDI
   push ESI
   mov EDI,[ESP+0x0C]     // Value of 'DstString'
   mov ESI,[ESP+0x10]     // Value of 'SrcString'
   mov ECX,[ESP+0x14]     // Value of 'CharsToCopy'
   lea ECX,[ECX*2]        // UNICODE chars to BYTES
   add ECX,EDI
   pushfd
   cld
CLoop:
   // Check if one WORD in DWORD is ZERO
   mov EAX,[ESI]
   test AX,AX
   setz DL
   bswap EAX
   test AX,AX
   setz DH
   test DX,DX
   jnz WordByWord         // Jump in one word is ZERO
   lea EAX,[EDI+4]
   cmp EAX,ECX
   jae WordByWord        // Counter exeeds
   // Copy DWORD
   movsd
   jmp CLoop
   // Copy byte by byte From 1 to 3 max
WordByWord:
   mov AX,[ESI]
   test AX,AX
   jz Exit                // Exit if word = 0
   cmp EDI,ECX
   jae Exit
   movsw
   jmp WordByWord
   // String 'SrcString' was < 'CharsToCopy' by DWORD
Exit:
   popfd
   sub EDI,[ESP+0x0C]
   mov EAX,EDI            // Number of bytes copied
   shr EAX,1              // Bytes to UNICODE chars (bytes/2)
   pop ESI
   pop EDI
   ret 12
  }
}
//===========================================================================
// More than enough for wildcard strings comparision !!!
//
__declspec(naked) BOOL  _stdcall STRWORK::swIsWildcardStringsMatchA(LPSTR StringA, LPSTR StringB)
{
 __asm
  {
   push ESI
   push EDI
   mov  ESI,[ESP+0x0C]     // Value of 'StringA'
   mov  EDI,[ESP+0x10]     // Value of 'StringB'
   test ESI,EDI
   jz ExitNE               // Exit  if 'StringA' or 'StringB' = NULL
   dec ESI
   dec EDI
   // Test masked chars with '?'
LoopPassQ:
   inc ESI
   inc EDI
QTest:
   cmp BYTE PTR [ESI],'?'
   setz AL
   cmp BYTE PTR [EDI],'?'
   setz AH
   test AX,AX
   jnz  LoopPassQ
   // Test drop chars in string A
   cmp BYTE PTR [ESI],'*'
   jnz DTestSB
LoopSA:
   inc ESI
   cmp BYTE PTR [ESI],'?'
   jz  LoopSA
   // Drop asterisked chars in string B
   xor EAX,EAX
   mov CL,[ESI]
LoopCB:
   mov CH,[EDI]
   cmp CL,CH
   setnz AL            // For loop must be set
   test CH,CH
   setnz CH            // For loop must be set
   and AL,CH           // If AL and AH = 1, AL will be = 1
   lea EDI,[EDI+EAX]   // Increase String B pointer by 1
   jnz LoopCB
   jmp QTest
   // Test drop chars in string B
DTestSB:
   cmp BYTE PTR [EDI],'*'
   jnz ETest
LoopSB:
   inc EDI
   cmp BYTE PTR [EDI],'?'
   jz  LoopSB
   // Drop asterisked chars in string A
   xor EAX,EAX
   mov CL,[EDI]
LoopCA:
   mov CH,[ESI]
   cmp CL,CH
   setnz AL            // For loop must be set
   test CH,CH
   setnz CH            // For loop must be set
   and AL,CH           // If AL and AH = 1, AL will be = 1
   lea ESI,[ESI+EAX]   // Increase String A pointer by 1
   jnz LoopCA
   jmp QTest
   // Check if chars is equal
ETest:
   mov AL,[ESI]
   mov AH,[EDI]
   cmp AL,AH
   jnz ExitNE     // Jump if chars not match
   // Check if one of strings are at end
   test AL,AL
   setz AL        // Set, if String1 is end
   test AH,AH
   setz AH        // Set, if String2 is end
   test AX,AX
   jz LoopPassQ
   // Exit: strings is match
Exit:
   mov EAX,1
   pop EDI
   pop ESI
   ret 8
   // Exit: strings is not match
ExitNE:
   xor EAX,EAX
   pop EDI
   pop ESI
   ret 8
  }
}
//===========================================================================
// More than enough for wildcard strings comparision !!!
//
__declspec(naked) BOOL  _stdcall STRWORK::swIsWildcardStringsMatchW(PWSTR StringA, PWSTR StringB)
{
 __asm
  {
   push ESI
   push EDI
   mov  ESI,[ESP+0x0C]     // Value of 'StringA'
   mov  EDI,[ESP+0x10]     // Value of 'StringB'
   test ESI,EDI
   jz ExitNE               // Exit  if 'StringA' or 'StringB' = NULL
   sub ESI,2
   sub EDI,2
   // Test masked chars with '?'
LoopPassQ:
   add ESI,2
   add EDI,2
QTest:
   cmp WORD PTR [ESI],'?'
   setz AL
   cmp WORD PTR [EDI],'?'
   setz AH
   test AX,AX
   jnz  LoopPassQ
   // Test drop chars in string A
   cmp WORD PTR [ESI],'*'
   jnz DTestSB
LoopSA:
   add ESI,2
   cmp WORD PTR [ESI],'?'
   jz  LoopSA
   // Drop asterisked chars in string B
   xor EAX,EAX
   mov CX,[ESI]
LoopCB:
   mov DX,[EDI]
   cmp CX,DX
   setnz AL              // For loop must be set
   test DX,DX
   setnz DL              // For loop must be set
   and AL,DL             // If AL and AH = 1, AL will be = 1
   lea EDI,[EDI+EAX*2]   // Increase String B pointer by 2
   jnz LoopCB
   jmp QTest
   // Test drop chars in string B
DTestSB:
   cmp WORD PTR [EDI],'*'
   jnz ETest
LoopSB:
   add EDI,2
   cmp WORD PTR [EDI],'?'
   jz  LoopSB
   // Drop asterisked chars in string A
   xor EAX,EAX
   mov CX,[EDI]
LoopCA:
   mov DX,[ESI]
   cmp CX,DX
   setnz AL              // For loop must be set
   test DX,DX
   setnz DL              // For loop must be set
   and AL,DL             // If AL and AH = 1, AL will be = 1
   lea ESI,[ESI+EAX*2]   // Increase String A pointer by 2
   jnz LoopCA
   jmp QTest
   // Check if chars is equal
ETest:
   mov AX,[ESI]
   mov CX,[EDI]
   cmp AX,CX
   jnz ExitNE     // Jump if chars not match
   // Check if one of strings are at end
   test AX,AX
   setz AL        // Set, if String1 is end
   test CX,CX
   setz AH        // Set, if String2 is end
   test AX,AX
   jz LoopPassQ
   // Exit: strings is match
Exit:
   mov EAX,1
   pop EDI
   pop ESI
   ret 8
   // Exit: strings is not match
ExitNE:
   xor EAX,EAX
   pop EDI
   pop ESI
   ret 8
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// Fast char pos finder
// String points to start of string
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromLeftAS(LPSTR String, BYTE CharCode)
{
 __asm
  {
   push ESI
   mov  ESI,[ESP+0x08]   // Value of 'String'
   mov  DH,[ESP+0x0C]    // Value of 'CharCode'
   test ESI,ESI
   jz NullStrPtr
SLoop:
   mov ECX,4             // Bytes in Dword
   mov EAX,[ESI]
BLoop:
   cmp AL,DH
   jz CharFound          // Flaf of CharCode
   inc ESI
   test AL,AL
   ror EAX,8             // Do not changes Zero flag
   loopnz BLoop          // Loop bytes  (Slow but it is tests CX and not changes flags)
   jnz SLoop             // Loop dwords
NullStrPtr:
   mov EAX,-1            // CharCode is not found
   pop ESI
   ret 8
CharFound:
   sub ESI,[ESP+0x08]    // Value of 'String'
   mov EAX,ESI           // Position of CharCode
   pop ESI
   ret 8
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// Fast char pos finder
// String points to start of string
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromLeftASL(LPSTR String, BYTE CharCode, DWORD StringLen)
{
 __asm
  {
   mov EDX,EDI
   mov EDI,[ESP+0x04]    // Value of 'String'
   mov ECX,[ESP+0x0C]    // Value of 'StringLen'
   mov AL,[ESP+0x08]     // Value of 'CharCode'
   test EDI,EDI
   jz NullStrPtr
   repnz scasb
   jz CharFound
NullStrPtr:
   mov EAX,-1            // CharCode is not found
   mov EDI,EDX
   ret 12
CharFound:
   dec EDI
   sub EDI,[ESP+0x04]    // Value of 'String'
   mov EAX,EDI           // Position of CharCode
   mov EDI,EDX
   ret 12
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// Fast char pos finder
// String points to start of string
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromLeftWS(PWSTR String, WORD CharCode)
{
 __asm
  {
   push ESI
   mov  ESI,[ESP+0x08]   // Value of 'String'
   mov  DX,[ESP+0x0C]    // Value of 'CharCode'
   test ESI,ESI
   jz NullStrPtr
SLoop:
   mov ECX,2             // Bytes in Dword
   mov EAX,[ESI]
BLoop:
   cmp AX,DX
   jz CharFound          // Flaf of CharCode
   add ESI,2
   test AX,AX
   ror EAX,16            // Do not changes Zero flag
   loopnz BLoop          // Loop bytes  (Slow but it is tests CX and not changes flags)
   jnz SLoop             // Loop dwords
NullStrPtr:
   mov EAX,-1            // CharCode is not found
   pop ESI
   ret 8
CharFound:
   sub ESI,[ESP+0x08]    // Value of 'String'
   mov EAX,ESI           // Position of CharCode
   shr EAX,1             // Bytes to UNICODE chars (bytes/2)
   pop ESI
   ret 8
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// Fast char pos finder
// String points to start of string
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromLeftWSL(PWSTR String, WORD CharCode, DWORD StringLen)
{
 __asm
  {
   mov EDX,EDI
   mov EDI,[ESP+0x04]   // Value of 'String'
   mov ECX,[ESP+0x0C]   // Value of 'StringLen'
   mov AX,[ESP+0x08]    // Value of 'CharCode'
   test EDI,EDI
   jz NullStrPtr
   repnz scasw
   jz CharFound
NullStrPtr:
   mov EAX,-1            // CharCode is not found
   mov EDI,EDX
   ret 12
CharFound:
   sub EDI,2
   sub EDI,[ESP+0x04]    // Value of 'String'
   mov EAX,EDI           // Position of CharCode
   shr EAX,1             // Bytes to UNICODE chars (bytes/2)
   mov EDI,EDX
   ret 12
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// Fast char pos finder
// String points to start of string
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromRightAS(LPSTR String, BYTE CharCode)
{
 __asm
  {
   push EBX
   push ESI
   mov  EBX,-1
   mov  ESI,[ESP+0x0C]   // Value of 'String'
   mov  DH,[ESP+0x10]    // Value of 'CharCode'
   test ESI,ESI
   jz NullStrPtr
SLoop:
   mov ECX,4             // Bytes in Dword
   mov EAX,[ESI]
BLoop:
   cmp AL,DH
   jnz CharNotFound      // Flaf of CharCode
   mov EBX,ESI           // CharCode FOUND !!!
   sub EBX,[ESP+0x0C]    // Value of 'String'
CharNotFound:
   inc ESI
   test AL,AL
   ror EAX,8             // Do not changes Zero flag
   loopnz BLoop          // Loop bytes  (Slow but it is tests CX and not changes flags)
   jnz SLoop             // Loop dwords
NullStrPtr:
   mov EAX,EBX
   pop ESI
   pop EBX
   ret 8
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// Fast char pos finder
// String points to start of string
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromRightASL(LPSTR String, BYTE CharCode, DWORD StringLen)
{
 __asm
  {
   mov EDX,EDI
   mov EDI,[ESP+0x04]    // Value of 'String'
   mov ECX,[ESP+0x0C]    // Value of 'StringLen'
   mov AL,[ESP+0x08]     // Value of 'CharCode'
   test EDI,EDI
   jz NullStrPtr
   add EDI,ECX
   dec EDI
   pushfd
   std
   repnz scasb
   jz CharFound
   popfd
NullStrPtr:
   mov EAX,-1            // CharCode is not found
   mov EDI,EDX
   ret 12
CharFound:
   popfd
   inc EDI
   sub EDI,[ESP+0x04]    // Value of 'String'
   mov EAX,EDI           // Position of CharCode
   mov EDI,EDX
   ret 12
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// Fast char pos finder
// String points to start of string
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromRightWS(PWSTR String, WORD CharCode)
{
 __asm
  {
   push EBX
   push ESI
   mov  EBX,-1
   mov  ESI,[ESP+0x0C]   // Value of 'String'
   mov  DX,[ESP+0x10]    // Value of 'CharCode'
   test ESI,ESI
   jz NullStrPtr
SLoop:
   mov ECX,2             // Bytes in Dword
   mov EAX,[ESI]
BLoop:
   cmp AX,DX
   jnz CharNotFound      // Flaf of CharCode
   mov EBX,ESI           // CharCode FOUND !!!
   sub EBX,[ESP+0x0C]    // Value of 'String'
   shr EBX,1             // Bytes to UNICODE chars (bytes/2)
CharNotFound:
   add ESI,2
   test AX,AX
   ror EAX,16            // Do not changes Zero flag
   loopnz BLoop          // Loop bytes  (Slow but it is tests CX and not changes flags)
   jnz SLoop             // Loop dwords
NullStrPtr:
   mov EAX,EBX
   pop ESI
   pop EBX
   ret 8
  }
}
//===========================================================================
// warning: Used DWORD steps - Input Data Blocks must be aligned to DWORD or may be 'AccesVialation'
// Fast char pos finder
// String points to start of string
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromRightWSL(PWSTR String, WORD CharCode, DWORD StringLen)
{
 __asm
  {
   mov EDX,EDI
   mov EDI,[ESP+0x04]    // Value of 'String'
   mov ECX,[ESP+0x0C]    // Value of 'StringLen'
   mov AX,[ESP+0x08]     // Value of 'CharCode'
   test EDI,EDI
   jz NullStrPtr
   lea EDI,[EDI+ECX*2]
   sub EDI,2
   pushfd
   std
   repnz scasw
   jz CharFound
   popfd
NullStrPtr:
   mov EAX,-1            // CharCode is not found
   mov EDI,EDX
   ret 12
CharFound:
   popfd
   add EDI,2
   sub EDI,[ESP+0x04]    // Value of 'String'
   mov EAX,EDI           // Position of CharCode
   shr EAX,1             // Bytes to UNICODE chars (bytes/2)
   mov EDI,EDX
   ret 12
  }
}
//===========================================================================
// More complex char pos finder
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromLeftASLIO(LPSTR String, BYTE CharCode, DWORD Index, DWORD Offset, DWORD StringLen)
{
 __asm
  {
   push EBX
   push ESI
   push EDI
   xor EAX,EAX
   mov ESI,[ESP+0x10]    // Value of 'String'
   mov EDI,[ESP+0x20]    // Value of 'StringLen'
   mov ECX,[ESP+0x1C]    // Value of 'Offset'
   mov EBX,[ESP+0x18]    // Value of 'Index'    (From '0')
   test ESI,ESI
   jz Exit               // Exit if 'String' = NULL
   cmp ECX,EDI
   jae Exit              // 'Offset' >= 'StringLen'
   cmp EBX,EDI
   ja Exit               // 'Index' > 'StringLen'  (Even if each symbol in string = 'CharCode' -> 'Index' cannot be greater than 'StringLen')
   add ESI,ECX           // Set OFFSET
   sub EDI,ECX           // Update 'StringLen' for new 'String' PTR
ScanLoop:
   push EDI              // Value of OFFSETTED 'StringLen'
   push [ESP+0x18]       // Value of 'CharCode'
   push ESI              // Value of OFFSETTED 'String'
   call swCharPosFromLeftASL
   inc EAX               // If NOT_FOUND(-1) EAX will be = 0, else EAX converted to ONE based offset
   add ESI,EAX           // Set Pointer to next part of String
   sub EDI,EAX           // Trim scanned part of String
   test EAX,EAX
   jz Exit               // No 'CharCode' is found in String. 'CharCode' is not found on specified 'Index'
   dec EBX
   jnl ScanLoop
   sub ESI,[ESP+0x10]    // Value of 'String'
   mov EAX,ESI
Exit:
   dec EAX
   pop EDI
   pop ESI
   pop EBX
   ret 20
  }
}
//===========================================================================
// More complex char pos finder
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromLeftWSLIO(PWSTR String, WORD CharCode, DWORD Index, DWORD Offset, DWORD StringLen)
{
 __asm
  {
   push EBX
   push ESI
   push EDI
   xor EAX,EAX
   mov ESI,[ESP+0x10]    // Value of 'String'
   mov EDI,[ESP+0x20]    // Value of 'StringLen'
   mov ECX,[ESP+0x1C]    // Value of 'Offset'
   mov EBX,[ESP+0x18]    // Value of 'Index'    (From '0')
   test ESI,ESI
   jz Exit               // Exit if 'String' = NULL
   cmp ECX,EDI
   jae Exit              // 'Offset' >= 'StringLen'
   cmp EBX,EDI
   ja Exit               // 'Index' > 'StringLen'  (Even if each symbol in string = 'CharCode' -> 'Index' cannot be greater than 'StringLen')
   lea ESI,[ESI+ECX*2]   // Set OFFSET
   sub EDI,ECX           // Update 'StringLen' for new 'String' PTR
ScanLoop:
   push EDI              // Value of OFFSETTED 'StringLen'
   push [ESP+0x18]       // Value of 'CharCode'
   push ESI              // Value of OFFSETTED 'String'
   call swCharPosFromLeftWSL
   inc EAX               // If NOT_FOUND(-1) EAX will be = 0, else EAX converted to ONE based offset
   lea ESI,[ESI+EAX*2]   // Set Pointer to next part of String
   sub EDI,EAX           // Trim scanned part of String
   test EAX,EAX
   jz Exit               // No 'CharCode' is found in String. 'CharCode' is not found on specified 'Index'
   dec EBX
   jnl ScanLoop
   sub ESI,[ESP+0x10]    // Value of 'String'
   mov EAX,ESI
   shr EAX,1             // Bytes to UNICODE chars (bytes/2)
Exit:
   dec EAX
   pop EDI
   pop ESI
   pop EBX
   ret 20
  }
}
//===========================================================================
// More complex char pos finder
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromRightASLIO(LPSTR String, BYTE CharCode, DWORD Index, DWORD Offset, DWORD StringLen)
{
 __asm
  {
   push EBX
   push ESI
   push EDI
   xor EAX,EAX
   mov ESI,[ESP+0x10]    // Value of 'String'
   mov EDI,[ESP+0x20]    // Value of 'StringLen'
   mov ECX,[ESP+0x1C]    // Value of 'Offset'
   mov EBX,[ESP+0x18]    // Value of 'Index'    (From '0')
   test ESI,ESI
   jz Exit               // Exit if 'String' = NULL
   cmp ECX,EDI
   jae Exit              // 'Offset' >= 'StringLen'
   cmp EBX,EDI
   ja Exit               // 'Index' > 'StringLen'  (Even if each symbol in string = 'CharCode' -> 'Index' cannot be greater than 'StringLen')
   sub EDI,ECX           // Update 'StringLen' for OFFSET
ScanLoop:
   push EDI              // Value of OFFSETTED 'StringLen'
   push [ESP+0x18]       // Value of 'CharCode'
   push ESI              // Value of 'String'
   call swCharPosFromRightASL
   mov EDI,EAX           // Trim scanned part of String
   inc EAX               // If NOT_FOUND(-1) EAX will be = 0, else EAX converted to ONE based offset
   test EAX,EAX
   jz Exit               // No 'CharCode' is found in String. 'CharCode' is not found on specified 'Index'
   dec EBX
   jnl ScanLoop
Exit:
   dec EAX
   pop EDI
   pop ESI
   pop EBX
   ret 20
  }
}
//===========================================================================
// More complex char pos finder
//
__declspec(naked) LONG  _stdcall STRWORK::swCharPosFromRightWSLIO(PWSTR String, WORD CharCode, DWORD Index, DWORD Offset, DWORD StringLen)
{
 __asm
  {
   push EBX
   push ESI
   push EDI
   xor EAX,EAX
   mov ESI,[ESP+0x10]    // Value of 'String'
   mov EDI,[ESP+0x20]    // Value of 'StringLen'
   mov ECX,[ESP+0x1C]    // Value of 'Offset'
   mov EBX,[ESP+0x18]    // Value of 'Index'    (From '0')
   test ESI,ESI
   jz Exit               // Exit if 'String' = NULL
   cmp ECX,EDI
   jae Exit              // 'Offset' >= 'StringLen'
   cmp EBX,EDI
   ja Exit               // 'Index' > 'StringLen'  (Even if each symbol in string = 'CharCode' -> 'Index' cannot be greater than 'StringLen')
   sub EDI,ECX           // Update 'StringLen' for OFFSET
ScanLoop:
   push EDI              // Value of OFFSETTED 'StringLen'
   push [ESP+0x18]       // Value of 'CharCode'
   push ESI              // Value of 'String'
   call swCharPosFromRightWSL
   mov EDI,EAX           // Trim scanned part of String
   inc EAX               // If NOT_FOUND(-1) EAX will be = 0, else EAX converted to ONE based offset
   test EAX,EAX
   jz Exit               // No 'CharCode' is found in String. 'CharCode' is not found on specified 'Index'
   dec EBX
   jnl ScanLoop
Exit:
   dec EAX
   pop EDI
   pop ESI
   pop EBX
   ret 20
  }
}
//===========================================================================

