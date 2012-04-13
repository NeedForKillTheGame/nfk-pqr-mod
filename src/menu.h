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
	Схема использования модуля menu в основной программе
	1. Создать объект типа CModMenu MyMenu
	2. Заполнить список его пунктов MyMenu.AddOption("текст");
	3. Указать координаты отображения на экране MyMenu.SetPosition(x,y);
	5. В нужный момент сделать его видимым MyMenu.Show();
	Указатель на меню попадает в конец глобального списка отображаемых меню и становитя активным.
	7. Если необходимо скрыть меню с экрана, выполнить MyMenu.Hide();
	При этом указатель на данное меню удаляется из глобального списка отображаемых меню. Активным
	становится предшествующий в списке указатель.
*/

#ifndef _MENU_H_
#define _MENU_H_

#include "pqrmod.h"

#define VERTICAL_INTERVAL 20
#define LIGHT_OPTION_COLOR '#'

class CModMenu {
private:
	bool visible;
	int x,y;
	int Width;
	ListStrings m_menu_option; //Список опций меню
	long SubMenuMask;
	BYTE ExternMenuId;
public:
	CModMenu(BYTE _eid) {ExternMenuId=_eid;}
	CModMenu();
	~CModMenu();
	int GetPositionX() {return x;}
	int GetPositionY() {return y;}
	int GetWidth() {return Width;}
	void RemoveAllOptions();
	void SetOption(BYTE,char*);//переписывает пунки с заданным номером
	void SetOptionColor(BYTE,char);
	void AddOption(char*); //Добавляет новую опцию в меню
	void Show();//Делает меню отображаемым на экране
	void Draw(); //Отрисовывает меню на экране. Из основной программы все меню отрисовываются через void DrawMenus()
	bool MoveDown(); //Выбирает следующий пункт меню
	bool MoveUp(); //Выбирает предыдущий пункт меню
	void Hide(); //Делает меню скрытым. указатель текущего меню перемещается на last_active.
	void SetPosition(int _x,int _y) {x=_x;y=_y;} //Устнавливает координаты на экране для отображения меню
	void SetWidth(int _w) {Width=_w;} //Ширина, необходима для отображения рамок
	void SetVisible(bool _v=true) {visible=_v;}
	bool IsVisible() {return visible;}
	bool SelectByXY(int,int); //Выбирает пункт меню под координатами x, y (для работы с мыщью)
	int GetOptionNumber(); //Возвращает номер текущего пункта меню, нумерация с 1.
	TRect GetOptionRect(int num); //Возвращает прямоугльник пункта меню под номером i, нумерация с 1.
	int GetMenuHeight(); //Вычисляет высоту меню
	void (*fnMenuEnter)(unsigned char);
	void SetSubMenuMask(long mask) {SubMenuMask=mask;}
	bool IsSubmenuOption();
	BYTE GetExternMenuId() {return ExternMenuId;}
	void SetExternMenuId(BYTE _eid) {ExternMenuId=_eid;}
};

extern CModMenu* ExternMenus[100];
extern BYTE ExternMenusCount;

void MENU_Init();
void MENU_EVENT_MainLoop();
void MENU_RegisterConsoleCommands();

void MenuEnter(char*);
void MenuMoveDown(char*); //Перемещает указатель на строчку вниз у активного меню
void MenuMoveUp(char*); //Перемещает указатель на строчку вверх у активного меню
void MENU_CMD_MenuHide(char*);
void MENU_CMD_MenuShow(char*);
//Параметр char* не используется. Он нужен для совместимости с типом fnOnExecute модуля Console

void MenuSubMenuShow(); //Исключительно для внешнего модуля input - вызывается при нажатии VK_RIGHT
void MenuClick(int _x,int _y); //Исключительно для внешнего модуля input - вызывается при клике ЛКМ
bool MenuSelectByXY(int _x,int _y); //ВИсключительно для внешнего модуля input - выбирает пункт меню под курсором по координатам x,y

#endif