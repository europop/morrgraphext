#include <mw_layout.h>
#include <HelperFunctions.h>
#include <stdio.h>

void NoCDPatch();
void FixLoadBsa();

void WarningPrn(char*error,...);
void Con_Printf( char *format, ... );

BOOL InitDialogEngine( void );
void InitMWKeyboard();
void InitItemInfo();
void InitConsole();
void InitArmorFix();
extern BOOL inmenu;
