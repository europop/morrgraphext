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
// $Id: armor_fix.cpp,v 1.1 2005-09-15 17:20:13 AngelD Exp $ 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

BOOL inmenu = TRUE;
void EnterMenu()
{
        inmenu = TRUE;
}
void LeaveMenu()
{
        inmenu = FALSE;
}
///////////////////////////////////////////////////
// 
// 
///////////////////////////////////////////////////
void __declspec(naked) EnterMenu_STUB()
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
		pushad
		call EnterMenu
		call [addr_update_ac]
                popad
		ret
	}
};

///////////////////////////////////////////////////
// 
// 
///////////////////////////////////////////////////
void __declspec(naked) LeaveMenu_STUB()
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
		pushad
		call LeaveMenu
                popad
		ret
	}
};
///////////////////////////////////////////////////
/*void ApplyACHook(float dmg, float ar)
{
        
}

void __declspec(naked) ApplyAC_STUB()
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
//		pushad
//                popad
		ret
	}
}*/
//////////////////////////////////////////////////////////////////////
// ItemInfo_Init()
// -------------------------------------------------------------------
// 
//////////////////////////////////////////////////////////////////////

void InitArmorFix()
{
        int fix_unarmored = GetPrivateProfileInt("Rules","fix_unarmored_bug",0,".\\Angel.ini");
	if(  fix_unarmored != 0 )
	{
/*	        if( fix_unarmored == 2)
	        {
	                ReplaceCode(INST_CALL, unarmored_fix_addr , (DWORD)&ApplyAC_STUB , 6); 
	                
	        }
	        else*/
		FillBytes(unarmored_fix_addr,0x90,6);
	}
        Intercept(INST_CALL, hook_addr_enter_menu , (DWORD)&EnterMenu_STUB , 6); 
        Intercept(INST_CALL, hook_addr_leave_menu , (DWORD)&LeaveMenu_STUB , 6); 
}



