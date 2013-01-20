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
// mw_layout
// -------------------------------------------------------------------
// $Id: mw_layout.h,v 1.2 2005-09-15 17:20:13 AngelD Exp $ 
//////////////////////////////////////////////////////////////////////

#pragma once
#include "../mwse/WinHeader.h"

typedef enum { MW_UNKNOWN = 0, MW_MORROWIND = 1, MW_TRIBUNAL, MW_BLOODMOON }mw_version_t;
struct mw_text_dll_patch_s{
	DWORD offset;
	DWORD length;
	BYTE original_data[20];
};
struct mw_text_dll_layout_s{
	void (__cdecl *game_con_printf) (DWORD,char*,...);
	void (__cdecl *game_popup_print) (char*,DWORD ,DWORD );

	DWORD* gamedatap;

	mw_text_dll_patch_s con_hook_error_exit;
	mw_text_dll_patch_s con_hook_normal_exit;
	mw_text_dll_patch_s hook_addr_console;

	//Dialog engine 
	mw_text_dll_patch_s hook_addr_dictstr;
	mw_text_dll_patch_s hook_addr_travel_printf;
	mw_text_dll_patch_s hook_addr_alphavit_prn1;
	mw_text_dll_patch_s hook_addr_alphavit_prn2;
	mw_text_dll_patch_s hook_addr_alphavit_cmp;
	mw_text_dll_patch_s hook_addr_checktopic;
	mw_text_dll_patch_s hook_addr_mark_topic;
	mw_text_dll_patch_s addr_char_convert_tbl1;
	mw_text_dll_patch_s addr_char_convert_tbl2;
	mw_text_dll_patch_s addr_page_break_str;

	//journal convertsion tribunal & bloodmoon only
	mw_text_dll_patch_s hook_addr_QuestsParse; 
	mw_text_dll_patch_s addr_journal_parse;

	//keyboard
	mw_text_dll_patch_s hook_addr_EnterChar;
	mw_text_dll_patch_s hook_addr_EnterCharMultiString;
	mw_text_dll_patch_s hook_addr_GetKeyData;

	mw_text_dll_patch_s unarmored_fix_addr;
	mw_text_dll_patch_s hook_addr_enter_menu;
	mw_text_dll_patch_s hook_addr_leave_menu;
	mw_text_dll_patch_s addr_update_ac;

	//items_info
	mw_text_dll_patch_s hook_addr_item_info;
	mw_text_dll_patch_s addr_item_info_normal_exit;
	mw_text_dll_patch_s addr_item_info_my_exit;

	mw_text_dll_patch_s hook_addr_gameframe;
};
struct morrowind_localization_layout_s {
	void (__cdecl *game_con_printf) (DWORD,char*,...);
	void (__cdecl *game_popup_print) (char*,DWORD ,DWORD );

	DWORD* gamedatap;
	DWORD con_hook_error_exit;
	DWORD con_hook_normal_exit;
	DWORD hook_addr_console;

	//Dialog engine 
	DWORD hook_addr_dictstr;
	DWORD hook_addr_travel_printf;
	DWORD hook_addr_alphavit_prn1;
	DWORD hook_addr_alphavit_prn2;
	DWORD hook_addr_alphavit_cmp;
	DWORD hook_addr_checktopic;
	DWORD hook_addr_mark_topic;
	DWORD addr_char_convert_tbl1;
	DWORD addr_char_convert_tbl2;
	DWORD addr_page_break_str;

	//journal convertsion tribunal & bloodmoon only
	DWORD hook_addr_QuestsParse; 
	DWORD addr_journal_parse;

	//keyboard
	DWORD hook_addr_EnterChar;
	DWORD hook_addr_EnterCharMultiString;
	DWORD hook_addr_GetKeyData;

	DWORD unarmored_fix_addr;
	DWORD hook_addr_enter_menu;
	DWORD hook_addr_leave_menu;
	DWORD addr_update_ac;

	//items_info
	DWORD hook_addr_item_info;
	DWORD addr_item_info_normal_exit;
	DWORD addr_item_info_my_exit;

	DWORD hook_addr_gameframe;
} morrowind_localization_layout;
void InitLocallizationLayout(DWORD dwVersionLS);
/*#define MORROWIND_CRC -1
#define TRIBUNAL_CRC 0x6E8390C1 
#define BLOODMOON_CRC 0x385688af 
extern mw_version_t mw_version;
extern DWORD exe_crc;

extern UCHAR drive;
BOOL DetectVersion();*/
