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
// $Id: mw_layout.cpp,v 1.3 2005-11-15 03:32:24 AngelD Exp $ 
//

#include "mw_layout.h"

void InitLocallizationLayout(DWORD dwVersionLS){
}
mw_version_t mw_version = MW_UNKNOWN;

static struct mw_text_dll_layout_s bm_layout={
	(void (__cdecl *)(DWORD,char *,...))0x40f970,//game_con_printf 
	(void (__cdecl *)(char *,DWORD ,DWORD ))0x5F90C0,//game_popup_print 
	(DWORD*)0x7C67dC,//gamedatap
	{ 0x004fce3e,  2, { 0x5f, 0x5e }},  //con_hook_error_exit
	{ 0x004fcd26,  2, { 0x8b, 0xcd }},  //con_hook_normal_exit
	{ 0x004fcd20,  6, { 0x0f, 0x84, 0x18, 0x01, 0x00, 0x00 }},  //hook_addr_console
	//Dialog engine
	{ 0x0057c6de,  5, { 0x83, 0xc9, 0xff, 0x33, 0xc0 }},  //hook_addr_dictstr	!COPY
	{ 0x006194b1,  6, { 0xff, 0x15, 0x74, 0x62, 0x74, 0x00 }},  //hook_addr_travel_printf
	{ 0x005d783c,  6, { 0xff, 0x15, 0x74, 0x62, 0x74, 0x00 }},  //hook_addr_alphavit_prn1
	{ 0x005d798e,  6, { 0xff, 0x15, 0x74, 0x62, 0x74, 0x00 }},  //hook_addr_alphavit_prn2
	{ 0x005d75a8,  6, { 0xff, 0x15, 0xb0, 0x62, 0x74, 0x00 }},  //hook_addr_alphavit_cmp
	{ 0x0040b872, 27, { 0x8a, 0x08, 0x80, 0xf9, 0x0a, 0x74, 0x06, 0x88, 0x0c, 0x02, 0x46, 0xeb, 0x04, 0xc6, 0x04, 0x02, 0x20, 0x8a, 0x48, 0x01, 0x47, 0x40, 0x80, 0xf9, 0x23, 0x75, 0xe5 }},  //hook_addr_checktopic
	{ 0x004b1ff5,  5, { 0x83, 0xc9, 0xff, 0x8b, 0xfe }},  //hook_addr_mark_topic !COPY
	{ 0x00775afc, 64, { 0x90 }},  //addr_char_convert_tbl1 !DATA
	{ 0x007934b8, 64, { 0x90 }},  //addr_char_convert_tbl2 !DATA
	{ 0x00776cd0,  6, { 0x90 }},  //addr_page_break_str 	  !DATA
	//journal convertsion tribunal & bloodmoon only
	{ 0x00432561,  5, { 0x8b, 0x54, 0x24, 0x10, 0x43 }},  //hook_addr_QuestsParse  !COPY
	{ 0x004b2c50,  2, { 0x74, 0x04 }},  //addr_journal_parse +4(1) +0x21(2) +0x26(1) !FILL
	//keyboard
	{ 0x0064b319,  5, { 0x83, 0xc9, 0xff, 0x33, 0xc0 }},  //hook_addr_EnterChar !COPY
	{ 0x00644d72,  5, { 0xa1, 0x28, 0x7f, 0x7d, 0x00 }},  //hook_addr_EnterCharMultiString !COPY
	{ 0x004066b4, 10, { 0x55, 0x68, 0x00, 0x01, 0x00, 0x00, 0x50, 0xff, 0x51, 0x24 }},  //hook_addr_GetKeyData !COPY
	{ 0x0054d87d,  6, { 0x0f, 0x84, 0x6b, 0x02, 0x00, 0x00 }},  //unarmored_fix_addr !FILL
	{ 0x00595020,  6, { 0x8b, 0x15, 0xdc, 0x67, 0x7c, 0x00 }},  //hook_addr_enter_menu !COPY
	{ 0x005951b0,  6, { 0x8b, 0x0d, 0xdc, 0x67, 0x7c, 0x00 }},  //hook_addr_leave_menu !COPY
	{ 0x005cdc30,  2, { 0x8b, 0x0d }},  //addr_update_ac
	//items_info
	{ 0x00591219,  5, { 0x3d, 0x41, 0x52, 0x4d, 0x4f }},  //hook_addr_item_info
	{ 0x0059121e,  1, { 0x0f }},  //addr_item_info_normal_exit
	{ 0x0059146b,  1, { 0x33 }},  //addr_item_info_my_exit
	{ 0x00417227,  6, { 0x8b, 0x0d, 0xdc, 0x6c, 0x7c, 0x00 }}  //hook_addr_gameframe

};
static struct mw_text_dll_layout_s tribunal_1313_layout={
	(void (__cdecl *)(DWORD,char *,...))0x40f990,//game_con_printf 
	(void (__cdecl *)(char *,DWORD ,DWORD ))0x5F30E0,//game_popup_print 
	(DWORD*)0x7BE11C,//gamedatap
	{ 0x004fa09e,  2, { 0x5f, 0x5e }},  //con_hook_error_exit
	{ 0x004f9f86,  2, { 0x8b, 0xcd }},  //con_hook_normal_exit
	{ 0x004f9f80,  6, { 0x0f, 0x84, 0x18, 0x01, 0x00, 0x00 }},  //hook_addr_console
	//Dialog enggine
	{ 0x005768fe,  5, { 0x83, 0xc9, 0xff, 0x33, 0xc0 }},  //hook_addr_dictstr	!COPY
	{ 0x00613476,  6, { 0xff, 0x15, 0x74, 0xf2, 0x73, 0x00 }},  //hook_addr_travel_printf
	{ 0x005d1b3c,  6, { 0xff, 0x15, 0x74, 0xf2, 0x73, 0x00 }},  //hook_addr_alphavit_prn1
	{ 0x005d1c8e,  6, { 0xff, 0x15, 0x74, 0xf2, 0x73, 0x00 }},  //hook_addr_alphavit_prn2
	{ 0x005d18a8,  6, { 0xff, 0x15, 0xb0, 0xf2, 0x73, 0x00 }},  //hook_addr_alphavit_cmp
	{ 0x0040b8d2, 27, { 0x8a, 0x08, 0x80, 0xf9, 0x0a, 0x74, 0x06, 0x88, 0x0c, 0x02, 0x46, 0xeb, 0x04, 0xc6, 0x04, 0x02, 0x20, 0x8a, 0x48, 0x01, 0x47, 0x40, 0x80, 0xf9, 0x23, 0x75, 0xe5 }},  //hook_addr_checktopic
	{ 0x004afc85,  5, { 0x83, 0xc9, 0xff, 0x8b, 0xfe }},  //hook_addr_mark_topic !COPY
	{ 0x0076eafc, 64, { 0x90 }},  //addr_char_convert_tbl1 !DATA
	{ 0x0078bb88, 64, { 0x90 }},  //addr_char_convert_tbl2 !DATA
	{ 0x0076fc48,  6, { 0x90 }},  //addr_page_break_str 	  !DATA
	//journal coonvertsion tribunal & bloodmoon only
	{ 0x00431d91,  5, { 0x8b, 0x54, 0x24, 0x10, 0x43 }},  //hook_addr_QuestsParse  !COPY
	{ 0x004b0910,  2, { 0x74, 0x04 }},  //addr_journal_parse +4(1) +0x21(2) +0x26(1) !FILL
	//keyboard
	{ 0x00645119,  5, { 0x83, 0xc9, 0xff, 0x33, 0xc0 }},  //hook_addr_EnterChar !COPY
	{ 0x0063eb72,  5, { 0xa1, 0x70, 0xf8, 0x7c, 0x00 }},  //hook_addr_EnterCharMultiString !COPY
	{ 0x00406754, 10, { 0x55, 0x68, 0x00, 0x01, 0x00, 0x00, 0x50, 0xff, 0x51, 0x24 }},  //hook_addr_GetKeyData !COPY
	{ 0x00549b4d,  6, { 0x0f, 0x84, 0x6b, 0x02, 0x00, 0x00 }},  //unarmored_fix_addr !FILL
	{ 0x0058f4d0,  6, { 0x8b, 0x15, 0x1c, 0xe1, 0x7b, 0x00 }},  //hook_addr_enter_menu !COPY
	{ 0x0058f660,  6, { 0x8b, 0x0d, 0x1c, 0xe1, 0x7b, 0x00 }},  //hook_addr_leave_menu !COPY
	{ 0x005c7f30,  2, { 0x8b, 0x0d }},  //addr_update_ac
	//items_info
	{ 0x0058b676,  5, { 0x3d, 0x41, 0x52, 0x4d, 0x4f }},  //hook_addr_item_info
	{ 0x0058b67b,  1, { 0x0f }},  //addr_item_info_normal_exit
	{ 0x0058b90a,  1, { 0x33 }},  //addr_item_info_my_exit
	{ 0x00416fe7,  6, { 0x8b, 0x0d, 0x1c, 0xe6, 0x7b, 0x00 }}  //hook_addr_gameframe
};
void setup_mw_layout()
{
	memset(&morrowind_localization_layout,0, sizeof(morrowind_localization_layout));
}
void setup_tribunal_layout()
{
	memset(&morrowind_localization_layout,0, sizeof(morrowind_localization_layout));
	
	morrowind_localization_layout.game_con_printf = (void (__cdecl *)(DWORD,char *,...))0x40F990;
	morrowind_localization_layout.game_popup_print = (void (__cdecl *)(char *,DWORD ,DWORD ))0x5F30E0;
	morrowind_localization_layout.gamedatap = (DWORD*)0x7BE11C;
	morrowind_localization_layout.con_hook_error_exit = 0x4FA09E;
	morrowind_localization_layout.con_hook_normal_exit = 0x4F9F86;
	morrowind_localization_layout.hook_addr_console = 0x4F9F80;

	//Dialog engine 
	morrowind_localization_layout.hook_addr_dictstr			= 0x5768fe;
	morrowind_localization_layout.hook_addr_travel_printf		= 0x613476;

	morrowind_localization_layout.hook_addr_alphavit_prn1		= 0x5D1B3C;
	morrowind_localization_layout.hook_addr_alphavit_prn2		= 0x5D1C8E;
	morrowind_localization_layout.hook_addr_alphavit_cmp		        = 0x5D18A8;
	morrowind_localization_layout.hook_addr_checktopic			= 0x40b8d2;
	morrowind_localization_layout.hook_addr_mark_topic			= 0x4afc85;
	morrowind_localization_layout.addr_char_convert_tbl1		       = 0x76eafc;
	morrowind_localization_layout.addr_char_convert_tbl2		       = 0x78bb88;
	morrowind_localization_layout.addr_page_break_str		       = 0x76fc48;

	//journal convertsion tribunal & bloodmoon only
	morrowind_localization_layout.hook_addr_QuestsParse		= 0x431d91; 
	morrowind_localization_layout.addr_journal_parse			= 0x4b0910;

	//keyboard
	morrowind_localization_layout.hook_addr_EnterChar			= 0x645119;
	morrowind_localization_layout.hook_addr_EnterCharMultiString = 0x63eb72;
	morrowind_localization_layout.hook_addr_GetKeyData			= 0x406754;

	morrowind_localization_layout.unarmored_fix_addr			= 0x549B4D;
	morrowind_localization_layout.hook_addr_enter_menu                   = 0x58f4d0;
	morrowind_localization_layout.hook_addr_leave_menu                   = 0x58F660;
	morrowind_localization_layout.addr_update_ac                         = 0x5C7F30;

	//items_info
	morrowind_localization_layout.hook_addr_item_info			= 0x58B676;
	morrowind_localization_layout.addr_item_info_normal_exit	= 0x58B67B;
	morrowind_localization_layout.addr_item_info_my_exit		= 0x58B90A;

	morrowind_localization_layout.hook_addr_gameframe = 0x0416FE7;

}
void setup_bloodmoon_layout()
{
	memset(&morrowind_localization_layout,0, sizeof(morrowind_localization_layout));
	
	morrowind_localization_layout.game_con_printf = (void (__cdecl *)(DWORD,char *,...))0x40f970;
	morrowind_localization_layout.game_popup_print = (void (__cdecl *)(char *,DWORD ,DWORD ))0x5F90C0;

	morrowind_localization_layout.gamedatap = (DWORD*)0x7C67dC;
	morrowind_localization_layout.con_hook_error_exit = 0x4FCE3E;
	morrowind_localization_layout.con_hook_normal_exit = 0x4FCD26;
	morrowind_localization_layout.hook_addr_console = 0x4fcd20;


	//Dialog engine 
	morrowind_localization_layout.hook_addr_dictstr			= 0x57c6de;
	morrowind_localization_layout.hook_addr_travel_printf		= 0x6194b1;
	morrowind_localization_layout.hook_addr_alphavit_prn1		= 0x5d783c;
	morrowind_localization_layout.hook_addr_alphavit_prn2		= 0x5d798e;
	morrowind_localization_layout.hook_addr_alphavit_cmp		= 0x5d75a8;
	morrowind_localization_layout.hook_addr_checktopic			= 0x40b872;
	morrowind_localization_layout.hook_addr_mark_topic			= 0x4b1ff5;
	morrowind_localization_layout.addr_char_convert_tbl1		= 0x775afc;
	morrowind_localization_layout.addr_char_convert_tbl2		= 0x7934b8;
	morrowind_localization_layout.addr_page_break_str			= 0x776cd0;

	//journal convertsion tribunal & bloodmoon only
	morrowind_localization_layout.hook_addr_QuestsParse		= 0x432561; 
	morrowind_localization_layout.addr_journal_parse			= 0x4b2c50;

	//keyboard
	morrowind_localization_layout.hook_addr_EnterChar			= 0x64b319;
	morrowind_localization_layout.hook_addr_EnterCharMultiString = 0x644d72;
	morrowind_localization_layout.hook_addr_GetKeyData			= 0x4066b4;

	morrowind_localization_layout.unarmored_fix_addr			= 0x0054d87d;
	morrowind_localization_layout.hook_addr_enter_menu                   = 0x595020;
	morrowind_localization_layout.hook_addr_leave_menu                   = 0x5951b0;
	morrowind_localization_layout.addr_update_ac                         = 0x5CDC30;

	//items_info
	morrowind_localization_layout.hook_addr_item_info			= 0x591219;
	morrowind_localization_layout.addr_item_info_normal_exit	= 0x59121E;
	morrowind_localization_layout.addr_item_info_my_exit		= 0x59146B;

	morrowind_localization_layout.hook_addr_gameframe = 0x417227;
}
