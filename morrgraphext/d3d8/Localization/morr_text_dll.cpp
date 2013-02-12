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
// morr_text_dll.cpp : Defines the entry point for the DLL application.
//
// $Id: morr_text_dll.cpp,v 1.2 2005-09-15 17:20:13 AngelD Exp $ 

#include "stdafx.h"

BOOL PRIVATE Initialize( HANDLE hModule );
BOOL PRIVATE Unload( void );

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	BOOL    hResult = TRUE;

	switch ( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		hResult = Initialize( hModule );
		break;
	case DLL_PROCESS_DETACH:
		hResult = Unload(  );
		break;
	}
	if ( !hResult )
		TerminateProcess( GetCurrentProcess(  ), -1 );	//exit(-1);
	return hResult;

}
char CD_Check();
extern "C" void EXPORT dummy(){};
void PrioritySet();
BOOL InitDialogEngine(void);
void UnloadDialogEngine(void);

BOOL PRIVATE Initialize( HANDLE hModule )
{
	remove("NewWarnings.txt");
	if( !DetectVersion() )
	{
		MessageBox( NULL, "Unknown morrowind.exe", "Ошибка", MB_OK | MB_ICONERROR );
		return FALSE;
	}
#ifdef AngelPROTECT
	if ( !CD_Check(  ) )
		return FALSE;
#else
	NoCDPatch();
#endif
	if ( !InitDialogEngine(  ) )
		return FALSE;
	PrioritySet();
	FixLoadBsa();
	InitMWKeyboard();
	InitItemInfo();
	InitArmorFix();
	if( GetPrivateProfileInt("General","EnableConsole",0,".\\Angel.ini") != 0 )
	        InitConsole();
	
	return TRUE;
}

BOOL PRIVATE Unload( void )
{
	UnloadDialogEngine( );
	return TRUE;
}

void WarningPrn( char *error, ... )
{
	FILE   *f = fopen( "NewWarnings.txt", "at" );
	va_list argptr;

	va_start( argptr, error );
	vfprintf( f, error, argptr );
	va_end( argptr );
	fprintf( f, "\n" );
	fclose( f );
}

void PrioritySet(  )
{
	DWORD   prior = NORMAL_PRIORITY_CLASS;

	switch ( GetPrivateProfileInt( "General", "ProcessPriority", 1, ".\\Angel.ini" ) )
	{
	case 1:
		prior = NORMAL_PRIORITY_CLASS;
		break;
	case 2:
		prior = HIGH_PRIORITY_CLASS;
		break;
	case 3:
		prior = REALTIME_PRIORITY_CLASS;
		break;
	default:
		prior = NORMAL_PRIORITY_CLASS;
		break;
	}
	SetPriorityClass( GetCurrentProcess(  ), prior );
}
