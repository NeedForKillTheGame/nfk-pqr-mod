#include "tools_menu.h"

#include "autorecord.h"
#include "teamhelp.h"
#include "ruleset.h"
/*
Copyright (C) 2004 PQR Mod.
http://nfk.pqr.ru
nfk-pqr@pqr.ru

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/ 

//#include "hud.h"
#include "config_manager.h"
#include "mw_hook.h" //for MW_HOOK_on_off()
#include "main_menu.h" //for m_MainMenu
#include "hud.h"
#include "lagometer.h"

#define TOOLS_MENU_Y 144

CModMenu m_ToolsMenu;
CModMenu m_AutorecordSubMenu;
CModMenu m_ConfigSubMenu;
CModMenu m_RulesetSubMenu;
CModMenu m_HUDSubMenu;

bool SbMnHaveFrorceStart;
bool SbMnHaveMatchSave;

void ToolsMenu_Enter(unsigned char n);
void AutorecordSubMenu_Enter(unsigned char n);
void ConfigSubMenu_Enter(unsigned char n);
void RulesetSubMenu_Enter(unsigned char n);
void HUDSubMenu_Enter(unsigned char n);

void TOOLS_MENU_Init()
{
	m_ToolsMenu.AddOption("Autorecord >");
	m_ToolsMenu.AddOption("Config >");
	m_ToolsMenu.AddOption("Ruleset >");
	m_ToolsMenu.AddOption("HUD >");

	m_ToolsMenu.AddOption("Enable MW Hook"); //они будут обновлены путём вызова TOOLS_MENU_Update();

	m_ToolsMenu.AddOption("Return to Main Menu");
	m_ToolsMenu.AddOption("Hide Menu");
	m_ToolsMenu.SetPosition(50,144);
	m_ToolsMenu.SetWidth(175);
	m_ToolsMenu.fnMenuEnter = ToolsMenu_Enter;
	m_ToolsMenu.SetSubMenuMask(15);

	m_AutorecordSubMenu.fnMenuEnter = AutorecordSubMenu_Enter;
	m_AutorecordSubMenu.SetPosition(250,TOOLS_MENU_Y);
	m_AutorecordSubMenu.SetWidth(170);

	m_ConfigSubMenu.AddOption("Save settings (pqrmod.cfg)");
	m_ConfigSubMenu.AddOption("Load settings (pqrmod.cfg)");
	m_ConfigSubMenu.AddOption("Save settings for this map");
	m_ConfigSubMenu.AddOption("Load settings for this map");
	m_ConfigSubMenu.AddOption("Back");
	m_ConfigSubMenu.fnMenuEnter = ConfigSubMenu_Enter;
	m_ConfigSubMenu.SetPosition(250,TOOLS_MENU_Y+VERTICAL_INTERVAL);
	m_ConfigSubMenu.SetWidth(225);

	m_RulesetSubMenu.AddOption("Ruleset OFF");
	m_RulesetSubMenu.AddOption("Ruleset Duel");
	m_RulesetSubMenu.AddOption("Ruleset Teamplay");
	m_RulesetSubMenu.AddOption("Back");
	m_RulesetSubMenu.fnMenuEnter = RulesetSubMenu_Enter;
	m_RulesetSubMenu.SetPosition(250,TOOLS_MENU_Y+VERTICAL_INTERVAL*2);
	m_RulesetSubMenu.SetWidth(175);

	m_HUDSubMenu.AddOption("Next HUD");
	m_HUDSubMenu.AddOption("Weaponlist on");
	m_HUDSubMenu.AddOption("Weapons as names");
	m_HUDSubMenu.AddOption("Colored weapons off");
	m_HUDSubMenu.AddOption("DrawPing off");
	m_HUDSubMenu.AddOption("Lagometer off");
	m_HUDSubMenu.AddOption("Teammaters health off"); //Текст этих пунктов меню может не соответствовать действительности
	m_HUDSubMenu.AddOption("Back");
	m_HUDSubMenu.fnMenuEnter = HUDSubMenu_Enter;
	m_HUDSubMenu.SetPosition(250,TOOLS_MENU_Y+VERTICAL_INTERVAL*3);
	m_HUDSubMenu.SetWidth(175);

	TOOLS_MENU_Update(); //Перекрашиваем и изменяем некоторые пункты меню в соответствии с текущими значениями консольных переменных
}

void ToolsMenu_Enter(unsigned char n)
{
	switch(n)
	{
		case 1:
		{
			m_AutorecordSubMenu.RemoveAllOptions();
			
			if (match_auto_record->value() && !Autorecord.is_record_in_process())
			{
				SbMnHaveFrorceStart=true;	
				m_AutorecordSubMenu.AddOption("Force autorecord");
			}
			else SbMnHaveFrorceStart=false;

			m_AutorecordSubMenu.AddOption("Autorecord off");
			m_AutorecordSubMenu.AddOption("Autorecord in temp");
			m_AutorecordSubMenu.AddOption("Autorecord on");

			if (Autorecord.is_tmp_demo_recorded() && !Autorecord.is_record_in_process())
			{
				SbMnHaveMatchSave=true;	
				m_AutorecordSubMenu.AddOption("Match Save");
			}
			else SbMnHaveMatchSave=false;
			m_AutorecordSubMenu.AddOption("Back");
			m_AutorecordSubMenu.SetOptionColor((match_auto_record->value() + (SbMnHaveFrorceStart?1:0) + 1), LIGHT_OPTION_COLOR);
			m_AutorecordSubMenu.Show(); //Отображаем Подменю для конкретной длл
		}
		break;
		case 2:
		{
			m_ConfigSubMenu.Show();
		}
		break;
		case 3:
		{
			m_RulesetSubMenu.Show();
		}
		break;
		case 4:
		{
			TOOLS_MENU_Update();
			m_HUDSubMenu.Show();
		}
		break;
		case 5:
		{
			mw_hook->togle();
		}
		break;
		case 6:
		{
			m_ToolsMenu.Hide();
			m_MainMenu.Show();
		}
		break;
		case 7: m_ToolsMenu.Hide();
	}
}

void TOOLS_MENU_Update()
{
	if (mw_hook->value())
	{
		m_ToolsMenu.SetOption(5,"Disable MW Hook");
	}
	else
	{
		m_ToolsMenu.SetOption(5,"Enable MW Hook");
	}
	//Перекрашиваем меню Ruleset
	for(int i = 0; i < 3; i++)
	{
		if ( ruleset->value() == i )
		{
			m_RulesetSubMenu.SetOptionColor(i+1,LIGHT_OPTION_COLOR);
		}
		else m_RulesetSubMenu.SetOptionColor(i+1,'7');
	}

	if (HUD.IsVisible())
	{
		m_HUDSubMenu.SetOption(2,"Weaponlist on");
	}
	else
	{
		m_HUDSubMenu.SetOption(2,"Weaponlist off");
	}
	switch(ch_weapon_nums->value())
	{
		case 0: m_HUDSubMenu.SetOption(3,"Weapons as names");break;
		case 1: m_HUDSubMenu.SetOption(3,"Weapons as numbers");break;
		case 2: m_HUDSubMenu.SetOption(3,"Weapons as Q3 numbers");break;
	}
	if (ch_weapon_colored->value())
	{
		m_HUDSubMenu.SetOption(4,"Colored weapons on");
	}
	else
	{
		m_HUDSubMenu.SetOption(4,"Colored weapons off");
	}
	switch(cg_drawping->value())
	{
		case 0: m_HUDSubMenu.SetOption(5,"DrawPing off");break;
		case 1: m_HUDSubMenu.SetOption(5,"DrawPing on");break;
		case 2: m_HUDSubMenu.SetOption(5,"DrawPing auto");break;
	}
	switch(cg_lagometer->value())
	{
		case 0: m_HUDSubMenu.SetOption(6,"Lagometer off");break;
		case 1: m_HUDSubMenu.SetOption(6,"Lagometer on");break;
		case 2: m_HUDSubMenu.SetOption(6,"Lagometer auto");break;
	}
	if (ch_teamhealth->value())
	{
		m_HUDSubMenu.SetOption(7,"Teammaters health on");
	}
	else
	{
		m_HUDSubMenu.SetOption(7,"Teammaters health off");
	}
}

void AutorecordSubMenu_Enter(unsigned char n)
{
	if (n==1)
	{
		if (SbMnHaveFrorceStart)
		{
			((c_cmd*)Console.FindVarByString("match_forcestart"))->OnExecute(NULL);
		}
		else
		{
			char s1[2];
			itoa(0,s1,10);
			match_auto_record->set_value(s1);
		}
	}
	else
	if (n==2)
	{
			char s1[2];
			itoa((SbMnHaveFrorceStart?0:1),s1,10);
			match_auto_record->set_value(s1);
	}
	else
	if (n==3)
	{
			char s1[2];
			itoa((SbMnHaveFrorceStart?1:2),s1,10);
			match_auto_record->set_value(s1);
	}
	else
	if (n==4)
	{
			if (SbMnHaveFrorceStart)
			{
				char s1[2];
				itoa(2,s1,10);
				match_auto_record->set_value(s1);
			}
			else
			if (SbMnHaveMatchSave)
			{
				((c_cmd*)Console.FindVarByString("match_save"))->OnExecute(NULL);
			}
	}
	else
	if (n==5)
	{
			if (SbMnHaveMatchSave && SbMnHaveFrorceStart)
			{
				((c_cmd*)Console.FindVarByString("match_save"))->OnExecute(NULL);
			}
	}

	m_AutorecordSubMenu.Hide();
}
void ConfigSubMenu_Enter(unsigned char n)
{
	switch(n)
	{
	case 1: {
				((c_cmd*)Console.FindVarByString("cfg_save_pm"))->OnExecute(__MOD_DEFAULT_CONFIG__);
			}
	break;
	case 2: {
				((c_cmd*)Console.FindVarByString("cfg_load"))->OnExecute(__MOD_DEFAULT_CONFIG__);
			}
	break;
	case 3: {
				((c_cmd*)Console.FindVarByString("cfg_save_map"))->OnExecute(NULL);
			}
	break;
	case 4: {
				((c_cmd*)Console.FindVarByString("cfg_load_map"))->OnExecute(NULL);
			}
	}

	m_ConfigSubMenu.Hide();
	
}

void RulesetSubMenu_Enter(unsigned char n)
{
	switch(n)
	{
	case 1: ruleset->set_value("0");
		break;
	case 2: ruleset->set_value("1");
		break;
	case 3: ruleset->set_value("2");
		break;
	}

	m_RulesetSubMenu.Hide();
}

void HUDSubMenu_Enter(unsigned char n)
{
	switch(n)
	{
	case 1: CMD_hudnext(NULL);
		break;
	case 2: HUD.weaponList_Togle();
		break;
	case 3:
		switch(ch_weapon_nums->value())
		{
		case 0: ch_weapon_nums->set_value("1");break;
		case 1: ch_weapon_nums->set_value("2");break;
		case 2: ch_weapon_nums->set_value("0");break;
		}
		break;
	case 4: ch_weapon_colored->togle();
		break;
	case 5:
		switch(cg_drawping->value())
		{
		case 0: cg_drawping->set_value("1");break;
		case 1: cg_drawping->set_value("2");break;
		case 2: cg_drawping->set_value("0");break;
		}
		break;
	case 6: 
		switch(cg_lagometer->value())
		{
		case 0: cg_lagometer->set_value("1");break;
		case 1: cg_lagometer->set_value("2");break;
		case 2: cg_lagometer->set_value("0");break;
		}
		break;
	case 7:
		ch_teamhealth->togle();
		break;
	default: m_HUDSubMenu.Hide();
	}

	TOOLS_MENU_Update();
}