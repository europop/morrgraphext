// Free Disassembler and Assembler -- Command data and service routines
//
// Copyright (C) 2001 Oleh Yuschuk
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// 05.03.2002: Corrected error, FSTSW AX assembled with data size prefix


#define STRICT
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#pragma hdrstop

#define unique

////////////////////////////////////////////////////////////////////////////////

#define NEGLIMIT       (-16384)        // Limit to display constans as signed
#define PSEUDOOP       128             // Base for pseudooperands
#define TEXTLEN        256             // Maximal length of text string

// Special command features.
#define WW             0x01            // Bit W (size of operand)
#define SS             0x02            // Bit S (sign extention of immediate)
#define WS             0x03            // Bits W and S
#define W3             0x08            // Bit W at position 3
#define CC             0x10            // Conditional jump
#define FF             0x20            // Forced 16-bit size
#define LL             0x40            // Conditional loop
#define PR             0x80            // Protected command
#define WP             0x81            // I/O command with bit W

// All possible types of operands in 80x86. A bit more than you expected, he?
#define NNN            0               // No operand
#define REG            1               // Integer register in Reg field
#define RCM            2               // Integer register in command byte
#define RG4            3               // Integer 4-byte register in Reg field
#define RAC            4               // Accumulator (AL/AX/EAX, implicit)
#define RAX            5               // AX (2-byte, implicit)
#define RDX            6               // DX (16-bit implicit port address)
#define RCL            7               // Implicit CL register (for shifts)
#define RS0            8               // Top of FPU stack (ST(0), implicit)
#define RST            9               // FPU register (ST(i)) in command byte
#define RMX            10              // MMX register MMx
#define R3D            11              // 3DNow! register MMx
#define MRG            12              // Memory/register in ModRM byte
#define MR1            13              // 1-byte memory/register in ModRM byte
#define MR2            14              // 2-byte memory/register in ModRM byte
#define MR4            15              // 4-byte memory/register in ModRM byte
#define RR4            16              // 4-byte memory/register (register only)
#define MR8            17              // 8-byte memory/MMX register in ModRM
#define RR8            18              // 8-byte MMX register only in ModRM
#define MRD            19              // 8-byte memory/3DNow! register in ModRM
#define RRD            20              // 8-byte memory/3DNow! (register only)
#define MRJ            21              // Memory/reg in ModRM as JUMP target
#define MMA            22              // Memory address in ModRM byte for LEA
#define MML            23              // Memory in ModRM byte (for LES)
#define MMS            24              // Memory in ModRM byte (as SEG:OFFS)
#define MM6            25              // Memory in ModRm (6-byte descriptor)
#define MMB            26              // Two adjacent memory locations (BOUND)
#define MD2            27              // Memory in ModRM (16-bit integer)
#define MB2            28              // Memory in ModRM (16-bit binary)
#define MD4            29              // Memory in ModRM byte (32-bit integer)
#define MD8            30              // Memory in ModRM byte (64-bit integer)
#define MDA            31              // Memory in ModRM byte (80-bit BCD)
#define MF4            32              // Memory in ModRM byte (32-bit float)
#define MF8            33              // Memory in ModRM byte (64-bit float)
#define MFA            34              // Memory in ModRM byte (80-bit float)
#define MFE            35              // Memory in ModRM byte (FPU environment)
#define MFS            36              // Memory in ModRM byte (FPU state)
#define MFX            37              // Memory in ModRM byte (ext. FPU state)
#define MSO            38              // Source in string op's ([ESI])
#define MDE            39              // Destination in string op's ([EDI])
#define MXL            40              // XLAT operand ([EBX+AL])
#define IMM            41              // Immediate data (8 or 16/32)
#define IMU            42              // Immediate unsigned data (8 or 16/32)
#define VXD            43              // VxD service
#define IMX            44              // Immediate sign-extendable byte
#define C01            45              // Implicit constant 1 (for shifts)
#define IMS            46              // Immediate byte (for shifts)
#define IM1            47              // Immediate byte
#define IM2            48              // Immediate word (ENTER/RET)
#define IMA            49              // Immediate absolute near data address
#define JOB            50              // Immediate byte offset (for jumps)
#define JOW            51              // Immediate full offset (for jumps)
#define JMF            52              // Immediate absolute far jump/call addr
#define SGM            53              // Segment register in ModRM byte
#define SCM            54              // Segment register in command byte
#define CRX            55              // Control register CRx
#define DRX            56              // Debug register DRx
// Pseudooperands (implicit operands, never appear in assembler commands). Must
// have index equal to or exceeding PSEUDOOP.
#define PRN            (PSEUDOOP+0)    // Near return address
#define PRF            (PSEUDOOP+1)    // Far return address
#define PAC            (PSEUDOOP+2)    // Accumulator (AL/AX/EAX)
#define PAH            (PSEUDOOP+3)    // AH (in LAHF/SAHF commands)
#define PFL            (PSEUDOOP+4)    // Lower byte of flags (in LAHF/SAHF)
#define PS0            (PSEUDOOP+5)    // Top of FPU stack (ST(0))
#define PS1            (PSEUDOOP+6)    // ST(1)
#define PCX            (PSEUDOOP+7)    // CX/ECX
#define PDI            (PSEUDOOP+8)    // EDI (in MMX extentions)

// Errors detected during command disassembling.
#define DAE_NOERR      0               // No error
#define DAE_BADCMD     1               // Unrecognized command
#define DAE_CROSS      2               // Command crosses end of memory block
#define DAE_BADSEG     3               // Undefined segment register
#define DAE_MEMORY     4               // Register where only memory allowed
#define DAE_REGISTER   5               // Memory where only register allowed
#define DAE_INTERN     6               // Internal error

typedef unsigned char  uchar;          // Unsigned character (byte)
typedef unsigned short ushort;         // Unsigned short
typedef unsigned int   uint;           // Unsigned integer
typedef unsigned long  ulong;          // Unsigned long

typedef struct t_addrdec {
  int            defseg;
  unsigned char           *descr;
} t_addrdec;

typedef struct t_cmddata {
  ulong          mask;                 // Mask for first 4 bytes of the command
  ulong          code;                 // Compare masked bytes with this
  unsigned char           len;                  // Length of the main command code
  unsigned char           bits;                 // Special bits within the command
  unsigned char           arg1,arg2,arg3;       // Types of possible arguments
  unsigned char           type;                 // C_xxx + additional information
  unsigned char           *name;                // Symbolic name for this command
} t_cmddata;

// Initialized constant data structures used by all programs from assembler
// package. Contain names of register, register combinations or commands and
// their properties.
extern const unsigned char      *regname[3][9];
extern const unsigned char      *segname[8];
extern const unsigned char      *sizename[11];
extern const t_addrdec addr16[8];
extern const t_addrdec addr32[8];
extern const unsigned char      *fpuname[9];
extern const unsigned char      *mmxname[9];
extern const unsigned char      *crname[9];
extern const unsigned char      *drname[9];
extern const unsigned char      *condition[16];
extern const t_cmddata cmddata[];
extern const t_cmddata vxdcmd;
extern const t_cmddata dangerous[];



////////////////////////////////////////////////////////////////////////////////
//////////////////// ASSEMBLER, DISASSEMBLER AND EXPRESSIONS ///////////////////

#define MAXCMDSIZE     16              // Maximal length of 80x86 command
#define MAXCALSIZE     8               // Max length of CALL without prefixes
#define NMODELS        8               // Number of assembler search models

#define INT3           0xCC            // Code of 1-byte breakpoint
#define NOP            0x90            // Code of 1-byte NOP command
#define TRAPFLAG       0x00000100      // Trap flag in CPU flag register

#define REG_EAX        0               // Indexes of general-purpose registers
#define REG_ECX        1               // in t_reg.
#define REG_EDX        2
#define REG_EBX        3
#define REG_ESP        4
#define REG_EBP        5
#define REG_ESI        6
#define REG_EDI        7

#define SEG_UNDEF     -1
#define SEG_ES         0               // Indexes of segment/selector registers
#define SEG_CS         1
#define SEG_SS         2
#define SEG_DS         3
#define SEG_FS         4
#define SEG_GS         5

#define C_TYPEMASK     0xF0            // Mask for command type
#define   C_CMD        0x00            // Ordinary instruction
#define   C_PSH        0x10            // 1-word PUSH instruction
#define   C_POP        0x20            // 1-word POP instruction
#define   C_MMX        0x30            // MMX instruction
#define   C_FLT        0x40            // FPU instruction
#define   C_JMP        0x50            // JUMP instruction
#define   C_JMC        0x60            // Conditional JUMP instruction
#define   C_CAL        0x70            // CALL instruction
#define   C_RET        0x80            // RET instruction
#define   C_FLG        0x90            // Changes system flags
#define   C_RTF        0xA0            // C_JMP and C_FLG simultaneously
#define   C_REP        0xB0            // Instruction with REPxx prefix
#define   C_PRI        0xC0            // Privileged instruction
#define   C_DAT        0xD0            // Data (address) doubleword
#define   C_NOW        0xE0            // 3DNow! instruction
#define   C_BAD        0xF0            // Unrecognized command
#define C_RARE         0x08            // Rare command, seldom used in programs
#define C_SIZEMASK     0x07            // MMX data size or special flag
#define   C_EXPL       0x01            // (non-MMX) Specify explicit memory size

#define C_DANGER95     0x01            // Command is dangerous under Win95/98
#define C_DANGER       0x03            // Command is dangerous everywhere
#define C_DANGERLOCK   0x07            // Dangerous with LOCK prefix

#define DEC_TYPEMASK   0x1F            // Type of memory byte
#define   DEC_UNKNOWN  0x00            // Unknown type
#define   DEC_BYTE     0x01            // Accessed as byte
#define   DEC_WORD     0x02            // Accessed as short
#define   DEC_NEXTDATA 0x03            // Subsequent byte of code or data
#define   DEC_DWORD    0x04            // Accessed as long
#define   DEC_FLOAT4   0x05            // Accessed as float
#define   DEC_FWORD    0x06            // Accessed as descriptor/long pointer
#define   DEC_FLOAT8   0x07            // Accessed as double
#define   DEC_QWORD    0x08            // Accessed as 8-byte integer
#define   DEC_FLOAT10  0x09            // Accessed as long double
#define   DEC_TBYTE    0x0A            // Accessed as 10-byte integer
#define   DEC_STRING   0x0B            // Zero-terminated ASCII string
#define   DEC_UNICODE  0x0C            // Zero-terminated UNICODE string
#define   DEC_3DNOW    0x0D            // Accessed as 3Dnow operand
#define   DEC_BYTESW   0x11            // Accessed as byte index to switch
#define   DEC_NEXTCODE 0x13            // Subsequent byte of command
#define   DEC_COMMAND  0x1D            // First byte of command
#define   DEC_JMPDEST  0x1E            // Jump destination
#define   DEC_CALLDEST 0x1F            // Call (and maybe jump) destination
#define DEC_PROCMASK   0x60            // Procedure analysis
#define   DEC_PROC     0x20            // Start of procedure
#define   DEC_PBODY    0x40            // Body of procedure
#define   DEC_PEND     0x60            // End of procedure
#define DEC_CHECKED    0x80            // Byte was analysed

#define DECR_TYPEMASK  0x3F            // Type of register or memory
#define   DECR_BYTE    0x21            // Byte register
#define   DECR_WORD    0x22            // Short integer register
#define   DECR_DWORD   0x24            // Long integer register
#define   DECR_QWORD   0x28            // MMX register
#define   DECR_FLOAT10 0x29            // Floating-point register
#define   DECR_SEG     0x2A            // Segment register
#define   DECR_3DNOW   0x2D            // 3Dnow! register
#define DECR_ISREG     0x20            // Mask to check that operand is register

#define DISASM_SIZE    0               // Determine command size only
#define DISASM_DATA    1               // Determine size and analysis data
#define DISASM_FILE    3               // Disassembly, no symbols
#define DISASM_CODE    4               // Full disassembly

// Warnings issued by Disasm():
#define DAW_FARADDR    0x0001          // Command is a far jump, call or return
#define DAW_SEGMENT    0x0002          // Command loads segment register
#define DAW_PRIV       0x0004          // Privileged command
#define DAW_IO         0x0008          // I/O command
#define DAW_SHIFT      0x0010          // Shift constant out of range 1..31
#define DAW_PREFIX     0x0020          // Superfluous prefix
#define DAW_LOCK       0x0040          // Command has LOCK prefix
#define DAW_STACK      0x0080          // Unaligned stack operation
#define DAW_DANGER95   0x1000          // May mess up Win95 if executed
#define DAW_DANGEROUS  0x3000          // May mess up any OS if executed

typedef struct t_disasm {              // Results of disassembling
  ulong          ip;                   // Instrucion pointer
  unsigned char           dump[TEXTLEN];        // Hexadecimal dump of the command
  unsigned char           result[TEXTLEN];      // Disassembled command
  unsigned char           comment[TEXTLEN];     // Brief comment
  int            cmdtype;              // One of C_xxx
  int            memtype;              // Type of addressed variable in memory
  int            nprefix;              // Number of prefixes
  int            indexed;              // Address contains register(s)
  ulong          jmpconst;             // Constant jump address
  ulong          jmptable;             // Possible address of switch table
  ulong          adrconst;             // Constant part of address
  ulong          immconst;             // Immediate constant
  int            zeroconst;            // Whether contains zero constant
  int            fixupoffset;          // Possible offset of 32-bit fixups
  int            fixupsize;            // Possible total size of fixups or 0
  int            error;                // Error while disassembling command
  int            warnings;             // Combination of DAW_xxx
} t_disasm;

typedef struct t_asmmodel {            // Model to search for assembler command
  unsigned char           code[MAXCMDSIZE];     // Binary code
  unsigned char           mask[MAXCMDSIZE];     // Mask for binary code (0: bit ignored)
  int            length;               // Length of code, bytes (0: empty)
  int            jmpsize;              // Offset size if relative jump
  int            jmpoffset;            // Offset relative to IP
  int            jmppos;               // Position of jump offset in command
} t_asmmodel;

unique int       ideal;                // Force IDEAL decoding mode
unique int       lowercase;            // Force lowercase display
unique int       tabarguments;         // Tab between mnemonic and arguments
unique int       extraspace;           // Extra space between arguments
unique int       putdefseg;            // Display default segments in listing
unique int       showmemsize;          // Always show memory size
unique int       shownear;             // Show NEAR modifiers
unique int       shortstringcmds;      // Use short form of string commands
unique int       sizesens;             // How to decode size-sensitive mnemonics
unique int       symbolic;             // Show symbolic addresses in disasm
unique int       farcalls;             // Accept far calls, returns & addresses
unique int       decodevxd;            // Decode VxD calls (Win95/98)
unique int       privileged;           // Accept privileged commands
unique int       iocommand;            // Accept I/O commands
unique int       badshift;             // Accept shift out of range 1..31
unique int       extraprefix;          // Accept superfluous prefixes
unique int       lockedbus;            // Accept LOCK prefixes
unique int       stackalign;           // Accept unaligned stack operations
unique int       iswindowsnt;          // When checking for dangers, assume NT

int    Assemble(unsigned char *cmd,ulong ip,t_asmmodel *model,int attempt,
         int constsize,unsigned char *errtext);
int    Checkcondition(int code,ulong flags);
int    Decodeaddress(ulong addr,unsigned char *symb,int nsymb,unsigned char *comment);
ulong  Disasm(unsigned char *src,ulong srcsize,ulong srcip,
         t_disasm *disasm,int disasmmode);
ulong  Disassembleback(unsigned char *block,ulong base,ulong size,ulong ip,int n);
ulong  Disassembleforward(unsigned char *block,ulong base,ulong size,ulong ip,int n);
int    Isfilling(ulong addr,unsigned char *data,ulong size,ulong align);
int    Print3dnow(unsigned char *s,unsigned char *f);
int    Printfloat10(unsigned char *s,long double ext);
int    Printfloat4(unsigned char *s,float f);
int    Printfloat8(unsigned char *s,double d);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  DEFINITIONS  /////////////////////////////////

const unsigned char *regname[3][9] = {
  { "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH", "R8"  },
  { "AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI", "R16" },
  { "EAX","ECX","EDX","EBX","ESP","EBP","ESI","EDI","R32" } };

const unsigned char *segname[8] = {
  "ES","CS","SS","DS","FS","GS","SEG?","SEG?" };

const unsigned char *sizename[11] = {
  "(0-BYTE)", "BYTE", "WORD", "(3-BYTE)",
  "DWORD", "(5-BYTE)", "FWORD", "(7-BYTE)",
  "QWORD", "(9-BYTE)", "TBYTE" };

const t_addrdec addr16[8] = {
  { SEG_DS, (unsigned char*)"BX+SI" }, { SEG_DS, (unsigned char*)"BX+DI" },
  { SEG_SS, (unsigned char*)"BP+SI" }, { SEG_SS, (unsigned char*)"BP+DI" },
  { SEG_DS, (unsigned char*)"SI" },    { SEG_DS, (unsigned char*)"DI" },
  { SEG_SS, (unsigned char*)"BP" },    { SEG_DS, (unsigned char*)"BX" } };

const t_addrdec addr32[8] = {
  { SEG_DS, (unsigned char*)"EAX" }, { SEG_DS, (unsigned char*)"ECX" },
  { SEG_DS, (unsigned char*)"EDX" }, { SEG_DS, (unsigned char*)"EBX" },
  { SEG_SS, (unsigned char*)"" },    { SEG_SS, (unsigned char*)"EBP" },
  { SEG_DS, (unsigned char*)"ESI" }, { SEG_DS, (unsigned char*)"EDI" } };

const unsigned char *fpuname[9] = {
  "ST0","ST1","ST2","ST3","ST4","ST5","ST6","ST7","FPU" };

const unsigned char *mmxname[9] = {
  "MM0","MM1","MM2","MM3","MM4","MM5","MM6","MM7","MMX" };

const unsigned char *crname[9] = {
  "CR0","CR1","CR2","CR3","CR4","CR5","CR6","CR7","CRX" };

const unsigned char *drname[9] = {
  "DR0","DR1","DR2","DR3","DR4","DR5","DR6","DR7","DRX" };

// List of available processor commands with decoding, types of parameters and
// other useful information. Last element has field mask=0. If mnemonic begins
// with ampersand ('&'), its mnemonic decodes differently depending on operand
// size (16 or 32 bits). If mnemonic begins with dollar ('$'), this mnemonic
// depends on address size. Semicolon (':') separates 16-bit form from 32-bit,
// asterisk ('*') will be substituted by either W (16), D (32) or none (16/32)
// character. If command is of type C_MMX or C_NOW, or if type contains C_EXPL
// (=0x01), Disassembler must specify explicit size of memory operand.
const t_cmddata cmddata[] = {
  { 0x0000FF, 0x000090, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"NOP" },
  { 0x0000FE, 0x00008A, 1,WW,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"MOV" },
  { 0x0000F8, 0x000050, 1,00,  RCM,NNN,NNN, C_PSH+0,        (unsigned char*)"PUSH" },
  { 0x0000FE, 0x000088, 1,WW,  MRG,REG,NNN, C_CMD+0,        (unsigned char*)"MOV" },
  { 0x0000FF, 0x0000E8, 1,00,  JOW,NNN,NNN, C_CAL+0,        (unsigned char*)"CALL" },
  { 0x0000FD, 0x000068, 1,SS,  IMM,NNN,NNN, C_PSH+0,        (unsigned char*)"PUSH" },
  { 0x0000FF, 0x00008D, 1,00,  REG,MMA,NNN, C_CMD+0,        (unsigned char*)"LEA" },
  { 0x0000FF, 0x000074, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JE,JZ" },
  { 0x0000F8, 0x000058, 1,00,  RCM,NNN,NNN, C_POP+0,        (unsigned char*)"POP" },
  { 0x0038FC, 0x000080, 1,WS,  MRG,IMM,NNN, C_CMD+1,        (unsigned char*)"ADD" },
  { 0x0000FF, 0x000075, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JNZ,JNE" },
  { 0x0000FF, 0x0000EB, 1,00,  JOB,NNN,NNN, C_JMP+0,        (unsigned char*)"JMP" },
  { 0x0000FF, 0x0000E9, 1,00,  JOW,NNN,NNN, C_JMP+0,        (unsigned char*)"JMP" },
  { 0x0000FE, 0x000084, 1,WW,  MRG,REG,NNN, C_CMD+0,        (unsigned char*)"TEST" },
  { 0x0038FE, 0x0000C6, 1,WW,  MRG,IMM,NNN, C_CMD+1,        (unsigned char*)"MOV" },
  { 0x0000FE, 0x000032, 1,WW,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"XOR" },
  { 0x0000FE, 0x00003A, 1,WW,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMP" },
  { 0x0038FC, 0x003880, 1,WS,  MRG,IMM,NNN, C_CMD+1,        (unsigned char*)"CMP" },
  { 0x0038FF, 0x0010FF, 1,00,  MRJ,NNN,NNN, C_CAL+0,        (unsigned char*)"CALL" },
  { 0x0000FF, 0x0000C3, 1,00,  PRN,NNN,NNN, C_RET+0,        (unsigned char*)"RETN,RET" },
  { 0x0000F0, 0x0000B0, 1,W3,  RCM,IMM,NNN, C_CMD+0,        (unsigned char*)"MOV" },
  { 0x0000FE, 0x0000A0, 1,WW,  RAC,IMA,NNN, C_CMD+0,        (unsigned char*)"MOV" },
  { 0x00FFFF, 0x00840F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JE,JZ" },
  { 0x0000F8, 0x000040, 1,00,  RCM,NNN,NNN, C_CMD+0,        (unsigned char*)"INC" },
  { 0x0038FE, 0x0000F6, 1,WW,  MRG,IMU,NNN, C_CMD+1,        (unsigned char*)"TEST" },
  { 0x0000FE, 0x0000A2, 1,WW,  IMA,RAC,NNN, C_CMD+0,        (unsigned char*)"MOV" },
  { 0x0000FE, 0x00002A, 1,WW,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"SUB" },
  { 0x0000FF, 0x00007E, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JLE,JNG" },
  { 0x00FFFF, 0x00850F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JNZ,JNE" },
  { 0x0000FF, 0x0000C2, 1,00,  IM2,PRN,NNN, C_RET+0,        (unsigned char*)"RETN" },
  { 0x0038FF, 0x0030FF, 1,00,  MRG,NNN,NNN, C_PSH+1,        (unsigned char*)"PUSH" },
  { 0x0038FC, 0x000880, 1,WS,  MRG,IMU,NNN, C_CMD+1,        (unsigned char*)"OR" },
  { 0x0038FC, 0x002880, 1,WS,  MRG,IMM,NNN, C_CMD+1,        (unsigned char*)"SUB" },
  { 0x0000F8, 0x000048, 1,00,  RCM,NNN,NNN, C_CMD+0,        (unsigned char*)"DEC" },
  { 0x00FFFF, 0x00BF0F, 2,00,  REG,MR2,NNN, C_CMD+1,        (unsigned char*)"MOVSX" },
  { 0x0000FF, 0x00007C, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JL,JNGE" },
  { 0x0000FE, 0x000002, 1,WW,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"ADD" },
  { 0x0038FC, 0x002080, 1,WS,  MRG,IMU,NNN, C_CMD+1,        (unsigned char*)"AND" },
  { 0x0000FE, 0x00003C, 1,WW,  RAC,IMM,NNN, C_CMD+0,        (unsigned char*)"CMP" },
  { 0x0038FF, 0x0020FF, 1,00,  MRJ,NNN,NNN, C_JMP+0,        (unsigned char*)"JMP" },
  { 0x0038FE, 0x0010F6, 1,WW,  MRG,NNN,NNN, C_CMD+1,        (unsigned char*)"NOT" },
  { 0x0038FE, 0x0028C0, 1,WW,  MRG,IMS,NNN, C_CMD+1,        (unsigned char*)"SHR" },
  { 0x0000FE, 0x000038, 1,WW,  MRG,REG,NNN, C_CMD+0,        (unsigned char*)"CMP" },
  { 0x0000FF, 0x00007D, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JGE,JNL" },
  { 0x0000FF, 0x00007F, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JG,JNLE" },
  { 0x0038FE, 0x0020C0, 1,WW,  MRG,IMS,NNN, C_CMD+1,        (unsigned char*)"SHL" },
  { 0x0000FE, 0x00001A, 1,WW,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"SBB" },
  { 0x0038FE, 0x0018F6, 1,WW,  MRG,NNN,NNN, C_CMD+1,        (unsigned char*)"NEG" },
  { 0x0000FF, 0x0000C9, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"LEAVE" },
  { 0x0000FF, 0x000060, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"&PUSHA*" },
  { 0x0038FF, 0x00008F, 1,00,  MRG,NNN,NNN, C_POP+1,        (unsigned char*)"POP" },
  { 0x0000FF, 0x000061, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"&POPA*" },
  { 0x0000F8, 0x000090, 1,00,  RAC,RCM,NNN, C_CMD+0,        (unsigned char*)"XCHG" },
  { 0x0000FE, 0x000086, 1,WW,  MRG,REG,NNN, C_CMD+0,        (unsigned char*)"XCHG" },
  { 0x0000FE, 0x000000, 1,WW,  MRG,REG,NNN, C_CMD+0,        (unsigned char*)"ADD" },
  { 0x0000FE, 0x000010, 1,WW,  MRG,REG,NNN, C_CMD+C_RARE+0, (unsigned char*)"ADC" },
  { 0x0000FE, 0x000012, 1,WW,  REG,MRG,NNN, C_CMD+C_RARE+0, (unsigned char*)"ADC" },
  { 0x0000FE, 0x000020, 1,WW,  MRG,REG,NNN, C_CMD+0,        (unsigned char*)"AND" },
  { 0x0000FE, 0x000022, 1,WW,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"AND" },
  { 0x0000FE, 0x000008, 1,WW,  MRG,REG,NNN, C_CMD+0,        (unsigned char*)"OR" },
  { 0x0000FE, 0x00000A, 1,WW,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"OR" },
  { 0x0000FE, 0x000028, 1,WW,  MRG,REG,NNN, C_CMD+0,        (unsigned char*)"SUB" },
  { 0x0000FE, 0x000018, 1,WW,  MRG,REG,NNN, C_CMD+C_RARE+0, (unsigned char*)"SBB" },
  { 0x0000FE, 0x000030, 1,WW,  MRG,REG,NNN, C_CMD+0,        (unsigned char*)"XOR" },
  { 0x0038FC, 0x001080, 1,WS,  MRG,IMM,NNN, C_CMD+C_RARE+1, (unsigned char*)"ADC" },
  { 0x0038FC, 0x001880, 1,WS,  MRG,IMM,NNN, C_CMD+C_RARE+1, (unsigned char*)"SBB" },
  { 0x0038FC, 0x003080, 1,WS,  MRG,IMU,NNN, C_CMD+1,        (unsigned char*)"XOR" },
  { 0x0000FE, 0x000004, 1,WW,  RAC,IMM,NNN, C_CMD+0,        (unsigned char*)"ADD" },
  { 0x0000FE, 0x000014, 1,WW,  RAC,IMM,NNN, C_CMD+C_RARE+0, (unsigned char*)"ADC" },
  { 0x0000FE, 0x000024, 1,WW,  RAC,IMU,NNN, C_CMD+0,        (unsigned char*)"AND" },
  { 0x0000FE, 0x00000C, 1,WW,  RAC,IMU,NNN, C_CMD+0,        (unsigned char*)"OR" },
  { 0x0000FE, 0x00002C, 1,WW,  RAC,IMM,NNN, C_CMD+0,        (unsigned char*)"SUB" },
  { 0x0000FE, 0x00001C, 1,WW,  RAC,IMM,NNN, C_CMD+C_RARE+0, (unsigned char*)"SBB" },
  { 0x0000FE, 0x000034, 1,WW,  RAC,IMU,NNN, C_CMD+0,        (unsigned char*)"XOR" },
  { 0x0038FE, 0x0000FE, 1,WW,  MRG,NNN,NNN, C_CMD+1,        (unsigned char*)"INC" },
  { 0x0038FE, 0x0008FE, 1,WW,  MRG,NNN,NNN, C_CMD+1,        (unsigned char*)"DEC" },
  { 0x0000FE, 0x0000A8, 1,WW,  RAC,IMU,NNN, C_CMD+0,        (unsigned char*)"TEST" },
  { 0x0038FE, 0x0020F6, 1,WW,  MRG,NNN,NNN, C_CMD+1,        (unsigned char*)"MUL" },
  { 0x0038FE, 0x0028F6, 1,WW,  MRG,NNN,NNN, C_CMD+1,        (unsigned char*)"IMUL" },
  { 0x00FFFF, 0x00AF0F, 2,00,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"IMUL" },
  { 0x0000FF, 0x00006B, 1,00,  REG,MRG,IMX, C_CMD+C_RARE+0, (unsigned char*)"IMUL" },
  { 0x0000FF, 0x000069, 1,00,  REG,MRG,IMM, C_CMD+C_RARE+0, (unsigned char*)"IMUL" },
  { 0x0038FE, 0x0030F6, 1,WW,  MRG,NNN,NNN, C_CMD+1,        (unsigned char*)"DIV" },
  { 0x0038FE, 0x0038F6, 1,WW,  MRG,NNN,NNN, C_CMD+1,        (unsigned char*)"IDIV" },
  { 0x0000FF, 0x000098, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"&CBW:CWDE" },
  { 0x0000FF, 0x000099, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"&CWD:CDQ" },
  { 0x0038FE, 0x0000D0, 1,WW,  MRG,C01,NNN, C_CMD+1,        (unsigned char*)"ROL" },
  { 0x0038FE, 0x0008D0, 1,WW,  MRG,C01,NNN, C_CMD+1,        (unsigned char*)"ROR" },
  { 0x0038FE, 0x0010D0, 1,WW,  MRG,C01,NNN, C_CMD+1,        (unsigned char*)"RCL" },
  { 0x0038FE, 0x0018D0, 1,WW,  MRG,C01,NNN, C_CMD+1,        (unsigned char*)"RCR" },
  { 0x0038FE, 0x0020D0, 1,WW,  MRG,C01,NNN, C_CMD+1,        (unsigned char*)"SHL" },
  { 0x0038FE, 0x0028D0, 1,WW,  MRG,C01,NNN, C_CMD+1,        (unsigned char*)"SHR" },
  { 0x0038FE, 0x0038D0, 1,WW,  MRG,C01,NNN, C_CMD+1,        (unsigned char*)"SAR" },
  { 0x0038FE, 0x0000D2, 1,WW,  MRG,RCL,NNN, C_CMD+1,        (unsigned char*)"ROL" },
  { 0x0038FE, 0x0008D2, 1,WW,  MRG,RCL,NNN, C_CMD+1,        (unsigned char*)"ROR" },
  { 0x0038FE, 0x0010D2, 1,WW,  MRG,RCL,NNN, C_CMD+1,        (unsigned char*)"RCL" },
  { 0x0038FE, 0x0018D2, 1,WW,  MRG,RCL,NNN, C_CMD+1,        (unsigned char*)"RCR" },
  { 0x0038FE, 0x0020D2, 1,WW,  MRG,RCL,NNN, C_CMD+1,        (unsigned char*)"SHL" },
  { 0x0038FE, 0x0028D2, 1,WW,  MRG,RCL,NNN, C_CMD+1,        (unsigned char*)"SHR" },
  { 0x0038FE, 0x0038D2, 1,WW,  MRG,RCL,NNN, C_CMD+1,        (unsigned char*)"SAR" },
  { 0x0038FE, 0x0000C0, 1,WW,  MRG,IMS,NNN, C_CMD+1,        (unsigned char*)"ROL" },
  { 0x0038FE, 0x0008C0, 1,WW,  MRG,IMS,NNN, C_CMD+1,        (unsigned char*)"ROR" },
  { 0x0038FE, 0x0010C0, 1,WW,  MRG,IMS,NNN, C_CMD+1,        (unsigned char*)"RCL" },
  { 0x0038FE, 0x0018C0, 1,WW,  MRG,IMS,NNN, C_CMD+1,        (unsigned char*)"RCR" },
  { 0x0038FE, 0x0038C0, 1,WW,  MRG,IMS,NNN, C_CMD+1,        (unsigned char*)"SAR" },
  { 0x0000FF, 0x000070, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JO" },
  { 0x0000FF, 0x000071, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JNO" },
  { 0x0000FF, 0x000072, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JB,JC" },
  { 0x0000FF, 0x000073, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JNB,JNC" },
  { 0x0000FF, 0x000076, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JBE,JNA" },
  { 0x0000FF, 0x000077, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JA,JNBE" },
  { 0x0000FF, 0x000078, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JS" },
  { 0x0000FF, 0x000079, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JNS" },
  { 0x0000FF, 0x00007A, 1,CC,  JOB,NNN,NNN, C_JMC+C_RARE+0, (unsigned char*)"JPE,JP" },
  { 0x0000FF, 0x00007B, 1,CC,  JOB,NNN,NNN, C_JMC+C_RARE+0, (unsigned char*)"JPO,JNP" },
  { 0x0000FF, 0x0000E3, 1,00,  JOB,NNN,NNN, C_JMC+C_RARE+0, (unsigned char*)"$JCXZ:JECXZ" },
  { 0x00FFFF, 0x00800F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JO" },
  { 0x00FFFF, 0x00810F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JNO" },
  { 0x00FFFF, 0x00820F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JB,JC" },
  { 0x00FFFF, 0x00830F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JNB,JNC" },
  { 0x00FFFF, 0x00860F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JBE,JNA" },
  { 0x00FFFF, 0x00870F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JA,JNBE" },
  { 0x00FFFF, 0x00880F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JS" },
  { 0x00FFFF, 0x00890F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JNS" },
  { 0x00FFFF, 0x008A0F, 2,CC,  JOW,NNN,NNN, C_JMC+C_RARE+0, (unsigned char*)"JPE,JP" },
  { 0x00FFFF, 0x008B0F, 2,CC,  JOW,NNN,NNN, C_JMC+C_RARE+0, (unsigned char*)"JPO,JNP" },
  { 0x00FFFF, 0x008C0F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JL,JNGE" },
  { 0x00FFFF, 0x008D0F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JGE,JNL" },
  { 0x00FFFF, 0x008E0F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JLE,JNG" },
  { 0x00FFFF, 0x008F0F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JG,JNLE" },
  { 0x0000FF, 0x0000F8, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"CLC" },
  { 0x0000FF, 0x0000F9, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"STC" },
  { 0x0000FF, 0x0000F5, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"CMC" },
  { 0x0000FF, 0x0000FC, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"CLD" },
  { 0x0000FF, 0x0000FD, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"STD" },
  { 0x0000FF, 0x0000FA, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"CLI" },
  { 0x0000FF, 0x0000FB, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"STI" },
  { 0x0000FF, 0x00008C, 1,FF,  MRG,SGM,NNN, C_CMD+C_RARE+0, (unsigned char*)"MOV" },
  { 0x0000FF, 0x00008E, 1,FF,  SGM,MRG,NNN, C_CMD+C_RARE+0, (unsigned char*)"MOV" },
  { 0x0000FE, 0x0000A6, 1,WW,  MSO,MDE,NNN, C_CMD+1,        (unsigned char*)"CMPS" },
  { 0x0000FE, 0x0000AC, 1,WW,  MSO,NNN,NNN, C_CMD+1,        (unsigned char*)"LODS" },
  { 0x0000FE, 0x0000A4, 1,WW,  MDE,MSO,NNN, C_CMD+1,        (unsigned char*)"MOVS" },
  { 0x0000FE, 0x0000AE, 1,WW,  MDE,PAC,NNN, C_CMD+1,        (unsigned char*)"SCAS" },
  { 0x0000FE, 0x0000AA, 1,WW,  MDE,PAC,NNN, C_CMD+1,        (unsigned char*)"STOS" },
  { 0x00FEFF, 0x00A4F3, 1,WW,  MDE,MSO,PCX, C_REP+1,        (unsigned char*)"REP MOVS" },
  { 0x00FEFF, 0x00ACF3, 1,WW,  MSO,PAC,PCX, C_REP+C_RARE+1, (unsigned char*)"REP LODS" },
  { 0x00FEFF, 0x00AAF3, 1,WW,  MDE,PAC,PCX, C_REP+1,        (unsigned char*)"REP STOS" },
  { 0x00FEFF, 0x00A6F3, 1,WW,  MDE,MSO,PCX, C_REP+1,        (unsigned char*)"REPE CMPS" },
  { 0x00FEFF, 0x00AEF3, 1,WW,  MDE,PAC,PCX, C_REP+1,        (unsigned char*)"REPE SCAS" },
  { 0x00FEFF, 0x00A6F2, 1,WW,  MDE,MSO,PCX, C_REP+1,        (unsigned char*)"REPNE CMPS" },
  { 0x00FEFF, 0x00AEF2, 1,WW,  MDE,PAC,PCX, C_REP+1,        (unsigned char*)"REPNE SCAS" },
  { 0x0000FF, 0x0000EA, 1,00,  JMF,NNN,NNN, C_JMP+C_RARE+0, (unsigned char*)"JMP" },
  { 0x0038FF, 0x0028FF, 1,00,  MMS,NNN,NNN, C_JMP+C_RARE+1, (unsigned char*)"JMP" },
  { 0x0000FF, 0x00009A, 1,00,  JMF,NNN,NNN, C_CAL+C_RARE+0, (unsigned char*)"CALL" },
  { 0x0038FF, 0x0018FF, 1,00,  MMS,NNN,NNN, C_CAL+C_RARE+1, (unsigned char*)"CALL" },
  { 0x0000FF, 0x0000CB, 1,00,  PRF,NNN,NNN, C_RET+C_RARE+0, (unsigned char*)"RETF" },
  { 0x0000FF, 0x0000CA, 1,00,  IM2,PRF,NNN, C_RET+C_RARE+0, (unsigned char*)"RETF" },
  { 0x00FFFF, 0x00A40F, 2,00,  MRG,REG,IMS, C_CMD+0,        (unsigned char*)"SHLD" },
  { 0x00FFFF, 0x00AC0F, 2,00,  MRG,REG,IMS, C_CMD+0,        (unsigned char*)"SHRD" },
  { 0x00FFFF, 0x00A50F, 2,00,  MRG,REG,RCL, C_CMD+0,        (unsigned char*)"SHLD" },
  { 0x00FFFF, 0x00AD0F, 2,00,  MRG,REG,RCL, C_CMD+0,        (unsigned char*)"SHRD" },
  { 0x00F8FF, 0x00C80F, 2,00,  RCM,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"BSWAP" },
  { 0x00FEFF, 0x00C00F, 2,WW,  MRG,REG,NNN, C_CMD+C_RARE+0, (unsigned char*)"XADD" },
  { 0x0000FF, 0x0000E2, 1,LL,  JOB,PCX,NNN, C_JMC+0,        (unsigned char*)"$LOOP*" },
  { 0x0000FF, 0x0000E1, 1,LL,  JOB,PCX,NNN, C_JMC+0,        (unsigned char*)"$LOOP*E" },
  { 0x0000FF, 0x0000E0, 1,LL,  JOB,PCX,NNN, C_JMC+0,        (unsigned char*)"$LOOP*NE" },
  { 0x0000FF, 0x0000C8, 1,00,  IM2,IM1,NNN, C_CMD+0,        (unsigned char*)"ENTER" },
  { 0x0000FE, 0x0000E4, 1,WP,  RAC,IM1,NNN, C_CMD+C_RARE+0, (unsigned char*)"IN" },
  { 0x0000FE, 0x0000EC, 1,WP,  RAC,RDX,NNN, C_CMD+C_RARE+0, (unsigned char*)"IN" },
  { 0x0000FE, 0x0000E6, 1,WP,  IM1,RAC,NNN, C_CMD+C_RARE+0, (unsigned char*)"OUT" },
  { 0x0000FE, 0x0000EE, 1,WP,  RDX,RAC,NNN, C_CMD+C_RARE+0, (unsigned char*)"OUT" },
  { 0x0000FE, 0x00006C, 1,WP,  MDE,RDX,NNN, C_CMD+C_RARE+1, (unsigned char*)"INS" },
  { 0x0000FE, 0x00006E, 1,WP,  RDX,MDE,NNN, C_CMD+C_RARE+1, (unsigned char*)"OUTS" },
  { 0x00FEFF, 0x006CF3, 1,WP,  MDE,RDX,PCX, C_REP+C_RARE+1, (unsigned char*)"REP INS" },
  { 0x00FEFF, 0x006EF3, 1,WP,  RDX,MDE,PCX, C_REP+C_RARE+1, (unsigned char*)"REP OUTS" },
  { 0x0000FF, 0x000037, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"AAA" },
  { 0x0000FF, 0x00003F, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"AAS" },
  { 0x00FFFF, 0x000AD4, 2,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"AAM" },
  { 0x0000FF, 0x0000D4, 1,00,  IM1,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"AAM" },
  { 0x00FFFF, 0x000AD5, 2,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"AAD" },
  { 0x0000FF, 0x0000D5, 1,00,  IM1,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"AAD" },
  { 0x0000FF, 0x000027, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"DAA" },
  { 0x0000FF, 0x00002F, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"DAS" },
  { 0x0000FF, 0x0000F4, 1,PR,  NNN,NNN,NNN, C_PRI+C_RARE+0, (unsigned char*)"HLT" },
  { 0x0000FF, 0x00000E, 1,00,  SCM,NNN,NNN, C_PSH+C_RARE+0, (unsigned char*)"PUSH" },
  { 0x0000FF, 0x000016, 1,00,  SCM,NNN,NNN, C_PSH+C_RARE+0, (unsigned char*)"PUSH" },
  { 0x0000FF, 0x00001E, 1,00,  SCM,NNN,NNN, C_PSH+C_RARE+0, (unsigned char*)"PUSH" },
  { 0x0000FF, 0x000006, 1,00,  SCM,NNN,NNN, C_PSH+C_RARE+0, (unsigned char*)"PUSH" },
  { 0x00FFFF, 0x00A00F, 2,00,  SCM,NNN,NNN, C_PSH+C_RARE+0, (unsigned char*)"PUSH" },
  { 0x00FFFF, 0x00A80F, 2,00,  SCM,NNN,NNN, C_PSH+C_RARE+0, (unsigned char*)"PUSH" },
  { 0x0000FF, 0x00001F, 1,00,  SCM,NNN,NNN, C_POP+C_RARE+0, (unsigned char*)"POP" },
  { 0x0000FF, 0x000007, 1,00,  SCM,NNN,NNN, C_POP+C_RARE+0, (unsigned char*)"POP" },
  { 0x0000FF, 0x000017, 1,00,  SCM,NNN,NNN, C_POP+C_RARE+0, (unsigned char*)"POP" },
  { 0x00FFFF, 0x00A10F, 2,00,  SCM,NNN,NNN, C_POP+C_RARE+0, (unsigned char*)"POP" },
  { 0x00FFFF, 0x00A90F, 2,00,  SCM,NNN,NNN, C_POP+C_RARE+0, (unsigned char*)"POP" },
  { 0x0000FF, 0x0000D7, 1,00,  MXL,NNN,NNN, C_CMD+C_RARE+1, (unsigned char*)"XLAT" },
  { 0x00FFFF, 0x00BE0F, 2,00,  REG,MR1,NNN, C_CMD+1,        (unsigned char*)"MOVSX" },
  { 0x00FFFF, 0x00B60F, 2,00,  REG,MR1,NNN, C_CMD+1,        (unsigned char*)"MOVZX" },
  { 0x00FFFF, 0x00B70F, 2,00,  REG,MR2,NNN, C_CMD+1,        (unsigned char*)"MOVZX" },
  { 0x0000FF, 0x00009B, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"WAIT" },
  { 0x0000FF, 0x00009F, 1,00,  PAH,PFL,NNN, C_CMD+C_RARE+0, (unsigned char*)"LAHF" },
  { 0x0000FF, 0x00009E, 1,00,  PFL,PAH,NNN, C_CMD+C_RARE+0, (unsigned char*)"SAHF" },
  { 0x0000FF, 0x00009C, 1,00,  NNN,NNN,NNN, C_PSH+0,        (unsigned char*)"&PUSHF*" },
  { 0x0000FF, 0x00009D, 1,00,  NNN,NNN,NNN, C_FLG+0,        (unsigned char*)"&POPF*" },
  { 0x0000FF, 0x0000CD, 1,00,  IM1,NNN,NNN, C_CAL+C_RARE+0, (unsigned char*)"INT" },
  { 0x0000FF, 0x0000CC, 1,00,  NNN,NNN,NNN, C_CAL+C_RARE+0, (unsigned char*)"INT3" },
  { 0x0000FF, 0x0000CE, 1,00,  NNN,NNN,NNN, C_CAL+C_RARE+0, (unsigned char*)"INTO" },
  { 0x0000FF, 0x0000CF, 1,00,  NNN,NNN,NNN, C_RTF+C_RARE+0, (unsigned char*)"&IRET*" },
  { 0x00FFFF, 0x00900F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETO" },
  { 0x00FFFF, 0x00910F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETNO" },
  { 0x00FFFF, 0x00920F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETB,SETC" },
  { 0x00FFFF, 0x00930F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETNB,SETNC" },
  { 0x00FFFF, 0x00940F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETE,SETZ" },
  { 0x00FFFF, 0x00950F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETNE,SETNZ" },
  { 0x00FFFF, 0x00960F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETBE,SETNA" },
  { 0x00FFFF, 0x00970F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETA,SETNBE" },
  { 0x00FFFF, 0x00980F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETS" },
  { 0x00FFFF, 0x00990F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETNS" },
  { 0x00FFFF, 0x009A0F, 2,CC,  MR1,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"SETPE,SETP" },
  { 0x00FFFF, 0x009B0F, 2,CC,  MR1,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"SETPO,SETNP" },
  { 0x00FFFF, 0x009C0F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETL,SETNGE" },
  { 0x00FFFF, 0x009D0F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETGE,SETNL" },
  { 0x00FFFF, 0x009E0F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETLE,SETNG" },
  { 0x00FFFF, 0x009F0F, 2,CC,  MR1,NNN,NNN, C_CMD+0,        (unsigned char*)"SETG,SETNLE" },
  { 0x38FFFF, 0x20BA0F, 2,00,  MRG,IM1,NNN, C_CMD+C_RARE+1, (unsigned char*)"BT" },
  { 0x38FFFF, 0x28BA0F, 2,00,  MRG,IM1,NNN, C_CMD+C_RARE+1, (unsigned char*)"BTS" },
  { 0x38FFFF, 0x30BA0F, 2,00,  MRG,IM1,NNN, C_CMD+C_RARE+1, (unsigned char*)"BTR" },
  { 0x38FFFF, 0x38BA0F, 2,00,  MRG,IM1,NNN, C_CMD+C_RARE+1, (unsigned char*)"BTC" },
  { 0x00FFFF, 0x00A30F, 2,00,  MRG,REG,NNN, C_CMD+C_RARE+1, (unsigned char*)"BT" },
  { 0x00FFFF, 0x00AB0F, 2,00,  MRG,REG,NNN, C_CMD+C_RARE+1, (unsigned char*)"BTS" },
  { 0x00FFFF, 0x00B30F, 2,00,  MRG,REG,NNN, C_CMD+C_RARE+1, (unsigned char*)"BTR" },
  { 0x00FFFF, 0x00BB0F, 2,00,  MRG,REG,NNN, C_CMD+C_RARE+1, (unsigned char*)"BTC" },
  { 0x0000FF, 0x0000C5, 1,00,  REG,MML,NNN, C_CMD+C_RARE+0, (unsigned char*)"LDS" },
  { 0x0000FF, 0x0000C4, 1,00,  REG,MML,NNN, C_CMD+C_RARE+0, (unsigned char*)"LES" },
  { 0x00FFFF, 0x00B40F, 2,00,  REG,MML,NNN, C_CMD+C_RARE+0, (unsigned char*)"LFS" },
  { 0x00FFFF, 0x00B50F, 2,00,  REG,MML,NNN, C_CMD+C_RARE+0, (unsigned char*)"LGS" },
  { 0x00FFFF, 0x00B20F, 2,00,  REG,MML,NNN, C_CMD+C_RARE+0, (unsigned char*)"LSS" },
  { 0x0000FF, 0x000063, 1,00,  MRG,REG,NNN, C_CMD+C_RARE+0, (unsigned char*)"ARPL" },
  { 0x0000FF, 0x000062, 1,00,  REG,MMB,NNN, C_CMD+C_RARE+0, (unsigned char*)"BOUND" },
  { 0x00FFFF, 0x00BC0F, 2,00,  REG,MRG,NNN, C_CMD+C_RARE+0, (unsigned char*)"BSF" },
  { 0x00FFFF, 0x00BD0F, 2,00,  REG,MRG,NNN, C_CMD+C_RARE+0, (unsigned char*)"BSR" },
  { 0x00FFFF, 0x00060F, 2,PR,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"CLTS" },
  { 0x00FFFF, 0x00400F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVO" },
  { 0x00FFFF, 0x00410F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVNO" },
  { 0x00FFFF, 0x00420F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVB,CMOVC" },
  { 0x00FFFF, 0x00430F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVNB,CMOVNC" },
  { 0x00FFFF, 0x00440F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVE,CMOVZ" },
  { 0x00FFFF, 0x00450F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVNE,CMOVNZ" },
  { 0x00FFFF, 0x00460F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVBE,CMOVNA" },
  { 0x00FFFF, 0x00470F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVA,CMOVNBE" },
  { 0x00FFFF, 0x00480F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVS" },
  { 0x00FFFF, 0x00490F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVNS" },
  { 0x00FFFF, 0x004A0F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVPE,CMOVP" },
  { 0x00FFFF, 0x004B0F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVPO,CMOVNP" },
  { 0x00FFFF, 0x004C0F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVL,CMOVNGE" },
  { 0x00FFFF, 0x004D0F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVGE,CMOVNL" },
  { 0x00FFFF, 0x004E0F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVLE,CMOVNG" },
  { 0x00FFFF, 0x004F0F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVG,CMOVNLE" },
  { 0x00FEFF, 0x00B00F, 2,WW,  MRG,REG,NNN, C_CMD+C_RARE+0, (unsigned char*)"CMPXCHG" },
  { 0x38FFFF, 0x08C70F, 2,00,  MD8,NNN,NNN, C_CMD+C_RARE+1, (unsigned char*)"CMPXCHG8B" },
  { 0x00FFFF, 0x00A20F, 2,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"CPUID" },
  { 0x00FFFF, 0x00080F, 2,PR,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"INVD" },
  { 0x00FFFF, 0x00020F, 2,00,  REG,MRG,NNN, C_CMD+C_RARE+0, (unsigned char*)"LAR" },
  { 0x00FFFF, 0x00030F, 2,00,  REG,MRG,NNN, C_CMD+C_RARE+0, (unsigned char*)"LSL" },
  { 0x38FFFF, 0x38010F, 2,PR,  MR1,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"INVLPG" },
  { 0x00FFFF, 0x00090F, 2,PR,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"WBINVD" },
  { 0x38FFFF, 0x10010F, 2,PR,  MM6,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"LGDT" },
  { 0x38FFFF, 0x00010F, 2,00,  MM6,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"SGDT" },
  { 0x38FFFF, 0x18010F, 2,PR,  MM6,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"LIDT" },
  { 0x38FFFF, 0x08010F, 2,00,  MM6,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"SIDT" },
  { 0x38FFFF, 0x10000F, 2,PR,  MR2,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"LLDT" },
  { 0x38FFFF, 0x00000F, 2,00,  MR2,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"SLDT" },
  { 0x38FFFF, 0x18000F, 2,PR,  MR2,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"LTR" },
  { 0x38FFFF, 0x08000F, 2,00,  MR2,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"STR" },
  { 0x38FFFF, 0x30010F, 2,PR,  MR2,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"LMSW" },
  { 0x38FFFF, 0x20010F, 2,00,  MR2,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"SMSW" },
  { 0x38FFFF, 0x20000F, 2,00,  MR2,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"VERR" },
  { 0x38FFFF, 0x28000F, 2,00,  MR2,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"VERW" },
  { 0xC0FFFF, 0xC0220F, 2,PR,  CRX,RR4,NNN, C_CMD+C_RARE+0, (unsigned char*)"MOV" },
  { 0xC0FFFF, 0xC0200F, 2,00,  RR4,CRX,NNN, C_CMD+C_RARE+0, (unsigned char*)"MOV" },
  { 0xC0FFFF, 0xC0230F, 2,PR,  DRX,RR4,NNN, C_CMD+C_RARE+0, (unsigned char*)"MOV" },
  { 0xC0FFFF, 0xC0210F, 2,PR,  RR4,DRX,NNN, C_CMD+C_RARE+0, (unsigned char*)"MOV" },
  { 0x00FFFF, 0x00310F, 2,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"RDTSC" },
  { 0x00FFFF, 0x00320F, 2,PR,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"RDMSR" },
  { 0x00FFFF, 0x00300F, 2,PR,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"WRMSR" },
  { 0x00FFFF, 0x00330F, 2,PR,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"RDPMC" },
  { 0x00FFFF, 0x00AA0F, 2,PR,  NNN,NNN,NNN, C_RTF+C_RARE+0, (unsigned char*)"RSM" },
  { 0x00FFFF, 0x000B0F, 2,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"UD2" },
  { 0x00FFFF, 0x00340F, 2,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"SYSENTER" },
  { 0x00FFFF, 0x00350F, 2,PR,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"SYSEXIT" },
  { 0x0000FF, 0x0000D6, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"SALC" },
  // FPU instructions. Never change the order of instructions!
  { 0x00FFFF, 0x00F0D9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"F2XM1" },
  { 0x00FFFF, 0x00E0D9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FCHS" },
  { 0x00FFFF, 0x00E1D9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FABS" },
  { 0x00FFFF, 0x00E2DB, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FCLEX" },
  { 0x00FFFF, 0x00E3DB, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FINIT" },
  { 0x00FFFF, 0x00F6D9, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FDECSTP" },
  { 0x00FFFF, 0x00F7D9, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FINCSTP" },
  { 0x00FFFF, 0x00E4D9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FTST" },
  { 0x00FFFF, 0x00FAD9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FSQRT" },
  { 0x00FFFF, 0x00FED9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FSIN" },
  { 0x00FFFF, 0x00FFD9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FCOS" },
  { 0x00FFFF, 0x00FBD9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FSINCOS" },
  { 0x00FFFF, 0x00F2D9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FPTAN" },
  { 0x00FFFF, 0x00F3D9, 2,00,  PS0,PS1,NNN, C_FLT+0,        (unsigned char*)"FPATAN" },
  { 0x00FFFF, 0x00F8D9, 2,00,  PS1,PS0,NNN, C_FLT+0,        (unsigned char*)"FPREM" },
  { 0x00FFFF, 0x00F5D9, 2,00,  PS1,PS0,NNN, C_FLT+0,        (unsigned char*)"FPREM1" },
  { 0x00FFFF, 0x00F1D9, 2,00,  PS0,PS1,NNN, C_FLT+0,        (unsigned char*)"FYL2X" },
  { 0x00FFFF, 0x00F9D9, 2,00,  PS0,PS1,NNN, C_FLT+0,        (unsigned char*)"FYL2XP1" },
  { 0x00FFFF, 0x00FCD9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FRNDINT" },
  { 0x00FFFF, 0x00E8D9, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FLD1" },
  { 0x00FFFF, 0x00E9D9, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FLDL2T" },
  { 0x00FFFF, 0x00EAD9, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FLDL2E" },
  { 0x00FFFF, 0x00EBD9, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FLDPI" },
  { 0x00FFFF, 0x00ECD9, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FLDLG2" },
  { 0x00FFFF, 0x00EDD9, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FLDLN2" },
  { 0x00FFFF, 0x00EED9, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FLDZ" },
  { 0x00FFFF, 0x00FDD9, 2,00,  PS0,PS1,NNN, C_FLT+0,        (unsigned char*)"FSCALE" },
  { 0x00FFFF, 0x00D0D9, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FNOP" },
  { 0x00FFFF, 0x00E0DF, 2,FF,  RAX,NNN,NNN, C_FLT+0,        (unsigned char*)"FSTSW" },
  { 0x00FFFF, 0x00E5D9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FXAM" },
  { 0x00FFFF, 0x00F4D9, 2,00,  PS0,NNN,NNN, C_FLT+0,        (unsigned char*)"FXTRACT" },
  { 0x00FFFF, 0x00D9DE, 2,00,  PS0,PS1,NNN, C_FLT+0,        (unsigned char*)"FCOMPP" },
  { 0x00FFFF, 0x00E9DA, 2,00,  PS0,PS1,NNN, C_FLT+0,        (unsigned char*)"FUCOMPP" },
  { 0x00F8FF, 0x00C0DD, 2,00,  RST,NNN,NNN, C_FLT+0,        (unsigned char*)"FFREE" },
  { 0x00F8FF, 0x00C0DA, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FCMOVB" },
  { 0x00F8FF, 0x00C8DA, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FCMOVE" },
  { 0x00F8FF, 0x00D0DA, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FCMOVBE" },
  { 0x00F8FF, 0x00D8DA, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FCMOVU" },
  { 0x00F8FF, 0x00C0DB, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FCMOVNB" },
  { 0x00F8FF, 0x00C8DB, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FCMOVNE" },
  { 0x00F8FF, 0x00D0DB, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FCMOVNBE" },
  { 0x00F8FF, 0x00D8DB, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FCMOVNU" },
  { 0x00F8FF, 0x00F0DB, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FCOMI" },
  { 0x00F8FF, 0x00F0DF, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FCOMIP" },
  { 0x00F8FF, 0x00E8DB, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FUCOMI" },
  { 0x00F8FF, 0x00E8DF, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FUCOMIP" },
  { 0x00F8FF, 0x00C0D8, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FADD" },
  { 0x00F8FF, 0x00C0DC, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FADD" },
  { 0x00F8FF, 0x00C0DE, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FADDP" },
  { 0x00F8FF, 0x00E0D8, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FSUB" },
  { 0x00F8FF, 0x00E8DC, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FSUB" },
  { 0x00F8FF, 0x00E8DE, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FSUBP" },
  { 0x00F8FF, 0x00E8D8, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FSUBR" },
  { 0x00F8FF, 0x00E0DC, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FSUBR" },
  { 0x00F8FF, 0x00E0DE, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FSUBRP" },
  { 0x00F8FF, 0x00C8D8, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FMUL" },
  { 0x00F8FF, 0x00C8DC, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FMUL" },
  { 0x00F8FF, 0x00C8DE, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FMULP" },
  { 0x00F8FF, 0x00D0D8, 2,00,  RST,PS0,NNN, C_FLT+0,        (unsigned char*)"FCOM" },
  { 0x00F8FF, 0x00D8D8, 2,00,  RST,PS0,NNN, C_FLT+0,        (unsigned char*)"FCOMP" },
  { 0x00F8FF, 0x00E0DD, 2,00,  RST,PS0,NNN, C_FLT+0,        (unsigned char*)"FUCOM" },
  { 0x00F8FF, 0x00E8DD, 2,00,  RST,PS0,NNN, C_FLT+0,        (unsigned char*)"FUCOMP" },
  { 0x00F8FF, 0x00F0D8, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FDIV" },
  { 0x00F8FF, 0x00F8DC, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FDIV" },
  { 0x00F8FF, 0x00F8DE, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FDIVP" },
  { 0x00F8FF, 0x00F8D8, 2,00,  RS0,RST,NNN, C_FLT+0,        (unsigned char*)"FDIVR" },
  { 0x00F8FF, 0x00F0DC, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FDIVR" },
  { 0x00F8FF, 0x00F0DE, 2,00,  RST,RS0,NNN, C_FLT+0,        (unsigned char*)"FDIVRP" },
  { 0x00F8FF, 0x00C0D9, 2,00,  RST,NNN,NNN, C_FLT+0,        (unsigned char*)"FLD" },
  { 0x00F8FF, 0x00D0DD, 2,00,  RST,PS0,NNN, C_FLT+0,        (unsigned char*)"FST" },
  { 0x00F8FF, 0x00D8DD, 2,00,  RST,PS0,NNN, C_FLT+0,        (unsigned char*)"FSTP" },
  { 0x00F8FF, 0x00C8D9, 2,00,  RST,PS0,NNN, C_FLT+0,        (unsigned char*)"FXCH" },
  { 0x0038FF, 0x0000D8, 1,00,  MF4,PS0,NNN, C_FLT+1,        (unsigned char*)"FADD" },
  { 0x0038FF, 0x0000DC, 1,00,  MF8,PS0,NNN, C_FLT+1,        (unsigned char*)"FADD" },
  { 0x0038FF, 0x0000DA, 1,00,  MD4,PS0,NNN, C_FLT+1,        (unsigned char*)"FIADD" },
  { 0x0038FF, 0x0000DE, 1,00,  MD2,PS0,NNN, C_FLT+1,        (unsigned char*)"FIADD" },
  { 0x0038FF, 0x0020D8, 1,00,  MF4,PS0,NNN, C_FLT+1,        (unsigned char*)"FSUB" },
  { 0x0038FF, 0x0020DC, 1,00,  MF8,PS0,NNN, C_FLT+1,        (unsigned char*)"FSUB" },
  { 0x0038FF, 0x0020DA, 1,00,  MD4,PS0,NNN, C_FLT+1,        (unsigned char*)"FISUB" },
  { 0x0038FF, 0x0020DE, 1,00,  MD2,PS0,NNN, C_FLT+1,        (unsigned char*)"FISUB" },
  { 0x0038FF, 0x0028D8, 1,00,  MF4,PS0,NNN, C_FLT+1,        (unsigned char*)"FSUBR" },
  { 0x0038FF, 0x0028DC, 1,00,  MF8,PS0,NNN, C_FLT+1,        (unsigned char*)"FSUBR" },
  { 0x0038FF, 0x0028DA, 1,00,  MD4,PS0,NNN, C_FLT+1,        (unsigned char*)"FISUBR" },
  { 0x0038FF, 0x0028DE, 1,00,  MD2,PS0,NNN, C_FLT+1,        (unsigned char*)"FISUBR" },
  { 0x0038FF, 0x0008D8, 1,00,  MF4,PS0,NNN, C_FLT+1,        (unsigned char*)"FMUL" },
  { 0x0038FF, 0x0008DC, 1,00,  MF8,PS0,NNN, C_FLT+1,        (unsigned char*)"FMUL" },
  { 0x0038FF, 0x0008DA, 1,00,  MD4,PS0,NNN, C_FLT+1,        (unsigned char*)"FIMUL" },
  { 0x0038FF, 0x0008DE, 1,00,  MD2,PS0,NNN, C_FLT+1,        (unsigned char*)"FIMUL" },
  { 0x0038FF, 0x0010D8, 1,00,  MF4,PS0,NNN, C_FLT+1,        (unsigned char*)"FCOM" },
  { 0x0038FF, 0x0010DC, 1,00,  MF8,PS0,NNN, C_FLT+1,        (unsigned char*)"FCOM" },
  { 0x0038FF, 0x0018D8, 1,00,  MF4,PS0,NNN, C_FLT+1,        (unsigned char*)"FCOMP" },
  { 0x0038FF, 0x0018DC, 1,00,  MF8,PS0,NNN, C_FLT+1,        (unsigned char*)"FCOMP" },
  { 0x0038FF, 0x0030D8, 1,00,  MF4,PS0,NNN, C_FLT+1,        (unsigned char*)"FDIV" },
  { 0x0038FF, 0x0030DC, 1,00,  MF8,PS0,NNN, C_FLT+1,        (unsigned char*)"FDIV" },
  { 0x0038FF, 0x0030DA, 1,00,  MD4,PS0,NNN, C_FLT+1,        (unsigned char*)"FIDIV" },
  { 0x0038FF, 0x0030DE, 1,00,  MD2,PS0,NNN, C_FLT+1,        (unsigned char*)"FIDIV" },
  { 0x0038FF, 0x0038D8, 1,00,  MF4,PS0,NNN, C_FLT+1,        (unsigned char*)"FDIVR" },
  { 0x0038FF, 0x0038DC, 1,00,  MF8,PS0,NNN, C_FLT+1,        (unsigned char*)"FDIVR" },
  { 0x0038FF, 0x0038DA, 1,00,  MD4,PS0,NNN, C_FLT+1,        (unsigned char*)"FIDIVR" },
  { 0x0038FF, 0x0038DE, 1,00,  MD2,PS0,NNN, C_FLT+1,        (unsigned char*)"FIDIVR" },
  { 0x0038FF, 0x0020DF, 1,00,  MDA,NNN,NNN, C_FLT+C_RARE+1, (unsigned char*)"FBLD" },
  { 0x0038FF, 0x0030DF, 1,00,  MDA,PS0,NNN, C_FLT+C_RARE+1, (unsigned char*)"FBSTP" },
  { 0x0038FF, 0x0010DE, 1,00,  MD2,PS0,NNN, C_FLT+1,        (unsigned char*)"FICOM" },
  { 0x0038FF, 0x0010DA, 1,00,  MD4,PS0,NNN, C_FLT+1,        (unsigned char*)"FICOM" },
  { 0x0038FF, 0x0018DE, 1,00,  MD2,PS0,NNN, C_FLT+1,        (unsigned char*)"FICOMP" },
  { 0x0038FF, 0x0018DA, 1,00,  MD4,PS0,NNN, C_FLT+1,        (unsigned char*)"FICOMP" },
  { 0x0038FF, 0x0000DF, 1,00,  MD2,NNN,NNN, C_FLT+1,        (unsigned char*)"FILD" },
  { 0x0038FF, 0x0000DB, 1,00,  MD4,NNN,NNN, C_FLT+1,        (unsigned char*)"FILD" },
  { 0x0038FF, 0x0028DF, 1,00,  MD8,NNN,NNN, C_FLT+1,        (unsigned char*)"FILD" },
  { 0x0038FF, 0x0010DF, 1,00,  MD2,PS0,NNN, C_FLT+1,        (unsigned char*)"FIST" },
  { 0x0038FF, 0x0010DB, 1,00,  MD4,PS0,NNN, C_FLT+1,        (unsigned char*)"FIST" },
  { 0x0038FF, 0x0018DF, 1,00,  MD2,PS0,NNN, C_FLT+1,        (unsigned char*)"FISTP" },
  { 0x0038FF, 0x0018DB, 1,00,  MD4,PS0,NNN, C_FLT+1,        (unsigned char*)"FISTP" },
  { 0x0038FF, 0x0038DF, 1,00,  MD8,PS0,NNN, C_FLT+1,        (unsigned char*)"FISTP" },
  { 0x0038FF, 0x0000D9, 1,00,  MF4,NNN,NNN, C_FLT+1,        (unsigned char*)"FLD" },
  { 0x0038FF, 0x0000DD, 1,00,  MF8,NNN,NNN, C_FLT+1,        (unsigned char*)"FLD" },
  { 0x0038FF, 0x0028DB, 1,00,  MFA,NNN,NNN, C_FLT+1,        (unsigned char*)"FLD" },
  { 0x0038FF, 0x0010D9, 1,00,  MF4,PS0,NNN, C_FLT+1,        (unsigned char*)"FST" },
  { 0x0038FF, 0x0010DD, 1,00,  MF8,PS0,NNN, C_FLT+1,        (unsigned char*)"FST" },
  { 0x0038FF, 0x0018D9, 1,00,  MF4,PS0,NNN, C_FLT+1,        (unsigned char*)"FSTP" },
  { 0x0038FF, 0x0018DD, 1,00,  MF8,PS0,NNN, C_FLT+1,        (unsigned char*)"FSTP" },
  { 0x0038FF, 0x0038DB, 1,00,  MFA,PS0,NNN, C_FLT+1,        (unsigned char*)"FSTP" },
  { 0x0038FF, 0x0028D9, 1,00,  MB2,NNN,NNN, C_FLT+0,        (unsigned char*)"FLDCW" },
  { 0x0038FF, 0x0038D9, 1,00,  MB2,NNN,NNN, C_FLT+0,        (unsigned char*)"FSTCW" },
  { 0x0038FF, 0x0020D9, 1,00,  MFE,NNN,NNN, C_FLT+0,        (unsigned char*)"FLDENV" },
  { 0x0038FF, 0x0030D9, 1,00,  MFE,NNN,NNN, C_FLT+0,        (unsigned char*)"FSTENV" },
  { 0x0038FF, 0x0020DD, 1,00,  MFS,NNN,NNN, C_FLT+0,        (unsigned char*)"FRSTOR" },
  { 0x0038FF, 0x0030DD, 1,00,  MFS,NNN,NNN, C_FLT+0,        (unsigned char*)"FSAVE" },
  { 0x0038FF, 0x0038DD, 1,00,  MB2,NNN,NNN, C_FLT+0,        (unsigned char*)"FSTSW" },
  { 0x38FFFF, 0x08AE0F, 2,00,  MFX,NNN,NNN, C_FLT+0,        (unsigned char*)"FXRSTOR" },
  { 0x38FFFF, 0x00AE0F, 2,00,  MFX,NNN,NNN, C_FLT+0,        (unsigned char*)"FXSAVE" },
  { 0x00FFFF, 0x00E0DB, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FENI" },
  { 0x00FFFF, 0x00E1DB, 2,00,  NNN,NNN,NNN, C_FLT+0,        (unsigned char*)"FDISI" },
  // MMX instructions. Length of MMX operand fields (in bytes) is added to the
  // type, length of 0 means 8-byte MMX operand.
  { 0x00FFFF, 0x00770F, 2,00,  NNN,NNN,NNN, C_MMX+0,        (unsigned char*)"EMMS" },
  { 0x00FFFF, 0x006E0F, 2,00,  RMX,MR4,NNN, C_MMX+0,        (unsigned char*)"MOVD" },
  { 0x00FFFF, 0x007E0F, 2,00,  MR4,RMX,NNN, C_MMX+0,        (unsigned char*)"MOVD" },
  { 0x00FFFF, 0x006F0F, 2,00,  RMX,MR8,NNN, C_MMX+0,        (unsigned char*)"MOVQ" },
  { 0x00FFFF, 0x007F0F, 2,00,  MR8,RMX,NNN, C_MMX+0,        (unsigned char*)"MOVQ" },
  { 0x00FFFF, 0x00630F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PACKSSWB" },
  { 0x00FFFF, 0x006B0F, 2,00,  RMX,MR8,NNN, C_MMX+4,        (unsigned char*)"PACKSSDW" },
  { 0x00FFFF, 0x00670F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PACKUSWB" },
  { 0x00FFFF, 0x00FC0F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PADDB" },
  { 0x00FFFF, 0x00FD0F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PADDW" },
  { 0x00FFFF, 0x00FE0F, 2,00,  RMX,MR8,NNN, C_MMX+4,        (unsigned char*)"PADDD" },
  { 0x00FFFF, 0x00F80F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PSUBB" },
  { 0x00FFFF, 0x00F90F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PSUBW" },
  { 0x00FFFF, 0x00FA0F, 2,00,  RMX,MR8,NNN, C_MMX+4,        (unsigned char*)"PSUBD" },
  { 0x00FFFF, 0x00EC0F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PADDSB" },
  { 0x00FFFF, 0x00ED0F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PADDSW" },
  { 0x00FFFF, 0x00E80F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PSUBSB" },
  { 0x00FFFF, 0x00E90F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PSUBSW" },
  { 0x00FFFF, 0x00DC0F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PADDUSB" },
  { 0x00FFFF, 0x00DD0F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PADDUSW" },
  { 0x00FFFF, 0x00D80F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PSUBUSB" },
  { 0x00FFFF, 0x00D90F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PSUBUSW" },
  { 0x00FFFF, 0x00DB0F, 2,00,  RMX,MR8,NNN, C_MMX+0,        (unsigned char*)"PAND" },
  { 0x00FFFF, 0x00DF0F, 2,00,  RMX,MR8,NNN, C_MMX+0,        (unsigned char*)"PANDN" },
  { 0x00FFFF, 0x00740F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PCMPEQB" },
  { 0x00FFFF, 0x00750F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PCMPEQW" },
  { 0x00FFFF, 0x00760F, 2,00,  RMX,MR8,NNN, C_MMX+4,        (unsigned char*)"PCMPEQD" },
  { 0x00FFFF, 0x00640F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PCMPGTB" },
  { 0x00FFFF, 0x00650F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PCMPGTW" },
  { 0x00FFFF, 0x00660F, 2,00,  RMX,MR8,NNN, C_MMX+4,        (unsigned char*)"PCMPGTD" },
  { 0x00FFFF, 0x00F50F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PMADDWD" },
  { 0x00FFFF, 0x00E50F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PMULHW" },
  { 0x00FFFF, 0x00D50F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PMULLW" },
  { 0x00FFFF, 0x00EB0F, 2,00,  RMX,MR8,NNN, C_MMX+0,        (unsigned char*)"POR" },
  { 0x00FFFF, 0x00F10F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PSLLW" },
  { 0x38FFFF, 0x30710F, 2,00,  MR8,IM1,NNN, C_MMX+2,        (unsigned char*)"PSLLW" },
  { 0x00FFFF, 0x00F20F, 2,00,  RMX,MR8,NNN, C_MMX+4,        (unsigned char*)"PSLLD" },
  { 0x38FFFF, 0x30720F, 2,00,  MR8,IM1,NNN, C_MMX+4,        (unsigned char*)"PSLLD" },
  { 0x00FFFF, 0x00F30F, 2,00,  RMX,MR8,NNN, C_MMX+0,        (unsigned char*)"PSLLQ" },
  { 0x38FFFF, 0x30730F, 2,00,  MR8,IM1,NNN, C_MMX+0,        (unsigned char*)"PSLLQ" },
  { 0x00FFFF, 0x00E10F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PSRAW" },
  { 0x38FFFF, 0x20710F, 2,00,  MR8,IM1,NNN, C_MMX+2,        (unsigned char*)"PSRAW" },
  { 0x00FFFF, 0x00E20F, 2,00,  RMX,MR8,NNN, C_MMX+4,        (unsigned char*)"PSRAD" },
  { 0x38FFFF, 0x20720F, 2,00,  MR8,IM1,NNN, C_MMX+4,        (unsigned char*)"PSRAD" },
  { 0x00FFFF, 0x00D10F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PSRLW" },
  { 0x38FFFF, 0x10710F, 2,00,  MR8,IM1,NNN, C_MMX+2,        (unsigned char*)"PSRLW" },
  { 0x00FFFF, 0x00D20F, 2,00,  RMX,MR8,NNN, C_MMX+4,        (unsigned char*)"PSRLD" },
  { 0x38FFFF, 0x10720F, 2,00,  MR8,IM1,NNN, C_MMX+4,        (unsigned char*)"PSRLD" },
  { 0x00FFFF, 0x00D30F, 2,00,  RMX,MR8,NNN, C_MMX+0,        (unsigned char*)"PSRLQ" },
  { 0x38FFFF, 0x10730F, 2,00,  MR8,IM1,NNN, C_MMX+0,        (unsigned char*)"PSRLQ" },
  { 0x00FFFF, 0x00680F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PUNPCKHBW" },
  { 0x00FFFF, 0x00690F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PUNPCKHWD" },
  { 0x00FFFF, 0x006A0F, 2,00,  RMX,MR8,NNN, C_MMX+4,        (unsigned char*)"PUNPCKHDQ" },
  { 0x00FFFF, 0x00600F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PUNPCKLBW" },
  { 0x00FFFF, 0x00610F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PUNPCKLWD" },
  { 0x00FFFF, 0x00620F, 2,00,  RMX,MR8,NNN, C_MMX+4,        (unsigned char*)"PUNPCKLDQ" },
  { 0x00FFFF, 0x00EF0F, 2,00,  RMX,MR8,NNN, C_MMX+0,        (unsigned char*)"PXOR" },
  // AMD extentions to MMX command set (including Athlon/PIII extentions).
  { 0x00FFFF, 0x000E0F, 2,00,  NNN,NNN,NNN, C_MMX+0,        (unsigned char*)"FEMMS" },
  { 0x38FFFF, 0x000D0F, 2,00,  MD8,NNN,NNN, C_MMX+0,        (unsigned char*)"PREFETCH" },
  { 0x38FFFF, 0x080D0F, 2,00,  MD8,NNN,NNN, C_MMX+0,        (unsigned char*)"PREFETCHW" },
  { 0x00FFFF, 0x00F70F, 2,00,  RMX,RR8,PDI, C_MMX+1,        (unsigned char*)"MASKMOVQ" },
  { 0x00FFFF, 0x00E70F, 2,00,  MD8,RMX,NNN, C_MMX+0,        (unsigned char*)"MOVNTQ" },
  { 0x00FFFF, 0x00E00F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PAVGB" },
  { 0x00FFFF, 0x00E30F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PAVGW" },
  { 0x00FFFF, 0x00C50F, 2,00,  RR4,RMX,IM1, C_MMX+2,        (unsigned char*)"PEXTRW" },
  { 0x00FFFF, 0x00C40F, 2,00,  RMX,MR2,IM1, C_MMX+2,        (unsigned char*)"PINSRW" },
  { 0x00FFFF, 0x00EE0F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PMAXSW" },
  { 0x00FFFF, 0x00DE0F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PMAXUB" },
  { 0x00FFFF, 0x00EA0F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PMINSW" },
  { 0x00FFFF, 0x00DA0F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PMINUB" },
  { 0x00FFFF, 0x00D70F, 2,00,  RG4,RR8,NNN, C_MMX+1,        (unsigned char*)"PMOVMSKB" },
  { 0x00FFFF, 0x00E40F, 2,00,  RMX,MR8,NNN, C_MMX+2,        (unsigned char*)"PMULHUW" },
  { 0x38FFFF, 0x00180F, 2,00,  MD8,NNN,NNN, C_MMX+0,        (unsigned char*)"PREFETCHNTA" },
  { 0x38FFFF, 0x08180F, 2,00,  MD8,NNN,NNN, C_MMX+0,        (unsigned char*)"PREFETCHT0" },
  { 0x38FFFF, 0x10180F, 2,00,  MD8,NNN,NNN, C_MMX+0,        (unsigned char*)"PREFETCHT1" },
  { 0x38FFFF, 0x18180F, 2,00,  MD8,NNN,NNN, C_MMX+0,        (unsigned char*)"PREFETCHT2" },
  { 0x00FFFF, 0x00F60F, 2,00,  RMX,MR8,NNN, C_MMX+1,        (unsigned char*)"PSADBW" },
  { 0x00FFFF, 0x00700F, 2,00,  RMX,MR8,IM1, C_MMX+2,        (unsigned char*)"PSHUFW" },
  { 0xFFFFFF, 0xF8AE0F, 2,00,  NNN,NNN,NNN, C_MMX+0,        (unsigned char*)"SFENCE" },
  // AMD 3DNow! instructions (including Athlon extentions).
  { 0x00FFFF, 0xBF0F0F, 2,00,  RMX,MR8,NNN, C_NOW+1,        (unsigned char*)"PAVGUSB" },
  { 0x00FFFF, 0x9E0F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFADD" },
  { 0x00FFFF, 0x9A0F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFSUB" },
  { 0x00FFFF, 0xAA0F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFSUBR" },
  { 0x00FFFF, 0xAE0F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFACC" },
  { 0x00FFFF, 0x900F0F, 2,00,  RMX,MRD,NNN, C_NOW+4,        (unsigned char*)"PFCMPGE" },
  { 0x00FFFF, 0xA00F0F, 2,00,  RMX,MRD,NNN, C_NOW+4,        (unsigned char*)"PFCMPGT" },
  { 0x00FFFF, 0xB00F0F, 2,00,  RMX,MRD,NNN, C_NOW+4,        (unsigned char*)"PFCMPEQ" },
  { 0x00FFFF, 0x940F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFMIN" },
  { 0x00FFFF, 0xA40F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFMAX" },
  { 0x00FFFF, 0x0D0F0F, 2,00,  R3D,MR8,NNN, C_NOW+4,        (unsigned char*)"PI2FD" },
  { 0x00FFFF, 0x1D0F0F, 2,00,  RMX,MRD,NNN, C_NOW+4,        (unsigned char*)"PF2ID" },
  { 0x00FFFF, 0x960F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFRCP" },
  { 0x00FFFF, 0x970F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFRSQRT" },
  { 0x00FFFF, 0xB40F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFMUL" },
  { 0x00FFFF, 0xA60F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFRCPIT1" },
  { 0x00FFFF, 0xA70F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFRSQIT1" },
  { 0x00FFFF, 0xB60F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFRCPIT2" },
  { 0x00FFFF, 0xB70F0F, 2,00,  RMX,MR8,NNN, C_NOW+2,        (unsigned char*)"PMULHRW" },
  { 0x00FFFF, 0x1C0F0F, 2,00,  RMX,MRD,NNN, C_NOW+4,        (unsigned char*)"PF2IW" },
  { 0x00FFFF, 0x8A0F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFNACC" },
  { 0x00FFFF, 0x8E0F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PFPNACC" },
  { 0x00FFFF, 0x0C0F0F, 2,00,  R3D,MR8,NNN, C_NOW+4,        (unsigned char*)"PI2FW" },
  { 0x00FFFF, 0xBB0F0F, 2,00,  R3D,MRD,NNN, C_NOW+4,        (unsigned char*)"PSWAPD" },
  // Some alternative mnemonics for Assembler, not used by Disassembler (so
  // implicit pseudooperands are not marked).
  { 0x0000FF, 0x0000A6, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"CMPSB" },
  { 0x00FFFF, 0x00A766, 2,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"CMPSW" },
  { 0x0000FF, 0x0000A7, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"CMPSD" },
  { 0x0000FF, 0x0000AC, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"LODSB" },
  { 0x00FFFF, 0x00AD66, 2,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"LODSW" },
  { 0x0000FF, 0x0000AD, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"LODSD" },
  { 0x0000FF, 0x0000A4, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"MOVSB" },
  { 0x00FFFF, 0x00A566, 2,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"MOVSW" },
  { 0x0000FF, 0x0000A5, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"MOVSD" },
  { 0x0000FF, 0x0000AE, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"SCASB" },
  { 0x00FFFF, 0x00AF66, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"SCASW" },
  { 0x0000FF, 0x0000AF, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"SCASD" },
  { 0x0000FF, 0x0000AA, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"STOSB" },
  { 0x00FFFF, 0x00AB66, 2,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"STOSW" },
  { 0x0000FF, 0x0000AB, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"STOSD" },
  { 0x00FFFF, 0x00A4F3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP MOVSB" },
  { 0xFFFFFF, 0xA5F366, 2,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP MOVSW" },
  { 0x00FFFF, 0x00A5F3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP MOVSD" },
  { 0x00FFFF, 0x00ACF3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP LODSB" },
  { 0xFFFFFF, 0xADF366, 2,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP LODSW" },
  { 0x00FFFF, 0x00ADF3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP LODSD" },
  { 0x00FFFF, 0x00AAF3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP STOSB" },
  { 0xFFFFFF, 0xABF366, 2,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP STOSW" },
  { 0x00FFFF, 0x00ABF3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP STOSD" },
  { 0x00FFFF, 0x00A6F3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPE CMPSB" },
  { 0xFFFFFF, 0xA7F366, 2,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPE CMPSW" },
  { 0x00FFFF, 0x00A7F3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPE CMPSD" },
  { 0x00FFFF, 0x00AEF3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPE SCASB" },
  { 0xFFFFFF, 0xAFF366, 2,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPE SCASW" },
  { 0x00FFFF, 0x00AFF3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPE SCASD" },
  { 0x00FFFF, 0x00A6F2, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPNE CMPSB" },
  { 0xFFFFFF, 0xA7F266, 2,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPNE CMPSW" },
  { 0x00FFFF, 0x00A7F2, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPNE CMPSD" },
  { 0x00FFFF, 0x00AEF2, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPNE SCASB" },
  { 0xFFFFFF, 0xAFF266, 2,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPNE SCASW" },
  { 0x00FFFF, 0x00AFF2, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REPNE SCASD" },
  { 0x0000FF, 0x00006C, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"INSB" },
  { 0x00FFFF, 0x006D66, 2,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"INSW" },
  { 0x0000FF, 0x00006D, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"INSD" },
  { 0x0000FF, 0x00006E, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"OUTSB" },
  { 0x00FFFF, 0x006F66, 2,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"OUTSW" },
  { 0x0000FF, 0x00006F, 1,00,  NNN,NNN,NNN, C_CMD+C_RARE+0, (unsigned char*)"OUTSD" },
  { 0x00FFFF, 0x006CF3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP INSB" },
  { 0xFFFFFF, 0x6DF366, 2,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP INSW" },
  { 0x00FFFF, 0x006DF3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP INSD" },
  { 0x00FFFF, 0x006EF3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP OUTSB" },
  { 0xFFFFFF, 0x6FF366, 2,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP OUTSW" },
  { 0x00FFFF, 0x006FF3, 1,00,  NNN,NNN,NNN, C_REP+0,        (unsigned char*)"REP OUTSD" },
  { 0x0000FF, 0x0000E1, 1,00,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"$LOOP*Z" },
  { 0x0000FF, 0x0000E0, 1,00,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"$LOOP*NZ" },
  { 0x0000FF, 0x00009B, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"FWAIT" },
  { 0x0000FF, 0x0000D7, 1,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"XLATB" },
  { 0x00FFFF, 0x00C40F, 2,00,  RMX,RR4,IM1, C_MMX+2,        (unsigned char*)"PINSRW" },
  { 0x00FFFF, 0x0020CD, 2,00,  VXD,NNN,NNN, C_CAL+C_RARE+0, (unsigned char*)"VxDCall" },
  // Pseudocommands used by Assembler for masked search only.
  { 0x0000F0, 0x000070, 1,CC,  JOB,NNN,NNN, C_JMC+0,        (unsigned char*)"JCC" },
  { 0x00F0FF, 0x00800F, 2,CC,  JOW,NNN,NNN, C_JMC+0,        (unsigned char*)"JCC" },
  { 0x00F0FF, 0x00900F, 2,CC,  MR1,NNN,NNN, C_CMD+1,        (unsigned char*)"SETCC" },
  { 0x00F0FF, 0x00400F, 2,CC,  REG,MRG,NNN, C_CMD+0,        (unsigned char*)"CMOVCC" },
  // End of command table.
  { 0x000000, 0x000000, 0,00,  NNN,NNN,NNN, C_CMD+0,        (unsigned char*)"" }
};

const t_cmddata vxdcmd =               // Decoding of VxD calls (Win95/98)
  { 0x00FFFF, 0x0020CD, 2,00,  VXD,NNN,NNN, C_CAL+C_RARE+0, (unsigned char*)"VxDCall" };

// Bit combinations that can be potentially dangerous when executed:
const t_cmddata dangerous[] = {
  { 0x00FFFF, 0x00DCF7, 0,0,0,0,0,C_DANGER95,
              (unsigned char*)"Win95/98 may crash when NEG ESP is executed" },
  { 0x00FFFF, 0x00D4F7, 0,0,0,0,0,C_DANGER95,
              (unsigned char*)"Win95/98 may crash when NOT ESP is executed" },
  { 0x00FFFF, 0x0020CD, 0,0,0,0,0,C_DANGER95,
              (unsigned char*)"Win95/98 may crash when VxD call is executed in user mode" },
  { 0xF8FFFF, 0xC8C70F, 0,0,0,0,1,C_DANGERLOCK,
              (unsigned char*)"LOCK CMPXCHG8B may crash some processors when executed" },
  { 0x000000, 0x000000, 0,0,0,0,0,0, (unsigned char*)"" }
};

// Decodes address into symb (nsymb bytes long, including the terminating zero
// character) and comments its possible meaning. Returns number of bytes in
// symb not including terminating zero.
int Decodeaddress(ulong addr,unsigned char *symb,int nsymb,unsigned char *comment) {


  // Environment-specific routine! Do it yourself!


  return 0;
};

// Decodes and prints 32-bit float f into string s (which must be at least 16
// bytes long). Returns resulting length of the string.
int Printfloat4(unsigned char *s,float f) {
  int k;
  if (*(ulong *)&f==0x7F800000L)
    k=sprintf( (char*)s,"+INF 7F800000");
  else if (*(ulong *)&f==0xFF800000L)
    k=sprintf( (char*)s,"-INF FF800000");
  else if ((*(ulong *)&f & 0xFF800000L)==0x7F800000L)
    k=sprintf( (char*)s,"+NAN %08lX",*(ulong *)&f);
  else if ((*(ulong *)&f & 0xFF800000L)==0xFF800000L)
    k=sprintf( (char*)s,"-NAN %08lX",*(ulong *)&f);
  else if (f==0.0)                     // By default, 0 is printed without
    k=sprintf( (char*)s,"0.0");                // decimal point, which I don't want.
  else
    k=sprintf( (char*)s,"%#.7g",f);
  return k;
};

// Decodes and prints 64-bit double d into string s (at least 25 bytes long).
// Returns resulting length of the string.
int Printfloat8(unsigned char *s,double d) {
  int k;
  ulong lod,hid;
  lod=((ulong *)&d)[0];
  hid=((ulong *)&d)[1];
  if (lod==0 && hid==0x7F800000L)
    k=sprintf( (char*)s,"+INF 7F800000 00000000");
  else if (lod==0 && hid==0xFF800000L)
    k=sprintf( (char*)s,"-INF FF800000 00000000");
  else if ((hid & 0xFFF00000L)==0x7FF00000)
    k=sprintf( (char*)s,"+NAN %08lX %08lX",hid,lod);
  else if ((hid & 0xFFF00000L)==0xFFF00000)
    k=sprintf( (char*)s,"-NAN %08lX %08lX",hid,lod);
  else if (d==0.0)                     // Print 0 with decimal point
    k=sprintf( (char*)s,"0.0");
  else
    k=sprintf( (char*)s,"%#.16lg",d);
  return k;
};

// Decodes and prints 80-bit long double ext into string s (at least 32 bytes
// long). Procedure correctly displays all, even invalid, numbers without
// arithmetical exceptions. Returns resulting length of the string.
int Printfloat10(unsigned char *s,long double ext) {
  int k;
  unsigned char *e=(unsigned char *)&ext;
  if (*(ulong *)e==0 && *(ushort *)(e+4)==0 && *(ulong *)(e+6)==0x7FFF8000L)
    k=sprintf( (char*)s,"+INF 7FFF 80000000 00000000");
  else if (*(ulong *)e==0 && *(ushort *)(e+4)==0 &&
    *(ulong *)(e+6)==0xFFFF8000L)
    k=sprintf( (char*)s,"-INF FFFF 80000000 00000000");
  else if ((*(ulong *)(e+6) & 0x7FFF8000L)==0x7FFF8000L)
    k=sprintf( (char*)s,"%cNAN %04X %08lX %08lX",(e[9] & 0x80)==0?'+':'-',
    (int)(*(ushort *)(e+8)),*(ulong *)(e+4),*(ulong *)e);
  else if ((*(ulong *)(e+6) & 0x7FFF0000L)==0x7FFF0000L)
    k=sprintf( (char*)s,"%c??? %04X %08lX %08lX",(e[9] & 0x80)==0?'+':'-',
    (int)(*(ushort *)(e+8)),*(ulong *)(e+4),*(ulong *)e);
  else if ((*(ulong *)(e+6) & 0x7FFF0000L)!=0 &&
    (*(ulong *)(e+6) & 0x00008000)==0)
    k=sprintf( (char*)s,"%cUNORM %04X %08lX %08lX",(e[9] & 0x80)==0?'+':'-',
    (int)(*(ushort *)(e+8)),*(ulong *)(e+4),*(ulong *)e);
  else if (*(ulong *)e==0 && *(ushort *)(e+4)==0 &&
    *(ulong *)(e+6)==0x80000000L)
    k=sprintf( (char*)s,"-0.0");               // Negative floating 0.0
  else if (ext==0.0)
    k=sprintf( (char*)s,"0.0");                // Print 0 with decimal point
  else if ((ext>=-1.e10 && ext<-1.0) || (ext>1.0 && ext<=1.e10))
    k=sprintf( (char*)s,"%#.20Lg",ext);
  else if ((ext>=-1.0 && ext<=-1.e-5) || (ext>=1.e-5 && ext<=1.0))
    k=sprintf( (char*)s,"%#.19Lf",ext);
  else
    k=sprintf( (char*)s,"%#.19Le",ext);
  return k;
};

// Decodes and prints 64-bit 3DNow! element f into string s (which must be at
// least 30 bytes long). Returns resulting length of the string.
int Print3dnow(unsigned char *s,unsigned char *f) {
  int n;
  n=Printfloat4(s,*(float *)(f+4));
  n+=sprintf( (char*)s+n,", ");
  n+=Printfloat4(s+n,*(float *)f);
  return n;
};

// Function attempts to calculate address of assembler instruction which is n
// lines back in the listing. Maximal stepback is limited to 127. In general,
// this is rather non-trivial task. Proposed solution may cause problems which
// however are not critical here.
ulong Disassembleback(unsigned char *block,ulong base,ulong size,ulong ip,int n) {
  int i;
  ulong abuf[131]={0}, addr, back, cmdsize;
  unsigned char *pdata;
  t_disasm da;
  if (block==NULL) return 0;           // Error, no code!
  if (n<0) n=0; else if (n>127) n=127; // Try to correct obvious errors
  if (ip>base+size) ip=base+size;
  if (n==0) return ip;                 // Obvious answers
  if (ip<=base+n) return base;
  back=MAXCMDSIZE*(n+3);               // Command length limited to MAXCMDSIZE
  if (ip<base+back) back=ip-base;
  addr=ip-back;
  pdata=block+(addr-base);
  for (i=0; addr<ip; i++) {
    abuf[i%128]=addr;
    cmdsize=Disasm(pdata,back,addr,&da,DISASM_SIZE);
    pdata+=cmdsize;
    addr+=cmdsize;
    back-=cmdsize; };
  if (i<n) return abuf[0];
  else return abuf[(i-n+128)%128];
};

// Function attempts to calculate address of assembler instruction which is n
// lines forward in the listing.
ulong Disassembleforward(unsigned char *block,ulong base,ulong size,ulong ip,int n) {
  int i;
  ulong cmdsize;
  unsigned char *pdata;
  t_disasm da;
  if (block==NULL) return 0;           // Error, no code!
  if (ip<base) ip=base;                // Try to correct obvious errors
  if (ip>base+size) ip=base+size;
  if (n<=0) return ip;
  pdata=block+(ip-base);
  size-=(ip-base);
  for (i=0; i<n && size>0; i++) {
    cmdsize=Disasm(pdata,size,ip,&da,DISASM_SIZE);
    pdata+=cmdsize;
    ip+=cmdsize;
    size-=cmdsize; };
  return ip;
};

// Service function, checks whether command at offset addr in data is a valid
// filling command (usually some kind of NOP) used to align code to a specified
// (align=power of 2, 0 means no alignment) border. Returns length of filling
// command in bytes or 0 if command is not a recognized filling.
int Isfilling(ulong addr,unsigned char *data,ulong size,ulong align) {
  if (data==NULL) return 0;            // Invalid parameters
  // Convert power of 2 to bitmask.
  align--;
  // Many compilers and assemblers use NOP or INT3 as filling:
  if (addr<size && (data[addr]==NOP || data[addr]==INT3) &&
    (addr & align)!=0)
    return 1;
  // Borland compilers use XCHG EBX,EBX (87,DB) to fill holes. For the sake of
  // completeness, allow any XCHG or MOV of register with self.
  if (addr+1<size &&
    ((data[addr] & 0xFE)==0x86 || (data[addr] & 0xFC)==0x88) &&
    (data[addr+1] & 0xC0)==0xC0 &&
    (((data[addr+1]>>3)^data[addr+1]) & 0x07)==0 &&
    (addr & align)!=0x0F && (addr & align)!=0x00)
    return 2;
  // Some other programs use LEA EAX,[EAX] (8D,40,00). For completeness, allow
  // any register except ESP (here address is constructed differently):
  if (addr+2<size &&
    data[addr]==0x8D && (data[addr+1] & 0xC0)==0x40 && data[addr+2]==0x00 &&
    (data[addr+1] & 0x07)!=REG_ESP &&
    (((data[addr+1]>>3)^data[addr+1]) & 0x07)==0)
    return 3;
  // WATCOM compilers use LEA EAX,[EAX] with SIB and 8-bit zero (8D,44,20,00)
  // and without SIB but with 32-bit immediate zero (8D,80,00,00,00,00) and
  // alike:
  if (addr+3<size &&
    data[addr]==0x8D && (data[addr+1] & 0xC0)==0x40 && data[addr+3]==0x00 &&
    (((data[addr+1]>>3)^data[addr+2]) & 0x07)==0)
    return 4;
  if (addr+5<size && data[addr]==0x8D &&
    (data[addr+1] & 0xC0)==0x80 && *(ulong *)(data+addr+2)==0 &&
    (data[addr+1] & 0x07)!=REG_ESP &&
    (((data[addr+1]>>3)^data[addr+1]) & 0x07)==0)
    return 6;
  // Unable to recognize this code as a valid filling.
  return 0;
};


// Free Disassembler and Assembler -- Assembler
//
// Copyright (C) 2001 Oleh Yuschuk
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

// 16.01.2002 - corrected error in processing of immediate constants.


////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ASSEMBLER FUNCTIONS //////////////////////////////

// Scanner modes.
#define SA_NAME        0x0001          // Don't try to decode labels
#define SA_IMPORT      0x0002          // Allow import pseudolabel

// Types of input tokens reported by scanner.
#define SCAN_EOL       0               // End of line
#define SCAN_REG8      1               // 8-bit register
#define SCAN_REG16     2               // 16-bit register
#define SCAN_REG32     3               // 32-bit register
#define SCAN_SEG       4               // Segment register
#define SCAN_FPU       5               // FPU register
#define SCAN_MMX       6               // MMX register
#define SCAN_CR        7               // Control register
#define SCAN_DR        8               // Debug register
#define SCAN_OPSIZE    9               // Operand size modifier
#define SCAN_JMPSIZE   10              // Jump size modifier
#define SCAN_LOCAL     11              // Address on stack in form LOCAL.decimal
#define SCAN_ARG       12              // Address on stack in form ARG.decimal
#define SCAN_PTR       20              // PTR in MASM addressing statements
#define SCAN_REP       21              // REP prefix
#define SCAN_REPE      22              // REPE prefix
#define SCAN_REPNE     23              // REPNE prefix
#define SCAN_LOCK      24              // LOCK prefix
#define SCAN_NAME      25              // Command or label
#define SCAN_ICONST    26              // Hexadecimal constant
#define SCAN_DCONST    27              // Decimal constant
#define SCAN_OFS       28              // Undefined constant
#define SCAN_FCONST    29              // Floating-point constant
#define SCAN_EIP       30              // Register EIP
#define SCAN_SIGNED    31              // Keyword "SIGNED" (in expressions)
#define SCAN_UNSIGNED  32              // Keyword "UNSIGNED" (in expressions)
#define SCAN_CHAR      33              // Keyword "CHAR" (in expressions)
#define SCAN_FLOAT     34              // Keyword "FLOAT" (in expressions)
#define SCAN_DOUBLE    35              // Keyword "DOUBLE" (in expressions)
#define SCAN_FLOAT10   36              // Keyword "FLOAT10" (in expressions)
#define SCAN_STRING    37              // Keyword "STRING" (in expressions)
#define SCAN_UNICODE   38              // Keyword "UNICODE" (in expressions)
#define SCAN_MSG       39              // Pseudovariable MSG (in expressions)

#define SCAN_SYMB      64              // Any other character
#define SCAN_IMPORT    65              // Import pseudolabel
#define SCAN_ERR       255             // Definitely bad item

// Definition used by Assembler to report command matching errors.
#define MA_JMP         0x0001          // Invalid jump size modifier
#define MA_NOP         0x0002          // Wrong number of operands
#define MA_TYP         0x0004          // Bad operand type
#define MA_NOS         0x0008          // Explicit operand size expected
#define MA_SIZ         0x0010          // Bad operand size
#define MA_DIF         0x0020          // Different operand sizes
#define MA_SEG         0x0040          // Invalid segment register
#define MA_RNG         0x0080          // Constant out of expected range

typedef struct t_asmoperand {
  int            type;                 // Operand type, see beginning of file
  int            size;                 // Operand size or 0 if yet unknown
  int            index;                // Index or other register
  int            scale;                // Scale
  int            base;                 // Base register if present
  long           offset;               // Immediate value or offset
  int            anyoffset;            // Offset is present but undefined
  int            segment;              // Segment in address if present
  int            jmpmode;              // Specified jump size
} t_asmoperand;

static unsigned char *asmcmd;          // Pointer to 0-terminated source line
static int       scan;                 // Type of last scanned element
static int       prio;                 // Priority of operation (0: highest)
static unsigned char sdata[TEXTLEN]={0};//Last scanned name (depends on type)
static long      idata;                // Last scanned value
static long      double fdata;         // Floating-point number
static unsigned char *asmerror;        // Explanation of last error, or NULL

// Simple and slightly recursive scanner shared by Assemble(). The scanner is
// straightforward and ineffective, but high speed is not a must here. As
// input, it uses global pointer to source line asmcmd. On exit, it fills in
// global variables scan, prio, sdata, idata and/or fdata. If some error is
// detected, asmerror points to error message, otherwise asmerror remains
// unchanged.
static void Scanasm(int mode) {
  int i,j,base,maxdigit;
  long decimal,hex;
  long double floating,divisor;
  unsigned char s[TEXTLEN]={0}, *pcmd;
  sdata[0]='\0';
  idata=0;
  if (asmcmd==NULL) {
    asmerror= (unsigned char*)"NULL input line"; scan=SCAN_ERR; return; };
  while (*asmcmd==' ' || *asmcmd=='\t')
    asmcmd++;                          // Skip leading spaces
  if (*asmcmd=='\0' || *asmcmd==';') {
    scan=SCAN_EOL; return; };          // Empty line
  if (isalpha(*asmcmd) || *asmcmd=='_' || *asmcmd=='@') {
    sdata[0]=*asmcmd++; i=1;           // Some keyword or identifier
    while ((isalnum(*asmcmd) || *asmcmd=='_' || *asmcmd=='@') &&
      i<sizeof(sdata))
      sdata[i++]=*asmcmd++;
    if (i>=sizeof(sdata)) {
      asmerror= (unsigned char*)"Too long identifier"; scan=SCAN_ERR; return; };
    sdata[i]='\0';
    while (*asmcmd==' ' || *asmcmd=='\t')
      asmcmd++;                        // Skip trailing spaces
    strcpy( (char*)s, (char*)sdata); _strupr( (char*)s);
    for (j=0; j<=8; j++) {             // j==8 means "any register"
      if (strcmp( (char*)s, (char*)regname[0][j])!=0) continue;
      idata=j; scan=SCAN_REG8;         // 8-bit register
      return; };
    for (j=0; j<=8; j++) {
      if (strcmp( (char*)s, (char*)regname[1][j])!=0) continue;
      idata=j; scan=SCAN_REG16;        // 16-bit register
      return; };
    for (j=0; j<=8; j++) {
      if (strcmp( (char*)s, (char*)regname[2][j])!=0) continue;
      idata=j; scan=SCAN_REG32;        // 32-bit register
      return; };
    for (j=0; j<6; j++) {
      if (strcmp( (char*)s, (char*)segname[j])!=0) continue;
      idata=j; scan=SCAN_SEG;          // Segment register
      while (*asmcmd==' ' || *asmcmd=='\t')
        asmcmd++;                      // Skip trailing spaces
      return; };
    if (strcmp( (char*)s,"ST")==0) {
      pcmd=asmcmd; Scanasm(SA_NAME);   // FPU register
      if (scan!=SCAN_SYMB || idata!='(') {
        asmcmd=pcmd;                   // Undo last scan
        idata=0; scan=SCAN_FPU; return; };
      Scanasm(SA_NAME); j=idata;
      if ((scan!=SCAN_ICONST && scan!=SCAN_DCONST) || idata<0 || idata>7) {
        asmerror= (unsigned char*)"FPU registers have indexes 0 to 7";
        scan=SCAN_ERR; return; };
      Scanasm(SA_NAME);
      if (scan!=SCAN_SYMB || idata!=')') {
        asmerror= (unsigned char*)"Closing parenthesis expected";
        scan=SCAN_ERR; return; };
      idata=j; scan=SCAN_FPU; return; };
    for (j=0; j<=8; j++) {
      if (strcmp( (char*)s, (char*)fpuname[j])!=0) continue;
      idata=j; scan=SCAN_FPU;          // FPU register (alternative coding)
      return; };
    for (j=0; j<=8; j++) {
      if (strcmp( (char*)s, (char*)mmxname[j])!=0) continue;
      idata=j; scan=SCAN_MMX;          // MMX register
      return; };
    for (j=0; j<=8; j++) {
      if (strcmp( (char*)s, (char*)crname[j])!=0) continue;
      idata=j; scan=SCAN_CR;           // Control register
      return; };
    for (j=0; j<=8; j++) {
      if (strcmp( (char*)s, (char*)drname[j])!=0) continue;
      idata=j; scan=SCAN_DR;           // Debug register
      return; };
    for (j=0; j<sizeof(sizename)/sizeof(sizename[0]); j++) {
      if (strcmp( (char*)s, (char*)sizename[j])!=0) continue;
      pcmd=asmcmd; Scanasm(SA_NAME);
      if (scan!=SCAN_PTR)              // Fetch non-functional "PTR"
        asmcmd=pcmd;
      idata=j; scan=SCAN_OPSIZE;       // Operand (data) size in bytes
      return; };
    if (strcmp( (char*)s,"EIP")==0) {          // Register EIP
      scan=SCAN_EIP; idata=0; return; };
    if (strcmp( (char*)s,"SHORT")==0) {        // Relative jump has 1-byte offset
      scan=SCAN_JMPSIZE; idata=1; return; };
    if (strcmp( (char*)s,"LONG")==0) {         // Relative jump has 4-byte offset
      scan=SCAN_JMPSIZE; idata=2; return; };
    if (strcmp( (char*)s,"NEAR")==0) {         // Jump within same code segment
      scan=SCAN_JMPSIZE; idata=4; return; };
    if (strcmp( (char*)s,"FAR")==0) {          // Jump to different code segment
      scan=SCAN_JMPSIZE; idata=8; return; };
    if (strcmp( (char*)s,"LOCAL")==0 && *asmcmd=='.') {
      asmcmd++;
      while (*asmcmd==' ' || *asmcmd=='\t')
        asmcmd++;                      // Skip trailing spaces
      if (!isdigit(*asmcmd)) {
        asmerror= (unsigned char*)"Integer number expected";
        scan=SCAN_ERR; return; };
      while (isdigit(*asmcmd))         // LOCAL index is decimal number!
        idata=idata*10+(*asmcmd++)-'0';
      scan=SCAN_LOCAL; return; };
    if (strcmp( (char*)s,"ARG")==0 && *asmcmd=='.') {
      asmcmd++;
      while (*asmcmd==' ' || *asmcmd=='\t')
        asmcmd++;                      // Skip trailing spaces
      if (!isdigit(*asmcmd)) {
        asmerror= (unsigned char*)"Integer number expected";
        scan=SCAN_ERR; return; };
      while (isdigit(*asmcmd))         // ARG index is decimal number!
        idata=idata*10+(*asmcmd++)-'0';
      scan=SCAN_ARG; return; };
    if (strcmp( (char*)s,"REP")==0) {
      scan=SCAN_REP; return; };        // REP prefix
    if (strcmp( (char*)s,"REPE")==0 || strcmp( (char*)s,"REPZ")==0) {
      scan=SCAN_REPE; return; };       // REPE prefix
    if (strcmp( (char*)s,"REPNE")==0 || strcmp( (char*)s,"REPNZ")==0) {
      scan=SCAN_REPNE; return; };      // REPNE prefix
    if (strcmp( (char*)s,"LOCK")==0) {
      scan=SCAN_LOCK; return; };       // LOCK prefix
    if (strcmp( (char*)s,"PTR")==0) {
      scan=SCAN_PTR; return; };        // PTR in MASM addressing statements
    if (strcmp( (char*)s,"CONST")==0 || strcmp( (char*)s,"OFFSET")==0) {
      scan=SCAN_OFS; return; };        // Present but undefined offset/constant
    if (strcmp( (char*)s,"SIGNED")==0) {
      scan=SCAN_SIGNED; return; };     // Keyword "SIGNED" (in expressions)
    if (strcmp( (char*)s,"UNSIGNED")==0) {
      scan=SCAN_UNSIGNED; return; };   // Keyword "UNSIGNED" (in expressions)
    if (strcmp( (char*)s,"CHAR")==0) {
      scan=SCAN_CHAR; return; };       // Keyword "CHAR" (in expressions)
    if (strcmp( (char*)s,"FLOAT")==0) {
      scan=SCAN_FLOAT; return; };      // Keyword "FLOAT" (in expressions)
    if (strcmp( (char*)s,"DOUBLE")==0) {
      scan=SCAN_DOUBLE; return; };     // Keyword "DOUBLE" (in expressions)
    if (strcmp( (char*)s,"FLOAT10")==0) {
      scan=SCAN_FLOAT10; return; };    // Keyword "FLOAT10" (in expressions)
    if (strcmp( (char*)s,"STRING")==0) {
      scan=SCAN_STRING; return; };     // Keyword "STRING" (in expressions)
    if (strcmp( (char*)s,"UNICODE")==0) {
      scan=SCAN_UNICODE; return; };    // Keyword "UNICODE" (in expressions)
    if (strcmp( (char*)s,"MSG")==0) {
      scan=SCAN_MSG; return; };        // Pseudovariable MSG (in expressions)
    if (mode & SA_NAME) {
      idata=i; scan=SCAN_NAME;         // Don't try to decode symbolic label
      return; }
    asmerror= (unsigned char*)"Unknown identifier";
    scan=SCAN_ERR; return; }
  else if (isdigit(*asmcmd)) {         // Constant
    base=0; maxdigit=0; decimal=hex=0L; floating=0.0;
    if (asmcmd[0]=='0' && toupper(asmcmd[1])=='X') {
      base=16; asmcmd+=2; };           // Force hexadecimal number
    while (1) {
      if (isdigit(*asmcmd)) {
        decimal=decimal*10+(*asmcmd)-'0';
        floating=floating*10.0+(*asmcmd)-'0';
        hex=hex*16+(*asmcmd)-'0';
        if (maxdigit==0) maxdigit=9;
        asmcmd++; }
      else if (isxdigit(*asmcmd)) {
        hex=hex*16+toupper(*asmcmd++)-'A'+10;
        maxdigit=15; }
      else break; };
    if (maxdigit==0) {
      asmerror= (unsigned char*)"Hexadecimal digits after 0x... expected";
      scan=SCAN_ERR; return; };
    if (toupper(*asmcmd)=='H') {       // Force hexadecimal number
      if (base==16) {
        asmerror= (unsigned char*)"Please don't mix 0xXXXX and XXXXh forms";
        scan=SCAN_ERR; return; };
      asmcmd++;
      idata=hex; scan=SCAN_ICONST;
      while (*asmcmd==' ' || *asmcmd=='\t') asmcmd++;
      return; };
    if (*asmcmd=='.') {                // Force decimal number
      if (base==16 || maxdigit>9) {
        asmerror= (unsigned char*)"Not a decimal number"; scan=SCAN_ERR; return; };
      asmcmd++;
      if (isdigit(*asmcmd) || toupper(*asmcmd)=='E') {
        divisor=1.0;
        while (isdigit(*asmcmd)) {     // Floating-point number
          divisor/=10.0;
          floating+=divisor*(*asmcmd-'0');
          asmcmd++; };
        if (toupper(*asmcmd)=='E') {
          asmcmd++;
          if (*asmcmd=='-') { base=-1; asmcmd++; }
          else base=1;
          if (!isdigit(*asmcmd)) {
            asmerror= (unsigned char*)"Invalid exponent"; scan=SCAN_ERR; return; };
          decimal=0;
          while (isdigit(*asmcmd)) {
            if (decimal<65536L) decimal=decimal*10+(*asmcmd++)-'0'; };
          floating*=pow(10.0, decimal*base); };
        fdata=floating;
        scan=SCAN_FCONST; return; }
      else {
        idata=decimal; scan=SCAN_DCONST;
        while (*asmcmd==' ' || *asmcmd=='\t') asmcmd++;
        return;
      };
    };
    idata=hex; scan=SCAN_ICONST;       // Default is hexadecimal
    while (*asmcmd==' ' || *asmcmd=='\t') asmcmd++;
    return; }
  else if (*asmcmd=='\'') {            // Character constant
    asmcmd++;
    if (*asmcmd=='\0' || (*asmcmd=='\\' && asmcmd[1]=='\0'))  {
      asmerror= (unsigned char*)"Unterminated character constant"; scan=SCAN_ERR; return; };
    if (*asmcmd=='\'') {
      asmerror= (unsigned char*)"Empty character constant"; scan=SCAN_ERR; return; };
    if (*asmcmd=='\\') asmcmd++;
    idata=*asmcmd++; 
    if (*asmcmd!='\'')  {
      asmerror= (unsigned char*)"Unterminated character constant"; scan=SCAN_ERR; return; };
    asmcmd++;
    while (*asmcmd==' ' || *asmcmd=='\t') asmcmd++;
    scan=SCAN_ICONST; return; }
  else {                               // Any other character or combination
    idata=sdata[0]=*asmcmd++; sdata[1]=sdata[2]='\0';
    if (idata=='|' && *asmcmd=='|') {
      idata='||'; prio=10;             // '||'
      sdata[1]=*asmcmd++; }
    else if (idata=='&' && *asmcmd=='&') {
      idata='&&'; prio=9;              // '&&'
      sdata[1]=*asmcmd++; }
    else if (idata=='=' && *asmcmd=='=') {
      idata='=='; prio=5;              // '=='
      sdata[1]=*asmcmd++; }
    else if (idata=='!' && *asmcmd=='=') {
      idata='!='; prio=5;              // '!='
      sdata[1]=*asmcmd++; }
    else if (idata=='<' && *asmcmd=='=') {
      idata='<='; prio=4;              // '<='
      sdata[1]=*asmcmd++; }
    else if (idata=='>' && *asmcmd=='=') {
      idata='>='; prio=4;              // '>='
      sdata[1]=*asmcmd++; }
    else if (idata=='<' && *asmcmd=='<') {
      idata='<<'; prio=3;              // '<<'
      sdata[1]=*asmcmd++; }
    else if (idata=='>' && *asmcmd=='>') {
      idata='>>'; prio=3;              // '>>'
      sdata[1]=*asmcmd++; }
    else if (idata=='|') prio=8;       // '|'
    else if (idata=='^') prio=7;       // '^'
    else if (idata=='&') prio=6;       // '&'
    else if (idata=='<') {
      if (*asmcmd=='&') {              // Import pseudolabel (for internal use)
        if ((mode & SA_IMPORT)==0) {
          asmerror= (unsigned char*)"Syntax error"; scan=SCAN_ERR; return; };
        asmcmd++; i=0;
        while (*asmcmd!='\0' && *asmcmd!='>') {
          sdata[i++]=*asmcmd++;
          if (i>=sizeof(sdata)) {
            asmerror= (unsigned char*)"Too long import name"; scan=SCAN_ERR; return;
          };
        };
        if (*asmcmd!='>') {
          asmerror= (unsigned char*)"Unterminated import name"; scan=SCAN_ERR; return; };
        asmcmd++; sdata[i]='\0';
        scan=SCAN_IMPORT; return; }
      else prio=4; }                   // '<'
    else if (idata=='>') prio=4;       // '>'
    else if (idata=='+') prio=2;       // '+'
    else if (idata=='-') prio=2;       // '-'
    else if (idata=='*') prio=1;       // '*'
    else if (idata=='/') prio=1;       // '/'
    else if (idata=='%') prio=1;       // '%'
    else if (idata==']') {
      pcmd=asmcmd; Scanasm(SA_NAME);
      if (scan!=SCAN_SYMB || idata!='[') {
        idata=']'; asmcmd=pcmd; prio=0; }
      else {
        idata='+'; prio=2;             // Translate '][' to '+'
      };
    }
    else prio=0;                       // Any other character
    scan=SCAN_SYMB;
    return;
  };
};

// Fetches one complete operand from the input line and fills in structure op
// with operand's data. Expects that first token of the operand is already
// scanned. Supports operands in generalized form (for example, R32 means any
// of general-purpose 32-bit integer registers).
static void Parseasmoperand(t_asmoperand *op) {
  int i,j,bracket,sign,xlataddr;
  int reg, r[9]={0};
  long offset;
  if (scan==SCAN_EOL || scan==SCAN_ERR)
    return;                            // No or bad operand
  // Jump or call address may begin with address size modifier(s) SHORT, LONG,
  // NEAR and/or FAR. Not all combinations are allowed. After operand is
  // completely parsed, this function roughly checks whether modifier is
  // allowed. Exact check is done in Assemble().
  if (scan==SCAN_JMPSIZE) {
    j=0;
    while (scan==SCAN_JMPSIZE) {
      j|=idata;                        // Fetch all size modifiers
      Scanasm(0); };
    if (
      ((j & 0x03)==0x03) ||            // Mixed SHORT and LONG
      ((j & 0x0C)==0x0C) ||            // Mixed NEAR and FAR
      ((j & 0x09)==0x09)               // Mixed FAR and SHORT
    ) {
      asmerror= (unsigned char*)"Invalid combination of jump address modifiers";
      scan=SCAN_ERR; return; };
    if ((j & 0x08)==0) j|=0x04;        // Force NEAR if not FAR
    op->jmpmode=j; };
  // Simple operands are either register or constant, their processing is
  // obvious and straightforward.
  if (scan==SCAN_REG8 || scan==SCAN_REG16 || scan==SCAN_REG32) {
    op->type=REG; op->index=idata;     // Integer general-purpose register
    if (scan==SCAN_REG8) op->size=1;
    else if (scan==SCAN_REG16) op->size=2;
    else op->size=4; }
  else if (scan==SCAN_FPU) {           // FPU register
    op->type=RST; op->index=idata; }
  else if (scan==SCAN_MMX) {           // MMX or 3DNow! register
    op->type=RMX; op->index=idata; }
  else if (scan==SCAN_CR) {            // Control register
    op->type=CRX; op->index=idata; }
  else if (scan==SCAN_DR) {            // Debug register
    op->type=DRX; op->index=idata; }
  else if (scan==SCAN_SYMB && idata=='-') {
    Scanasm(0);                        // Negative constant
    if (scan!=SCAN_ICONST && scan!=SCAN_DCONST && scan!=SCAN_OFS) {
      asmerror= (unsigned char*)"Integer number expected";
      scan=SCAN_ERR; return; };
    op->type=IMM; op->offset=-idata;
    if (scan==SCAN_OFS) op->anyoffset=1; }
  else if (scan==SCAN_SYMB && idata=='+') {
    Scanasm(0);                        // Positive constant
    if (scan!=SCAN_ICONST && scan!=SCAN_DCONST && scan!=SCAN_OFS) {
      asmerror= (unsigned char*)"Integer number expected";
      scan=SCAN_ERR; return; };
    op->type=IMM; op->offset=idata;
    if (scan==SCAN_OFS) op->anyoffset=1; }
  else if (scan==SCAN_ICONST || scan==SCAN_DCONST || scan==SCAN_OFS) {
    j=idata;
    if (scan==SCAN_OFS) op->anyoffset=1;
    Scanasm(0);
    if (scan==SCAN_SYMB && idata==':') {
      Scanasm(0);                      // Absolute long address (seg:offset)
      if (scan!=SCAN_ICONST && scan!=SCAN_DCONST && scan!=SCAN_OFS) {
        asmerror= (unsigned char*)"Integer address expected";
        scan=SCAN_ERR; return; };
      op->type=JMF; op->offset=idata; op->segment=j;
      if (scan==SCAN_OFS) op->anyoffset=1; }
    else {
      op->type=IMM; op->offset=j;      // Constant without sign
      return;                          // Next token already scanned
    }; }
  else if (scan==SCAN_FCONST) {
    asmerror= (unsigned char*)"Floating-point numbers are not allowed in command";
    scan=SCAN_ERR; return; }
  // Segment register or address.
  else if (scan==SCAN_SEG || scan==SCAN_OPSIZE ||
    (scan==SCAN_SYMB && idata=='[')
  ) {                                  // Segment register or address
    bracket=0;
    if (scan==SCAN_SEG) {
      j=idata; Scanasm(0);
      if (scan!=SCAN_SYMB || idata!=':') {
        op->type=SGM; op->index=j;     // Segment register as operand
        return; };                     // Next token already scanned
      op->segment=j; Scanasm(0); };
    // Scan 32-bit address. This parser does not support 16-bit addresses.
    // First of all, get size of operand (optional), segment register (optional)
    // and opening bracket (required).
    while (1) {
      if (scan==SCAN_SYMB && idata=='[') {
        if (bracket) {                 // Bracket
          asmerror= (unsigned char*)"Only one opening bracket allowed";
          scan=SCAN_ERR; return; };
        bracket=1; }
      else if (scan==SCAN_OPSIZE) {
        if (op->size!=0) {             // Size of operand
          asmerror= (unsigned char*)"Duplicated size modifier";
          scan=SCAN_ERR; return; };
        op->size=idata; }
      else if (scan==SCAN_SEG) {
        if (op->segment!=SEG_UNDEF) {  // Segment register
          asmerror= (unsigned char*)"Duplicated segment register";
          scan=SCAN_ERR; return; };
        op->segment=idata; Scanasm(0);
        if (scan!=SCAN_SYMB || idata!=':') {
          asmerror= (unsigned char*)"Semicolon expected";
          scan=SCAN_ERR; return;
        }; }
      else if (scan==SCAN_ERR)
        return;
      else break;                      // None of expected address elements
      Scanasm(0); };
    if (bracket==0) {
      asmerror= (unsigned char*)"Address expression requires brackets";
      scan=SCAN_ERR; return; };
    // Assembling a 32-bit address may be a kind of nigthmare, due to a large
    // number of allowed forms. Parser collects immediate offset in op->offset
    // and count for each register in array r[]. Then it decides whether this
    // combination is valid and determines scale, index and base. Assemble()
    // will use these numbers to select address form (with or without SIB byte,
    // 8- or 32-bit offset, use segment prefix or not). As a useful side effect
    // of this technique, one may specify, for example, [EAX*5] which will
    // correctly assemble to [EAX*4+EAX].
    for (i=0; i<=8; i++) r[i]=0;
    sign='+';                          // Default sign for the first operand
    xlataddr=0;
    while (1) {                        // Get SIB and offset
      if (scan==SCAN_SYMB && (idata=='+' || idata=='-')) {
        sign=idata; Scanasm(0); };
      if (scan==SCAN_ERR) return;
      if (sign=='?') {
        asmerror= (unsigned char*)"Syntax error"; scan=SCAN_ERR; return; };
      // Register AL appears as part of operand of (seldom used) command XLAT.
      if (scan==SCAN_REG8 && idata==REG_EAX) {
        if (sign=='-') {
          asmerror= (unsigned char*)"Unable to subtract register"; scan=SCAN_ERR; return; };
        if (xlataddr!=0) {
          asmerror= (unsigned char*)"Too many registers"; scan=SCAN_ERR; return; };
        xlataddr=1;
        Scanasm(0); }
      else if (scan==SCAN_REG16) {
        asmerror= (unsigned char*)"Sorry, 16-bit addressing is not supported";
        scan=SCAN_ERR; return; }
      else if (scan==SCAN_REG32) {
        if (sign=='-') {
          asmerror= (unsigned char*)"Unable to subtract register"; scan=SCAN_ERR; return; };
        reg=idata; Scanasm(0);
        if (scan==SCAN_SYMB && idata=='*') {
          Scanasm(0);                  // Try index*scale
          if (scan==SCAN_ERR) return;
          if (scan==SCAN_OFS) {
            asmerror= (unsigned char*)"Undefined scale is not allowed"; scan=SCAN_ERR; return; };
          if (scan!=SCAN_ICONST && scan!=SCAN_DCONST) {
            asmerror= (unsigned char*)"Syntax error"; scan=SCAN_ERR; return; };
          if (idata==6 || idata==7 || idata>9) {
            asmerror= (unsigned char*)"Invalid scale"; scan=SCAN_ERR; return; };
          r[reg]+=idata;
          Scanasm(0); }
        else r[reg]++; }               // Simple register
      else if (scan==SCAN_LOCAL) {
        r[REG_EBP]++;
        op->offset-=idata*4;
        Scanasm(0); }
      else if (scan==SCAN_ARG) {
        r[REG_EBP]++;
        op->offset+=(idata+1)*4;
        Scanasm(0); }
      else if (scan==SCAN_ICONST || scan==SCAN_DCONST) {
        offset=idata; Scanasm(0);
        if (scan==SCAN_SYMB && idata=='*') {
          Scanasm(0);                  // Try scale*index
          if (scan==SCAN_ERR) return;
          if (sign=='-') {
            asmerror= (unsigned char*)"Unable to subtract register"; scan=SCAN_ERR; return; };
          if (scan==SCAN_REG16) {
            asmerror= (unsigned char*)"Sorry, 16-bit addressing is not supported";
            scan=SCAN_ERR; return; };
          if (scan!=SCAN_REG32) {
            asmerror= (unsigned char*)"Syntax error"; scan=SCAN_ERR; return; };
          if (offset==6 || offset==7 || offset>9) {
            asmerror= (unsigned char*)"Invalid scale"; scan=SCAN_ERR; return; };
          r[idata]+=offset;
          Scanasm(0); }
        else {
          if (sign=='-') op->offset-=offset;
          else op->offset+=offset;
        }; }
      else if (scan==SCAN_OFS) {
        Scanasm(0);
        if (scan==SCAN_SYMB && idata=='*') {
          asmerror= (unsigned char*)"Undefined scale is not allowed"; scan=SCAN_ERR; return; }
        else {
          op->anyoffset=1;
        }; }
      else break;                      // None of expected address elements
      if (scan==SCAN_SYMB && idata==']') break;
      sign='?';
    };
    if (scan==SCAN_ERR) return;
    if (scan!=SCAN_SYMB || idata!=']') {
      asmerror= (unsigned char*)"Syntax error";
      scan=SCAN_ERR; return; };
    // Process XLAT address separately.
    if (xlataddr!=0) {                 // XLAT address in form [EBX+AX]
      for (i=0; i<=8; i++) {           // Check which registers used
        if (i==REG_EBX) continue;
        if (r[i]!=0) break; };
      if (i<=8 || r[REG_EBX]!=1 || op->offset!=0 || op->anyoffset!=0) {
        asmerror= (unsigned char*)"Invalid address"; scan=SCAN_ERR; return; };
      op->type=MXL; }
    // Determine scale, index and base.
    else {
      j=0;                             // Number of used registers
      for (i=0; i<=8; i++) {
        if (r[i]==0)
          continue;                    // Unused register
        if (r[i]==3 || r[i]==5 || r[i]==9) {
          if (op->index>=0 || op->base>=0) {
            if (j==0) asmerror= (unsigned char*)"Invalid scale";
            else asmerror= (unsigned char*)"Too many registers";
            scan=SCAN_ERR; return; };
          op->index=op->base=i;
          op->scale=r[i]-1; }
        else if (r[i]==2 || r[i]==4 || r[i]==8) {
          if (op->index>=0) {
            if (j<=1) asmerror= (unsigned char*)"Only one register may be scaled";
            else asmerror= (unsigned char*)"Too many registers";
            scan=SCAN_ERR; return; };
          op->index=i; op->scale=r[i]; }
        else if (r[i]==1) {
          if (op->base<0)
            op->base=i;
          else if (op->index<0) {
            op->index=i; op->scale=1; }
          else {
            asmerror= (unsigned char*)"Too many registers";
            scan=SCAN_ERR; return;
          }; }
        else {
          asmerror= (unsigned char*)"Invalid scale"; scan=SCAN_ERR; return; };
        j++;
      };
      op->type=MRG;
    }; }
  else {
    asmerror= (unsigned char*)"Unrecognized operand"; scan=SCAN_ERR; return; };
  // In general, address modifier is allowed only with address expression which
  // is a constant, a far address or a memory expression. More precise check
  // will be done later in Assemble().
  if (op->jmpmode!=0 && op->type!=IMM && op->type!=JMF && op->type!=MRG) {
    asmerror= (unsigned char*)"Jump address modifier is not allowed";
    scan=SCAN_ERR; return; };
  Scanasm(0);                          // Fetch next token from input line
};

// Function assembles text into 32-bit 80x86 machine code. It supports imprecise
// operands (for example, R32 stays for any general-purpose 32-bit register).
// This allows to search for incomplete commands. Command is precise when all
// significant bytes in model.mask are 0xFF. Some commands have more than one
// decoding. By calling Assemble() with attempt=0,1... and constsize=0,1,2,3 one
// gets also alternative variants (bit 0x1 of constsize is responsible for size
// of address constant and bit 0x2 - for immediate data). However, only one
// address form is generated ([EAX*2], but not [EAX+EAX]; [EBX+EAX] but not
// [EAX+EBX]; [EAX] will not use SIB byte; no DS: prefix and so on). Returns
// number of bytes in assembled code or non-positive number in case of detected
// error. This number is the negation of the offset in the input text where the
// error encountered. Unfortunately, BC 4.52 is unable to compile the switch
// (arg) in this code when any common subexpression optimization is on. The
// next #pragma statement disables all optimizations.

//#pragma option -Od                     // No optimizations, or BC 4.52 crashes

int Assemble(unsigned char *cmd,ulong ip,t_asmmodel *model,int attempt,
  int constsize,unsigned char *errtext) {
  int i,j,k,namelen,nameok,arg,match,datasize,addrsize,bytesize,minop,maxop;
  int rep,lock,segment,jmpsize,jmpmode,longjump;
  int hasrm,hassib,dispsize,immsize;
  int anydisp,anyimm,anyjmp;
  long l,displacement,immediate,jmpoffset;
  unsigned char name[32]={0}, *nameend;
  unsigned char tcode[MAXCMDSIZE]={0}, tmask[MAXCMDSIZE]={0};
  t_asmoperand aop[3]={0}, *op;             // Up to 3 operands allowed
  const t_cmddata *pd;
  if (model!=NULL) model->length=0;
  if (cmd==NULL || model==NULL || errtext==NULL) {
    if (errtext!=NULL) strcpy( (char*)errtext,"Internal OLLYDBG error");
    return 0; };                       // Error in parameters
  asmcmd=cmd;
  rep=lock=0; errtext[0]='\0';
  Scanasm(SA_NAME);
  if (scan==SCAN_EOL)                  // End of line, nothing to assemble
    return 0;
  while (1) {                          // Fetch all REPxx and LOCK prefixes
    if (scan==SCAN_REP || scan==SCAN_REPE || scan==SCAN_REPNE) {
      if (rep!=0) {
        strcpy( (char*)errtext,"Duplicated REP prefix"); goto error; };
      rep=scan; }
    else if (scan==SCAN_LOCK) {
      if (lock!=0) {
        strcpy( (char*)errtext,"Duplicated LOCK prefix"); goto error; };
      lock=scan; }
    else break;                        // No more prefixes
    Scanasm(SA_NAME); };
  if (scan!=SCAN_NAME || idata>16) {
    strcpy( (char*)errtext,"Command mnemonic expected"); goto error; };
  nameend=asmcmd;
  _strupr( (char*)sdata);
  // Prepare full mnemonic (including repeat prefix, if any).
  if (rep==SCAN_REP) sprintf( (char*)name,"REP %s",sdata);
  else if (rep==SCAN_REPE) sprintf( (char*)name,"REPE %s",sdata);
  else if (rep==SCAN_REPNE) sprintf( (char*)name,"REPNE %s",sdata);
  else strcpy( (char*)name, (char*)sdata);
  Scanasm(0);
  // Parse command operands (up to 3). Note: jump address is always the first
  // (and only) operand in actual command set.
  for (i=0; i<3; i++) {
    aop[i].type=NNN;                   // No operand
    aop[i].size=0;                     // Undefined size
    aop[i].index=-1;                   // No index
    aop[i].scale=0;                    // No scale
    aop[i].base=-1;                    // No base
    aop[i].offset=0;                   // No offset
    aop[i].anyoffset=0;                // No offset
    aop[i].segment=SEG_UNDEF;          // No segment
    aop[i].jmpmode=0; };               // No jump size modifier
  Parseasmoperand(aop+0);
  jmpmode=aop[0].jmpmode;
  if (jmpmode!=0) jmpmode|=0x80;
  if (scan==SCAN_SYMB && idata==',') {
    Scanasm(0);
    Parseasmoperand(aop+1);
    if (scan==SCAN_SYMB && idata==',') {
      Scanasm(0);
      Parseasmoperand(aop+2);
    };
  };
  if (scan==SCAN_ERR) {
    strcpy( (char*)errtext, (char*)asmerror); goto error; };
  if (scan!=SCAN_EOL) {
    strcpy( (char*)errtext,"Extra input after operand"); goto error; };
  // If jump size is not specified, function tries to use short jump. If
  // attempt fails, it retries with long form.
  longjump=0;                          // Try short jump on the first pass
retrylongjump:
  nameok=0;
  // Some commands allow different number of operands. Variables minop and
  // maxop accumulate their minimal and maximal counts. The numbers are not
  // used in assembly process but allow for better error diagnostics.
  minop=3; maxop=0;
  // Main assembly loop: try to find the command which matches all operands,
  // but do not process operands yet.
  namelen=strlen( (char*)name);
  for (pd=cmddata; pd->mask!=0; pd++) {
    if (pd->name[0]=='&') {            // Mnemonic depends on operand size
      j=1;
      datasize=2;
      addrsize=4;
      while (1) {                      // Try all mnemonics (separated by ':')
        for (i=0; pd->name[j]!='\0' && pd->name[j]!=':'; j++) {
          if (pd->name[j]=='*') {
            if (name[i]=='W') { datasize=2; i++; }
            else if (name[i]=='D') { datasize=4; i++; }
            else if (sizesens==0) datasize=2;
            else datasize=4; }
          else if (pd->name[j]==name[i]) i++;
          else break;
        };
        if (name[i]=='\0' && (pd->name[j]=='\0' || pd->name[j]==':'))
          break;                       // Bingo!
        while (pd->name[j]!='\0' && pd->name[j]!=':')
          j++;
        if (pd->name[j]==':') {
          j++; datasize=4; }           // Retry with 32-bit mnenonic
        else {
          i=0; break;                  // Comparison failed
        };
      };
      if (i==0) continue; }
    else if (pd->name[0]=='$') {       // Mnemonic depends on address size
      j=1;
      datasize=0;
      addrsize=2;
      while (1) {                      // Try all mnemonics (separated by ':')
        for (i=0; pd->name[j]!='\0' && pd->name[j]!=':'; j++) {
          if (pd->name[j]=='*') {
            if (name[i]=='W') { addrsize=2; i++; }
            else if (name[i]=='D') { addrsize=4; i++; }
            else if (sizesens==0) addrsize=2;
            else addrsize=4; }
          else if (pd->name[j]==name[i]) i++;
          else break;
        };
        if (name[i]=='\0' && (pd->name[j]=='\0' || pd->name[j]==':'))
          break;                       // Bingo!
        while (pd->name[j]!='\0' && pd->name[j]!=':')
          j++;
        if (pd->name[j]==':') {
          j++; addrsize=4; }           // Retry with 32-bit mnenonic
        else {
          i=0; break;                  // Comparison failed
        };
      };
      if (i==0) continue; }
    else {                             // Compare with all synonimes
      j=k=0;
      datasize=0;                      // Default settings
      addrsize=4;
      while (1) {
        while (pd->name[j]!=',' && pd->name[j]!='\0') j++;
        if (j-k==namelen && _strnicmp( (char*)name, (char*)pd->name+k,namelen)==0) break;
        k=j+1; if (pd->name[j]=='\0') break;
        j=k; };
      if (k>j) continue;
    };
    // For error diagnostics it is important to know whether mnemonic exists.
    nameok++;
    if (pd->arg1==NNN || pd->arg1>=PSEUDOOP)
       minop=0;
    else if (pd->arg2==NNN || pd->arg2>=PSEUDOOP) {
       if (minop>1) minop=1;
       if (maxop<1) maxop=1; }
    else if (pd->arg3==NNN || pd->arg3>=PSEUDOOP) {
       if (minop>2) minop=2;
       if (maxop<2) maxop=2; }
    else
      maxop=3;
    // Determine default and allowed operand size(s).
    if (pd->bits==FF) datasize=2;      // Forced 16-bit size
    if (pd->bits==WW || pd->bits==WS || pd->bits==W3 || pd->bits==WP)
      bytesize=1;                      // 1-byte size allowed
    else
      bytesize=0;                      // Word/dword size only
    // Check whether command operands match specified. If so, variable match
    // remains zero, otherwise it contains kind of mismatch. This allows for
    // better error diagnostics.
    match=0;
    for (j=0; j<3; j++) {              // Up to 3 operands
      op=aop+j;
      if (j==0) arg=pd->arg1;
      else if (j==1) arg=pd->arg2;
      else arg=pd->arg3;
      if (arg==NNN || arg>=PSEUDOOP) {
        if (op->type!=NNN)             // No more arguments
          match|=MA_NOP;
        break; };
      if (op->type==NNN) {
        match|=MA_NOP; break; };       // No corresponding operand
      switch (arg) {
        case REG:                      // Integer register in Reg field
        case RCM:                      // Integer register in command byte
        case RAC:                      // Accumulator (AL/AX/EAX, implicit)
          if (op->type!=REG) match|=MA_TYP;
          if (arg==RAC && op->index!=REG_EAX && op->index!=8) match|=MA_TYP;
          if (bytesize==0 && op->size==1) match|=MA_SIZ;
          if (datasize==0) datasize=op->size;
          if (datasize!=op->size) match|=MA_DIF;
          break;
        case RG4:                      // Integer 4-byte register in Reg field
          if (op->type!=REG) match|=MA_TYP;
          if (op->size!=4) match|=MA_SIZ;
          if (datasize==0) datasize=op->size;
          if (datasize!=op->size) match|=MA_DIF;
          break;
        case RAX:                      // AX (2-byte, implicit)
          if (op->type!=REG || (op->index!=REG_EAX && op->index!=8))
            match|=MA_TYP;
          if (op->size!=2) match|=MA_SIZ;
          if (datasize==0) datasize=op->size;
          if (datasize!=op->size) match|=MA_DIF;
          break;
        case RDX:                      // DX (16-bit implicit port address)
          if (op->type!=REG || (op->index!=REG_EDX && op->index!=8))
            match|=MA_TYP;
          if (op->size!=2) match|=MA_SIZ; break;
        case RCL:                      // Implicit CL register (for shifts)
          if (op->type!=REG || (op->index!=REG_ECX && op->index!=8))
            match|=MA_TYP;
          if (op->size!=1) match|=MA_SIZ;
          break;
        case RS0:                      // Top of FPU stack (ST(0))
          if (op->type!=RST || (op->index!=0 && op->index!=8))
            match|=MA_TYP;
          break;
        case RST:                      // FPU register (ST(i)) in command byte
          if (op->type!=RST) match|=MA_TYP; break;
        case RMX:                      // MMX register MMx
        case R3D:                      // 3DNow! register MMx
          if (op->type!=RMX) match|=MA_TYP; break;
        case MRG:                      // Memory/register in ModRM byte
          if (op->type!=MRG && op->type!=REG) match|=MA_TYP;
          if (bytesize==0 && op->size==1) match|=MA_SIZ;
          if (datasize==0) datasize=op->size;
          if (op->size!=0 && op->size!=datasize) match|=MA_DIF;
          break;
        case MR1:                      // 1-byte memory/register in ModRM byte
          if (op->type!=MRG && op->type!=REG) match|=MA_TYP;
          if (op->size!=0 && op->size!=1) match|=MA_SIZ;
          break;
        case MR2:                      // 2-byte memory/register in ModRM byte
          if (op->type!=MRG && op->type!=REG) match|=MA_TYP;
          if (op->size!=0 && op->size!=2) match|=MA_SIZ;
          break;
        case MR4:                      // 4-byte memory/register in ModRM byte
          if (op->type!=MRG && op->type!=REG) match|=MA_TYP;
          if (op->size!=0 && op->size!=4) match|=MA_SIZ;
          break;
        case RR4:                      // 4-byte memory/register (register only)
          if (op->type!=REG) match|=MA_TYP;
          if (op->size!=0 && op->size!=4) match|=MA_SIZ;
          break;
        case MRJ:                      // Memory/reg in ModRM as JUMP target
          if (op->type!=MRG && op->type!=REG) match|=MA_TYP;
          if (op->size!=0 && op->size!=4) match|=MA_SIZ;
          if ((jmpmode & 0x09)!=0) match|=MA_JMP;
          jmpmode&=0x7F; break;
        case MR8:                      // 8-byte memory/MMX register in ModRM
        case MRD:                      // 8-byte memory/3DNow! register in ModRM
          if (op->type!=MRG && op->type!=RMX) match|=MA_TYP;
          if (op->size!=0 && op->size!=8) match|=MA_SIZ;
          break;
        case RR8:                      // 8-byte MMX register only in ModRM
        case RRD:                      // 8-byte memory/3DNow! (register only)
          if (op->type!=RMX) match|=MA_TYP;
          if (op->size!=0 && op->size!=8) match|=MA_SIZ;
          break;
        case MMA:                      // Memory address in ModRM byte for LEA
          if (op->type!=MRG) match|=MA_TYP; break;
        case MML:                      // Memory in ModRM byte (for LES)
          if (op->type!=MRG) match|=MA_TYP;
          if (op->size!=0 && op->size!=6) match|=MA_SIZ;
          if (datasize==0) datasize=4; else if (datasize!=4) match|=MA_DIF;
          break;
        case MMS:                      // Memory in ModRM byte (as SEG:OFFS)
          if (op->type!=MRG) match|=MA_TYP;
          if (op->size!=0 && op->size!=6) match|=MA_SIZ;
          if ((jmpmode & 0x07)!=0) match|=MA_JMP;
          jmpmode&=0x7F; break;
        case MM6:                      // Memory in ModRm (6-byte descriptor)
          if (op->type!=MRG) match|=MA_TYP;
          if (op->size!=0 && op->size!=6) match|=MA_SIZ;
          break;
        case MMB:                      // Two adjacent memory locations (BOUND)
          if (op->type!=MRG) match|=MA_TYP;
          k=op->size; if (ideal==0 && k>1) k/=2;
          if (k!=0 && k!=datasize) match|=MA_DIF;
          break;
        case MD2:                      // Memory in ModRM byte (16-bit integer)
        case MB2:                      // Memory in ModRM byte (16-bit binary)
          if (op->type!=MRG) match|=MA_TYP;
          if (op->size!=0 && op->size!=2) match|=MA_SIZ;
          break;
        case MD4:                      // Memory in ModRM byte (32-bit integer)
        case MF4:                      // Memory in ModRM byte (32-bit float)
          if (op->type!=MRG) match|=MA_TYP;
          if (op->size!=0 && op->size!=4) match|=MA_SIZ;
          break;
        case MD8:                      // Memory in ModRM byte (64-bit integer)
        case MF8:                      // Memory in ModRM byte (64-bit float)
          if (op->type!=MRG) match|=MA_TYP;
          if (op->size!=0 && op->size!=8) match|=MA_SIZ;
          break;
        case MDA:                      // Memory in ModRM byte (80-bit BCD)
        case MFA:                      // Memory in ModRM byte (80-bit float)
          if (op->type!=MRG) match|=MA_TYP;
          if (op->size!=0 && op->size!=10) match|=MA_SIZ;
          break;
        case MFE:                      // Memory in ModRM byte (FPU environment)
        case MFS:                      // Memory in ModRM byte (FPU state)
        case MFX:                      // Memory in ModRM byte (ext. FPU state)
          if (op->type!=MRG) match|=MA_TYP;
          if (op->size!=0) match|=MA_SIZ;
          break;
        case MSO:                      // Source in string operands ([ESI])
          if (op->type!=MRG || op->base!=REG_ESI ||
            op->index!=-1 || op->offset!=0 || op->anyoffset!=0) match|=MA_TYP;
          if (datasize==0) datasize=op->size;
          if (op->size!=0 && op->size!=datasize) match|=MA_DIF;
          break;
        case MDE:                      // Destination in string operands ([EDI])
          if (op->type!=MRG || op->base!=REG_EDI ||
            op->index!=-1 || op->offset!=0 || op->anyoffset!=0) match|=MA_TYP;
          if (op->segment!=SEG_UNDEF && op->segment!=SEG_ES) match|=MA_SEG;
          if (datasize==0) datasize=op->size;
          if (op->size!=0 && op->size!=datasize) match|=MA_DIF;
          break;
        case MXL:                      // XLAT operand ([EBX+AL])
          if (op->type!=MXL) match|=MA_TYP; break;
        case IMM:                      // Immediate data (8 or 16/32)
        case IMU:                      // Immediate unsigned data (8 or 16/32)
          if (op->type!=IMM) match|=MA_TYP;
          break;
        case VXD:                      // VxD service (32-bit only)
          if (op->type!=IMM) match|=MA_TYP;
          if (datasize==0) datasize=4;
          if (datasize!=4) match|=MA_SIZ;
          break;
        case JMF:                      // Immediate absolute far jump/call addr
          if (op->type!=JMF) match|=MA_TYP;
          if ((jmpmode & 0x05)!=0) match|=MA_JMP;
          jmpmode&=0x7F; break;
        case JOB:                      // Immediate byte offset (for jumps)
          if (op->type!=IMM || longjump) match|=MA_TYP;
          if ((jmpmode & 0x0A)!=0) match|=MA_JMP;
          jmpmode&=0x7F; break;
        case JOW:                      // Immediate full offset (for jumps)
          if (op->type!=IMM) match|=MA_TYP;
          if ((jmpmode & 0x09)!=0) match|=MA_JMP;
          jmpmode&=0x7F; break;
        case IMA:                      // Immediate absolute near data address
          if (op->type!=MRG || op->base>=0 || op->index>=0) match|=MA_TYP;
          break;
        case IMX:                      // Immediate sign-extendable byte
          if (op->type!=IMM) match|=MA_TYP;
          if (op->offset<-128 || op->offset>127) match|=MA_RNG;
          break;
        case C01:                      // Implicit constant 1 (for shifts)
          if (op->type!=IMM || (op->offset!=1 && op->anyoffset==0))
            match|=MA_TYP;
          break;
        case IMS:                      // Immediate byte (for shifts)
        case IM1:                      // Immediate byte
          if (op->type!=IMM) match|=MA_TYP;
          if (op->offset<-128 || op->offset>255) match|=MA_RNG;
          break;
        case IM2:                      // Immediate word (ENTER/RET)
          if (op->type!=IMM) match|=MA_TYP;
          if (op->offset<0 || op->offset>65535) match|=MA_RNG;
          break;
        case SGM:                      // Segment register in ModRM byte
          if (op->type!=SGM) match|=MA_TYP;
          if (datasize==0) datasize=2;
          if (datasize!=2) match|=MA_DIF;
          break;
        case SCM:                      // Segment register in command byte
          if (op->type!=SGM) match|=MA_TYP;
          break;
        case CRX:                      // Control register CRx
        case DRX:                      // Debug register DRx
          if (op->type!=arg) match|=MA_TYP;
          if (datasize==0) datasize=4;
          if (datasize!=4) match|=MA_DIF;
          break;
        case PRN:                      // Near return address (pseudooperand)
        case PRF:                      // Far return address (pseudooperand)
        case PAC:                      // Accumulator (AL/AX/EAX, pseudooperand)
        case PAH:                      // AH (in LAHF/SAHF, pseudooperand)
        case PFL:                      // Lower byte of flags (pseudooperand)
        case PS0:                      // Top of FPU stack (pseudooperand)
        case PS1:                      // ST(1) (pseudooperand)
        case PCX:                      // CX/ECX (pseudooperand)
        case PDI:                      // EDI (pseudooperand in MMX extentions)
          break;
        default:                       // Undefined type of operand
          strcpy( (char*)errtext,"Internal Assembler error");
        goto error;
      };                               // End of switch (arg)
      if ((jmpmode & 0x80)!=0) match|=MA_JMP;
      if (match!=0) break;             // Some of the operands doesn't match
    };                                 // End of operand matching loop
    if (match==0) {                    // Exact match found
      if (attempt>0) {
        --attempt; nameok=0; }         // Well, try to find yet another match
      else break;
    };
  };                                   // End of command search loop
  // Check whether some error was detected. If several errors were found
  // similtaneously, report one (roughly in order of significance).
  if (nameok==0) {                     // Mnemonic unavailable
    strcpy( (char*)errtext,"Unrecognized command");
    asmcmd=nameend; goto error; };
  if (match!=0) {                      // Command not found
    if (minop>0 && aop[minop-1].type==NNN)
      strcpy( (char*)errtext,"Too few operands");
    else if (maxop<3 && aop[maxop].type!=NNN)
      strcpy( (char*)errtext,"Too many operands");
    else if (nameok>1)                 // More that 1 command
      strcpy( (char*)errtext,"Command does not support given operands");
    else if (match & MA_JMP)
      strcpy( (char*)errtext,"Invalid jump size modifier");
    else if (match & MA_NOP)
      strcpy( (char*)errtext,"Wrong number of operands");
    else if (match & MA_TYP)
      strcpy( (char*)errtext,"Command does not support given operands");
    else if (match & MA_NOS)
      strcpy( (char*)errtext,"Please specify operand size");
    else if (match & MA_SIZ)
      strcpy( (char*)errtext,"Bad operand size");
    else if (match & MA_DIF)
      strcpy( (char*)errtext,"Different size of operands");
    else if (match & MA_SEG)
      strcpy( (char*)errtext,"Invalid segment register");
    else if (match & MA_RNG)
      strcpy( (char*)errtext,"Constant out of expected range");
    else
      strcpy( (char*)errtext,"Erroneous command");
    goto error;
  };
  // Exact match found. Now construct the code.
  hasrm=0;                             // Whether command has ModR/M byte
  hassib=0;                            // Whether command has SIB byte
  dispsize=0;                          // Size of displacement (if any)
  immsize=0;                           // Size of immediate data (if any)
  segment=SEG_UNDEF;                   // Necessary segment prefix
  jmpsize=0;                           // No relative jumps
  memset(tcode,0,sizeof(tcode));
  *(ulong *)tcode=pd->code & pd->mask;
  memset(tmask,0,sizeof(tmask));
  *(ulong *)tmask=pd->mask;
  i=pd->len-1;                         // Last byte of command itself
  if (rep) i++;                        // REPxx prefixes count as extra byte
  // In some cases at least one operand must have explicit size declaration (as
  // in MOV [EAX],1). This preliminary check does not include all cases.
  if (pd->bits==WW || pd->bits==WS || pd->bits==WP) {
    if (datasize==0) {
      strcpy( (char*)errtext,"Please specify operand size"); goto error; }
    else if (datasize>1)
      tcode[i]|=0x01;                  // WORD or DWORD size of operands
    tmask[i]|=0x01; }
  else if (pd->bits==W3) {
    if (datasize==0) {
      strcpy( (char*)errtext,"Please specify operand size"); goto error; }
    else if (datasize>1)
      tcode[i]|=0x08;                  // WORD or DWORD size of operands
    tmask[i]|=0x08; };
  // Present suffix of 3DNow! command as immediate byte operand.
  if ((pd->type & C_TYPEMASK)==C_NOW) {
    immsize=1;
    immediate=(pd->code>>16) & 0xFF; };
  // Process operands again, this time constructing the code.
  anydisp=anyimm=anyjmp=0;
  for (j=0; j<3; j++) {                // Up to 3 operands
    op=aop+j;
    if (j==0) arg=pd->arg1;
    else if (j==1) arg=pd->arg2;
    else arg=pd->arg3;
    if (arg==NNN) break;               // All operands processed
    switch (arg) {
      case REG:                        // Integer register in Reg field
      case RG4:                        // Integer 4-byte register in Reg field
      case RMX:                        // MMX register MMx
      case R3D:                        // 3DNow! register MMx
      case CRX:                        // Control register CRx
      case DRX:                        // Debug register DRx
        hasrm=1;
        if (op->index<8) {
          tcode[i+1]|=(unsigned char)(op->index<<3); tmask[i+1]|=0x38; };
        break;
      case RCM:                        // Integer register in command byte
      case RST:                        // FPU register (ST(i)) in command byte
        if (op->index<8) {
          tcode[i]|=(unsigned char)op->index; tmask[i]|=0x07; };
        break;
      case RAC:                        // Accumulator (AL/AX/EAX, implicit)
      case RAX:                        // AX (2-byte, implicit)
      case RDX:                        // DX (16-bit implicit port address)
      case RCL:                        // Implicit CL register (for shifts)
      case RS0:                        // Top of FPU stack (ST(0))
      case MDE:                        // Destination in string op's ([EDI])
      case C01:                        // Implicit constant 1 (for shifts)
        break;                         // Simply skip implicit operands
      case MSO:                        // Source in string op's ([ESI])
      case MXL:                        // XLAT operand ([EBX+AL])
        if (op->segment!=SEG_UNDEF && op->segment!=SEG_DS)
          segment=op->segment;
        break;
      case MRG:                        // Memory/register in ModRM byte
      case MRJ:                        // Memory/reg in ModRM as JUMP target
      case MR1:                        // 1-byte memory/register in ModRM byte
      case MR2:                        // 2-byte memory/register in ModRM byte
      case MR4:                        // 4-byte memory/register in ModRM byte
      case RR4:                        // 4-byte memory/register (register only)
      case MR8:                        // 8-byte memory/MMX register in ModRM
      case RR8:                        // 8-byte MMX register only in ModRM
      case MRD:                        // 8-byte memory/3DNow! register in ModRM
      case RRD:                        // 8-byte memory/3DNow! (register only)
        hasrm=1;
        if (op->type!=MRG) {           // Register in ModRM byte
          tcode[i+1]|=0xC0; tmask[i+1]|=0xC0;
          if (op->index<8) {
            tcode[i+1]|=(unsigned char)op->index; tmask[i+1]|=0x07; };
          break;
        };                             // Note: NO BREAK, continue with address
      case MMA:                        // Memory address in ModRM byte for LEA
      case MML:                        // Memory in ModRM byte (for LES)
      case MMS:                        // Memory in ModRM byte (as SEG:OFFS)
      case MM6:                        // Memory in ModRm (6-byte descriptor)
      case MMB:                        // Two adjacent memory locations (BOUND)
      case MD2:                        // Memory in ModRM byte (16-bit integer)
      case MB2:                        // Memory in ModRM byte (16-bit binary)
      case MD4:                        // Memory in ModRM byte (32-bit integer)
      case MD8:                        // Memory in ModRM byte (64-bit integer)
      case MDA:                        // Memory in ModRM byte (80-bit BCD)
      case MF4:                        // Memory in ModRM byte (32-bit float)
      case MF8:                        // Memory in ModRM byte (64-bit float)
      case MFA:                        // Memory in ModRM byte (80-bit float)
      case MFE:                        // Memory in ModRM byte (FPU environment)
      case MFS:                        // Memory in ModRM byte (FPU state)
      case MFX:                        // Memory in ModRM byte (ext. FPU state)
        hasrm=1; displacement=op->offset; anydisp=op->anyoffset;
        if (op->base<0 && op->index<0) {
          dispsize=4;                  // Special case of immediate address
          if (op->segment!=SEG_UNDEF && op->segment!=SEG_DS)
            segment=op->segment;
          tcode[i+1]|=0x05;
          tmask[i+1]|=0xC7; }
        else if (op->index<0 && op->base!=REG_ESP) {
          tmask[i+1]|=0xC0;            // SIB byte unnecessary
          if (op->offset==0 && op->anyoffset==0 && op->base!=REG_EBP)
            ;                          // [EBP] always requires offset
          else if ((constsize & 1)!=0 &&
            ((op->offset>=-128 && op->offset<128) || op->anyoffset!=0)
          ) {
            tcode[i+1]|=0x40;          // Disp8
            dispsize=1; }
          else {
            tcode[i+1]|=0x80;          // Disp32
            dispsize=4; };
          if (op->base<8) {
            if (op->segment!=SEG_UNDEF && op->segment!=addr32[op->base].defseg)
              segment=op->segment;
            tcode[i+1]|=
              (unsigned char)op->base;          // Note that case [ESP] has base<0.
            tmask[i+1]|=0x07; }
          else segment=op->segment; }
        else {                         // SIB byte necessary
          hassib=1;
          if (op->base==REG_EBP &&     // EBP as base requires offset, optimize
            op->index>=0 && op->scale==1 && op->offset==0 && op->anyoffset==0) {
            op->base=op->index; op->index=REG_EBP; };
          if (op->index==REG_ESP &&    // ESP cannot be an index, reorder
            op->scale<=1) {
            op->index=op->base; op->base=REG_ESP; op->scale=1; };
          if (op->base<0 &&            // No base means 4-byte offset, optimize
            op->index>=0 && op->scale==2 &&
            op->offset>=-128 && op->offset<128 && op->anyoffset==0) {
            op->base=op->index; op->scale=1; };
          if (op->index==REG_ESP) {    // Reordering was unsuccessfull
            strcpy( (char*)errtext,"Invalid indexing mode");
            goto error; };
          if (op->base<0) {
            tcode[i+1]|=0x04;
            dispsize=4; }
          else if (op->offset==0 && op->anyoffset==0 && op->base!=REG_EBP)
            tcode[i+1]|=0x04;          // No displacement
          else if ((constsize & 1)!=0 &&
            ((op->offset>=-128 && op->offset<128) || op->anyoffset!=0)
          ) {
            tcode[i+1]|=0x44;          // Disp8
            dispsize=1; }
          else {
            tcode[i+1]|=0x84;          // Disp32
            dispsize=4; };
          tmask[i+1]|=0xC7;            // ModRM completed, proceed with SIB
          if (op->scale==2) tcode[i+2]|=0x40;
          else if (op->scale==4) tcode[i+2]|=0x80;
          else if (op->scale==8) tcode[i+2]|=0xC0;
          tmask[i+2]|=0xC0;
          if (op->index<8) {
            if (op->index<0) op->index=0x04;
            tcode[i+2]|=(unsigned char)(op->index<<3);
            tmask[i+2]|=0x38; };
          if (op->base<8) {
            if (op->base<0) op->base=0x05;
            if (op->segment!=SEG_UNDEF && op->segment!=addr32[op->base].defseg)
              segment=op->segment;
            tcode[i+2]|=(unsigned char)op->base;
            tmask[i+2]|=0x07; }
          else segment=op->segment; };
        break;
      case IMM:                        // Immediate data (8 or 16/32)
      case IMU:                        // Immediate unsigned data (8 or 16/32)
      case VXD:                        // VxD service (32-bit only)
        if (datasize==0 && pd->arg2==NNN && (pd->bits==SS || pd->bits==WS))
          datasize=4;
        if (datasize==0) {
          strcpy( (char*)errtext,"Please specify operand size");
          goto error; };
        immediate=op->offset; anyimm=op->anyoffset;
        if (pd->bits==SS || pd->bits==WS) {
          if (datasize>1 && (constsize & 2)!=0 &&
            ((immediate>=-128 && immediate<128) || op->anyoffset!=0)) {
            immsize=1; tcode[i]|=0x02; }
          else immsize=datasize;
          tmask[i]|=0x02; }
        else immsize=datasize;
        break;
      case IMX:                        // Immediate sign-extendable byte
      case IMS:                        // Immediate byte (for shifts)
      case IM1:                        // Immediate byte
        if (immsize==2)                // To accomodate ENTER instruction
          immediate=(immediate & 0xFFFF) | (op->offset<<16);
        else immediate=op->offset;
        anyimm|=op->anyoffset;
        immsize++; break;
      case IM2:                        // Immediate word (ENTER/RET)
        immediate=op->offset; anyimm=op->anyoffset;
        immsize=2; break;
      case IMA:                        // Immediate absolute near data address
        if (op->segment!=SEG_UNDEF && op->segment!=SEG_DS)
          segment=op->segment;
        displacement=op->offset; anydisp=op->anyoffset;
        dispsize=4; break;
      case JOB:                        // Immediate byte offset (for jumps)
        jmpoffset=op->offset; anyjmp=op->anyoffset;
        jmpsize=1; break;
      case JOW:                        // Immediate full offset (for jumps)
        jmpoffset=op->offset; anyjmp=op->anyoffset;
        jmpsize=4; break;
      case JMF:                        // Immediate absolute far jump/call addr
        displacement=op->offset; anydisp=op->anyoffset; dispsize=4;
        immediate=op->segment; anyimm=op->anyoffset; immsize=2;
        break;
      case SGM:                        // Segment register in ModRM byte
        hasrm=1;
        if (op->index<6) {
          tcode[i+1]|=(unsigned char)(op->index<<3); tmask[i+1]|=0x38; };
        break;
      case SCM:                        // Segment register in command byte
        if (op->index==SEG_FS || op->index==SEG_GS) {
          tcode[0]=0x0F; tmask[0]=0xFF;
          i=1;
          if (strcmp( (char*)name,"PUSH")==0)
            tcode[i]=(unsigned char)((op->index<<3) | 0x80);
          else
            tcode[i]=(unsigned char)((op->index<<3) | 0x81);
          tmask[i]=0xFF; }
        else if (op->index<6) {
          if (op->index==SEG_CS && strcmp( (char*)name,"POP")==0) {
            strcpy( (char*)errtext,"Unable to POP CS");
            goto error; };
          tcode[i]=(unsigned char)((tcode[i] & 0xC7) | (op->index<<3)); }
        else {
          tcode[i]&=0xC7;
          tmask[i]&=0xC7; };
        break;
      case PRN:                        // Near return address (pseudooperand)
      case PRF:                        // Far return address (pseudooperand)
      case PAC:                        // Accumulator (AL/AX/EAX, pseudooperand)
      case PAH:                        // AH (in LAHF/SAHF, pseudooperand)
      case PFL:                        // Lower byte of flags (pseudooperand)
      case PS0:                        // Top of FPU stack (pseudooperand)
      case PS1:                        // ST(1) (pseudooperand)
      case PCX:                        // CX/ECX (pseudooperand)
      case PDI:                        // EDI (pseudooperand in MMX extentions)
        break;                         // Simply skip preudooperands
      default:                         // Undefined type of operand
        strcpy( (char*)errtext,"Internal Assembler error");
      goto error;
    };
  };
  // Gather parts of command together in the complete command.
  j=0;
  if (lock!=0) {                       // Lock prefix specified
    model->code[j]=0xF0;
    model->mask[j]=0xFF; j++; };
  if (datasize==2 && pd->bits!=FF) {   // Data size prefix necessary
    model->code[j]=0x66;
    model->mask[j]=0xFF; j++; };
  if (addrsize==2) {                   // Address size prefix necessary
    model->code[j]=0x67;
    model->mask[j]=0xFF; j++; };
  if (segment!=SEG_UNDEF) {            // Segment prefix necessary
    if (segment==SEG_ES) model->code[j]=0x26;
    else if (segment==SEG_CS) model->code[j]=0x2E;
    else if (segment==SEG_SS) model->code[j]=0x36;
    else if (segment==SEG_DS) model->code[j]=0x3E;
    else if (segment==SEG_FS) model->code[j]=0x64;
    else if (segment==SEG_GS) model->code[j]=0x65;
    else { strcpy( (char*)errtext,"Internal Assembler error"); goto error; };
    model->mask[j]=0xFF; j++; };
  if (dispsize>0) {
    memcpy(tcode+i+1+hasrm+hassib,&displacement,dispsize);
    if (anydisp==0) memset(tmask+i+1+hasrm+hassib,0xFF,dispsize); };
  if (immsize>0) {
    if (immsize==1) l=0xFFFFFF00L;
    else if (immsize==2) l=0xFFFF0000L;
    else l=0L;
    if ((immediate & l)!=0 && (immediate & l)!=l) {
      strcpy( (char*)errtext,"Constant does not fit into operand");
      goto error; };
    memcpy(tcode+i+1+hasrm+hassib+dispsize,&immediate,immsize);
    if (anyimm==0) memset(tmask+i+1+hasrm+hassib+dispsize,0xFF,immsize); };
  i=i+1+hasrm+hassib+dispsize+immsize;
  jmpoffset=jmpoffset-(i+j+jmpsize);
  model->jmpsize=jmpsize;
  model->jmpoffset=jmpoffset;
  model->jmppos=i+j;
  if (jmpsize!=0) {
    if (ip!=0) {
      jmpoffset=jmpoffset-ip;
      if (jmpsize==1 && anyjmp==0 && (jmpoffset<-128 || jmpoffset>=128)) {
        if (longjump==0 && (jmpmode & 0x03)==0) {
          longjump=1;
          goto retrylongjump; };
        sprintf( (char*)errtext,
          "Relative jump out of range, use %s LONG form",name);
        goto error; };
      memcpy(tcode+i,&jmpoffset,jmpsize);
    };
    if (anyjmp==0) memset(tmask+i,0xFF,jmpsize);
    i+=jmpsize; };
  memcpy(model->code+j,tcode,i);
  memcpy(model->mask+j,tmask,i);
  i+=j;
  model->length=i;
  return i;                            // Positive value: length of code
error:
  model->length=0;
  return cmd-asmcmd;                   // Negative value: position of error
};

//#pragma option -O.                     // Restore old optimization options


// Free Disassembler and Assembler -- Disassembler
//
// Copyright (C) 2001 Oleh Yuschuk
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


////////////////////////////////////////////////////////////////////////////////
//////////////////////////// DISASSEMBLER FUNCTIONS ////////////////////////////

// Work variables of disassembler
static ulong     datasize;             // Size of data (1,2,4 bytes)
static ulong     addrsize;             // Size of address (2 or 4 bytes)
static int       segprefix;            // Segment override prefix or SEG_UNDEF
static int       hasrm;                // Command has ModR/M byte
static int       hassib;               // Command has SIB byte
static int       dispsize;             // Size of displacement (if any)
static int       immsize;              // Size of immediate data (if any)
static int       softerror;            // Noncritical disassembler error
static int       ndump;                // Current length of command dump
static int       nresult;              // Current length of disassembly
static int       addcomment;           // Comment value of operand

// Copy of input parameters of function Disasm()
static unsigned char      *cmd;                 // Pointer to binary data
static unsigned char      *pfixup;              // Pointer to possible fixups or NULL
static ulong     size;                 // Remaining size of the command buffer
static t_disasm  *da;                  // Pointer to disassembly results
static int       mode;                 // Disassembly mode (DISASM_xxx)

// Disassemble name of 1, 2 or 4-byte general-purpose integer register and, if
// requested and available, dump its contents. Parameter type changes decoding
// of contents for some operand types.
static void DecodeRG(int index,int datasize,int type) {
  int sizeindex;
  unsigned char name[9]={0};
  if (mode<DISASM_DATA) return;        // No need to decode
  index&=0x07;
  if (datasize==1)
    sizeindex=0;
  else if (datasize==2)
    sizeindex=1;
  else if (datasize==4)
    sizeindex=2;
  else {
    da->error=DAE_INTERN; return; };
  if (mode>=DISASM_FILE) {
    strcpy( (char*)name, (char*)regname[sizeindex][index]);
    if (lowercase) _strlwr( (char*)name);
    if (type<PSEUDOOP)                 // Not a pseudooperand
      nresult+=sprintf( (char*)da->result+nresult,"%s",name);
    ;
  };
};

// Disassemble name of 80-bit floating-point register and, if available, dump
// its contents.
static void DecodeST(int index,int pseudoop) {
  int i;
  unsigned char s[32]={0};
  if (mode<DISASM_FILE) return;        // No need to decode
  index&=0x07;
  i=sprintf( (char*)s,"%s(%i)",(lowercase?"st":"ST"),index);
  if (pseudoop==0) {
    strcpy( (char*)da->result+nresult, (char*)s);
    nresult+=i;
  };
};

// Disassemble name of 64-bit MMX register.
static void DecodeMX(int index) {
  unsigned char *pr;
  if (mode<DISASM_FILE) return;        // No need to decode
  index&=0x07;
  pr=da->result+nresult;
  nresult+=sprintf( (char*)pr,"%s%i",(lowercase?"mm":"MM"),index);
};

// Disassemble name of 64-bit 3DNow! register and, if available, dump its
// contents.
static void DecodeNR(int index) {
  unsigned char *pr;
  if (mode<DISASM_FILE) return;        // No need to decode
  index&=0x07;
  pr=da->result+nresult;
  nresult+=sprintf( (char*)pr,"%s%i",(lowercase?"mm":"MM"),index);
};

// Service function, adds valid memory adress in MASM or Ideal format to
// disassembled string. Parameters: defseg - default segment for given
// register combination, descr - fully decoded register part of address,
// offset - constant part of address, dsize - data size in bytes. If global
// flag 'symbolic' is set, function also tries to decode offset as name of
// some label.
static void Memadr(int defseg,const unsigned char *descr,long offset,int dsize) {
  int i,n,seg;
  unsigned char *pr;
  unsigned char s[TEXTLEN]={0};
  if (mode<DISASM_FILE || descr==NULL)
    return;                            // No need or possibility to decode
  pr=da->result+nresult; n=0;
  if (segprefix!=SEG_UNDEF) seg=segprefix; else seg=defseg;
  if (ideal!=0) pr[n++]='[';
  // In some cases Disassembler may omit size of memory operand. Namely, flag
  // showmemsize must be 0, type bit C_EXPL must be 0 (this bit namely means
  // that explicit operand size is necessary) and type of command must not be
  // C_MMX or C_NOW (because bit C_EXPL has in these cases different meaning).
  // Otherwise, exact size must be supplied.
  if (showmemsize!=0 || (da->cmdtype & C_TYPEMASK)==C_MMX ||
    (da->cmdtype & C_TYPEMASK)==C_NOW || (da->cmdtype & C_EXPL)!=0
  ) {
    if (dsize<sizeof(sizename)/sizeof(sizename[0]))
      n+=sprintf( (char*)pr+n,"%s %s",sizename[dsize],(ideal==0?"PTR ":""));
    else
      n+=sprintf( (char*)pr+n,"(%i-BYTE) %s",dsize,(ideal==0?"PTR ":""));
    ;
  };
  if ((putdefseg!=0 || seg!=defseg) && seg!=SEG_UNDEF)
    n+=sprintf( (char*)pr+n,"%s:",segname[seg]);
  if (ideal==0) pr[n++]='[';
  n+=sprintf( (char*)pr+n,"%s",descr);
  if (lowercase) _strlwr( (char*)pr);
  if (offset==0L) {
    if (*descr=='\0') pr[n++]='0'; }
  else {
    if (symbolic && mode>=DISASM_CODE)
      i=Decodeaddress(offset,s,TEXTLEN-n-24,NULL);
    else i=0;
    if (i>0) {                         // Offset decoded in symbolic form
      if (*descr!='\0') pr[n++]='+';
      strcpy( (char*)pr+n, (char*)s); n+=i; }
    else if (offset<0 && offset>-16384 && *descr!='\0')
      n+=sprintf( (char*)pr+n,"-%lX",-offset);
    else {
      if (*descr!='\0') pr[n++]='+';
      n+=sprintf( (char*)pr+n,"%lX",offset);
    };
  };
  pr[n++]=']'; pr[n]='\0';
  nresult+=n;
};

// Disassemble memory/register from the ModRM/SIB bytes and, if available, dump
// address and contents of memory.
static void DecodeMR(int type) {
  int j,memonly,inmemory,seg;
  int c,sib;
  ulong dsize,regsize,addr;
  unsigned char s[TEXTLEN]={0};
  if (size<2) {
    da->error=DAE_CROSS; return; };    // ModR/M byte outside the memory block
  hasrm=1;
  dsize=regsize=datasize;              // Default size of addressed reg/memory
  memonly=0;                           // Register in ModM field is allowed
  // Size and kind of addressed memory or register in ModM has no influence on
  // the command size, and exact calculations are omitted if only command size
  // is requested. If register is used, optype will be incorrect and we need
  // to correct it later.
  c=cmd[1] & 0xC7;                     // Leave only Mod and M fields
  if (mode>=DISASM_DATA) {
    if ((c & 0xC0)==0xC0)              // Register operand
      inmemory=0;
    else                               // Memory operand
      inmemory=1;
    switch (type) {
      case MRG:                        // Memory/register in ModRM byte
        if (inmemory) {
          if (datasize==1) da->memtype=DEC_BYTE;
          else if (datasize==2) da->memtype=DEC_WORD;
          else da->memtype=DEC_DWORD; };
        break;
      case MRJ:                        // Memory/reg in ModRM as JUMP target
        if (datasize!=2 && inmemory)
          da->memtype=DEC_DWORD; 
        if (mode>=DISASM_FILE && shownear!=0)
          nresult+=sprintf( (char*)da->result+nresult,"%s ",(lowercase?"near":"NEAR"));
        break;
      case MR1:                        // 1-byte memory/register in ModRM byte
        dsize=regsize=1;
        if (inmemory) da->memtype=DEC_BYTE; break;
      case MR2:                        // 2-byte memory/register in ModRM byte
        dsize=regsize=2;
        if (inmemory) da->memtype=DEC_WORD; break;
      case MR4:                        // 4-byte memory/register in ModRM byte
      case RR4:                        // 4-byte memory/register (register only)
        dsize=regsize=4;
        if (inmemory) da->memtype=DEC_DWORD; break;
      case MR8:                        // 8-byte memory/MMX register in ModRM
      case RR8:                        // 8-byte MMX register only in ModRM
        dsize=8;
        if (inmemory) da->memtype=DEC_QWORD; break;
      case MRD:                        // 8-byte memory/3DNow! register in ModRM
      case RRD:                        // 8-byte memory/3DNow! (register only)
        dsize=8;
        if (inmemory) da->memtype=DEC_3DNOW; break;
      case MMA:                        // Memory address in ModRM byte for LEA
        memonly=1; break;
      case MML:                        // Memory in ModRM byte (for LES)
        dsize=datasize+2; memonly=1;
        if (datasize==4 && inmemory)
          da->memtype=DEC_FWORD;
        da->warnings|=DAW_SEGMENT;
        break;
      case MMS:                        // Memory in ModRM byte (as SEG:OFFS)
        dsize=datasize+2; memonly=1;
        if (datasize==4 && inmemory)
          da->memtype=DEC_FWORD;
        if (mode>=DISASM_FILE)
          nresult+=sprintf( (char*)da->result+nresult,"%s ",(lowercase?"far":"FAR"));
        break;
      case MM6:                        // Memory in ModRM (6-byte descriptor)
        dsize=6; memonly=1;
        if (inmemory) da->memtype=DEC_FWORD; break;
      case MMB:                        // Two adjacent memory locations (BOUND)
        dsize=(ideal?datasize:datasize*2); memonly=1; break;
      case MD2:                        // Memory in ModRM byte (16-bit integer)
      case MB2:                        // Memory in ModRM byte (16-bit binary)
        dsize=2; memonly=1;
        if (inmemory) da->memtype=DEC_WORD; break;
      case MD4:                        // Memory in ModRM byte (32-bit integer)
        dsize=4; memonly=1;
        if (inmemory) da->memtype=DEC_DWORD; break;
      case MD8:                        // Memory in ModRM byte (64-bit integer)
        dsize=8; memonly=1;
        if (inmemory) da->memtype=DEC_QWORD; break;
      case MDA:                        // Memory in ModRM byte (80-bit BCD)
        dsize=10; memonly=1;
        if (inmemory) da->memtype=DEC_TBYTE; break;
      case MF4:                        // Memory in ModRM byte (32-bit float)
        dsize=4; memonly=1;
        if (inmemory) da->memtype=DEC_FLOAT4; break;
      case MF8:                        // Memory in ModRM byte (64-bit float)
        dsize=8; memonly=1;
        if (inmemory) da->memtype=DEC_FLOAT8; break;
      case MFA:                        // Memory in ModRM byte (80-bit float)
        dsize=10; memonly=1;
        if (inmemory) da->memtype=DEC_FLOAT10; break;
      case MFE:                        // Memory in ModRM byte (FPU environment)
        dsize=28; memonly=1; break;
      case MFS:                        // Memory in ModRM byte (FPU state)
        dsize=108; memonly=1; break;
      case MFX:                        // Memory in ModRM byte (ext. FPU state)
        dsize=512; memonly=1; break;
      default:                         // Operand is not in ModM!
        da->error=DAE_INTERN;
      break;
    };
  };
  addr=0;
  // There are many possibilities to decode ModM/SIB address. The first
  // possibility is register in ModM - general-purpose, MMX or 3DNow!
  if ((c & 0xC0)==0xC0) {              // Decode register operand
    if (type==MR8 || type==RR8)
      DecodeMX(c);                     // MMX register
    else if (type==MRD || type==RRD)
      DecodeNR(c);                     // 3DNow! register
    else  
      DecodeRG(c,regsize,type);        // General-purpose register
    if (memonly!=0)
      softerror=DAE_MEMORY;            // Register where only memory allowed
    return; };
  // Next possibility: 16-bit addressing mode, very seldom in 32-bit flat model
  // but still supported by processor. SIB byte is never used here.
  if (addrsize==2) {
    if (c==0x06) {                     // Special case of immediate address
      dispsize=2;
      if (size<4)
        da->error=DAE_CROSS;           // Disp16 outside the memory block
      else if (mode>=DISASM_DATA) {
        da->adrconst=addr=*(ushort *)(cmd+2);
        if (addr==0) da->zeroconst=1;
        seg=SEG_DS;
        Memadr(seg,"",addr,dsize);
      }; }
    else {
      da->indexed=1;
      if ((c & 0xC0)==0x40) {          // 8-bit signed displacement
        if (size<3) da->error=DAE_CROSS;
        else addr=(signed char)cmd[2] & 0xFFFF;
        dispsize=1; }
      else if ((c & 0xC0)==0x80) {     // 16-bit unsigned displacement
        if (size<4) da->error=DAE_CROSS;
        else addr=*(ushort *)(cmd+2);
        dispsize=2; };
      if (mode>=DISASM_DATA && da->error==DAE_NOERR) {
        da->adrconst=addr;
        if (addr==0) da->zeroconst=1;
        seg=addr16[c & 0x07].defseg;
        Memadr(seg,addr16[c & 0x07].descr,addr,dsize);
      };
    };
  }
  // Next possibility: immediate 32-bit address.
  else if (c==0x05) {                  // Special case of immediate address
    dispsize=4;
    if (size<6)
      da->error=DAE_CROSS;             // Disp32 outside the memory block
    else if (mode>=DISASM_DATA) {
      da->adrconst=addr=*(ulong *)(cmd+2);
      if (pfixup==NULL) pfixup=cmd+2;
      da->fixupsize+=4;
      if (addr==0) da->zeroconst=1;
      seg=SEG_DS;
      Memadr(seg,"",addr,dsize);
    }; }
  // Next possibility: 32-bit address with SIB byte.
  else if ((c & 0x07)==0x04) {         // SIB addresation
    sib=cmd[2]; hassib=1;
    *s='\0';
    if (c==0x04 && (sib & 0x07)==0x05) {
      dispsize=4;                      // Immediate address without base
      if (size<7)
        da->error=DAE_CROSS;           // Disp32 outside the memory block
      else {
        da->adrconst=addr=*(ulong *)(cmd+3);
        if (pfixup==NULL) pfixup=cmd+3;
        da->fixupsize+=4;
        if (addr==0) da->zeroconst=1;
        if ((sib & 0x38)!=0x20) {      // Index register present
          da->indexed=1;
          if (type==MRJ) da->jmptable=addr; };
        seg=SEG_DS;
      }; }
    else {                             // Base and, eventually, displacement
      if ((c & 0xC0)==0x40) {          // 8-bit displacement
        dispsize=1;
        if (size<4) da->error=DAE_CROSS;
        else {
          da->adrconst=addr=(signed char)cmd[3];
          if (addr==0) da->zeroconst=1;
        }; }
      else if ((c & 0xC0)==0x80) {     // 32-bit displacement
        dispsize=4;
        if (size<7)
          da->error=DAE_CROSS;         // Disp32 outside the memory block
        else {
          da->adrconst=addr=*(ulong *)(cmd+3);
          if (pfixup==NULL) pfixup=cmd+3;
          da->fixupsize+=4;
          if (addr==0) da->zeroconst=1;
          // Most compilers use address of type [index*4+displacement] to
          // address jump table (switch). But, for completeness, I allow all
          // cases which include index with scale 1 or 4, base or both.
          if (type==MRJ) da->jmptable=addr;
        }; };
      da->indexed=1;
      j=sib & 0x07;
      if (mode>=DISASM_FILE) {
        strcpy( (char*)s, (char*)regname[2][j]);
        seg=addr32[j].defseg;
      };
    };
    if ((sib & 0x38)!=0x20) {          // Scaled index present
      if ((sib & 0xC0)==0x40) da->indexed=2;
      else if ((sib & 0xC0)==0x80) da->indexed=4;
      else if ((sib & 0xC0)==0xC0) da->indexed=8;
      else da->indexed=1;
    };
    if (mode>=DISASM_FILE && da->error==DAE_NOERR) {
      if ((sib & 0x38)!=0x20) {        // Scaled index present
        if (*s!='\0') strcat( (char*)s,"+");
        strcat( (char*)s, (char*)addr32[(sib>>3) & 0x07].descr);
        if ((sib & 0xC0)==0x40) {
          da->jmptable=0;              // Hardly a switch!
          strcat( (char*)s,"*2"); }
        else if ((sib & 0xC0)==0x80)
          strcat( (char*)s,"*4");
        else if ((sib & 0xC0)==0xC0) {
          da->jmptable=0;              // Hardly a switch!
          strcat( (char*)s,"*8");
        };
      };
      Memadr(seg,s,addr,dsize);
    };
  }
  // Last possibility: 32-bit address without SIB byte.
  else {                               // No SIB
    if ((c & 0xC0)==0x40) {
      dispsize=1;
      if (size<3) da->error=DAE_CROSS; // Disp8 outside the memory block
      else {
        da->adrconst=addr=(signed char)cmd[2];
        if (addr==0) da->zeroconst=1;
      }; }
    else if ((c & 0xC0)==0x80) {
      dispsize=4;
      if (size<6)
        da->error=DAE_CROSS;           // Disp32 outside the memory block
      else {
        da->adrconst=addr=*(ulong *)(cmd+2);
        if (pfixup==NULL) pfixup=cmd+2;
        da->fixupsize+=4;
        if (addr==0) da->zeroconst=1;
        if (type==MRJ) da->jmptable=addr;
      };
    };
    da->indexed=1;
    if (mode>=DISASM_FILE && da->error==DAE_NOERR) {
      seg=addr32[c & 0x07].defseg;
      Memadr(seg,addr32[c & 0x07].descr,addr,dsize);
    };
  };
};

// Disassemble implicit source of string operations and, if available, dump
// address and contents.
static void DecodeSO(void) {
  if (mode<DISASM_FILE) return;        // No need to decode
  if (datasize==1) da->memtype=DEC_BYTE;
  else if (datasize==2) da->memtype=DEC_WORD;
  else if (datasize==4) da->memtype=DEC_DWORD;
  da->indexed=1;
  Memadr(SEG_DS,regname[addrsize==2?1:2][REG_ESI],0L,datasize);
};

// Disassemble implicit destination of string operations and, if available,
// dump address and contents. Destination always uses segment ES, and this
// setting cannot be overridden.
static void DecodeDE(void) {
  int seg;
  if (mode<DISASM_FILE) return;        // No need to decode
  if (datasize==1) da->memtype=DEC_BYTE;
  else if (datasize==2) da->memtype=DEC_WORD;
  else if (datasize==4) da->memtype=DEC_DWORD;
  da->indexed=1;
  seg=segprefix; segprefix=SEG_ES;     // Fake Memadr by changing segment prefix
  Memadr(SEG_DS,regname[addrsize==2?1:2][REG_EDI],0L,datasize);
  segprefix=seg;                       // Restore segment prefix
};

// Decode XLAT operand and, if available, dump address and contents.
static void DecodeXL(void) {
  if (mode<DISASM_FILE) return;        // No need to decode
  da->memtype=DEC_BYTE;
  da->indexed=1;
  Memadr(SEG_DS,(addrsize==2?"BX+AL":"EBX+AL"),0L,1);
};

// Decode immediate operand of size constsize. If sxt is non-zero, byte operand
// should be sign-extended to sxt bytes. If type of immediate constant assumes
// this, small negative operands may be displayed as signed negative numbers.
// Note that in most cases immediate operands are not shown in comment window.
static void DecodeIM(int constsize,int sxt,int type) {
  int i;
  signed long data;
  ulong l;
  unsigned char name[TEXTLEN]={0}, comment[TEXTLEN]={0};
  immsize+=constsize;                    // Allows several immediate operands
  if (mode<DISASM_DATA) return;
  l=1+hasrm+hassib+dispsize+(immsize-constsize);
  data=0;
  if (size<l+constsize)
    da->error=DAE_CROSS;
  else if (constsize==1) {
    if (sxt==0) data=(uchar)cmd[l];
    else data=(signed char)cmd[l];
    if (type==IMS && ((data & 0xE0)!=0 || data==0)) {
      da->warnings|=DAW_SHIFT;
      da->cmdtype|=C_RARE;
    }; }
  else if (constsize==2) {
    if (sxt==0) data=*(ushort *)(cmd+l);
    else data=*(short *)(cmd+l); }
  else {
    data=*(long *)(cmd+l);
    if (pfixup==NULL) pfixup=cmd+l;
    da->fixupsize+=4; };
  if (sxt==2) data&=0x0000FFFF;
  if (data==0 && da->error==0) da->zeroconst=1;
  // Command ENTER, as an exception from Intel's rules, has two immediate
  // constants. As the second constant is rarely used, I exclude it from
  // search if the first constant is non-zero (which is usually the case).
  if (da->immconst==0)
    da->immconst=data;
  if (mode>=DISASM_FILE && da->error==DAE_NOERR) {
    if (mode>=DISASM_CODE && type!=IMU)
      i=Decodeaddress(data,name,TEXTLEN-nresult-24,comment);
    else {
      i=0; comment[0]='\0'; };
    if (i!=0 && symbolic!=0) {
      strcpy( (char*)da->result+nresult, (char*)name); nresult+=i; }
    else if (type==IMU || type==IMS || type==IM2 || data>=0 || data<NEGLIMIT)
      nresult+=sprintf( (char*)da->result+nresult,"%lX",data);
    else
      nresult+=sprintf( (char*)da->result+nresult,"-%lX",-data);
    if (addcomment && comment[0]!='\0') strcpy( (char*)da->comment, (char*)comment);
  };
};

// Decode VxD service name (always 4-byte).
static void DecodeVX(void) {
  ulong l,data;
  immsize+=4;                          // Allows several immediate operands
  if (mode<DISASM_DATA) return;
  l=1+hasrm+hassib+dispsize+(immsize-4);
  if (size<l+4) {
    da->error=DAE_CROSS;
    return; };
  data=*(long *)(cmd+l);
  if (data==0 && da->error==0) da->zeroconst=1;
  if (da->immconst==0)
    da->immconst=data;
  if (mode>=DISASM_FILE && da->error==DAE_NOERR) {
    if ((data & 0x00008000)!=0 && _memicmp("VxDCall",da->result,7)==0)
      memcpy(da->result,lowercase?"vxdjump":"VxDJump",7);
    nresult+=sprintf( (char*)da->result+nresult,"%lX",data);
  };
};

// Decode implicit constant 1 (used in shift commands). This operand is so
// insignificant that it is never shown in comment window.
static void DecodeC1(void) {
  if (mode<DISASM_DATA) return;
  da->immconst=1;
  if (mode>=DISASM_FILE) nresult+=sprintf( (char*)da->result+nresult,"1");
};

// Decode immediate absolute data address. This operand is used in 8080-
// compatible commands which allow to move data from memory to accumulator and
// back. Note that bytes ModRM and SIB never appear in commands with IA operand.
static void DecodeIA(void) {
  ulong addr;
  if (size<1+addrsize) {
    da->error=DAE_CROSS; return; };
  dispsize=addrsize;
  if (mode<DISASM_DATA) return;
  if (datasize==1) da->memtype=DEC_BYTE;
  else if (datasize==2) da->memtype=DEC_WORD;
  else if (datasize==4) da->memtype=DEC_DWORD;
  if (addrsize==2)
    addr=*(ushort *)(cmd+1);
  else {
    addr=*(ulong *)(cmd+1);
    if (pfixup==NULL) pfixup=cmd+1;
    da->fixupsize+=4; };
  da->adrconst=addr;
  if (addr==0) da->zeroconst=1;
  if (mode>=DISASM_FILE) {
    Memadr(SEG_DS,"",addr,datasize);
  };
};

// Decodes jump relative to nextip of size offsize.
static void DecodeRJ(ulong offsize,ulong nextip) {
  int i;
  ulong addr;
  unsigned char s[TEXTLEN]={0};
  if (size<offsize+1) {
    da->error=DAE_CROSS; return; };
  dispsize=offsize;                    // Interpret offset as displacement
  if (mode<DISASM_DATA) return;
  if (offsize==1)
    addr=(signed char)cmd[1]+nextip;
  else if (offsize==2)
    addr=*(signed short *)(cmd+1)+nextip;
  else
    addr=*(ulong *)(cmd+1)+nextip;
  if (datasize==2)
    addr&=0xFFFF;
  da->jmpconst=addr;
  if (addr==0) da->zeroconst=1;
  if (mode>=DISASM_FILE) {
    if (offsize==1) nresult+=sprintf( (char*)da->result+nresult,
      "%s ",(lowercase==0?"SHORT":"short"));
    if (mode>=DISASM_CODE)
      i=Decodeaddress(addr,s,TEXTLEN,da->comment);
    else
      i=0;
    if (symbolic==0 || i==0)
      nresult+=sprintf( (char*)da->result+nresult,"%08lX",addr);
    else
      nresult+=sprintf( (char*)da->result+nresult,"%.*s",TEXTLEN-nresult-25,s);
    if (symbolic==0 && i!=0 && da->comment[0]=='\0')
      strcpy( (char*)da->comment, (char*)s);
    ;
  };
};

// Decode immediate absolute far jump address. In flat model, such addresses
// are not used (mostly because selector is specified directly in the command),
// so I neither decode as symbol nor comment it. To allow search for selector
// by value, I interprete it as an immediate constant.
static void DecodeJF(void) {
  ulong addr,seg;
  if (size<1+addrsize+2) {
    da->error=DAE_CROSS; return; };
  dispsize=addrsize; immsize=2;        // Non-trivial but allowed interpretation
  if (mode<DISASM_DATA) return;
  if (addrsize==2) {
    addr=*(ushort *)(cmd+1);
    seg=*(ushort *)(cmd+3); }
  else {
    addr=*(ulong *)(cmd+1);
    seg=*(ushort *)(cmd+5); };
  da->jmpconst=addr;
  da->immconst=seg;
  if (addr==0 || seg==0) da->zeroconst=1;
  if (mode>=DISASM_FILE) {
    nresult+=sprintf( (char*)da->result+nresult,"%s %04X:%08X",
    (lowercase==0?"FAR":"far"),seg,addr);
  };
};

// Decode segment register. In flat model, operands of this type are seldom.
static void DecodeSG(int index) {
  int i;
  if (mode<DISASM_DATA) return;
  index&=0x07;
  if (index>=6) softerror=DAE_BADSEG;  // Undefined segment register
  if (mode>=DISASM_FILE) {
    i=sprintf( (char*)da->result+nresult,"%s",segname[index]);
    if (lowercase) _strlwr( (char*)da->result+nresult);
    nresult+=i;
  };
};

// Decode control register addressed in R part of ModRM byte. Operands of
// this type are extremely rare. Contents of control registers are accessible
// only from privilege level 0, so I cannot dump them here.
static void DecodeCR(int index) {
  hasrm=1;
  if (mode>=DISASM_FILE) {
    index=(index>>3) & 0x07;
    nresult+=sprintf( (char*)da->result+nresult,"%s",crname[index]);
    if (lowercase) _strlwr( (char*)da->result+nresult);
  };
};

// Decode debug register addressed in R part of ModRM byte. Operands of
// this type are extremely rare. I can dump only those debug registers
// available in CONTEXT structure.
static void DecodeDR(int index) {
  int i;
  hasrm=1;
  if (mode>=DISASM_FILE) {
    index=(index>>3) & 0x07;
    i=sprintf( (char*)da->result+nresult,"%s",drname[index]);
    if (lowercase) _strlwr( (char*)da->result+nresult);
    nresult+=i;
  };
};

// Skips 3DNow! operands and extracts command suffix. Returns suffix or -1 if
// suffix lies outside the memory block. This subroutine assumes that cmd still
// points to the beginning of 3DNow! command (i.e. to the sequence of two bytes
// 0F, 0F).
static int Get3dnowsuffix(void) {
  int c,sib;
  ulong offset;
  if (size<3) return -1;               // Suffix outside the memory block
  offset=3;
  c=cmd[2] & 0xC7;                     // Leave only Mod and M fields
  // Register in ModM - general-purpose, MMX or 3DNow!
  if ((c & 0xC0)==0xC0)
    ;
  // 16-bit addressing mode, SIB byte is never used here.
  else if (addrsize==2) {
    if (c==0x06)                       // Special case of immediate address
      offset+=2;
    else if ((c & 0xC0)==0x40)         // 8-bit signed displacement
      offset++;
    else if ((c & 0xC0)==0x80)         // 16-bit unsigned displacement
      offset+=2;
    ; }
  // Immediate 32-bit address.
  else if (c==0x05)                    // Special case of immediate address
    offset+=4;
  // 32-bit address with SIB byte.
  else if ((c & 0x07)==0x04) {         // SIB addresation
    if (size<4) return -1;             // Suffix outside the memory block
    sib=cmd[3]; offset++;
    if (c==0x04 && (sib & 0x07)==0x05)
      offset+=4;                       // Immediate address without base
    else if ((c & 0xC0)==0x40)         // 8-bit displacement
      offset+=1;
    else if ((c & 0xC0)==0x80)         // 32-bit dislacement
      offset+=4;
    ; }
  // 32-bit address without SIB byte
  else if ((c & 0xC0)==0x40)
    offset+=1;
  else if ((c & 0xC0)==0x80)
    offset+=4;
  if (offset>=size) return -1;         // Suffix outside the memory block
  return cmd[offset];
};

// Function checks whether 80x86 flags meet condition set in the command.
// Returns 1 if condition is met, 0 if not and -1 in case of error (which is
// not possible).
int Checkcondition(int code,ulong flags) {
  ulong cond,temp;
  switch (code & 0x0E) {
    case 0:                            // If overflow
      cond=flags & 0x0800; break;
    case 2:                            // If below
      cond=flags & 0x0001; break;
    case 4:                            // If equal
      cond=flags & 0x0040; break;
    case 6:                            // If below or equal
      cond=flags & 0x0041; break;
    case 8:                            // If sign
      cond=flags & 0x0080; break;
    case 10:                           // If parity
      cond=flags & 0x0004; break;
    case 12:                           // If less
      temp=flags & 0x0880;
      cond=(temp==0x0800 || temp==0x0080); break;
    case 14:                           // If less or equal
      temp=flags & 0x0880;
      cond=(temp==0x0800 || temp==0x0080 || (flags & 0x0040)!=0); break;
    default: return -1;                // Internal error, not possible!
  };
  if ((code & 0x01)==0) return (cond!=0);
  else return (cond==0);               // Invert condition
};

ulong Disasm(unsigned char *src,ulong srcsize,ulong srcip,
  t_disasm *disasm,int disasmmode) {
  int i,j,isprefix,is3dnow,repeated,operand,mnemosize,arg;
  ulong u,code;
  int lockprefix;                      // Non-zero if lock prefix present
  int repprefix;                       // REPxxx prefix or 0
  int cxsize;
  unsigned char name[TEXTLEN]={0}, *pname;
  const t_cmddata *pd,*pdan;
  // Prepare disassembler variables and initialize structure disasm.
  datasize=addrsize=4;                 // 32-bit code and data segments only!
  segprefix=SEG_UNDEF;
  hasrm=hassib=0; dispsize=immsize=0;
  lockprefix=0; repprefix=0;
  ndump=0; nresult=0;
  cmd=src; size=srcsize; pfixup=NULL;
  softerror=0; is3dnow=0;
  da=disasm;
  da->ip=srcip;
  da->comment[0]='\0';
  da->cmdtype=C_BAD; da->nprefix=0;
  da->memtype=DEC_UNKNOWN; da->indexed=0;
  da->jmpconst=0; da->jmptable=0;
  da->adrconst=0; da->immconst=0;
  da->zeroconst=0;
  da->fixupoffset=0; da->fixupsize=0;
  da->warnings=0;
  da->error=DAE_NOERR;
  mode=disasmmode;                     // No need to use register contents
  // Correct 80x86 command may theoretically contain up to 4 prefixes belonging
  // to different prefix groups. This limits maximal possible size of the
  // command to MAXCMDSIZE=16 bytes. In order to maintain this limit, if
  // Disasm() detects second prefix from the same group, it flushes first
  // prefix in the sequence as a pseudocommand.
  u=0; repeated=0;
  while (size>0) {
    isprefix=1;                        // Assume that there is some prefix
    switch (*cmd) {
      case 0x26: if (segprefix==SEG_UNDEF) segprefix=SEG_ES;
        else repeated=1; break;
      case 0x2E: if (segprefix==SEG_UNDEF) segprefix=SEG_CS;
        else repeated=1; break;
      case 0x36: if (segprefix==SEG_UNDEF) segprefix=SEG_SS;
        else repeated=1; break;
      case 0x3E: if (segprefix==SEG_UNDEF) segprefix=SEG_DS;
        else repeated=1; break;
      case 0x64: if (segprefix==SEG_UNDEF) segprefix=SEG_FS;
        else repeated=1; break;
      case 0x65: if (segprefix==SEG_UNDEF) segprefix=SEG_GS;
        else repeated=1; break;
      case 0x66: if (datasize==4) datasize=2;
        else repeated=1; break;
      case 0x67: if (addrsize==4) addrsize=2;
        else repeated=1; break;
      case 0xF0: if (lockprefix==0) lockprefix=0xF0;
        else repeated=1; break;
      case 0xF2: if (repprefix==0) repprefix=0xF2;
        else repeated=1; break;
      case 0xF3: if (repprefix==0) repprefix=0xF3;
        else repeated=1; break;
      default: isprefix=0; break; };
    if (isprefix==0 || repeated!=0)
      break;                           // No more prefixes or duplicated prefix
    if (mode>=DISASM_FILE)
      ndump+=sprintf( (char*)da->dump+ndump,"%02X:",*cmd);
    da->nprefix++;
    cmd++; srcip++; size--; u++; };
  // We do have repeated prefix. Flush first prefix from the sequence.
  if (repeated) {
    if (mode>=DISASM_FILE) {
      da->dump[3]='\0';                // Leave only first dumped prefix
      da->nprefix=1;
      switch (cmd[-(long)u]) {
        case 0x26: pname=(unsigned char *)(segname[SEG_ES]); break;
        case 0x2E: pname=(unsigned char *)(segname[SEG_CS]); break;
        case 0x36: pname=(unsigned char *)(segname[SEG_SS]); break;
        case 0x3E: pname=(unsigned char *)(segname[SEG_DS]); break;
        case 0x64: pname=(unsigned char *)(segname[SEG_FS]); break;
        case 0x65: pname=(unsigned char *)(segname[SEG_GS]); break;
        case 0x66: pname= (unsigned char*)"DATASIZE"; break;
        case 0x67: pname= (unsigned char*)"ADDRSIZE"; break;
        case 0xF0: pname= (unsigned char*)"LOCK"; break;
        case 0xF2: pname= (unsigned char*)"REPNE"; break;
        case 0xF3: pname= (unsigned char*)"REPE"; break;
        default: pname= (unsigned char*)"?"; break; };
      nresult+=sprintf( (char*)da->result+nresult,"PREFIX %s:",pname);
      if (lowercase) _strlwr( (char*)da->result);
      if (extraprefix==0) strcpy( (char*)da->comment,"Superfluous prefix"); };
    da->warnings|=DAW_PREFIX;
    if (lockprefix) da->warnings|=DAW_LOCK;
    da->cmdtype=C_RARE;
    return 1;                          // Any prefix is 1 byte long
  };
  // If lock prefix available, display it and forget, because it has no
  // influence on decoding of rest of the command.
  if (lockprefix!=0) {
    if (mode>=DISASM_FILE) nresult+=sprintf( (char*)da->result+nresult,"LOCK ");
    da->warnings|=DAW_LOCK; };
  // Fetch (if available) first 3 bytes of the command, add repeat prefix and
  // find command in the command table.
  code=0;
  if (size>0) *(((unsigned char *)&code)+0)=cmd[0];
  if (size>1) *(((unsigned char *)&code)+1)=cmd[1];
  if (size>2) *(((unsigned char *)&code)+2)=cmd[2];
  if (repprefix!=0)                    // RER/REPE/REPNE is considered to be
    code=(code<<8) | repprefix;        // part of command.
  if (decodevxd && (code & 0xFFFF)==0x20CD)
    pd=&vxdcmd;                        // Decode VxD call (Win95/98)
  else {
    for (pd=cmddata; pd->mask!=0; pd++) {
      if (((code^pd->code) & pd->mask)!=0) continue;
      if (mode>=DISASM_FILE && shortstringcmds &&
        (pd->arg1==MSO || pd->arg1==MDE || pd->arg2==MSO || pd->arg2==MDE))
        continue;                      // Search short form of string command
      break;
    };
  };
  if ((pd->type & C_TYPEMASK)==C_NOW) {
    // 3DNow! commands require additional search.
    is3dnow=1;
    j=Get3dnowsuffix();
    if (j<0)
      da->error=DAE_CROSS;
    else {
      for ( ; pd->mask!=0; pd++) {
        if (((code^pd->code) & pd->mask)!=0) continue;
        if (((uchar *)&(pd->code))[2]==j) break;
      };
    };
  };
  if (pd->mask==0) {                   // Command not found
    da->cmdtype=C_BAD;
    if (size<2) da->error=DAE_CROSS;
    else da->error=DAE_BADCMD; }
  else {                               // Command recognized, decode it
    da->cmdtype=pd->type;
    cxsize=datasize;                   // Default size of ECX used as counter
    if (segprefix==SEG_FS || segprefix==SEG_GS || lockprefix!=0)
      da->cmdtype|=C_RARE;             // These prefixes are rare
    if (pd->bits==PR)
      da->warnings|=DAW_PRIV;          // Privileged command (ring 0)
    else if (pd->bits==WP)
      da->warnings|=DAW_IO;            // I/O command
    // Win32 programs usually try to keep stack dword-aligned, so INC ESP
    // (44) and DEC ESP (4C) usually don't appear in real code. Also check for
    // ADD ESP,imm and SUB ESP,imm (81,C4,imm32; 83,C4,imm8; 81,EC,imm32;
    // 83,EC,imm8).
    if (cmd[0]==0x44 || cmd[0]==0x4C ||
      (size>=3 && (cmd[0]==0x81 || cmd[0]==0x83) &&
      (cmd[1]==0xC4 || cmd[1]==0xEC) && (cmd[2] & 0x03)!=0)
    ) {
      da->warnings|=DAW_STACK;
      da->cmdtype|=C_RARE; };
    // Warn also on MOV SEG,... (8E...). Win32 works in flat mode.
    if (cmd[0]==0x8E)
      da->warnings|=DAW_SEGMENT;
    // If opcode is 2-byte, adjust command.
    if (pd->len==2) {
      if (size==0) da->error=DAE_CROSS;
      else {
        if (mode>=DISASM_FILE)
          ndump+=sprintf( (char*)da->dump+ndump,"%02X",*cmd);
        cmd++; srcip++; size--;
      }; };
    if (size==0) da->error=DAE_CROSS;
    // Some commands either feature non-standard data size or have bit which
    // allowes to select data size.
    if ((pd->bits & WW)!=0 && (*cmd & WW)==0)
      datasize=1;                      // Bit W in command set to 0
    else if ((pd->bits & W3)!=0 && (*cmd & W3)==0)
      datasize=1;                      // Another position of bit W
    else if ((pd->bits & FF)!=0)
      datasize=2;                      // Forced word (2-byte) size
    // Some commands either have mnemonics which depend on data size (8/16 bits
    // or 32 bits, like CWD/CDQ), or have several different mnemonics (like
    // JNZ/JNE). First case is marked by either '&' (mnemonic depends on
    // operand size) or '$' (depends on address size). In the second case,
    // there is no special marker and disassembler selects main mnemonic.
    if (mode>=DISASM_FILE) {
      if (pd->name[0]=='&') mnemosize=datasize;
      else if (pd->name[0]=='$') mnemosize=addrsize;
      else mnemosize=0;
      if (mnemosize!=0) {
        for (i=0,j=1; pd->name[j]!='\0'; j++) {
          if (pd->name[j]==':') {      // Separator between 16/32 mnemonics
            if (mnemosize==4) i=0;
            else break; }
          else if (pd->name[j]=='*') { // Substitute by 'W', 'D' or none
            if (mnemosize==4 && sizesens!=2) name[i++]='D';
            else if (mnemosize!=4 && sizesens!=0) name[i++]='W'; }
          else name[i++]=pd->name[j];
        };
        name[i]='\0'; }
      else {
        strcpy( (char*)name, (char*)pd->name);
        for (i=0; name[i]!='\0'; i++) {
          if (name[i]==',') {          // Use main mnemonic
            name[i]='\0'; break;
          };
        };
      };
      if (repprefix!=0 && tabarguments) {
        for (i=0; name[i]!='\0' && name[i]!=' '; i++)
          da->result[nresult++]=name[i];
        if (name[i]==' ') {
          da->result[nresult++]=' '; i++; };
        while (nresult<8) da->result[nresult++]=' ';
        for ( ; name[i]!='\0'; i++)
          da->result[nresult++]=name[i];
        ; }
      else
        nresult+=sprintf( (char*)da->result+nresult,"%s",name);
      if (lowercase) _strlwr( (char*)da->result);
    };
    // Decode operands (explicit - encoded in command, implicit - present in
    // mmemonic or assumed - used or modified by command). Assumed operands
    // must stay after all explicit and implicit operands. Up to 3 operands
    // are allowed.
    for (operand=0; operand<3; operand++) {
      if (da->error) break;            // Error - no sense to continue
      // If command contains both source and destination, one usually must not
      // decode destination to comment because it will be overwritten on the
      // next step. Global addcomment takes care of this. Decoding routines,
      // however, may ignore this flag.
      if (operand==0 && pd->arg2!=NNN && pd->arg2<PSEUDOOP)
        addcomment=0;
      else
        addcomment=1;
      // Get type of next argument.
      if (operand==0) arg=pd->arg1;
      else if (operand==1) arg=pd->arg2;
      else arg=pd->arg3;
      if (arg==NNN) break;             // No more operands
      // Arguments with arg>=PSEUDOOP are assumed operands and are not
      // displayed in disassembled result, so they require no delimiter.
      if ((mode>=DISASM_FILE) && arg<PSEUDOOP) {
        if (operand==0) {
          da->result[nresult++]=' ';
          if (tabarguments) {
            while (nresult<8) da->result[nresult++]=' ';
          }; }
        else {
          da->result[nresult++]=',';
          if (extraspace) da->result[nresult++]=' ';
        };
      };
      // Decode, analyse and comment next operand of the command.
      switch (arg) {
        case REG:                      // Integer register in Reg field
          if (size<2) da->error=DAE_CROSS;
          else DecodeRG(cmd[1]>>3,datasize,REG);
          hasrm=1; break;
        case RCM:                      // Integer register in command byte
          DecodeRG(cmd[0],datasize,RCM); break;
        case RG4:                      // Integer 4-byte register in Reg field
          if (size<2) da->error=DAE_CROSS;
          else DecodeRG(cmd[1]>>3,4,RG4);
          hasrm=1; break;
        case RAC:                      // Accumulator (AL/AX/EAX, implicit)
          DecodeRG(REG_EAX,datasize,RAC); break;
        case RAX:                      // AX (2-byte, implicit)
          DecodeRG(REG_EAX,2,RAX); break;
        case RDX:                      // DX (16-bit implicit port address)
          DecodeRG(REG_EDX,2,RDX); break;
        case RCL:                      // Implicit CL register (for shifts)
          DecodeRG(REG_ECX,1,RCL); break;
        case RS0:                      // Top of FPU stack (ST(0))
          DecodeST(0,0); break;
        case RST:                      // FPU register (ST(i)) in command byte
          DecodeST(cmd[0],0); break;
        case RMX:                      // MMX register MMx
          if (size<2) da->error=DAE_CROSS;
          else DecodeMX(cmd[1]>>3);
          hasrm=1; break;
        case R3D:                      // 3DNow! register MMx
          if (size<2) da->error=DAE_CROSS;
          else DecodeNR(cmd[1]>>3);
          hasrm=1; break;
        case MRG:                      // Memory/register in ModRM byte
        case MRJ:                      // Memory/reg in ModRM as JUMP target
        case MR1:                      // 1-byte memory/register in ModRM byte
        case MR2:                      // 2-byte memory/register in ModRM byte
        case MR4:                      // 4-byte memory/register in ModRM byte
        case MR8:                      // 8-byte memory/MMX register in ModRM
        case MRD:                      // 8-byte memory/3DNow! register in ModRM
        case MMA:                      // Memory address in ModRM byte for LEA
        case MML:                      // Memory in ModRM byte (for LES)
        case MM6:                      // Memory in ModRm (6-byte descriptor)
        case MMB:                      // Two adjacent memory locations (BOUND)
        case MD2:                      // Memory in ModRM byte (16-bit integer)
        case MB2:                      // Memory in ModRM byte (16-bit binary)
        case MD4:                      // Memory in ModRM byte (32-bit integer)
        case MD8:                      // Memory in ModRM byte (64-bit integer)
        case MDA:                      // Memory in ModRM byte (80-bit BCD)
        case MF4:                      // Memory in ModRM byte (32-bit float)
        case MF8:                      // Memory in ModRM byte (64-bit float)
        case MFA:                      // Memory in ModRM byte (80-bit float)
        case MFE:                      // Memory in ModRM byte (FPU environment)
        case MFS:                      // Memory in ModRM byte (FPU state)
        case MFX:                      // Memory in ModRM byte (ext. FPU state)
          DecodeMR(arg); break;
        case MMS:                      // Memory in ModRM byte (as SEG:OFFS)
          DecodeMR(arg);
          da->warnings|=DAW_FARADDR; break;
        case RR4:                      // 4-byte memory/register (register only)
        case RR8:                      // 8-byte MMX register only in ModRM
        case RRD:                      // 8-byte memory/3DNow! (register only)
          if ((cmd[1] & 0xC0)!=0xC0) softerror=DAE_REGISTER;
          DecodeMR(arg); break;
        case MSO:                      // Source in string op's ([ESI])
          DecodeSO(); break;
        case MDE:                      // Destination in string op's ([EDI])
          DecodeDE(); break;
        case MXL:                      // XLAT operand ([EBX+AL])
          DecodeXL(); break;
        case IMM:                      // Immediate data (8 or 16/32)
        case IMU:                      // Immediate unsigned data (8 or 16/32)
          if ((pd->bits & SS)!=0 && (*cmd & 0x02)!=0)
            DecodeIM(1,datasize,arg);
          else
            DecodeIM(datasize,0,arg);
          break;
        case VXD:                      // VxD service (32-bit only)
          DecodeVX(); break;
        case IMX:                      // Immediate sign-extendable byte
          DecodeIM(1,datasize,arg); break;
        case C01:                      // Implicit constant 1 (for shifts)
          DecodeC1(); break;
        case IMS:                      // Immediate byte (for shifts)
        case IM1:                      // Immediate byte
          DecodeIM(1,0,arg); break;
        case IM2:                      // Immediate word (ENTER/RET)
          DecodeIM(2,0,arg);
          if ((da->immconst & 0x03)!=0) da->warnings|=DAW_STACK;
          break;
        case IMA:                      // Immediate absolute near data address
          DecodeIA(); break;
        case JOB:                      // Immediate byte offset (for jumps)
          DecodeRJ(1,srcip+2); break;
        case JOW:                      // Immediate full offset (for jumps)
          DecodeRJ(datasize,srcip+datasize+1); break;
        case JMF:                      // Immediate absolute far jump/call addr
          DecodeJF();
          da->warnings|=DAW_FARADDR; break;
        case SGM:                      // Segment register in ModRM byte
          if (size<2) da->error=DAE_CROSS;
          DecodeSG(cmd[1]>>3); hasrm=1; break;
        case SCM:                      // Segment register in command byte
          DecodeSG(cmd[0]>>3);
          if ((da->cmdtype & C_TYPEMASK)==C_POP) da->warnings|=DAW_SEGMENT;
          break;
        case CRX:                      // Control register CRx
          if ((cmd[1] & 0xC0)!=0xC0) da->error=DAE_REGISTER;
          DecodeCR(cmd[1]); break;
        case DRX:                      // Debug register DRx
          if ((cmd[1] & 0xC0)!=0xC0) da->error=DAE_REGISTER;
          DecodeDR(cmd[1]); break;
        case PRN:                      // Near return address (pseudooperand)
          break;
        case PRF:                      // Far return address (pseudooperand)
          da->warnings|=DAW_FARADDR; break;
        case PAC:                      // Accumulator (AL/AX/EAX, pseudooperand)
          DecodeRG(REG_EAX,datasize,PAC); break;
        case PAH:                      // AH (in LAHF/SAHF, pseudooperand)
        case PFL:                      // Lower byte of flags (pseudooperand)
          break;
        case PS0:                      // Top of FPU stack (pseudooperand)
          DecodeST(0,1); break;
        case PS1:                      // ST(1) (pseudooperand)
          DecodeST(1,1); break;
        case PCX:                      // CX/ECX (pseudooperand)
          DecodeRG(REG_ECX,cxsize,PCX); break;
        case PDI:                      // EDI (pseudooperand in MMX extentions)
          DecodeRG(REG_EDI,4,PDI); break;
        default:
          da->error=DAE_INTERN;        // Unknown argument type
        break;
      };
    };
    // Check whether command may possibly contain fixups.
    if (pfixup!=NULL && da->fixupsize>0)
      da->fixupoffset=pfixup-src;
    // Segment prefix and address size prefix are superfluous for command which
    // does not access memory. If this the case, mark command as rare to help
    // in analysis.
    if (da->memtype==DEC_UNKNOWN &&
      (segprefix!=SEG_UNDEF || (addrsize!=4 && pd->name[0]!='$'))
    ) {
      da->warnings|=DAW_PREFIX;
      da->cmdtype|=C_RARE; };
    // 16-bit addressing is rare in 32-bit programs. If this is the case,
    // mark command as rare to help in analysis.
    if (addrsize!=4) da->cmdtype|=C_RARE;
  };
  // Suffix of 3DNow! command is accounted best by assuming it immediate byte
  // constant.
  if (is3dnow) {
    if (immsize!=0) da->error=DAE_BADCMD;
    else immsize=1; };
  // Right or wrong, command decoded. Now dump it.
  if (da->error!=0) {                  // Hard error in command detected
    if (mode>=DISASM_FILE)
      nresult=sprintf( (char*)da->result,"???");
    if (da->error==DAE_BADCMD &&
      (*cmd==0x0F || *cmd==0xFF) && size>0
    ) {
      if (mode>=DISASM_FILE) ndump+=sprintf( (char*)da->dump+ndump,"%02X",*cmd);
      cmd++; size--; };
    if (size>0) {
      if (mode>=DISASM_FILE) ndump+=sprintf( (char*)da->dump+ndump,"%02X",*cmd);
      cmd++; size--;
    }; }
  else {                               // No hard error, dump command
    if (mode>=DISASM_FILE) {
      ndump+=sprintf( (char*)da->dump+ndump,"%02X",*cmd++);
      if (hasrm) ndump+=sprintf( (char*)da->dump+ndump,"%02X",*cmd++);
      if (hassib) ndump+=sprintf( (char*)da->dump+ndump,"%02X",*cmd++);
      if (dispsize!=0) {
        da->dump[ndump++]=' ';
        for (i=0; i<dispsize; i++) {
          ndump+=sprintf( (char*)da->dump+ndump,"%02X",*cmd++);
        };
      };
      if (immsize!=0) {
        da->dump[ndump++]=' ';
        for (i=0; i<immsize; i++) {
          ndump+=sprintf( (char*)da->dump+ndump,"%02X",*cmd++);
        };
      };
    }
    else
      cmd+=1+hasrm+hassib+dispsize+immsize;
    size-=1+hasrm+hassib+dispsize+immsize;
  };
  // Check that command is not a dangerous one.
  if (mode>=DISASM_DATA) {
    for (pdan=dangerous; pdan->mask!=0; pdan++) {
      if (((code^pdan->code) & pdan->mask)!=0)
        continue;
      if (pdan->type==C_DANGERLOCK && lockprefix==0)
        break;                         // Command harmless without LOCK prefix
      if (iswindowsnt && pdan->type==C_DANGER95)
        break;                         // Command harmless under Windows NT
      // Dangerous command!
      if (pdan->type==C_DANGER95) da->warnings|=DAW_DANGER95;
      else da->warnings|=DAW_DANGEROUS;
      break;
    };
  };
  if (da->error==0 && softerror!=0)
    da->error=softerror;               // Error, but still display command
  if (mode>=DISASM_FILE) {
    if (da->error!=DAE_NOERR) switch (da->error) {
      case DAE_CROSS:
        strcpy( (char*)da->comment,"Command crosses end of memory block"); break;
      case DAE_BADCMD:
        strcpy( (char*)da->comment,"Unknown command"); break;
      case DAE_BADSEG:
        strcpy( (char*)da->comment,"Undefined segment register"); break;
      case DAE_MEMORY:
        strcpy( (char*)da->comment,"Illegal use of register"); break;
      case DAE_REGISTER:
        strcpy( (char*)da->comment,"Memory address not allowed"); break;
      case DAE_INTERN:
        strcpy( (char*)da->comment,"Internal OLLYDBG error"); break;
      default:
        strcpy( (char*)da->comment,"Unknown error");
      break; }
    else if ((da->warnings & DAW_PRIV)!=0 && privileged==0)
      strcpy( (char*)da->comment,"Privileged command");
    else if ((da->warnings & DAW_IO)!=0 && iocommand==0)
      strcpy( (char*)da->comment,"I/O command");
    else if ((da->warnings & DAW_FARADDR)!=0 && farcalls==0) {
      if ((da->cmdtype & C_TYPEMASK)==C_JMP)
        strcpy( (char*)da->comment,"Far jump");
      else if ((da->cmdtype & C_TYPEMASK)==C_CAL)
        strcpy( (char*)da->comment,"Far call");
      else if ((da->cmdtype & C_TYPEMASK)==C_RET)
        strcpy( (char*)da->comment,"Far return");
      ; }
    else if ((da->warnings & DAW_SEGMENT)!=0 && farcalls==0)
      strcpy( (char*)da->comment,"Modification of segment register");
    else if ((da->warnings & DAW_SHIFT)!=0 && badshift==0)
      strcpy( (char*)da->comment,"Shift constant out of range 1..31");
    else if ((da->warnings & DAW_PREFIX)!=0 && extraprefix==0)
      strcpy( (char*)da->comment,"Superfluous prefix");
    else if ((da->warnings & DAW_LOCK)!=0 && lockedbus==0)
      strcpy( (char*)da->comment,"LOCK prefix");
    else if ((da->warnings & DAW_STACK)!=0 && stackalign==0)
      strcpy( (char*)da->comment,"Unaligned stack operation");
    ;
  };
  return (srcsize-size);               // Returns number of recognized bytes
};

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// FUNCTIONS FOR EXPORT /////////////////////////////

int AssembleInstruction(char *asmInstruction, unsigned instructionAddress, unsigned attempt, unsigned constSize, char *assembledHexString, char *errorString) {
	t_asmmodel am;
	int i, n = 0;
	int length;
	length = Assemble( (unsigned char*)asmInstruction, instructionAddress, &am, attempt, constSize, (unsigned char*)errorString);
	for (i = 0; i < (int)length; i++) n += sprintf(assembledHexString + n, "%02X ", am.code[i]);
	return length;
}

int DisassembleInstruction(unsigned char *source, unsigned sourceSize, unsigned startIndex, unsigned instructionAddress, int disassembleMode, char *disassembledString) {	
	t_disasm da;
	int length;
	if (startIndex >= sourceSize) return 0;
	lowercase = 1;
	tabarguments = 1;
	extraspace = 1;
	length = Disasm(source+startIndex, sourceSize-startIndex, instructionAddress, &da, disassembleMode);
	strcpy_s(disassembledString, TEXTLEN, (char*)da.result);
	return length;
}
