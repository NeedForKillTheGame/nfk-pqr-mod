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

/*
*	input.h
*	Модуль следит за перемещениями мыши и генерирует сообщения при нажатии кнопок
*/

#ifndef __INPUT_H__
#define __INPUT_H__
#include "pqrmod.h"

typedef void(*fnOnClick)();

class CMouse //Объект мыши
{
	int x;
	int y;
	bool visible;
	
public:
	CMouse();
	POINT GetPosition();
	void SetPosition(POINT pos);
	void Update(); //Обновляет информацию о координатах мыши вызовом функиций NFK.exe
	void Draw(); //Рисует мышь
	bool IsVisible() {return visible;}
	void Show() {visible=true;}
	void Hide() {visible=false;}
};

class CKey
{
	char* cmd_up;
	char* cmd_down;
	bool IsPushed;
	BYTE code; //Код клавиши в терминах NFK, нужен для обновления информации о нажатии кнопок
public:
	CKey(int _c=0, fnOnClick _dwn=NULL, fnOnClick _up=NULL);
	~CKey();
	void SetCode(int _c);
	void SetCmd(char*);
	char* GetCmdUp();
	char* GetCmdDown();
	void Update();
	void DefaultOnKeyDown();
	void DefaultOnKeyUp();
	fnOnClick OnKeyDown;
	fnOnClick OnKeyUp;
};

typedef Item<CKey*> ItemKey; 
typedef List<ItemKey> ListKeys;

class CInput
{
	ListKeys Keys;
public:
	CInput();
	void Init(); //Инициализация, вызывается из PQRMOD_init() - сразу после подключения pqrmod.dll
	void EVENT_MainLoop();
	void RegisterKey(CKey* K);
};

extern CInput Input;
extern CMouse Mouse;

extern CKey KEY_Mouse1;
extern CKey KEY_Mouse2;
extern CKey KEY_Mouse3;
//Доп кнопки мышек серии Logitech MX
extern CKey KEY_Mouse4;
extern CKey KEY_Mouse5;
extern CKey KEY_Mouse6;
extern CKey KEY_Mouse7;
extern CKey KEY_Mouse8;

extern CKey KEY_MWheelUp;
extern CKey KEY_MWheelDown;

extern CKey KEY_Return;
extern CKey KEY_Up;
extern CKey KEY_Down;
extern CKey KEY_Left;
extern CKey KEY_Right;
extern CKey KEY_Home;
extern CKey KEY_End;
extern CKey KEY_Shift;
extern CKey KEY_Ctrl;
extern CKey KEY_Tab;
extern CKey KEY_Space;
extern CKey KEY_BackSpace;
extern CKey KEY_PgUp;
extern CKey KEY_PgDown;

extern CKey KEY_Array[];

#endif //__INPUT_H__