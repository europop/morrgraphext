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
// items_info.cpp
// -------------------------------------------------------------------
// $Id: items_info.cpp,v 1.1 2005-09-06 19:02:30 AngelD Exp $ 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

///////////////////////////////////////////////////
// 
// 
///////////////////////////////////////////////////
void __declspec(naked) AMMO_dmg_STUB()
{
	__asm {
		nop									// Make room for original code
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		cmp eax, 4f4d4d41h // AMMO
		je done_my

		cmp eax, 4f4d5241h  // ARMO
		jmp [addr_item_info_normal_exit]
done_my:
                jmp  [addr_item_info_my_exit]

	}
};

//////////////////////////////////////////////////////////////////////
// ItemInfo_Init()
// -------------------------------------------------------------------
// 
//////////////////////////////////////////////////////////////////////

void InitItemInfo()
{
        ReplaceCode(INST_JMP, hook_addr_item_info , (DWORD)&AMMO_dmg_STUB , 5); 
}



