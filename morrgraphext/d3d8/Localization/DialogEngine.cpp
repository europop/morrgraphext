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
// DialogEngine.cpp
// -------------------------------------------------------------------
// $Id: DialogEngine.cpp,v 1.1 2005-09-06 19:02:30 AngelD Exp $ 
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <assert.h>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

const char ALPHAVIT[] = "¿¡¬√ƒ≈∆«»… ÀÃÕŒœ–—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂ‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ˜¯˘˙˚¸˝˛ˇ";
const char sPage[] = "\n<P>\n";


void    InitFileLists(  );

void UnloadDialogEngine(void);
void DictStrSTUB();

BOOL LoadCellData();
void FreeCellData(void);

int PRIVATE travel_printf(char*str,char*fspec,char*s1,char*s2,int nn,char*s3);
int PRIVATE alphavit_prn(char*b,char*sFormat,int i);
int PRIVATE alphavit_cmp(char*bukva,char*sTopic,int i);

BOOL LoadTopicData(void);
void FreeTopicData(void);
void CopyTopicFromDialogSTUB();

BOOL LoadMarkTopicData();
void FreeMarkTopicData(void);
void MarkTopicSTUB();

void QuestsParseSTUB();

BOOL InitDialogEngine( void )
{
	InitFileLists(  );
	if ( !LoadCellData(  ) )
		return FALSE;
	if ( !LoadTopicData(  ) )
		return FALSE;
	if ( !LoadMarkTopicData(  ) )
		return FALSE;
	Intercept	( INST_CALL, hook_addr_dictstr		, ( DWORD ) &DictStrSTUB, 5);
	ReplaceCode	( INST_CALL, hook_addr_travel_printf, ( DWORD ) &travel_printf, 6);
	ReplaceCode	( INST_CALL, hook_addr_alphavit_prn1, ( DWORD ) &alphavit_prn, 6);
	ReplaceCode	( INST_CALL, hook_addr_alphavit_prn2, ( DWORD ) &alphavit_prn, 6);
	ReplaceCode	( INST_CALL, hook_addr_alphavit_cmp , ( DWORD ) &alphavit_cmp, 6);
	ReplaceCode	( INST_CALL, hook_addr_checktopic	, ( DWORD ) &CopyTopicFromDialogSTUB, 27);
	Intercept	( INST_CALL, hook_addr_mark_topic	, ( DWORD ) &MarkTopicSTUB, 5);
	newmemcpy( addr_char_convert_tbl1, ( DWORD ) ALPHAVIT, 64 );	//chars convert table1
	newmemcpy( addr_char_convert_tbl2, ( DWORD ) ALPHAVIT, 64 );	//chars convert table1
	newmemcpy( addr_page_break_str, ( DWORD ) sPage, 6 );	// <P>\n
	if( mw_version >= MW_TRIBUNAL ) 
	{
		Intercept	( INST_CALL, hook_addr_QuestsParse, ( DWORD ) &QuestsParseSTUB, 5);
		FillBytes( addr_journal_parse		, INST_NOP, 2 );	// remove @# from journal
		FillBytes( addr_journal_parse + 4	, 0xeb, 1 );	// remove @# from journal
		FillBytes( addr_journal_parse +	0x21, INST_NOP, 2 );	// remove @# from journal
		FillBytes( addr_journal_parse +	0x26, 0xeb, 1 );	// remove @# from journal
	}
	
	return TRUE;
};

vector <string> gamefilelist;

void UnloadDialogEngine( void )
{
	FreeTopicData(  );
	FreeCellData(  );
	FreeMarkTopicData(  );
	gamefilelist.clear(  );
}

void InitFileLists(  )
{
	char    KeyName[20], fname[1000];

	for ( int i = 0;; i++ )
	{
		sprintf( KeyName, "GameFile%d", i );
		if ( 0 == GetPrivateProfileString( "Game Files", KeyName, "", fname, 1000, ".\\Morrowind.ini" ) )
			break;
		gamefilelist.push_back( fname );
	}
}
//Alphavit
const unsigned char TranslateTable[] = "¿¡¬√ƒ≈∆«» ÀÃÕŒœ–—“”‘’÷◊ÿŸ⁄";
int PRIVATE alphavit_cmp( char *bukva, char *sTopic, int i )
{
	if ( i != 1 )
		return _strnicmp( bukva, sTopic, i );

#ifdef AngelPROTECT
	if ( !drive )
		exe_crc = ( DWORD ) sTopic;
#endif

	BYTE    cTop, cBukva;

	cTop = ( BYTE ) * sTopic;
	cBukva = ( BYTE ) * bukva;
	if ( cTop < 0xc0 )
	{
		if ( cBukva == 'A' )
			return 0;
	}
	if ( cTop >= 0xe0 && cTop <= 0xFF )
		cTop -= 0x20;
	if ( cTop == TranslateTable[cBukva - 'A'] )
		return 0;
	if ( cTop == 201 && cBukva == 'I' )
		return 0;
	if ( cBukva == 'Z' )
	{
		if ( cTop == 219 )
			return 0;	//€
		if ( cTop == 220 )
			return 0;	//‹
		if ( cTop == 221 )
			return 0;	//›
		if ( cTop == 222 )
			return 0;	//ﬁ
		if ( cTop == 223 )
			return 0;	//ﬂ
	}
	return ( cTop > TranslateTable[cBukva - 'A'] ) ? 1 : -1;
}

int PRIVATE alphavit_prn( char *b, char *sFormat, int i )
{
	if ( i == 'I' )
		return sprintf( b, "(»-…)" );
	if ( i == 'Z' )
		return sprintf( b, "(›-ﬂ)" );
	return sprintf( b, "( %c )", TranslateTable[i - 'A'] );
}

///////////////////////////////////////////////////
// ebp - journal
// edi curroff
// 
///////////////////////////////////////////////////
void __declspec(naked) QuestsParseSTUB()
{
	__asm {
		nop									// Make room for original code
		nop
		nop
		nop
		nop
		nop
		nop
                test edi,edi
		jz done
		cmp byte ptr [edi][ebp][-1],0Ah
		jne done
		dec edi

done:
		ret
	}
}
