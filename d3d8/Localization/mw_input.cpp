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
// $Id: mw_input.cpp,v 1.1 2005-09-06 19:02:30 AngelD Exp $ 
#include "stdafx.h"

#include <stdio.h>
#include <assert.h>
#include <dinput.h>

int     outch;
int     eng_input = 1;
int     TBL_KEY[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xdd, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x00, 0xe1, 0x00, 0xfe, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xE6, 0xc1, 0x00, 0xde, 0x00,
	0x00, 0xd4, 0xc8, 0xd1, 0xc2, 0xd3, 0xc0, 0xcf, 0xd0, 0xd8, 0xce, 0xcb, 0xc4, 0xdc, 0xd2, 0xd9,
	0xc7, 0xc9, 0xca, 0xdb, 0xc5, 0xc3, 0xcc, 0xd6, 0xd7, 0xcd, 0xdf, 0xf5, 0x00, 0xfa, 0x00, 0x00,
	0x00, 0xf4, 0xe8, 0xf1, 0xe2, 0xf3, 0xe0, 0xef, 0xf0, 0xf8, 0xee, 0xeb, 0xe4, 0xfc, 0xf2, 0xf9,
	0xe7, 0xe9, 0xea, 0xfb, 0xe5, 0xe3, 0xec, 0xf6, 0xf7, 0xed, 0xff, 0xd5, 0x00, 0xda, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void PRIVATE EnterChar( int ch )
{
	if ( eng_input )
	{
		outch = ch;
		return;
	}
	if ( ch >= 0x100 )
		outch = ch;
	else
	{
		if ( TBL_KEY[ch] )
		{
			outch = TBL_KEY[ch];
		} else
			outch = ch;
	}
	return;
}
///////////////////////////////////////////////////
// ebx - string
///////////////////////////////////////////////////
void __declspec(naked) EnterCharSTUB()
{
	__asm {
		nop									// Make room for original code
		nop
		nop
		nop
		nop
		nop
		nop

		pushad
		push ebx
		call EnterChar
		add esp,4
		//test eax,eax
		popad
		//jz done
		
		mov ebx, outch

//done:
		ret
	}
}
///////////////////////////////////////////////////
// ebx - char
///////////////////////////////////////////////////
void __declspec(naked) EnterCharMultiStringSTUB()
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

		pushad
		push ebx
		call EnterChar
		add esp,4
		popad

		mov ebx, outch
		ret
	}
}

////////////////////////////////////////////
int     state = 0;
void KeyboardInput( BYTE * data )
{
	int     newstate = data[DIK_SCROLL] & 0x80;

	if ( ( state == 0 ) & ( newstate != 0 ) )
	{
		if ( eng_input )
		{
			game_popup_print( "�������", 0, 1 );
			eng_input = 0;
		} else
		{
			game_popup_print( "English", 0, 1 );
			eng_input = 1;
		}
	}
	state = newstate;
}
///////////////////////////////////////////////////
// ebp - data
///////////////////////////////////////////////////

void __declspec(naked) GetKeyDataSTUB()
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
		nop


		pushad
		push ebp
		call KeyboardInput
		add esp,4
		popad
		//jz done
		
//done:
		ret
	}
};
////////////////////////////////////////////
void InitMWKeyboard()
{
	eng_input	=	GetPrivateProfileInt("General","EnglishInput",1,".\\Angel.ini");
	Intercept	( INST_CALL, hook_addr_EnterChar	, ( DWORD ) &EnterCharSTUB, 5);
	Intercept	( INST_CALL, hook_addr_EnterCharMultiString, ( DWORD ) &EnterCharMultiStringSTUB, 5);
	Intercept	( INST_CALL, hook_addr_GetKeyData, ( DWORD ) &GetKeyDataSTUB, 10);
}