/*

Copyright (C) 2004-2005 Angel Death 

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the included (LICENSE.txt) GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
//////////////////////////////////////////////////////////////////////
// HelperFunctions
// -------------------------------------------------------------------
// $Id: HelperFunctions.h,v 1.1 2005-09-06 19:02:30 AngelD Exp $ 
//////////////////////////////////////////////////////////////////////

#pragma once

#include "define.h"

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */


#define INST_NOP 0x90
#define INST_CALL 0xe8
#define INST_JMP 0xe9

typedef struct patch_code_t
{
    DWORD		Type;
    DWORD		Instruction;
    DWORD		Source;
    DWORD		Dest;
    DWORD		PatchSize;
}PATCHCODESTRUCT;


VOID*	PRIVATE newmemcpy(DWORD lpDest, DWORD lpSource, int len);
BOOL	PRIVATE Intercept(int instruction, DWORD lpSource, DWORD lpDest, int len);
BOOL    PRIVATE ReplaceCode(int instruction, DWORD lpSource, DWORD lpDest, int len);
VOID*   PRIVATE FillBytes(DWORD lpDest, BYTE ch, int len=1);
VOID*   PRIVATE PutDWord(DWORD lpDest, DWORD ch);
VOID*   PRIVATE PutWord(DWORD lpDest, WORD ch);
//void    PRIVATE InterceptTable(PATCHCODESTRUCT*);
#ifdef __cplusplus
}		             /* Assume C declarations for C++ */
#endif  /* __cplusplus */
