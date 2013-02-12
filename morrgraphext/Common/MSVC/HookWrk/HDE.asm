;-----------------------------------------------------------------------------------
;
;  Hacker Disassembler Engine version 0.02
;  Copyright (c) 2006 Vjacheslav Patkov. All rights reserved.
;
;  module:    HDE.INC
;  purpose:   main module with main procedures
;  compiler:  masm32 v8.2
;  version:   10/07/2006
;
;-----------------------------------------------------------------------------------

.686P
.MMX
.MODEL FLAT, stdcall
;-----------------------------------------------------------------------------------
;   prototypes
;-----------------------------------------------------------------------------------
PUBLIC hde_disasm
;hde_disasm           PROTO STDCALL :DWORD, :DWORD

;-----------------------------------------------------------------------------------
;   data
;-----------------------------------------------------------------------------------
.data
;-----------------------------------------------------------------------------------
;   structures
;-----------------------------------------------------------------------------------
HDE_STRUCT STRUCT
      len            DB ?      ; length of command
      p_rep          DB ?      ; rep/repnz/.. prefix: 0xF2 or 0xF3
      p_lock         DB ?      ; lock prefix 0xF0
      p_seg          DB ?      ; segment prefix: 0x2E, 0x36, 0x3E, 0x26, 0x64, 0x65
      p_66           DB ?      ; prefix 0x66
      p_67           DB ?      ; prefix 0x67
      opcode         DB ?      ; opcode
      opcode2        DB ?      ; second opcode, if first opcode equal 0x0F
      modrm          DB ?      ; ModR/M byte
      modrm_mod      DB ?      ;   - mod byte of ModR/M
      modrm_reg      DB ?      ;   - reg byte of ModR/M
      modrm_rm       DB ?      ;   - r/m byte of ModR/M
      sib            DB ?      ; SIB byte
      sib_scale      DB ?      ;   - scale (ss) byte of SIB
      sib_index      DB ?      ;   - index byte of SIB
      sib_base       DB ?      ;   - base byte of SIB
      imm8           DB ?      ; immediate imm8
      imm16          DW ?      ; immediate imm16
      imm32          DD ?      ; immediate imm32
      disp8          DB ?      ; displacement disp8
      disp16         DW ?      ; displacement disp16, if prefix 0x67 exist
      disp32         DD ?      ; displacement disp32
      rel8           DB ?      ; relative address rel8
      rel16          DW ?      ; relative address rel16, if prefix 0x66 exist
      rel32          DD ?      ; relative address rel32
      immpresent     DB ?
      dispresent     DB ?
      relpresent     DB ?
HDE_STRUCT ENDS
;-----------------------------------------------------------------------------------
;   definitions
;-----------------------------------------------------------------------------------
C_NONE           EQU 00h
C_MODRM          EQU 01h
C_DATA8          EQU 02h
C_DATA16         EQU 04h
C_DATA32         EQU 08h
C_PRE66          EQU 10h
C_WORD           EQU 20h
C_REL8           EQU 40h
C_REL32          EQU 80h
;---------------------------------------------------------------------------
HDE_TABLE  DB C_MODRM                       ; 00
  DB C_MODRM                       ; 01
  DB C_MODRM                       ; 02
  DB C_MODRM                       ; 03
  DB C_DATA8                       ; 04
  DB C_PRE66                       ; 05
  DB C_NONE                        ; 06
  DB C_NONE                        ; 07
  DB C_MODRM                       ; 08
  DB C_MODRM                       ; 09
  DB C_MODRM                       ; 0A
  DB C_MODRM                       ; 0B
  DB C_DATA8                       ; 0C
  DB C_PRE66                       ; 0D
  DB C_NONE                        ; 0E
  DB C_NONE                        ; 0F
  DB C_MODRM                       ; 10
  DB C_MODRM                       ; 11
  DB C_MODRM                       ; 12
  DB C_MODRM                       ; 13
  DB C_DATA8                       ; 14
  DB C_PRE66                       ; 15
  DB C_NONE                        ; 16
  DB C_NONE                        ; 17
  DB C_MODRM                       ; 18
  DB C_MODRM                       ; 19
  DB C_MODRM                       ; 1A
  DB C_MODRM                       ; 1B
  DB C_DATA8                       ; 1C
  DB C_PRE66                       ; 1D
  DB C_NONE                        ; 1E
  DB C_NONE                        ; 1F
  DB C_MODRM                       ; 20
  DB C_MODRM                       ; 21
  DB C_MODRM                       ; 22
  DB C_MODRM                       ; 23
  DB C_DATA8                       ; 24
  DB C_PRE66                       ; 25
  DB C_NONE                        ; 26
  DB C_NONE                        ; 27
  DB C_MODRM                       ; 28
  DB C_MODRM                       ; 29
  DB C_MODRM                       ; 2A
  DB C_MODRM                       ; 2B
  DB C_DATA8                       ; 2C
  DB C_PRE66                       ; 2D
  DB C_NONE                        ; 2E
  DB C_NONE                        ; 2F
  DB C_MODRM                       ; 30
  DB C_MODRM                       ; 31
  DB C_MODRM                       ; 32
  DB C_MODRM                       ; 33
  DB C_DATA8                       ; 34
  DB C_PRE66                       ; 35
  DB C_NONE                        ; 36
  DB C_NONE                        ; 37
  DB C_MODRM                       ; 38
  DB C_MODRM                       ; 39
  DB C_MODRM                       ; 3A
  DB C_MODRM                       ; 3B
  DB C_DATA8                       ; 3C
  DB C_PRE66                       ; 3D
  DB C_NONE                        ; 3E
  DB C_NONE                        ; 3F
  DB C_NONE                        ; 40
  DB C_NONE                        ; 41
  DB C_NONE                        ; 42
  DB C_NONE                        ; 43
  DB C_NONE                        ; 44
  DB C_NONE                        ; 45
  DB C_NONE                        ; 46
  DB C_NONE                        ; 47
  DB C_NONE                        ; 48
  DB C_NONE                        ; 49
  DB C_NONE                        ; 4A
  DB C_NONE                        ; 4B
  DB C_NONE                        ; 4C
  DB C_NONE                        ; 4D
  DB C_NONE                        ; 4E
  DB C_NONE                        ; 4F
  DB C_NONE                        ; 50
  DB C_NONE                        ; 51
  DB C_NONE                        ; 52
  DB C_NONE                        ; 53
  DB C_NONE                        ; 54
  DB C_NONE                        ; 55
  DB C_NONE                        ; 56
  DB C_NONE                        ; 57
  DB C_NONE                        ; 58
  DB C_NONE                        ; 59
  DB C_NONE                        ; 5A
  DB C_NONE                        ; 5B
  DB C_NONE                        ; 5C
  DB C_NONE                        ; 5D
  DB C_NONE                        ; 5E
  DB C_NONE                        ; 5F
  DB C_NONE                        ; 60
  DB C_NONE                        ; 61
  DB C_MODRM                       ; 62
  DB C_MODRM                       ; 63
  DB C_NONE                        ; 64
  DB C_NONE                        ; 65
  DB C_NONE                        ; 66
  DB C_NONE                        ; 67
  DB C_PRE66                       ; 68
  DB C_MODRM or C_PRE66            ; 69
  DB C_DATA8                       ; 6A
  DB C_MODRM or C_DATA8            ; 6B
  DB C_NONE                        ; 6C
  DB C_NONE                        ; 6D
  DB C_NONE                        ; 6E
  DB C_NONE                        ; 6F
  DB C_REL8                        ; 70
  DB C_REL8                        ; 71
  DB C_REL8                        ; 72
  DB C_REL8                        ; 73
  DB C_REL8                        ; 74
  DB C_REL8                        ; 75
  DB C_REL8                        ; 76
  DB C_REL8                        ; 77
  DB C_REL8                        ; 78
  DB C_REL8                        ; 79
  DB C_REL8                        ; 7A
  DB C_REL8                        ; 7B
  DB C_REL8                        ; 7C
  DB C_REL8                        ; 7D
  DB C_REL8                        ; 7E
  DB C_REL8                        ; 7F
  DB C_MODRM or C_DATA8            ; 80
  DB C_MODRM or C_PRE66            ; 81
  DB C_MODRM or C_DATA8            ; 82
  DB C_MODRM or C_DATA8            ; 83
  DB C_MODRM                       ; 84
  DB C_MODRM                       ; 85
  DB C_MODRM                       ; 86
  DB C_MODRM                       ; 87
  DB C_MODRM                       ; 88
  DB C_MODRM                       ; 89
  DB C_MODRM                       ; 8A
  DB C_MODRM                       ; 8B
  DB C_MODRM                       ; 8C
  DB C_MODRM                       ; 8D
  DB C_MODRM                       ; 8E
  DB C_MODRM                       ; 8F
  DB C_NONE                        ; 90
  DB C_NONE                        ; 91
  DB C_NONE                        ; 92
  DB C_NONE                        ; 93
  DB C_NONE                        ; 94
  DB C_NONE                        ; 95
  DB C_NONE                        ; 96
  DB C_NONE                        ; 97
  DB C_NONE                        ; 98
  DB C_NONE                        ; 99
  DB C_PRE66 or C_DATA16           ; 9A
  DB C_NONE                        ; 9B
  DB C_NONE                        ; 9C
  DB C_NONE                        ; 9D
  DB C_NONE                        ; 9E
  DB C_NONE                        ; 9F
  DB C_PRE66                       ; A0
  DB C_PRE66                       ; A1
  DB C_PRE66                       ; A2
  DB C_PRE66                       ; A3
  DB C_NONE                        ; A4
  DB C_NONE                        ; A5
  DB C_NONE                        ; A6
  DB C_NONE                        ; A7
  DB C_DATA8                       ; A8
  DB C_PRE66                       ; A9
  DB C_NONE                        ; AA
  DB C_NONE                        ; AB
  DB C_NONE                        ; AC
  DB C_NONE                        ; AD
  DB C_NONE                        ; AE
  DB C_NONE                        ; AF
  DB C_DATA8                       ; B0
  DB C_DATA8                       ; B1
  DB C_DATA8                       ; B2
  DB C_DATA8                       ; B3
  DB C_DATA8                       ; B4
  DB C_DATA8                       ; B5
  DB C_DATA8                       ; B6
  DB C_DATA8                       ; B7
  DB C_PRE66                       ; B8
  DB C_PRE66                       ; B9
  DB C_PRE66                       ; BA
  DB C_PRE66                       ; BB
  DB C_PRE66                       ; BC
  DB C_PRE66                       ; BD
  DB C_PRE66                       ; BE
  DB C_PRE66                       ; BF
  DB C_MODRM or C_DATA8            ; C0
  DB C_MODRM or C_DATA8            ; C1
  DB C_DATA16                      ; C2
  DB C_NONE                        ; C3
  DB C_MODRM                       ; C4
  DB C_MODRM                       ; C5
  DB C_MODRM or C_DATA8            ; C6
  DB C_MODRM or C_PRE66            ; C7
  DB C_DATA8 or C_DATA16           ; C8
  DB C_NONE                        ; C9
  DB C_DATA16                      ; CA
  DB C_NONE                        ; CB
  DB C_NONE                        ; CC
  DB C_DATA8                       ; CD
  DB C_NONE                        ; CE
  DB C_NONE                        ; CF
  DB C_MODRM                       ; D0
  DB C_MODRM                       ; D1
  DB C_MODRM                       ; D2
  DB C_MODRM                       ; D3
  DB C_DATA8                       ; D4
  DB C_DATA8                       ; D5
  DB C_NONE                        ; D6
  DB C_NONE                        ; D7
  DB C_WORD                        ; D8
  DB C_WORD                        ; D9
  DB C_WORD                        ; DA
  DB C_WORD                        ; DB
  DB C_WORD                        ; DC
  DB C_WORD                        ; DD
  DB C_WORD                        ; DE
  DB C_WORD                        ; DF
  DB C_REL8                        ; E0
  DB C_REL8                        ; E1
  DB C_REL8                        ; E2
  DB C_REL8                        ; E3
  DB C_DATA8                       ; E4
  DB C_DATA8                       ; E5
  DB C_DATA8                       ; E6
  DB C_DATA8                       ; E7
  DB C_PRE66 or C_REL32            ; E8
  DB C_PRE66 or C_REL32            ; E9
  DB C_PRE66 or C_DATA16           ; EA
  DB C_REL8                        ; EB
  DB C_NONE                        ; EC
  DB C_NONE                        ; ED
  DB C_NONE                        ; EE
  DB C_NONE                        ; EF
  DB C_NONE                        ; F0
  DB C_NONE                        ; F1
  DB C_NONE                        ; F2
  DB C_NONE                        ; F3
  DB C_NONE                        ; F4
  DB C_NONE                        ; F5
  DB C_MODRM                       ; F6
  DB C_MODRM                       ; F7
  DB C_NONE                        ; F8
  DB C_NONE                        ; F9
  DB C_NONE                        ; FA
  DB C_NONE                        ; FB
  DB C_NONE                        ; FC
  DB C_NONE                        ; FD
  DB C_MODRM                       ; FE
  DB C_MODRM                       ; FF
;---------------------------------------------------------------------------
HDE_TABLE_EXT  DB C_MODRM                       ; 00
  DB C_MODRM                       ; 01
  DB C_MODRM                       ; 02
  DB C_MODRM                       ; 03
  DB C_NONE                        ; 04
  DB C_NONE                        ; 05
  DB C_NONE                        ; 06
  DB C_NONE                        ; 07
  DB C_NONE                        ; 08
  DB C_NONE                        ; 09
  DB C_NONE                        ; 0A
  DB C_NONE                        ; 0B
  DB C_NONE                        ; 0C
  DB C_MODRM                       ; 0D
  DB C_NONE                        ; 0E
  DB C_MODRM or C_DATA8            ; 0F
  DB C_MODRM                       ; 10
  DB C_MODRM                       ; 11
  DB C_MODRM                       ; 12
  DB C_MODRM                       ; 13
  DB C_MODRM                       ; 14
  DB C_MODRM                       ; 15
  DB C_MODRM                       ; 16
  DB C_MODRM                       ; 17
  DB C_MODRM                       ; 18
  DB C_NONE                        ; 19
  DB C_NONE                        ; 1A
  DB C_NONE                        ; 1B
  DB C_NONE                        ; 1C
  DB C_NONE                        ; 1D
  DB C_NONE                        ; 1E
  DB C_NONE                        ; 1F
  DB C_MODRM                       ; 20
  DB C_MODRM                       ; 21
  DB C_MODRM                       ; 22
  DB C_MODRM                       ; 23
  DB C_MODRM                       ; 24
  DB C_NONE                        ; 25
  DB C_MODRM                       ; 26
  DB C_NONE                        ; 27
  DB C_MODRM                       ; 28
  DB C_MODRM                       ; 29
  DB C_MODRM                       ; 2A
  DB C_MODRM                       ; 2B
  DB C_MODRM                       ; 2C
  DB C_MODRM                       ; 2D
  DB C_MODRM                       ; 2E
  DB C_MODRM                       ; 2F
  DB C_NONE                        ; 30
  DB C_NONE                        ; 31
  DB C_NONE                        ; 32
  DB C_NONE                        ; 33
  DB C_NONE                        ; 34
  DB C_NONE                        ; 35
  DB C_NONE                        ; 36
  DB C_NONE                        ; 37
  DB C_NONE                        ; 38
  DB C_NONE                        ; 39
  DB C_NONE                        ; 3A
  DB C_NONE                        ; 3B
  DB C_NONE                        ; 3C
  DB C_NONE                        ; 3D
  DB C_NONE                        ; 3E
  DB C_NONE                        ; 3F
  DB C_MODRM                       ; 40
  DB C_MODRM                       ; 41
  DB C_MODRM                       ; 42
  DB C_MODRM                       ; 43
  DB C_MODRM                       ; 44
  DB C_MODRM                       ; 45
  DB C_MODRM                       ; 46
  DB C_MODRM                       ; 47
  DB C_MODRM                       ; 48
  DB C_MODRM                       ; 49
  DB C_MODRM                       ; 4A
  DB C_MODRM                       ; 4B
  DB C_MODRM                       ; 4C
  DB C_MODRM                       ; 4D
  DB C_MODRM                       ; 4E
  DB C_MODRM                       ; 4F
  DB C_MODRM                       ; 50
  DB C_MODRM                       ; 51
  DB C_MODRM                       ; 52
  DB C_MODRM                       ; 53
  DB C_MODRM                       ; 54
  DB C_MODRM                       ; 55
  DB C_MODRM                       ; 56
  DB C_MODRM                       ; 57
  DB C_MODRM                       ; 58
  DB C_MODRM                       ; 59
  DB C_MODRM                       ; 5A
  DB C_MODRM                       ; 5B
  DB C_MODRM                       ; 5C
  DB C_MODRM                       ; 5D
  DB C_MODRM                       ; 5E
  DB C_MODRM                       ; 5F
  DB C_MODRM                       ; 60
  DB C_MODRM                       ; 61
  DB C_MODRM                       ; 62
  DB C_MODRM                       ; 63
  DB C_MODRM                       ; 64
  DB C_MODRM                       ; 65
  DB C_MODRM                       ; 66
  DB C_MODRM                       ; 67
  DB C_MODRM                       ; 68
  DB C_MODRM                       ; 69
  DB C_MODRM                       ; 6A
  DB C_MODRM                       ; 6B
  DB C_MODRM                       ; 6C
  DB C_MODRM                       ; 6D
  DB C_MODRM                       ; 6E
  DB C_MODRM                       ; 6F
  DB C_MODRM or C_DATA8            ; 70
  DB C_MODRM or C_DATA8            ; 71
  DB C_MODRM or C_DATA8            ; 72
  DB C_MODRM or C_DATA8            ; 73
  DB C_MODRM                       ; 74
  DB C_MODRM                       ; 75
  DB C_MODRM                       ; 76
  DB C_NONE                        ; 77
  DB C_NONE                        ; 78
  DB C_NONE                        ; 79
  DB C_NONE                        ; 7A
  DB C_NONE                        ; 7B
  DB C_MODRM                       ; 7C
  DB C_MODRM                       ; 7D
  DB C_MODRM                       ; 7E
  DB C_MODRM                       ; 7F
  DB C_PRE66 or C_REL32            ; 80
  DB C_PRE66 or C_REL32            ; 81
  DB C_PRE66 or C_REL32            ; 82
  DB C_PRE66 or C_REL32            ; 83
  DB C_PRE66 or C_REL32            ; 84
  DB C_PRE66 or C_REL32            ; 85
  DB C_PRE66 or C_REL32            ; 86
  DB C_PRE66 or C_REL32            ; 87
  DB C_PRE66 or C_REL32            ; 88
  DB C_PRE66 or C_REL32            ; 89
  DB C_PRE66 or C_REL32            ; 8A
  DB C_PRE66 or C_REL32            ; 8B
  DB C_PRE66 or C_REL32            ; 8C
  DB C_PRE66 or C_REL32            ; 8D
  DB C_PRE66 or C_REL32            ; 8E
  DB C_PRE66 or C_REL32            ; 8F
  DB C_MODRM                       ; 90
  DB C_MODRM                       ; 91
  DB C_MODRM                       ; 92
  DB C_MODRM                       ; 93
  DB C_MODRM                       ; 94
  DB C_MODRM                       ; 95
  DB C_MODRM                       ; 96
  DB C_MODRM                       ; 97
  DB C_MODRM                       ; 98
  DB C_MODRM                       ; 99
  DB C_MODRM                       ; 9A
  DB C_MODRM                       ; 9B
  DB C_MODRM                       ; 9C
  DB C_MODRM                       ; 9D
  DB C_MODRM                       ; 9E
  DB C_MODRM                       ; 9F
  DB C_NONE                        ; A0
  DB C_NONE                        ; A1
  DB C_NONE                        ; A2
  DB C_MODRM                       ; A3
  DB C_MODRM or C_DATA8            ; A4
  DB C_MODRM                       ; A5
  DB C_NONE                        ; A6
  DB C_NONE                        ; A7
  DB C_NONE                        ; A8
  DB C_NONE                        ; A9
  DB C_NONE                        ; AA
  DB C_MODRM                       ; AB
  DB C_MODRM or C_DATA8            ; AC
  DB C_MODRM                       ; AD
  DB C_MODRM                       ; AE
  DB C_MODRM                       ; AF
  DB C_MODRM                       ; B0
  DB C_MODRM                       ; B1
  DB C_MODRM                       ; B2
  DB C_MODRM                       ; B3
  DB C_MODRM                       ; B4
  DB C_MODRM                       ; B5
  DB C_MODRM                       ; B6
  DB C_MODRM                       ; B7
  DB C_NONE                        ; B8
  DB C_NONE                        ; B9
  DB C_MODRM or C_DATA8            ; BA
  DB C_MODRM                       ; BB
  DB C_MODRM                       ; BC
  DB C_MODRM                       ; BD
  DB C_MODRM                       ; BE
  DB C_MODRM                       ; BF
  DB C_MODRM                       ; C0
  DB C_MODRM                       ; C1
  DB C_MODRM or C_DATA8            ; C2
  DB C_MODRM                       ; C3
  DB C_MODRM or C_DATA8            ; C4
  DB C_MODRM or C_DATA8            ; C5
  DB C_MODRM or C_DATA8            ; C6
  DB C_MODRM                       ; C7
  DB C_NONE                        ; C8
  DB C_NONE                        ; C9
  DB C_NONE                        ; CA
  DB C_NONE                        ; CB
  DB C_NONE                        ; CC
  DB C_NONE                        ; CD
  DB C_NONE                        ; CE
  DB C_NONE                        ; CF
  DB C_MODRM                       ; D0
  DB C_MODRM                       ; D1
  DB C_MODRM                       ; D2
  DB C_MODRM                       ; D3
  DB C_MODRM                       ; D4
  DB C_MODRM                       ; D5
  DB C_MODRM                       ; D6
  DB C_MODRM                       ; D7
  DB C_MODRM                       ; D8
  DB C_MODRM                       ; D9
  DB C_MODRM                       ; DA
  DB C_MODRM                       ; DB
  DB C_MODRM                       ; DC
  DB C_MODRM                       ; DD
  DB C_MODRM                       ; DE
  DB C_MODRM                       ; DF
  DB C_MODRM                       ; E0
  DB C_MODRM                       ; E1
  DB C_MODRM                       ; E2
  DB C_MODRM                       ; E3
  DB C_MODRM                       ; E4
  DB C_MODRM                       ; E5
  DB C_MODRM                       ; E6
  DB C_MODRM                       ; E7
  DB C_MODRM                       ; E8
  DB C_MODRM                       ; E9
  DB C_MODRM                       ; EA
  DB C_MODRM                       ; EB
  DB C_MODRM                       ; EC
  DB C_MODRM                       ; ED
  DB C_MODRM                       ; EE
  DB C_MODRM                       ; EF
  DB C_MODRM                       ; F0
  DB C_MODRM                       ; F1
  DB C_MODRM                       ; F2
  DB C_MODRM                       ; F3
  DB C_MODRM                       ; F4
  DB C_MODRM                       ; F5
  DB C_MODRM                       ; F6
  DB C_MODRM                       ; F7
  DB C_MODRM                       ; F8
  DB C_MODRM                       ; F9
  DB C_MODRM                       ; FA
  DB C_MODRM                       ; FB
  DB C_MODRM                       ; FC
  DB C_MODRM                       ; FD
  DB C_MODRM                       ; FE
  DB C_NONE                        ; FF
;-----------------------------------------------------------------------------------
;   code
;-----------------------------------------------------------------------------------
.code

;
;  Disassemble command
;
;  void hde_disasm(
;    void *pCode               // pointer to code
;    HDE_STRUCT *pHDE_STRUCT   // pointer to structure HDE_STRUCT
;    )
;
hde_disasm PROC, pCode:DWORD, pHDE_STRUCT:DWORD  ;;;;;, pHdeTable:DWORD, pHdeTableExt:DWORD
LOCAL flags  :BYTE
LOCAL p66    :BYTE
LOCAL p67    :BYTE
LOCAL m_mod  :BYTE
LOCAL m_reg  :BYTE
LOCAL m_rm   :BYTE
LOCAL is_sib :BYTE
LOCAL dispsz :BYTE

	pushad

	mov    ebx, pHDE_STRUCT
	assume ebx : ptr HDE_STRUCT

	xor    eax, eax
	mov    edi, ebx
	mov    ecx, (TYPE HDE_STRUCT)
	rep    stosb
	mov    p66, 0
	mov    p67, 0
	mov    is_sib, 0
	mov    dispsz, 0

	mov    ecx, pCode
	mov    al, [ecx]
pref:
	cmp    al, 0F3h
	jz     XX1
	cmp    al, 0F2h
	jnz    no_rep
XX1:
	mov    [ebx].p_rep, al
	jmp    pref_next
no_rep:
	cmp    al, 0F0h
	jnz    XX2
	mov    [ebx].p_lock, al
	jmp    pref_next
XX2:
	cmp    al, 2Eh
	jz     XX3
	cmp    al, 36h
	jz     XX3
	cmp    al, 3Eh
	jz     XX3
	cmp    al, 26h
	jz     XX3
	cmp    al, 64h
	jz     XX3
	cmp    al, 65h
	jnz    no_seg
XX3:
	mov    [ebx].p_seg, al
	jmp    pref_next
no_seg:
	cmp    al, 66h
	jnz    XX4
	mov    [ebx].p_66, al
	mov    p66, 1
	jmp    pref_next
XX4:
	cmp    al, 67h
	jnz    XX5
	mov    [ebx].p_67, al
	mov    p67, 1
pref_next:
	inc    ecx
	mov    al, [ecx]
	jmp    pref
XX5:
	mov    [ebx].opcode, al
	cmp    al, 0Fh
	jnz    no_opcode2
	inc    ecx
	mov    al, [ecx]
	mov    [ebx].opcode2, al
	lea    edi,HDE_TABLE_EXT            ;;;; mov    edi,pHdeTableExt     ;***************
	mov    dl, [edi+eax]
	mov    flags, dl
	jmp    XX6
no_opcode2:
	lea    edi,HDE_TABLE                ;;;; mov    edi,pHdeTable        ;***************
	mov    dl, [edi+eax]
	mov    flags, dl
	cmp    al, 0A0h
	jb     XX6
	cmp    al, 0A3h
	ja     XX6
	mov    dl, p67
	mov    p66, dl
XX6:
	inc    ecx
	mov    al, [ecx]
	test   flags, C_WORD
	jz     XX7
	inc    ecx
	mov    al, [ecx]
XX7:
	test   flags, C_MODRM
	jz     imm
	mov    [ebx].modrm, al
	mov    dl, al
	shr    dl, 6
	mov    m_mod, dl
	mov    [ebx].modrm_mod, dl
	mov    dl, al
	and    dl, 38h
	shr    dl, 3
	mov    m_reg, dl
	mov    [ebx].modrm_reg, dl
	mov    dl, al
	and    dl, 7
	mov    m_rm, dl
	mov    [ebx].modrm_rm, dl
	inc    ecx
	mov    al, [ecx]
	cmp    m_reg, 0
	jnz    XX8
	cmp    [ebx].opcode, 0F6h
	jnz    no_F6
	or     flags, C_DATA8
no_F6:
	cmp    [ebx].opcode, 0F7h
	jnz    XX8
	or     flags, C_PRE66
XX8:
	cmp    m_mod, 0
	jnz    no_mod_0
	cmp    p67, 0
	jz     XX9
	cmp    m_rm, 6
	jnz    no_mod_0
	mov    dispsz, 2
	jmp    no_mod_0
XX9:
	cmp    m_rm, 5
	jnz    no_mod_0
	mov    dispsz, 4
no_mod_0:
	cmp    m_mod, 1
	jnz    no_mod_1
	mov    dispsz, 1
no_mod_1:
	cmp    m_mod, 2
	jnz    no_mod_2
	cmp    p67, 0
	jz     XX10
	mov    dispsz, 2
	jmp    no_mod_2
XX10:
	mov    dispsz, 4
no_mod_2:
	cmp    m_mod, 3
	jz     disp
	cmp    m_rm, 4
	jnz    disp
	cmp    p67, 0
	jnz    disp
	mov    is_sib, 1
	mov    [ebx].sib, al
	mov    dl, al
	shr    dl, 6
	mov    [ebx].sib_scale, dl
	mov    dl, al
	and    dl, 38h
	shr    dl, 3
	mov    [ebx].sib_index, dl
	mov    dl, al
	and    dl, 7
	mov    [ebx].sib_base, dl
	cmp    dl, 5
	jnz    disp
	cmp    m_mod, 0
	jz     XX11
	cmp    m_mod, 2
	jnz    disp
XX11:
	mov    dispsz, 4
disp:
	cmp    dispsz, 1
	jnz    XX12
	mov    dl, [ecx]
	mov    [ebx].disp8, dl
    inc    [ebx].dispresent
XX12:
	cmp    dispsz, 2
	jnz    XX13
	mov    dx, [ecx]
	mov    [ebx].disp16, dx
    inc    [ebx].dispresent
XX13:
	cmp    dispsz, 4
	jnz    XX14
	mov    edx, [ecx]
	mov    [ebx].disp32, edx
    inc    [ebx].dispresent
XX14:
	cmp    is_sib, 0
	jz     XX15
	inc    ecx
XX15:
	movzx  edx, dispsz
	add    ecx, edx
imm:
	test   flags, C_PRE66
	jz     no_pre66
	test   flags, C_REL32
	jz     no_rel32
	cmp    p66, 0
	jz     XX16
	mov    dx, [ecx]
	mov    [ebx].rel16, dx
  inc    [ebx].relpresent
	add    ecx, 2
	jmp    hde_dasm_end
XX16:
	mov    edx, [ecx]
	mov    [ebx].rel32, edx
  inc    [ebx].relpresent
	add    ecx, 4
	jmp    hde_dasm_end
no_rel32:
	cmp    p66, 0
	jz     XX17
	mov    dx, [ecx]
	mov    [ebx].imm16, dx
  inc    [ebx].immpresent
	add    ecx, 2
	jmp    no_pre66
XX17:
	mov    edx, [ecx]
	mov    [ebx].imm32, edx
  inc    [ebx].immpresent
	add    ecx, 4
no_pre66:
	test   flags, C_DATA32
	jz     XX18
	mov    edx, [ecx]
	mov    [ebx].imm32, edx
  inc    [ebx].immpresent
	add    ecx, 4
XX18:
	test   flags, C_DATA16
	jz     XX19
	mov    dx, [ecx]
	mov    [ebx].imm16, dx
  inc    [ebx].immpresent
	add    ecx, 2
XX19:
	test   flags, C_DATA8
	jz     XX20
	mov    dl, [ecx]
	mov    [ebx].imm8, dl
    inc    [ebx].immpresent
	inc    ecx
XX20:
	test   flags, C_REL32
	jz     XX21
	mov    edx, [ecx]
	mov    [ebx].rel32, edx
    inc    [ebx].relpresent
	add    ecx, 4
XX21:
	test   flags, C_REL8
	jz     hde_dasm_end
	mov    dl, [ecx]
	mov    [ebx].rel8, dl
    inc    [ebx].relpresent
	inc    ecx
hde_dasm_end:
	sub    ecx, pCode
	mov    [ebx].len, cl
	popad
	ret
hde_disasm ENDP
;-----------------------------------------------------------------------------------
END