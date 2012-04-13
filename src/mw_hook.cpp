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
#include "mw_hook.h"

#include "input.h"
#include "tools_menu.h"

#define K_MOUSE5 16
#define K_MOUSE6 8
#define K_MOUSE4 64
#define K_MOUSE7 128
#define K_MOUSE8 32

HHOOK	Hk		= NULL;
HWND	MW_Window	= NULL;

void Key_Event(BYTE,bool);
void MW_Hook_Message (long buttons);
void mw_hook_OnChange(char* new_val);

//HINSTANCE hInstance	= NULL;

typedef void (*MW_DllFunc1)(void);
typedef int (*MW_DllFunc2)(void);
typedef unsigned char (*MW_DllFunc3)(void);
MW_DllFunc1 DLL_MW_RemoveHook = NULL;
MW_DllFunc2 DLL_MW_SetHook = NULL;
MW_DllFunc3 DLL_GetButtons = NULL;

c_var_bool* mw_hook;

HINSTANCE mw_hDLL;
HWND NFK_Window = NULL; 
unsigned char old_buttons;

int MW_SetHook () {
	if (mw_hook->value()) {
		return 1;
	}

	char* path = new char[255];
	//`char* p;
	strcpy(path,NFKGameDir);
	//p = strstr(path,"basenfk");
	strcat(path,"\\mw_hook.dll");
	if (!(mw_hDLL = LoadLibrary(path))) {
		return 2;
	}
	DLL_MW_RemoveHook = (MW_DllFunc1) GetProcAddress(mw_hDLL, "MW_RemoveHook");
	DLL_MW_SetHook = (MW_DllFunc2) GetProcAddress(mw_hDLL, "MW_SetHook");
	DLL_GetButtons = (MW_DllFunc3) GetProcAddress(mw_hDLL, "MW_GetButtons");
	if (!DLL_MW_SetHook || !DLL_MW_RemoveHook) {
		FreeLibrary(mw_hDLL);
		return 3;
	}
	int error_code;
	if (error_code=DLL_MW_SetHook()) {
		FreeLibrary(mw_hDLL);
		return error_code;
	}
	if (!(NFK_Window = FindWindow(NULL, "Need For Kill")))
		return 4; 
	return 0;
}

void MW_RemoveHook (void) {
	if (mw_hook->value()) {
		DLL_MW_RemoveHook();
		FreeLibrary(mw_hDLL);
		return;
	}
}
 
void MW_HOOK_ShutDown()
{
	MW_RemoveHook();
}

void MW_HOOK_RegisterConsoleCommands()
{
	//Регистрируем консольные команды внутри PQR Mod
	mw_hook = new c_var_bool("MouseWare Hook","mw_hook",false,mw_hook_OnChange);
	
	//Регистрируем консольные команды в NFK.exe
	fnP_DLL_RegisterConsoleCommand("mw_hook");
}
/*-------------------------------------------
*	EVENT_MainLoop()
*	Основной цикл этого модуля
*-------------------------------------------*/
void MW_HOOK_EVENT_MainLoop()
{
	if (mw_hook->value()) {
		unsigned char buttons = DLL_GetButtons();
		switch (buttons ^ old_buttons) {
			case K_MOUSE5:	Key_Event(K_MOUSE5, buttons > old_buttons ? true : false); break;
			case K_MOUSE6:	Key_Event(K_MOUSE6, buttons > old_buttons ? true : false); break;
			case K_MOUSE4:	Key_Event(K_MOUSE4, buttons > old_buttons ? true : false); break;
			case K_MOUSE7:	Key_Event(K_MOUSE7, buttons > old_buttons ? true : false); break;
			case K_MOUSE8:	Key_Event(K_MOUSE8, buttons > old_buttons ? true : false); break;
			default:	break;
		}
		old_buttons = buttons;
	}
}

void Key_Event(BYTE mKey,bool status)
{	
	char* str;
	switch(mKey) {
		case K_MOUSE5:	str = (status?KEY_Mouse5.GetCmdDown():KEY_Mouse5.GetCmdUp()); break;
		case K_MOUSE6:	str = (status?KEY_Mouse6.GetCmdDown():KEY_Mouse6.GetCmdUp()); break;
		case K_MOUSE4:	str = (status?KEY_Mouse4.GetCmdDown():KEY_Mouse4.GetCmdUp()); break;
		case K_MOUSE7:	str = (status?KEY_Mouse7.GetCmdDown():KEY_Mouse7.GetCmdUp()); break;
		case K_MOUSE8:	str = (status?KEY_Mouse8.GetCmdDown():KEY_Mouse8.GetCmdUp()); break;
		default: Console.print("^1Error: Unknown MouseWare Key");	return;
	}
	if (str!=NULL) Console.ParseAlias(str); //Команда может быть составной (т.е. альяс) ParseAlias разделяет её на куски и вызывает по отдельности
}

void mw_hook_OnChange(char* new_val)
{
	bool new_val_bool;
	bool result = true;
	//Переводим строку new_val в переменную типа bool new_val_bool
	//Если перевод невозможен, то флаг result становится false
	if (strcmp(new_val,"true")==0 || strcmp(new_val,"on")==0 || strcmp(new_val,"0")) new_val_bool = true;
	else if (strcmp(new_val,"false")==0 || strcmp(new_val,"off")==0 || strcmp(new_val,"0")==0) new_val_bool = false;
	else result=false;
	
	if ( result )
	{
		if ( new_val_bool ) //Попытка включить MW Hook
		{
			if ( mw_hook->value() ) //Уже включён
				mw_hook->print(); //выводим текущее состояние = включён
			else
			{
				int ec;
				ec = MW_SetHook();
				if ( ec ) //Ошибка
					Console.printf("mw_hook sets to 0, MouseWare Hook ^1failed, error code: %i",ec);
				else
				{
					mw_hook->set_value_bool(true); //Включаем
					mw_hook->print(); //выводим текущее состояние = включён
				}
			}
		}
		else //Попытка вЫключить MW Hook
		{
			MW_RemoveHook();
			mw_hook->set_value_bool(false); //ВЫключаем
			mw_hook->print(); //выводим текущее состояние = вЫключён
		}
		TOOLS_MENU_Update();
	}
	else //Перевод текстового параметра невозможен в тип bool
		mw_hook->print(); //выводим текущее состояние	
}