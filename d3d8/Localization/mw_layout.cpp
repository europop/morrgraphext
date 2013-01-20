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
	(DWORD*)0x7C67dC,
	{0x4FCE3E,1,{0x90,0x90}},//con_hook_error_exit 	
	{0x4FCD26,1,{0x90}},//con_hook_normal_exit 	
	{0x4fcd20,1,{0x90}},//hook_addr_console 
	//Dialog engine 
	{0x57c6de,1,{0x90}},//hook_addr_dictstr			
	{0x6194b1,1,{0x90}},//hook_addr_travel_printf		
	{0x5d783c,1,{0x90}},//hook_addr_alphavit_prn1 
	{0x5d798e,1,{0x90}},//hook_addr_alphavit_prn2		
	{0x5d75a8,1,{0x90}},//hook_addr_alphavit_cmp 
	{0x40b872,1,{0x90}},//hook_addr_checktopic 
	{0x4b1ff5,1,{0x90}},//hook_addr_mark_topic 
	{0x775afc,1,{0x90}},//addr_char_convert_tbl1 
	{0x7934b8,1,{0x90}},//addr_char_convert_tbl2 
	{0x776cd0,1,{0x90}},//addr_page_break_str 
	//journal convertsion tribunal & bloodmoon only
	{0x432561,1,{0x90}},//hook_addr_QuestsParse 
	{0x4b2c50,1,{0x90}},//addr_journal_parse 
	//keyboard
	{0x64b319,1,{0x90}},//hook_addr_EnterChar 
	{0x644d72,1,{0x90}},//hook_addr_EnterCharMultiString 
	{0x4066b4,1,{0x90}},//hook_addr_GetKeyData 

	{0x0054d87d,1,{0x90}},//unarmored_fix_addr 
	{0x595020,1,{0x90}},//hook_addr_enter_menu 
	{0x5951b0,1,{0x90}},//hook_addr_leave_menu 
	{0x5CDC30,1,{0x90}},//addr_update_ac 
	//items_info
	{0x591219,1,{0x90}},//hook_addr_item_info 
	{0x59121E,1,{0x90}},//addr_item_info_normal_exit 
	{0x59146B,1,{0x90}},//addr_item_info_my_exit 

	{0x417227,1,{0x90}},//hook_addr_gameframe 
};
static struct mw_text_dll_layout_s tribunal_bm_layout={
	(void (__cdecl *)(DWORD,char *,...))0x40f990,//game_con_printf 
	(void (__cdecl *)(char *,DWORD ,DWORD ))0x5F30E0,//game_popup_print 
	(DWORD*)0x7BE11C,
	{0x4FA09E,1,[0x90]},//con_hook_error_exit 
	{0x4F9F86,1,[0x90]},//con_hook_normal_exit 
	{0x4F9F80,1,[0x90]},//hook_addr_console 
	//Dialog engine 
	{0x5768fe,1,[0x90]},//hook_addr_dictstr			
	{0x613476,1,[0x90]},//hook_addr_travel_printf		

	{0x5D1B3C,1,[0x90]},//hook_addr_alphavit_prn1		
	{0x5D1C8E,1,[0x90]},//hook_addr_alphavit_prn2		
	{0x5D18A8,1,[0x90]},//hook_addr_alphavit_cmp		        
	{0x40b8d2,1,[0x90]},//hook_addr_checktopic			
	{0x4afc85,1,[0x90]},//hook_addr_mark_topic			
	{0x76eafc,1,[0x90]},//addr_char_convert_tbl1		       
	{0x78bb88,1,[0x90]},//addr_char_convert_tbl2		       
	{0x76fc48,1,[0x90]},//addr_page_break_str		       
	//journal convertsion tribunal & bloodmoon only
	{0x431d91,1,[0x90]},//hook_addr_QuestsParse		
	{0x4b0910,1,[0x90]},//addr_journal_parse			
	//keyboard
	{0x645119,1,[0x90]},//hook_addr_EnterChar			
	{0x63eb72,1,[0x90]},//hook_addr_EnterCharMultiString 
	{0x406754,1,[0x90]},//hook_addr_GetKeyData			
	{0x549B4D,1,[0x90]},//unarmored_fix_addr			
	{0x58f4d0,1,[0x90]},//hook_addr_enter_menu                   
	{0x58F660,1,[0x90]},//hook_addr_leave_menu                   
	{0x5C7F30,1,[0x90]},//addr_update_ac                         
	//items_info
	{0x58B676,1,[0x90]},//hook_addr_item_info			
	{0x58B67B,1,[0x90]},//addr_item_info_normal_exit	
	{0x58B90A,1,[0x90]},//addr_item_info_my_exit		

	{0x0416FE7,1,[0x90]},//hook_addr_gameframe 
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
