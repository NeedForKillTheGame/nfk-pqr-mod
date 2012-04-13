/*
Copyright (C) 2004 PQR Mod.
http://nfk.pqr.ru
nfk-pqr@pqr.ru

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Fre e Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/ 
#include "hud.h"

#include "player.h"
#include "tools_menu.h"

#define AMMO_NULL '!'
#define AMMO_NOT_NULL '7'
#define NO_WEAPON '0'

#define WEAPON_CUR_FNT 1
#define WEAPON_FNT 1
//ch_weaponList "x y layout flags"
//  <layout> is:
//    1 - draw vertically, left-justified at x
//    2 - draw vertically, right-justified at x
//    3 - draw horizontally at y, x is ignored
//  [flags] is a bitmask:
//    1 - include weapons that you don't have
//	primarily for TDM games so you can see ammo counts
//    2 - draw a background for the current weapon
//    4 - flash "new" weapons when picked up
//    8 - draw lines to separate sections of the vertical weaponlist

CHud HUD;
c_var_int* ch_weapon_nums;
c_var_bool* ch_weapon_colored;
c_var_string* ch_weaponList;
c_var_string* ch_weapon_bg;
c_var_string* ch_weapon_bg_selected;
c_var_string* ch_weapon_border;
c_var_string* ch_weapon_border_selected;
c_var_int* cf_weapon;
c_var_int* cf_weapon_selected;

void ch_weaponList_onChange(char* new_val);

char wl_names[9][4]={"gn","mg","sg","gl","rl","lg","rg","pg","bfg"};
char wl_colors[9]={'7','3','&','2','1','7','2','6','4'};

CHud::CHud()
{
	weaponList_style = 1;
}

void CHud::RegisterConsoleCommands()
{
	c_cmd* hudnext= new c_cmd("hudnext","hudnext",CMD_hudnext);
	
	ch_weapon_nums = new c_var_int("ch_weapon_nums","ch_weapon_nums",0,0,2);
	ch_weapon_colored = new c_var_bool("ch_weapon_colored","ch_weapon_colored",true);
	ch_weaponList = new c_var_string("ch_weaponlist","ch_weaponlist","0 190 1 10",ch_weaponList_onChange);
	ch_weapon_bg = new c_var_string("ch_weapon_bg","ch_weapon_bg","99000000");
	ch_weapon_bg_selected = new c_var_string("ch_weapon_bg_selected","ch_weapon_bg_selected","33FF5555");
	ch_weapon_border = new c_var_string("ch_weapon_border","ch_weapon_border","99000000");
	ch_weapon_border_selected = new c_var_string("ch_weapon_border_selected","ch_weapon_border_selected","99FF5555");
	cf_weapon = new c_var_int("cf_weapon","cf_weapon",WEAPON_FNT,0,6);
	cf_weapon_selected = new c_var_int("cf_weapon_selected","cf_weapon_selected",WEAPON_CUR_FNT,0,6);
	CMD_hudnext(NULL);

	fnP_DLL_RegisterConsoleCommand("hudnext");
	fnP_DLL_RegisterConsoleCommand("ch_weapon_nums");
	fnP_DLL_RegisterConsoleCommand("ch_weapon_colored");
	fnP_DLL_RegisterConsoleCommand("ch_weaponlist");
	fnP_DLL_RegisterConsoleCommand("ch_weapon_bg");
	fnP_DLL_RegisterConsoleCommand("ch_weapon_bg_selected");
	fnP_DLL_RegisterConsoleCommand("ch_weapon_border");
	fnP_DLL_RegisterConsoleCommand("ch_weapon_border_selected");
	fnP_DLL_RegisterConsoleCommand("cf_weapon");
	fnP_DLL_RegisterConsoleCommand("cf_weapon_selected");
}

void CHud::EVENT_MainLoop()
{
	if (weaponList_layout && LocalPlayer!=NULL) //Если эта переменная не 0, то отображаем weaponList
	{
		int dx;
		int dy;

		if (weaponList_layout==1) {dx=0;dy=17;}
		else if (weaponList_layout==2) {dx=0;dy=17;}
		else {dx=50;dy=0;}

		char wl_ammo_color;
		char wl_name[7];
		int wl_font;

		if (weaponList_flags[2]) //Фоновая подложка
		{
			if (weaponList_layout==1 || weaponList_layout==2) fnP_DLL_FX_Rectangle(weaponList_x-3,weaponList_y-2,52,dy*8+2,strhex2long(ch_weapon_border->value()),strhex2long(ch_weapon_bg->value()),0x102,false);
			else fnP_DLL_FX_Rectangle(weaponList_x-3,weaponList_y,dx*8-3,18,strhex2long(ch_weapon_border->value()),strhex2long(ch_weapon_bg->value()),0x102,false);
		}
		
		for(int i=1;i<=8;i++)
		{
			if (weaponList_flags[0] || LocalPlayer->have_weapon(i)) //Выводим оружие если оно есть или установлен флаг в ch_weaponlist
			{
				if (LocalPlayer->GetAmmo(i)) wl_ammo_color=AMMO_NOT_NULL; else wl_ammo_color=AMMO_NULL;
				if (LocalPlayer->GetWapon() == i) //Если данное оружие выбрано
				{
					wl_font = cf_weapon_selected->value(); //шрифт
					if (weaponList_flags[1]) fnP_DLL_FX_Rectangle(weaponList_x+dx*(i-1)-2,weaponList_y+dy*(i-1)+1,50,15,strhex2long(ch_weapon_border_selected->value()),strhex2long(ch_weapon_bg_selected->value()),0x102,false);
				}
				else wl_font = cf_weapon->value();

				wl_name[0]='^';
				if (!LocalPlayer->have_weapon(i)) wl_name[1]=NO_WEAPON; //нет оружия
				else if (wl_ammo_color==AMMO_NULL) wl_name[1]=AMMO_NULL; //нет патрон
				else if (ch_weapon_colored->value()) wl_name[1]=wl_colors[i]; //если вывод цветом
				else wl_name[1]=AMMO_NOT_NULL;
				wl_name[2]='\0';
				switch (ch_weapon_nums->value())
				{
				case 0: //Вывод названий буквами
					strcat(wl_name,wl_names[i]);
					break;
				case 1: //Вывод цифрами в стиле NFK
					char tmp_s[3];
					itoa(i,tmp_s,10);
					strcat(wl_name,tmp_s);
					break;
				case 2: //Вывод цифрами в стиле Q3
					itoa(i+1,tmp_s,10);
					strcat(wl_name,tmp_s);
					break;
				}
				
				fnP_DLL_ExtendedTextout(weaponList_x+dx*(i-1),weaponList_y+dy*(i-1),wl_font,false,va("%s:^%c %d",wl_name,wl_ammo_color,LocalPlayer->GetAmmo(i)));

			} //if оруже присутствует или установлен флаг отображать все оружия
		} //for(...) цикл перебора всех оружий

		//Линии сверху и снизу
		if (weaponList_flags[3] && (weaponList_layout==1 || weaponList_layout==2)) //Линии
		{
			fnP_DLL_FX_Line(weaponList_x-3,weaponList_y-2,weaponList_x+48,weaponList_y-2,0xFFFFFFFF,0x102,false);
			fnP_DLL_FX_Line(weaponList_x-3,weaponList_y+dy*8,weaponList_x+48,weaponList_y+dy*8,0xFFFFFFFF,0x102,false);
		}
	}
}

void CHud::weaponList_Togle() //Скрыть/показать HUD
{
	if (weaponList_layout == 0) //Если HUD скрыт, то отображаем его на основе текущего стиля hud_style
	{
		switch(weaponList_style)
		{
		case 0:
			weaponList_layout = 1;
			break;
		case 1:
			weaponList_layout = 1;
			break;
		case 2:
			weaponList_layout = 3;
			break;
		}
	}
	else //Если HUD отображается, скрываем его
	{
		weaponList_layout = 0;
	}

	weaponList_set_value(); //Формируем консольную переменную ch_weaponlist на основе внутренних переменных объекта HUD
}

void CHud::weaponList_set_value() //Создаёт консольную переменную ch_weaponlist на основе внутренних перемнных объекта HUD
{
	int tmp_int = 0;
	if (weaponList_flags[0]) tmp_int += 1;
	if (weaponList_flags[1]) tmp_int += 2;
	if (weaponList_flags[2]) tmp_int += 4;
	if (weaponList_flags[3]) tmp_int += 8;

	ch_weaponList->set_value_string(va("%d %d %d %d",weaponList_x,weaponList_y,weaponList_layout,tmp_int));

	//TOOLS_MENU_Update(); //Изменяем пункты меню
}

bool CHud::IsVisible()
{
	if (weaponList_layout==0) return false;
	return true;
}

void ch_weaponList_onChange(char* new_val)
{
	//Раскладываем строку new_val по параметрам и обновляем переменные HUD
	if (new_val == NULL || strlen(new_val) == 0)
	{		
			ch_weaponList->print();
			return;
	}
	char *p = NULL;
	int tmp_int;
	char *tmp_str = new char[strlen(new_val)+1];
	strcpy(tmp_str,new_val);

	if ((p = strtok(tmp_str," ")) != NULL) //Первое слово до пробела
	{
		tmp_int = atoi(p); //Позиция по x
		if (tmp_int>=0 && tmp_int<=VID_WIDTH) HUD.weaponList_x = tmp_int;

		if ((p = strtok(NULL," ")) != NULL) //Второе слово до пробела
		{
			tmp_int = atoi(p); //Позиция по y
			if (tmp_int>=0 && tmp_int<=VID_HEIGHT) HUD.weaponList_y = tmp_int;
		
			if ((p = strtok(NULL," ")) != NULL) //Третье слово до пробела
			{
				tmp_int = atoi(p); //layout
				if (tmp_int>=0 && tmp_int<=4) {HUD.weaponList_layout = tmp_int;HUD.weaponList_last_layout;}
			
				if ((p = strtok(NULL," ")) != NULL) //Четвёртое слово до пробела
				{
					 /* [flags] is a bitmask:
						1 - include weapons that you don't have
						primarily for TDM games so you can see ammo counts
						2 - draw a background for the current weapon
						4 - flash "new" weapons when picked up
						8 - draw lines to separate sections of the vertical weaponlist
					*/

					tmp_int = atoi(p); //flags
					if (tmp_int>=0 && tmp_int<=15)
					{
						if (tmp_int & 1) HUD.weaponList_flags[0]=true; else HUD.weaponList_flags[0]=false; 
						if ((tmp_int >> 1) & 1) HUD.weaponList_flags[1]=true; else HUD.weaponList_flags[1]=false;
						if ((tmp_int >> 2) & 1) HUD.weaponList_flags[2]=true; else HUD.weaponList_flags[2]=false;
						if ((tmp_int >> 3) & 1) HUD.weaponList_flags[3]=true; else HUD.weaponList_flags[3]=false;
					}
				}
			}
		}
	}
	delete[] tmp_str;
	//Обновляем текстовое значение ch_weaponList на основе переменных HUD
	HUD.weaponList_set_value();
}

void CMD_hudnext(char* new_val)
{
	HUD.weaponList_style++;
	if (HUD.weaponList_style>2) HUD.weaponList_style=0;
	HUD.set_style();

}

void CHud::set_style() //Выполняем все команды для перехода к текущему стилю HUD.weaponList_style
{
	switch(weaponList_style)
	{
	case 0:
		Console.SendConsoleHCommand("ch_hudStretch 8");
		Console.SendConsoleHCommand("ch_hudShadow 1");
		Console.SendConsoleHCommand("ch_hudIcons 1");
		Console.SendConsoleHCommand("ch_hudWidth 16");
		Console.SendConsoleHCommand("ch_hudHeight 16");
		Console.SendConsoleHCommand("ch_hudX 320");
		Console.SendConsoleHCommand("ch_hudY 460");
		ch_weaponList_onChange("0 193 1 14");
		Console.SendConsoleHCommand("ch_lagometer_x 608");
		Console.SendConsoleHCommand("ch_lagometer_y 448");
		Console.SendConsoleHCommand("ch_ping_x 608");
		Console.SendConsoleHCommand("ch_ping_y 430");
		break;
	case 1:
		Console.SendConsoleHCommand("ch_hudStretch 8");
		Console.SendConsoleHCommand("ch_hudShadow 1");
		Console.SendConsoleHCommand("ch_hudIcons 1");
		Console.SendConsoleHCommand("ch_hudWidth 16");
		Console.SendConsoleHCommand("ch_hudHeight 16");
		Console.SendConsoleHCommand("ch_hudX 320");
		Console.SendConsoleHCommand("ch_hudY 460");
		ch_weaponList_onChange("595 193 1 14");
		Console.SendConsoleHCommand("ch_lagometer_x 608");
		Console.SendConsoleHCommand("ch_lagometer_y 448");
		Console.SendConsoleHCommand("ch_ping_x 608");
		Console.SendConsoleHCommand("ch_ping_y 430");
		break;
	case 2:
		Console.SendConsoleHCommand("ch_hudStretch 8");
		Console.SendConsoleHCommand("ch_hudShadow 1");
		Console.SendConsoleHCommand("ch_hudIcons 1");
		Console.SendConsoleHCommand("ch_hudWidth 16");
		Console.SendConsoleHCommand("ch_hudHeight 16");
		Console.SendConsoleHCommand("ch_hudX 165");
		Console.SendConsoleHCommand("ch_hudY 461");
		Console.SendConsoleHCommand("weapbartime 0");
		Console.SendConsoleHCommand("ch_lagometer_x 608");
		Console.SendConsoleHCommand("ch_lagometer_y 428");
		Console.SendConsoleHCommand("ch_ping_x 608");
		Console.SendConsoleHCommand("ch_ping_y 410");
		ch_weaponList_onChange("285 461 3 6");
		break;
	}
}
