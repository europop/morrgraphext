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
// topic.cpp
// -------------------------------------------------------------------
// $Id: Topic.cpp,v 1.1 2005-09-06 19:02:30 AngelD Exp $ 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>

#include <assert.h>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;
extern vector<string> gamefilelist;
map < string, string > top_data;

//////////////////////////////////////////////////////////////////////
// CopyTopicFromDialog
// -------------------------------------------------------------------
// подмена линка
//////////////////////////////////////////////////////////////////////
int     esi_len;
int     edi_len;
int PRIVATE CopyTopicFromDialog( unsigned char *text, char *buff )
{
	char    c;

	esi_len = 0;
	edi_len = 0;

	while ( ( c = text[edi_len] ) != '#' )
	{
		if ( c >= 'A' && c <= 'Z' )
			c += 0x20;
		if ( c == 0xa )
		{
			buff[edi_len] = 0x20;
		} else
		{
			buff[edi_len] = c;
			esi_len++;
		}
		edi_len++;
	}
	buff[edi_len] = 0;
	map < string, string >::iterator cur = top_data.find( buff );
#ifdef AngelPROTECT
	if ( ( cur != top_data.end(  ) ) && ( ( mw_version != MW_UNKNOWN ) || ( ( *cur ).second.size(  ) % 2 ) ) )
#else
	if ( cur != top_data.end(  ) )
#endif
	{
		strcpy( buff, ( *cur ).second.c_str(  ) );
		edi_len = ( *cur ).second.size(  );
	};
	return 0;

}

///////////////////////////////////////////////////
//CopyTopicFromDialogSTUB
// eax -text
// eax+edx -buff for topic
// return:
//  esi длина топика в тексте без 0x0a
//  edi длина топика в buff
///////////////////////////////////////////////////
void __declspec( naked ) CopyTopicFromDialogSTUB(  )
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

        add edx,eax
		push edx
        push eax
		call CopyTopicFromDialog
		add esp,8
		
//		test eax,eax
		popad
//		jz done
		
		add esi,[esi_len]
		add edi,[edi_len]
		//mov ebx, offset DictReturnString
//done:
		ret
	}
};

//////////////////////////////////////////////////////////////////////
// LoadTopFile
// -------------------------------------------------------------------
// загрузка *.top файла
//////////////////////////////////////////////////////////////////////
BOOL LoadTopFile( const char *fname )	//LoadTextCellFile
{
	BOOL    Problems = FALSE;
	string  line;
	string  link_str, topic_str;

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

		link_str = line.substr( 0, pos );
		topic_str = line.substr( pos + 1, line.size(  ) - pos - 1 );

		pos = 0;
		while ( ( pos = link_str.find( '*', pos ) ) != string::npos )
			link_str[pos] = 0x7F;

		pos = 0;
		while ( ( pos = topic_str.find( '*', pos ) ) != string::npos )
			topic_str[pos] = 0x7F;

		if ( link_str.find( '\t' ) != string::npos )
		{
			WarningPrn( "Два символа табуляции в строке %5d файла '%s'", i, fname );
			Problems = TRUE;
		};

		map < string, string >::iterator cur = top_data.find( link_str );
		if ( cur != top_data.end(  ) )
		{
			Problems = TRUE;
			WarningPrn( "Линк'%s' уже существует. Файл '%s' строка %d.", link_str.c_str(  ), fname, i );
			continue;
		};
		top_data[link_str] = topic_str;
	};
	if ( Problems )
	{
		char    tmp[100];

		sprintf( tmp, "Ошибки в файле '%s'.\nПодробности в файле NewWarnings.txt", fname );
		MessageBox( NULL, tmp, "Ошибка", MB_OK | MB_ICONERROR );
	};
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// LoadTopFile
// -------------------------------------------------------------------
// загрузка *.top файлов для файлов из списка GameFile
//////////////////////////////////////////////////////////////////////
BOOL LoadTopicData( void )
{
	int     Load4Plugins = GetPrivateProfileInt( "General", "LoadTopdataForPlugins", 0, ".\\Angel.ini" );
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
			fname.replace( fname.size(  ) - 4, 4, ".top" );
		}
		if ( !stricmp( ext.c_str(  ), ".esp" ) )
		{
			if ( 0 == Load4Plugins )
				continue;
			is_plugin = 1;
			fname.replace( fname.size(  ) - 4, 4, ".top" );
		}

		if ( !LoadTopFile( fname.c_str(  ) ) && is_plugin == 0 )
		{
			WarningPrn( "Cannot load topdata file '%s'", fname.c_str(  ) );
			char    tmp[100];

			sprintf( tmp, "Cannot load topdata file '%s'", fname.c_str(  ) );
			MessageBox( NULL, tmp, "Ошибка", MB_OK | MB_ICONERROR );
			return FALSE;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// FreeTopicData
// -------------------------------------------------------------------
// 
//////////////////////////////////////////////////////////////////////
void FreeTopicData( void )
{
	top_data.clear(  );
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
map < string, string > marktopic;
BOOL LoadMarkFile( const char *fname )	//LoadTextCellFile
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
		//while((pos=eng_str.find('*',pos))!=string::npos)eng_str[pos]=0x7F;
		map < string, string >::iterator cur = marktopic.find( eng_str );
		if ( cur != marktopic.end(  ) )
		{
//        Problems=TRUE;
			if ( ( ( *cur ).second ) != rus_str )
//WarningPrn("MarkTopicdata '%s'->'%s' already exists. Discarded",eng_str.c_str(),rus_str.c_str());
//        else 
			{
				Problems = TRUE;
				WarningPrn( "MarkTopicdata '%s'->'%s' have differect translate '%s'. Discarded",
					    eng_str.c_str(  ), rus_str.c_str(  ), ( ( *cur ).second ).c_str(  ) );
			}
		} else
			marktopic[eng_str] = rus_str;
	};
	if ( Problems )
	{
		char    tmp[100];

		sprintf( tmp, "Ошибки в файле '%s'.\nПодробности в файле NewWarnings.txt", fname );
		MessageBox( NULL, tmp, "Ошибка", MB_OK | MB_ICONERROR );
	};
	return TRUE;
}

BOOL LoadMarkTopicData(  )
{

	int     Load4Plugins = GetPrivateProfileInt( "General", "LoadMarkTopicForPlugins", 0, ".\\Angel.ini" );
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
			fname.replace( fname.size(  ) - 4, 4, ".mrk" );
		}
		if ( !stricmp( ext.c_str(  ), ".esp" ) )
		{
			if ( 0 == Load4Plugins )
				continue;
			is_plugin = 1;
			fname.replace( fname.size(  ) - 4, 4, ".mrk" );
		}

		if ( !LoadMarkFile( fname.c_str(  ) ) && is_plugin == 0 )
		{
			WarningPrn( "Cannot load marktopic file '%s'", fname.c_str(  ) );
//    MessageBox(NULL,"Cannot load celldata\nSee NewWarnings.txt","Ошибка",MB_OK|MB_ICONERROR);
//    return FALSE; 
		}
	}

	return TRUE;
}

void FreeMarkTopicData( void )
{
	marktopic.clear(  );
};


char    TopicReturnString[1000];

int get_topic( char *str )
{
	//char*text=0x7C0BF4;
	if ( str == NULL )
		return 0;
	if ( str[0] == 0 )
		return 0;

	map < string, string >::iterator cur = marktopic.find( str );
	if ( cur == marktopic.end(  ) )
	{
		return 0;
	} else
	{
		strcpy( TopicReturnString, ( *cur ).second.c_str(  ) );
		return 1;
	}
	return 0;
}

///////////////////////////////////////////////////
// esi - topic
// edx - dialog str
///////////////////////////////////////////////////
void __declspec( naked ) MarkTopicSTUB(  )
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
		push esi
		call get_topic
		add esp,4
		test eax,eax
		popad
		jz done
		
		mov esi, offset TopicReturnString

done:
		ret
	}
}

