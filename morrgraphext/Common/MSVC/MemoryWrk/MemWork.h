//---------------------------------------------------------------------------
#ifndef MemWorkH
#define MemWorkH

#include "Windows.h"

#define INLINESPEC __inline
#define CALLSPEC   __stdcall    // Only for OVERLOADED, and not ASSEMBLER
//============================================================================
//
//                         ONLY FOR 32bit CPUs
//
//---------------------------------------------------------------------------
#undef  FastFillMemory
#undef  FastZeroMemory
#undef  FastMoveMemory
#undef  FastCopyMemory
#undef  FastCompMemory
#define FastFillMemory MEMWORK::mwFillMemory
#define FastZeroMemory MEMWORK::mwZeroMemory
#define FastMoveMemory MEMWORK::mwMoveMemory
#define FastCopyMemory MEMWORK::mwCopyMemory
#define FastCompMemory !MEMWORK::mwIsMemoryEqual

#define CPUALIGNDATA 4
#define CPUALIGNTRIM (CPUALIGNDATA-1)
#define CPUALIGNMASK (-CPUALIGNDATA)
#define CPUALIGNSDIV 2                // 16=1; 32=2; 64=3
#define HASAFEAREA   8
//---------------------------------------------------------------------------
// Always make code compatible with BCB and MSVC !!!
// Do not overload exact functions (assembler cannot use overloaded funcs),
//   instead overload INLINE REDIRECTORS
// Do Not Let to Compiler GUESS wich function exactly use, always cpecify right one
// Do not use in separate namespaces - some function with names in standart IMPORT headers
// Use 'NAKED' specificator only in definitions, not in declarations !!!
// Always use '__asm' for assembler code !!!
//---------------------------------------------------------------------------
namespace MEMWORK
{
 DWORD _stdcall mwAlignForward(DWORD Value, DWORD Alignment);
 DWORD _stdcall mwAlignBackward(DWORD Value, DWORD Alignment);

 PVOID _stdcall mwPushBlockToStack(DWORD Size);
 void  _stdcall mwPopBlockFromStack(DWORD Size);

 PVOID _stdcall mwFillMemory(PVOID DstBlk, DWORD Length, BYTE Value);
 PVOID _stdcall mwCopyMemory(PVOID DstBlk, PVOID SrcBlk, DWORD Length);   // A bit faster but blocks cannot overlap
 PVOID _stdcall mwMoveMemory(PVOID DstBlk, PVOID SrcBlk, DWORD Length);   // Blocks may overlap

 BOOL  _stdcall mwIsMemoryEqualBBL(PVOID BlockA, PVOID BlockB, DWORD Length);
 long  _stdcall mwCompareMemoryBBL(PVOID BlockA, PVOID BlockB, DWORD Length);
 BOOL  _stdcall mwIsMemoryEqualBBLL(PVOID BlockA, PVOID BlockB, DWORD LengthA, DWORD LengthB);
 long  _stdcall mwCompareMemoryBBLL(PVOID BlockA, PVOID BlockB, DWORD LengthA, DWORD LengthB);

 DWORD _stdcall mwFreeHeapMemoryEx(PVOID Block);
 DWORD _stdcall mwFreeMemoryBlockEx(PVOID Block);

 //+++++++++++++++++++ OVERLOADED INLINE REDIRECTORS +++++++++++++++++++++++
 // Stupid compiler cannot compile PreDeclared INLINE functions - Code must be right here

 INLINESPEC PVOID CALLSPEC mwZeroMemory(PVOID DstBlk, DWORD Length){return  mwFillMemory(DstBlk, Length, 0);}
 INLINESPEC BOOL  CALLSPEC mwIsMemoryEqual(PVOID BlockA, PVOID BlockB, DWORD Length){return mwIsMemoryEqualBBL(BlockA,BlockB,Length);}
 INLINESPEC long  CALLSPEC mwCompareMemory(PVOID BlockA, PVOID BlockB, DWORD Length){return mwCompareMemoryBBL(BlockA,BlockB,Length);}
 INLINESPEC BOOL  CALLSPEC mwIsMemoryEqual(PVOID BlockA, PVOID BlockB, DWORD LengthA, DWORD LengthB){return mwIsMemoryEqualBBLL(BlockA,BlockB,LengthA,LengthB);}
 INLINESPEC long  CALLSPEC mwCompareMemory(PVOID BlockA, PVOID BlockB, DWORD LengthA, DWORD LengthB){return mwCompareMemoryBBLL(BlockA,BlockB,LengthA,LengthB);}

 //+++++++++++++++++++ INLINE FUNCTIONS +++++++++++++++++++++++
 INLINESPEC PVOID _stdcall ReAllocHeapMem(PVOID Mem,DWORD Size){return HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,Mem,Size);}
 INLINESPEC PVOID _stdcall GetHeapMemory(DWORD Size){return HeapAlloc(GetProcessHeap(),HEAP_GENERATE_EXCEPTIONS|HEAP_ZERO_MEMORY,Size);}
 INLINESPEC PVOID _stdcall GetMemoryBlock(DWORD Size){return VirtualAlloc(NULL,Size,MEM_COMMIT,PAGE_EXECUTE_READWRITE);}
 INLINESPEC BOOL  _stdcall FreeHeapMemory(PVOID Block){return HeapFree(GetProcessHeap(),0,Block);}
 INLINESPEC BOOL  _stdcall FreeMemoryBlock(PVOID Block){return VirtualFree(Block,0,MEM_RELEASE);}
}
//---------------------------------------------------------------------------
#endif
