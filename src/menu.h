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
	����� ������������� ������ menu � �������� ���������
	1. ������� ������ ���� CModMenu MyMenu
	2. ��������� ������ ��� ������� MyMenu.AddOption("�����");
	3. ������� ���������� ����������� �� ������ MyMenu.SetPosition(x,y);
	5. � ������ ������ ������� ��� ������� MyMenu.Show();
	��������� �� ���� �������� � ����� ����������� ������ ������������ ���� � ��������� ��������.
	7. ���� ���������� ������ ���� � ������, ��������� MyMenu.Hide();
	��� ���� ��������� �� ������ ���� ��������� �� ����������� ������ ������������ ����. ��������
	���������� �������������� � ������ ���������.
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
	ListStrings m_menu_option; //������ ����� ����
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
	void SetOption(BYTE,char*);//������������ ����� � �������� �������
	void SetOptionColor(BYTE,char);
	void AddOption(char*); //��������� ����� ����� � ����
	void Show();//������ ���� ������������ �� ������
	void Draw(); //������������ ���� �� ������. �� �������� ��������� ��� ���� �������������� ����� void DrawMenus()
	bool MoveDown(); //�������� ��������� ����� ����
	bool MoveUp(); //�������� ���������� ����� ����
	void Hide(); //������ ���� �������. ��������� �������� ���� ������������ �� last_active.
	void SetPosition(int _x,int _y) {x=_x;y=_y;} //������������ ���������� �� ������ ��� ����������� ����
	void SetWidth(int _w) {Width=_w;} //������, ���������� ��� ����������� �����
	void SetVisible(bool _v=true) {visible=_v;}
	bool IsVisible() {return visible;}
	bool SelectByXY(int,int); //�������� ����� ���� ��� ������������ x, y (��� ������ � �����)
	int GetOptionNumber(); //���������� ����� �������� ������ ����, ��������� � 1.
	TRect GetOptionRect(int num); //���������� ������������ ������ ���� ��� ������� i, ��������� � 1.
	int GetMenuHeight(); //��������� ������ ����
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
void MenuMoveDown(char*); //���������� ��������� �� ������� ���� � ��������� ����
void MenuMoveUp(char*); //���������� ��������� �� ������� ����� � ��������� ����
void MENU_CMD_MenuHide(char*);
void MENU_CMD_MenuShow(char*);
//�������� char* �� ������������. �� ����� ��� ������������� � ����� fnOnExecute ������ Console

void MenuSubMenuShow(); //������������� ��� �������� ������ input - ���������� ��� ������� VK_RIGHT
void MenuClick(int _x,int _y); //������������� ��� �������� ������ input - ���������� ��� ����� ���
bool MenuSelectByXY(int _x,int _y); //�������������� ��� �������� ������ input - �������� ����� ���� ��� �������� �� ����������� x,y

#endif