//---------------------------------------------------------------------------
#pragma hdrstop

#include "XString.h"

//===========================================================================
// Returns pointer to allocated buffer with string
//
PXSTRA _stdcall XSTRING::xsAllocXStringASL(LPSTR String, DWORD Length)
{
 register PVOID Buffer;

 if(String)
  {
   Buffer = MEMWORK::mwGetHeapMemory(Length+(XSMINSIZEA+HASAFEAREA));
   if(Buffer)
	{
	 if(XSTRING::xsInitXStringA(Buffer, String, (Length+XSMINSIZEA)))((DWORD*)Buffer)[XSOFPFLG] = ((((DWORD*)Buffer)[XSOFPFLG]) | XSFALMEM);
	 return (PXSTRA)_XPTRTOXSTR(Buffer);
	}
  }
 return NULL;
}
//---------------------------------------------------------------------------
// Returns pointer to allocated buffer with string
//
PXSTRW _stdcall XSTRING::xsAllocXStringWSL(PWSTR String, DWORD Length)
{
 register PVOID Buffer;

 if(String)
  {
   Length = (Length*sizeof(WORD));
   Buffer = MEMWORK::mwGetHeapMemory(Length+(XSMINSIZEW+HASAFEAREA));
   if(Buffer)
	{
	 if(XSTRING::xsInitXStringW(Buffer, String, (Length+XSMINSIZEW)))((DWORD*)Buffer)[XSOFPFLG] = ((((DWORD*)Buffer)[XSOFPFLG]) | XSFALMEM);
	 return (PXSTRW)_XPTRTOXSTR(Buffer);
	}
  }
 return NULL;
}
//---------------------------------------------------------------------------
// WARNING: DO NOT SAFE !!!!!
// Dynamically allocates string in stack - String will be NOT valid for PARENT of caller routine
// Will be automatically freed if used EBP stack frames
// Returns pointer to allocated buffer with string (valid only for caller routine)
//
__declspec(naked) PXSTRA _stdcall XSTRING::xsAllocXStringTmp(LPSTR String){__asm {Jmp xsAllocXStringTmpAS}}
__declspec(naked) PXSTRA _stdcall XSTRING::xsAllocXStringTmpAS(LPSTR String)
{
 using namespace STRWORK;
 __asm
  {
   pop  EDX                          // Get RETURN ADDRESS
   pop  ECX			                 // Get value of 'String'
   push EDX                          // Save RETURN ADDRESS
   push ECX                          // Save value of 'String'
   push ECX
   call swCharsInStringA             // (String)
   pop  ECX                          // Restore value of 'String'
   pop  EDX
   push EAX                          // Set value of 'StringLen'
   push ECX                          // Set value of 'String'
   push EDX                          // Set RETURN ADDRESS
   jmp xsAllocXStringTmpASL          // Returns direct to parent routine
  }
}
//---------------------------------------------------------------------------
// WARNING: DO NOT SAFE !!!!!
// Dynamically allocates string in stack - String will be NOT valid for PARENT of caller routine
// Will be automatically freed if used EBP stack frames
// Returns pointer to allocated buffer with string (valid only for caller routine)
//
__declspec(naked) PXSTRW _stdcall XSTRING::xsAllocXStringTmp(PWSTR String){__asm {Jmp xsAllocXStringTmpWS}}
__declspec(naked) PXSTRW _stdcall XSTRING::xsAllocXStringTmpWS(PWSTR String)
{
 using namespace STRWORK;
 __asm
  {
   pop  EDX                          // Get RETURN ADDRESS
   pop  ECX			                 // Get value of 'String'
   push EDX                          // Save RETURN ADDRESS
   push ECX                          // Save value of 'String'
   push ECX
   call swCharsInStringW             // (String)
   pop  ECX                          // Restore value of 'String'
   pop  EDX
   push EAX                          // Set value of 'StringLen'
   push ECX                          // Set value of 'String'
   push EDX                          // Set RETURN ADDRESS
   jmp xsAllocXStringTmpWSL          // Returns direct to parent routine
  }
}
//---------------------------------------------------------------------------
// WARNING: NOT SAFE !!!!!
// Dynamically allocates string in stack - String will be NOT valid for PARENT of caller routine
// Will be automatically freed if used EBP stack frames
// Returns pointer to allocated buffer with string (valid only for caller routine)
//
__declspec(naked) PXSTRA _stdcall XSTRING::xsAllocXStringTmp(LPSTR String, DWORD StringLen){__asm {Jmp xsAllocXStringTmpASL}}
__declspec(naked) PXSTRA _stdcall XSTRING::xsAllocXStringTmpASL(LPSTR String, DWORD StringLen)
{
 __asm
  {
   pop ECX                   // Get RETURN ADDRESS
   pop EAX                   // Get value of 'String'
   pop EDX                   // Get value of 'StringLen'
   test EAX,EAX
   jz Exit                   // Exit, if 'String' = NULL
   add EDX,XSMINSIZEA        // (Length+XSMINSIZEA)
   sub ESP,EDX               // Allocate 32bit aligned block in stack
   and ESP,CPUALIGNMASK      // Align to 32bit size of block
   push ESP                  // Save Buffer
   push ECX                  // Save Return ADDRESS
   push EDX                  // Length
   lea EDX,[ESP+0x0C]        // Fix pointer to prev PUSHs
   push EAX                  // String
   push EDX                  // Buffer
   call xsInitXStringA       // (Buffer, String, (Length+XSWRKSIZE)))
   pop ECX                   // Restore Return ADDRESS
   pop EDX                   // Restore Buffer
   test EAX,EAX
   jz NotInit
   mov EAX,[EDX]
   or EAX,XSFALTMP           // Set flag of allocation
   mov [EDX],EAX
NotInit:
   lea EAX,[EDX+XSWRKSIZE]
Exit:
   jmp ECX
  }
}
//---------------------------------------------------------------------------
// WARNING: NOT SAFE !!!!!
// Dynamically allocates string in stack - String will be NOT valid for PARENT of caller routine
// Will be automatically freed if used EBP stack frames
// Returns pointer to allocated buffer with string (valid only for caller routine)
//
__declspec(naked) PXSTRW _stdcall XSTRING::xsAllocXStringTmp(PWSTR String, DWORD StringLen){__asm {Jmp xsAllocXStringTmpWSL}}
__declspec(naked) PXSTRW _stdcall XSTRING::xsAllocXStringTmpWSL(PWSTR String, DWORD StringLen)
{
 __asm
  {
   pop ECX                   // Get RETURN ADDRESS
   pop EAX                   // Get value of 'String'
   pop EDX                   // Get value of 'StringLen'
   test EAX,EAX
   jz Exit                   // Exit, if 'String' = NULL
   lea EDX,[EDX*2]           // UNICODE chars to BYTES
   add EDX,XSMINSIZEW        // (Length+XSMINSIZEW)
   sub ESP,EDX               // Allocate 32bit aligned block in stack
   and ESP,CPUALIGNMASK      // Align to 32bit size of block
   push ESP                  // Save Buffer
   push ECX                  // Save Return ADDRESS
   push EDX                  // Length
   lea EDX,[ESP+0x0C]        // Fix pointer to prev PUSHs
   push EAX                  // String
   push EDX                  // Buffer
   call xsInitXStringW // (Buffer, String, (Length+XSWRKSIZE)))
   pop ECX                   // Restore Return ADDRESS
   pop EDX                   // Restore Buffer
   test EAX,EAX
   jz NotInit
   mov EAX,[EDX]
   or EAX,XSFALTMP           // Set flag of allocation
   mov [EDX],EAX
NotInit:
   lea EAX,[EDX+XSWRKSIZE]
Exit:
   jmp ECX
  }
}
//---------------------------------------------------------------------------
// Returns nubmer of bytes freed
//
__declspec(naked) DWORD _stdcall XSTRING::xsFreeXString(PVOID String)
{
 using namespace MEMWORK;
 __asm
  {
   mov EAX,[ESP+0x04]
   push EAX                      // Save value of 'String'
   push EAX
   call xsIsStringXString  // IsStringXString
   pop EDX                       // Restore value of 'String'
   lea EDX,[EDX-XSWRKSIZE]       // Ptr to XString --> Ptr to Buffer
   test EAX,EAX
   jz Exit                       // Exit, if 'String' is not XString
   mov EAX,[EDX]
   mov ECX,EAX
   and EAX,XSFALMSK
   test EAX,EAX
   jz Exit                       // Exit, if XString was not allocated with 'AllocXString' or AllocXStringTmp'
   // Get allocation type of memory block
   mov EAX,ECX
   and EAX,XSFALMEM
   test EAX,EAX
   jnz FreeHeapMem               // Jump, if XString was allocated in Heap
   // Get size of memory block to free
   mov EAX,ECX
   and EAX,XSFUNICD
   mov ECX,[EDX+4]
   inc ECX
   test EAX,EAX
   lea EAX,[ECX*2]
   cmovnz ECX,EAX                //  UNICODE string size
   add ECX,XSWRKSIZE
   // Align to 32bit size of block
   mov EAX,ECX
   and ECX,CPUALIGNMASK
   and EAX,CPUALIGNTRIM
   setnz AL
   shl EAX,CPUALIGNSDIV
   add EAX,ECX
   // Free momory from stack
   pop EDX                       // Get RETURN ADDRESS
   pop ECX                       // Remove value of 'String' from stack
   add ESP,EAX                   // Free 32bit aligned block in stack
   jmp EDX
   // Free momory from heap
FreeHeapMem:
   push EDX                      // Buffer
   call mwFreeHeapBlockEx  // A BUG IN BCC - Call must be to LAST command, Realized in CPP text file
Exit:
   ret 4
  }
}
//---------------------------------------------------------------------------

