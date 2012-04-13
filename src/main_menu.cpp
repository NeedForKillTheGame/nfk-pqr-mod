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
#include "pqrmod.h"

#include "main_menu.h"
#include "multibot.h" //for m_MultibotMenu
#include "tools_menu.h" //for m_ToolsMenu

CModMenu m_MainMenu;

void MAIN_MENU_Enter(unsigned char n)
{
	/*Для того чтобы при сохранении и загрузке дефаултнго конфига
	выводились сообщения как при консольных командах cfg_save и cfg_load,
	эмитируем ситуацию вызыва консольной команды */
	switch(n) {
	case 1: {
				m_MainMenu.Hide(); 
				m_MultibotMenu.Show();
			}
	break;
	case 2: {
				m_MainMenu.Hide(); 
				m_ToolsMenu.Show();
			}
	break;
	case 3: {
				m_MainMenu.Hide();
			}
	}
}

void MAIN_MENU_Init()
{
	m_MainMenu.AddOption("Select bot DLL...");
	m_MainMenu.AddOption("Tools...");
	m_MainMenu.AddOption("Hide Menu");
	m_MainMenu.SetPosition(50,144);
	m_MainMenu.SetWidth(227);
	m_MainMenu.fnMenuEnter = MAIN_MENU_Enter;
	m_MainMenu.SetSubMenuMask(3);
}