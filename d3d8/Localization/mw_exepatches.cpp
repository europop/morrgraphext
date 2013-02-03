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
// $Id: mw_exepatches.cpp,v 1.1 2005-09-06 19:02:30 AngelD Exp $ 
#include "stdafx.h"

void NoCDPatch()
{
  	if( mw_version == MW_BLOODMOON )
        {
         	FillBytes( 0x00416EC0, 0x74 );
         	FillBytes( 0x00416EC8, 0x74 );
         	FillBytes( 0x00416F43, 0x84 );
         	FillBytes( 0x00424D39, 0xB0 );
         	FillBytes( 0x00424D3A, 0x2E );
         	FillBytes( 0x00424D3B, 0x90 );
         	FillBytes( 0x00424D3C, 0x90 );
         	FillBytes( 0x00424D3D, 0x90 );
         	FillBytes( 0x00424EE0, 0xB0 );
         	FillBytes( 0x00424EE1, 0x2E );
         	FillBytes( 0x00424EE2, 0x90 );
         	FillBytes( 0x00424EE3, 0x90 );
         	FillBytes( 0x00424EE4, 0x90 );
         	FillBytes( 0x0077797A, 0x5C );
         	FillBytes( 0x007779AE, 0x5C );
        }
}

void FixLoadBsaBM()
{

        FillBytes(0x47AA37,0x38);
        FillBytes(0x47AA5B,0xDC);
        FillBytes(0x47AA5C,0x90);
        FillBytes(0x47AA5D,0x90);
        FillBytes(0x47AA5E,0x90);
        FillBytes(0x47AA5F,0x90);
        FillBytes(0x47AA60,0x6A);
        FillBytes(0x47AA61,0x00);
        FillBytes(0x47AA68,0xE9);
        FillBytes(0x47AA69,0x80);
        FillBytes(0x47AA6A,0x00);
        FillBytes(0x47AA6B,0x00);
        FillBytes(0x47AA6C,0x00);
        FillBytes(0x47AAE5,0x44);
        FillBytes(0x47AAE8,0xE9);
        FillBytes(0x47AAE9,0x65);
        FillBytes(0x47AAEA,0xFF);
        FillBytes(0x47AAEC,0xFF);
        FillBytes(0x47AAED,0x84);
        FillBytes(0x47AAEE,0xC0);
        FillBytes(0x47AAEF,0x75);
        FillBytes(0x47AAF0,0x2C);
        FillBytes(0x47AAF1,0xE9);
        FillBytes(0x47AAF2,0x42);
        FillBytes(0x47AAF3,0xFF);
        FillBytes(0x47AAF4,0xFF);
        FillBytes(0x47AAF5,0xFF);
        FillBytes(0x47AAF6,0x90);
        FillBytes(0x47AAF7,0x90);
        FillBytes(0x47AAF8,0x90);
}
void FixLoadBsaTribunal()
{

        FillBytes(0x4785f7,0x38);//47AA37
        FillBytes(0x47861b,0xDC);
        FillBytes(0x47861C,0x90);
        FillBytes(0x47861D,0x90);
        FillBytes(0x47861E,0x90);
        FillBytes(0x47861F,0x90);
        FillBytes(0x478620,0x6A);
        FillBytes(0x478621,0x00);
        FillBytes(0x478628,0xE9);
        FillBytes(0x478629,0x80);
        FillBytes(0x47862A,0x00);
        FillBytes(0x47862B,0x00);
        FillBytes(0x47862C,0x00);
        FillBytes(0x4786a5,0x44);
        FillBytes(0x4786A8,0xE9);
        FillBytes(0x4786A9,0x65);
        FillBytes(0x4786AA,0xFF);
        FillBytes(0x4786AC,0xFF);
        FillBytes(0x4786AD,0x84);
        FillBytes(0x4786AE,0xC0);
        FillBytes(0x4786AF,0x75);
        FillBytes(0x4786B0,0x2C);
        FillBytes(0x4786B1,0xE9);
        FillBytes(0x4786B2,0x42);
        FillBytes(0x4786B3,0xFF);
        FillBytes(0x4786B4,0xFF);
        FillBytes(0x4786B5,0xFF);
        FillBytes(0x4786B6,0x90);
        FillBytes(0x4786B7,0x90);
        FillBytes(0x4786B8,0x90);
}

void FixLoadBsa()
{
        if( mw_version == MW_BLOODMOON)
                FixLoadBsaBM();
        if( mw_version == MW_TRIBUNAL)
                FixLoadBsaTribunal();
}
