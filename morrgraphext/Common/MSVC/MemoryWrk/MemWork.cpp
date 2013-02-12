//---------------------------------------------------------------------------
#pragma hdrstop

#include "MemWork.h"

//===========================================================================
//                          ASSEMBLER FUNCTIONS
//---------------------------------------------------------------------------
// Align value by INCREASING
//             No need use this in ASM Routines, too universal
//
__declspec(naked) DWORD _stdcall MEMWORK::mwAlignForward(DWORD Value, DWORD Alignment)
{
 __asm
  {
   mov EAX,[ESP+0x04]      // Get value of 'Value'
   mov ECX,[ESP+0x08]      // Get value of 'Alignment'
   xor EDX,EDX             // Divided EDX:EAX
   div ECX
   mov   EAX,ECX
   test  EDX,EDX
   cmovz EAX,EDX
   sub   EAX,EDX           // EAX = Aligned Value
   add   EAX,[ESP+0x04]
   ret 8
  }
}
//===========================================================================
// Align value by DECREASING
//             No need use this in ASM Routines, too universal
//
__declspec(naked) DWORD _stdcall MEMWORK::mwAlignBackward(DWORD Value, DWORD Alignment)
{
 __asm
  {
   mov EAX,[ESP+0x04]      // Get value of 'Value'
   mov ECX,[ESP+0x08]      // Get value of 'Alignment'
   xor EDX,EDX             // Divided EDX:EAX
   div ECX
   mov EAX,[ESP+0x04]
   sub EAX,EDX             // EAX = Aligned Value
   ret 8
  }
}
//===========================================================================
//  Watch for stack usage !!!
//  Corrupts all PUSHed values before !!!
//  CANNOT be used with direct ESP addressing of LOCAL VARIABLES !!!
//
__declspec(naked) PVOID _stdcall MEMWORK::mwPushBlockToStack(DWORD Size)
{
 __asm
  {
   // Atign forward size of block for CPUs BYTEMAX
   mov EAX,[ESP+0x04]           // Value of 'Size'
   mov EDX,EAX
   and EAX,CPUALIGNMASK
   and EDX,CPUALIGNTRIM
   setnz DL
   shl EDX,CPUALIGNSDIV
   add EAX,EDX
   // Reserve space in stack
   pop EDX                     // Return address
   pop ECX                     // Get value of 'Size'
   sub ESP,EAX                 // Allocate 32bit aligned block in stack
   mov EAX,ESP
   jmp EDX
  }
}
//===========================================================================
// Use only if need reallocate another block - Stack always restored
// at end of routine with EBP based stack frame
//
__declspec(naked) void _stdcall MEMWORK::mwPopBlockFromStack(DWORD Size)
{
 __asm
  {
   // Atign forward size of block for CPUs BYTEMAX
   mov EAX,[ESP+0x04]           // Value of 'Size'
   mov EDX,EAX
   and EAX,CPUALIGNMASK
   and EDX,CPUALIGNTRIM
   setnz DL
   shl EDX,CPUALIGNSDIV
   add EAX,EDX
   // Free space in stack
   pop EDX                     // Return address
   pop ECX                     // Get value of 'Size'
   add ESP,EAX                 // Free 32bit aligned block in stack
   jmp EDX
  }
}
//===========================================================================
// Fill a memory block by specified value
//
__declspec(naked) PVOID _stdcall MEMWORK::mwFillMemory(PVOID DstBlk, DWORD Length, BYTE Value)
{
 __asm
  {
   push EDI
   // Make DWORD for fill value
   xor EAX,EAX
   mov AL,[ESP+0x10]      // Value of 'Value'
   mov AH,AL
   mov EDX,EAX
   bswap EAX
   or  EAX,EDX
   // Calculate lengths
   mov EDI,[ESP+0x08]     // Value of 'DstBlk'
   mov ECX,[ESP+0x0C]     // Value of 'Length'
   mov EDX,ECX
   and EDX,CPUALIGNTRIM   // Trim value to < BYTEMAX
   shr ECX,CPUALIGNSDIV   // Divide counter to CPUs BYTEMAX
   // Fill memory
   pushfd
   cld
   rep stosd              // Fill by DWORDs
   mov ECX,EDX
   rep stosb              // Fill by BYTEs , if ECX=0 nothing happens
   popfd
   pop EDI
   ret 12
  }
}
//===========================================================================
// Do not contains any JUMPS
// Simple and fast
//
__declspec(naked) PVOID _stdcall MEMWORK::mwCopyMemory(PVOID DstBlk, PVOID SrcBlk, DWORD Length)
{
 __asm
  {
   push EDI
   push ESI
   mov EDI,[ESP+0x0C]     // Value of 'DstBlk'
   mov ESI,[ESP+0x10]     // Value of 'SrcBlk'
   mov ECX,[ESP+0x14]     // Value of 'Length'
   mov EDX,ECX
   and EDX,CPUALIGNTRIM   // Trim value to < BYTEMAX
   shr ECX,CPUALIGNSDIV   // Divide counter to CPUs BYTEMAX
   // Copy memory
   pushfd
   cld
   rep movsd              // Copy by DWORDs
   mov ECX,EDX
   rep movsb              // Copy by BYTEs , if ECX=0 nothing happens
   popfd
   pop ESI
   pop EDI
   ret 12
  }
}
//===========================================================================
// Slower than CopyMemory but blocks may overlap
// RtlMoveMemory moves backward only by BYTES !!!
//
__declspec(naked) PVOID _stdcall MEMWORK::mwMoveMemory(PVOID DstBlk, PVOID SrcBlk, DWORD Length)
{
 __asm
  {
   push EDI
   push ESI
   pushfd
   mov EDI,[ESP+0x10]     // Value of 'DstBlk'
   mov ESI,[ESP+0x14]     // Value of 'SrcBlk'
   mov ECX,[ESP+0x18]     // Value of 'Length'
   cmp ESI,EDI
   ja  CpyMem
   je  MMExit             // exit if Src and Dst match
   // SrcAddr <= DstAddr
   mov EAX,EDI
   sub EAX,ESI            // DstAddr - SrcAddr
   cmp ECX,EAX            // Compare Length with AddrDiff
   jbe CpyMem             // Jump if blocks do not overlaps
   // Move backwards
   add ESI,ECX
   add EDI,ECX
   dec ESI
   dec EDI
   mov EDX,ECX
   and ECX,CPUALIGNTRIM   // Trim value to < BYTEMAX
   shr EDX,CPUALIGNSDIV   // Divide counter to CPUs BYTEMAX
   std
   rep movsb              // move bytes to DWORD
   sub ESI,CPUALIGNTRIM
   sub EDI,CPUALIGNTRIM
   mov ECX,EDX
   rep movsd              // move DWORDS
   popfd
   pop ESI
   pop EDI
   ret 12
  // Simple CopyMemory
CpyMem:
   cld
   mov EDX,ECX
   and EDX,CPUALIGNTRIM   // Trim value to < BYTEMAX
   shr ECX,CPUALIGNSDIV   // Divide counter to CPUs BYTEMAX
   rep movsd
   mov ECX,EDX
   rep movsb
MMExit:
   popfd
   pop ESI
   pop EDI
   ret 12
  }
}
//===========================================================================
// Compare blocks of same length
// Returns EAX=1 if content of blocks are equal
// Comparision stops on first different element
//
__declspec(naked) BOOL  _stdcall MEMWORK::mwIsMemoryEqualBBL(PVOID BlockA, PVOID BlockB, DWORD Length)
{
 __asm
  {
   push EDI
   push ESI
   pushfd
   mov EDI,[ESP+0x10]     // Value of 'BlockA'
   mov ESI,[ESP+0x14]     // Value of 'BlockB'
   mov ECX,[ESP+0x18]     // Value of 'Length'
   mov EDX,ECX
   and EDX,CPUALIGNTRIM   // Trim value to < BYTEMAX
   shr ECX,CPUALIGNSDIV   // Divide counter to CPUs BYTEMAX
   xor EAX,EAX
   cld
   repz cmpsd
   setz AL                // If NZ - EAX remains=0
   cmovnz EDX,EAX         // Reset BYTE counter if Mem do not match
   mov ECX,EDX
   repz cmpsb
   setz AH
   and AL,AH
   xor AH,AH
   popfd
   pop ESI
   pop EDI
   ret 12
  }
}
//===========================================================================
// Compare blocks of different size (no need CALCULATE MIN for length,
// c++ don`t do this fast)
// Returns EAX=1 if content of blocks are equal
// Comparision stops on first different element
//
__declspec(naked) BOOL  _stdcall MEMWORK::mwIsMemoryEqualBBLL(PVOID BlockA, PVOID BlockB, DWORD LengthA, DWORD LengthB)
{
 __asm
  {
   push EDI
   push ESI
   pushfd
   mov EDI,[ESP+0x10]     // Value of 'BlockA'
   mov ESI,[ESP+0x14]     // Value of 'BlockB'
   mov ECX,[ESP+0x18]     // Value of 'LengthA'
   mov EDX,[ESP+0x1C]     // Value of 'LengthB'
   cmp ECX,EDX
   cmova ECX,EDX          // Use lesser length as counter
   mov EDX,ECX
   and EDX,CPUALIGNTRIM   // Trim value to < BYTEMAX
   shr ECX,CPUALIGNSDIV   // Divide counter to CPUs BYTEMAX
   xor EAX,EAX
   cld
   repz cmpsd
   setz AL                // If NZ - EAX remains=0
   cmovnz EDX,EAX         // Reset BYTE counter if Mem do not match
   mov ECX,EDX
   repz cmpsb
   setz AH
   and AL,AH
   xor AH,AH
   popfd
   pop ESI
   pop EDI
   ret 12
  }
}
//===========================================================================
//
//
//
__declspec(naked) long  _stdcall MEMWORK::mwCompareMemoryBBL(PVOID BlockA, PVOID BlockB, DWORD Length)
{
 __asm
  {
   //
   //  ???????????????????????????????????????????????????????
   //
   ret 12
  }
}
//===========================================================================
//
//
//
__declspec(naked) long  _stdcall MEMWORK::mwCompareMemoryBBLL(PVOID BlockA, PVOID BlockB, DWORD LengthA, DWORD LengthB)
{
 __asm
  {
   //
   //  ???????????????????????????????????????????????????????
   //
   ret 12
  }
}
//===========================================================================
// Frees block and returns its size
//
DWORD _stdcall MEMWORK::mwFreeMemoryBlockEx(PVOID Block)
{
 register DWORD Size = NULL;
 register PVOID Base = NULL;
 MEMORY_BASIC_INFORMATION MemInf;

 do
  {
   VirtualQuery(Block,&MemInf,sizeof(MEMORY_BASIC_INFORMATION));
   Block = &((BYTE*)Block)[MemInf.RegionSize];
   if(!Base)Base = MemInf.AllocationBase;
   Size += MemInf.RegionSize;
  }
 while(MemInf.AllocationBase == Base);
 return (VirtualFree(Base,0,MEM_RELEASE)?Size:0);
}
//===========================================================================
// Frees block and returns its size
//
DWORD _stdcall MEMWORK::mwFreeHeapMemoryEx(PVOID Block)
{
 register DWORD Size;

 Size = HeapSize(GetProcessHeap(),0,Block);
 return (HeapFree(GetProcessHeap(),0,Block)?Size:0);
}
//===========================================================================

