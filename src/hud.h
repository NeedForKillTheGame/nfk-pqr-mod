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
#ifndef __HUD_H__
#define __HUD_H__
#include "pqrmod.h"

class CHud
{
	

public:
	CHud();
	void RegisterConsoleCommands();
	void EVENT_MainLoop();

	int weaponList_x;
	int weaponList_y;
	int weaponList_layout;
	int weaponList_last_layout;

	bool weaponList_flags[4];
	int weaponList_style;
	
	void set_style();//Выполняем все команды для перехода к текущему стилю HUD.weaponList_style

	void weaponList_set_value(); //Создаёт консольную переменную ch_weaponlist на основе внутренних перемнных объекта HUD
	void weaponList_Togle(); //Скрыть/показать HUD

	bool IsVisible();

};

extern CHud HUD; //Главный объект модуля
void CMD_hudnext(char* new_val);

extern c_var_int* ch_weapon_nums;
extern c_var_bool* ch_weapon_colored;
#endif //__HUD_H__