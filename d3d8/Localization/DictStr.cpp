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
// DictStr.cpp
// -------------------------------------------------------------------
// $Id: DictStr.cpp,v 1.1 2005-09-06 19:02:30 AngelD Exp $ 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
//#define _ANGEL_LOG_DICT
map < string, string > cells;
extern vector<string> gamefilelist;

/////////////////////////////////////
int PRIVATE travel_printf( char *str, char *fspec, char *s1, char *s2, int nn, char *s3 )
{
#ifdef AngelPROTECT
	if( exe_crc != BLOODMOON_CRC && exe_crc != TRIBUNAL_CRC && exe_crc != MORROWIND_CRC )
		mw_version = MW_UNKNOWN;
#endif

	if ( s1 == NULL )
		return sprintf( str, fspec, s1, s2, nn, s3 );
	if ( s1[0] == 0 )
		return sprintf( str, fspec, s1, s2, nn, s3 );
	if ( strlen( s1 ) > 990 )
		return sprintf( str, fspec, s1, s2, nn, s3 );
	if ( s1[0] >= 0xc0 )
		return sprintf( str, fspec, s1, s2, nn, s3 );

	map < string, string >::iterator cur = cells.find( string( s1 ) );
	if ( cur == cells.end(  ) )
	{
		return sprintf( str, fspec, s1, s2, nn, s3 );
	} else
	{
		return sprintf( str, fspec, ( ( *cur ).second ).c_str(  ), s2, nn, s3 );
	}

}

/////////////////////////////////////
char    DictReturnString[1000];
int PRIVATE DictStr( char *str )
{

	if ( str == NULL )
		return 0;
	if ( str[0] == 0 )
		return 0;

	if ( strlen( str ) > 990 )
		return 0;
	if ( str[0] >= 0xc0 )
		return 0;


	map < string, string >::iterator cur = cells.find( string( str ) );
	if ( cur == cells.end(  ) )
	{
#ifdef _ANGEL_LOG_DICT
	  FILE*f;
	  f=fopen("angel.log","at");
	  fprintf(f,"dictstr:%s\n",str);
      fclose(f);
#endif
		return 0;
	} else
	{
		strcpy( DictReturnString, ( ( *cur ).second ).c_str(  ) );
	}
#ifdef AngelPROTECT
		if ( !drive )
			exe_crc = ( DWORD ) str;
#endif
	return 1;
};

///////////////////////////////////////////////////
// ebx - string
///////////////////////////////////////////////////
void __declspec( naked ) DictStrSTUB(  )
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
		call DictStr
		add esp,4
		test eax,eax
		popad
		jz done
		
		mov ebx, offset DictReturnString

done:
		ret
	}
}

////////////////////////////////////////////
////////////////////////////////////////////
BOOL LoadCellFile( const char *fname )	
{
	BOOL    Problems = FALSE;
	string  line;
	string  eng_str, rus_str;

	string::size_type pos;

	ifstream infile( fname );

	if ( 0 == infile.is_open(  ) )
		return FALSE;

	for ( int i = 1; getline( infile, line, '\n' ); i++ )
	{
		pos = line.find( '\t' );
		if ( pos == string::npos )
		{
			WarningPrn( "Отсутсвует символ табуляции в строке %5d файла '%s'", i, fname );
			Problems = TRUE;
			continue;
		};

		eng_str = line.substr( 0, pos );
		rus_str = line.substr( pos + 1, line.size(  ) - pos - 1 );
		if ( rus_str.find( '\t' ) != string::npos )
		{
			WarningPrn( "Два символа табуляции в строке %5d файла '%s'", i, fname );
			Problems = TRUE;
		};
		map < string, string >::iterator cur = cells.find( eng_str );
		if ( cur != cells.end(  ) )
		{
			Problems = TRUE;
			if ( ( ( *cur ).second ) == rus_str )
				WarningPrn( "Celldata '%s'->'%s' already exists. Discarded", eng_str.c_str(  ),
					    rus_str.c_str(  ) );
			else
				WarningPrn( "Celldata '%s'->'%s' have differect translate '%s'. Discarded",
					    eng_str.c_str(  ), rus_str.c_str(  ), ( ( *cur ).second ).c_str(  ) );
		} else
			cells[eng_str] = rus_str;
	}
	if ( Problems )
	{
		char    tmp[100];

		sprintf( tmp, "Ошибки в файле '%s'.\nПодробности в файле NewWarnings.txt", fname );
		MessageBox( NULL, tmp, "Ошибка", MB_OK | MB_ICONERROR );
	}
	return TRUE;
}

BOOL LoadCellData(  )
{

#ifdef _ANGEL_LOG_DICT
	remove( "angel.log" );
#endif
	int     LoadCells4Plugins = GetPrivateProfileInt( "General", "LoadCellsForPlugins", 0, ".\\Angel.ini" );
	string  fname, ext;
	int     is_plugin = 0;

	for ( int i = 0; i < gamefilelist.size(  ); i++ )
	{
		fname = "Data Files\\" + gamefilelist[i];
		ext = fname.substr( fname.size(  ) - 4, 4 );
		if ( ext[0] != '.' )
			continue;
		if ( !stricmp( ext.c_str(  ), ".esm" ) )
		{
			is_plugin = 0;
			fname.replace( fname.size(  ) - 4, 4, ".cel" );
		}
		if ( !stricmp( ext.c_str(  ), ".esp" ) )
		{
			if ( 0 == LoadCells4Plugins )
				continue;
			is_plugin = 1;
			fname.replace( fname.size(  ) - 4, 4, ".cel" );
		}

		if ( !LoadCellFile( fname.c_str(  ) ) && is_plugin == 0 )
		{
			WarningPrn( "Cannot load celldata file '%s'", fname.c_str(  ) );
			char    tmp[100];

			sprintf( tmp, "Cannot load celldata file '%s'", fname.c_str(  ) );
			MessageBox( NULL, tmp, "Ошибка", MB_OK | MB_ICONERROR );
			//MessageBox(NULL,"Cannot load celldata\nSee NewWarnings.txt","Ошибка",MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}

	return TRUE;
}

void FreeCellData( void )
{
	cells.clear(  );
}
