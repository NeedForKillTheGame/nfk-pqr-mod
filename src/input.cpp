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
#include "input.h"

#include "menu.h"

void Mouse1_KeyDown();
void KEY_Return_KeyDown();
void KEY_Up_KeyDown();
void KEY_Down_KeyDown();
void KEY_Left_KeyDown();
void KEY_Right_KeyDown();

void KEY_Array_KeyDown();
void KEY_Array_KeyUp();

CInput Input;
CMouse Mouse;

CKey KEY_Mouse1(250,Mouse1_KeyDown);
CKey KEY_Mouse2(251);
CKey KEY_Mouse3(252);

CKey KEY_MWheelUp(253);
CKey KEY_MWheelDown(254);

CKey KEY_Return(VK_RETURN,KEY_Return_KeyDown);
CKey KEY_Up(VK_UP,KEY_Up_KeyDown);
CKey KEY_Down(VK_DOWN,KEY_Down_KeyDown);
CKey KEY_Left(VK_LEFT,KEY_Left_KeyDown);
CKey KEY_Right(VK_RIGHT,KEY_Right_KeyDown);
CKey KEY_Home(VK_HOME);
CKey KEY_End(VK_END);
CKey KEY_Shift(VK_SHIFT);
CKey KEY_Ctrl(VK_CONTROL);
CKey KEY_Tab(VK_TAB);
CKey KEY_Space(VK_SPACE);
CKey KEY_BackSpace(VK_BACK);
CKey KEY_PgUp(VK_PRIOR);
CKey KEY_PgDown(VK_NEXT);

CKey KEY_Mouse4;
CKey KEY_Mouse5;
CKey KEY_Mouse6;
CKey KEY_Mouse7;
CKey KEY_Mouse8;

CKey KEY_Array[63];

CMouse::CMouse()
{
	x=320;
	y=240;
	visible = false;
}

POINT CMouse::GetPosition()
{
	POINT p;
	p.x = x;
	p.y = y;
	return p;
}

void CMouse::SetPosition(POINT pos)
{
	x = pos.x;
	y = pos.y;
}

void CMouse::Update()
{
	POINT p = *fnP_DLL_MouseDelta();
	x += p.x/2;
	if (x<0) x=0;
	else if (x>=VID_WIDTH) x=VID_WIDTH-1;
	y += p.y/2;
	if (y<0) y=0;
	else if (y>=VID_HEIGHT) y=VID_HEIGHT-1;

	if (p.x || p.y) MenuSelectByXY(x,y);
}

void CMouse::Draw()
{
	fnP_DLL_FX_FillRectMap(x,y,x+11,y+10,x+5,y+10,x,y+13,0x990000FF,0x102,false);
}

CKey::CKey(int _c, fnOnClick _dwn, fnOnClick _up)
{
	cmd_up		= NULL;
	cmd_down	= NULL;
	OnKeyDown	= NULL;
	OnKeyUp		= NULL;
	IsPushed	= false;
	code		= _c;
	OnKeyDown	= _dwn;
	OnKeyUp		= _up;
}

CKey::~CKey()
{
	if (cmd_up!=NULL) delete cmd_up;
	if (cmd_down!=NULL) delete cmd_down;
}
void CKey::SetCode(int _c)
{
	code = _c;
}

void CKey::SetCmd(char *cmd)
{
	if (cmd_up != NULL) {delete cmd_up;cmd_up=NULL;}
	if (cmd_down != NULL) {delete cmd_down;cmd_down=NULL;}
	if (cmd==NULL) return;
	if (strlen(cmd)==2)
		if (cmd[0]=='"' && cmd[1]=='"') return;
	cmd_down = new char[strlen(cmd)+1];
	strcpy(cmd_down,cmd);
	if (cmd[0]=='+')
	{
		cmd_up = new char[strlen(cmd)+1];
		strcpy(cmd_up,cmd);
		cmd_up[0]='-';
	}
}
char* CKey::GetCmdUp()
{
	return cmd_up;
}
char* CKey::GetCmdDown()
{
	return cmd_down;
}
void CKey::Update()
{
	if (!code) return; //У кнопки не иницилизирован код, невозможно обновить статус нажатия
	bool NewSatus = fnP_DLL_KeyPressed(code);
	if (IsPushed && !NewSatus) {if (OnKeyUp!=NULL) OnKeyUp(); else DefaultOnKeyUp();}
	else if (!IsPushed && NewSatus) {if (OnKeyDown!=NULL) OnKeyDown(); else DefaultOnKeyDown();}
	IsPushed = NewSatus;
}

void CKey::DefaultOnKeyDown()
{
	if (cmd_down!=NULL) Console.ParseAlias(cmd_down);
}
void CKey::DefaultOnKeyUp()
{
	if (cmd_up!=NULL) Console.ParseAlias(cmd_up);
}

CInput::CInput()
{
}

void CInput::Init() //Инициализация, вызывается из PQRMOD_init() - сразу после подключения pqrmod.dll
{
	RegisterKey(&KEY_Mouse1);
	RegisterKey(&KEY_Mouse2);
	RegisterKey(&KEY_Mouse3);

	RegisterKey(&KEY_MWheelUp);
	RegisterKey(&KEY_MWheelDown);

	RegisterKey(&KEY_Return);
	RegisterKey(&KEY_Up);
	RegisterKey(&KEY_Down);
	RegisterKey(&KEY_Left);
	RegisterKey(&KEY_Right);

	RegisterKey(&KEY_Return);
	RegisterKey(&KEY_Up);
	RegisterKey(&KEY_Down);
	RegisterKey(&KEY_Left);
	RegisterKey(&KEY_Right);
	RegisterKey(&KEY_Home);
	RegisterKey(&KEY_End);
	RegisterKey(&KEY_Shift);
	RegisterKey(&KEY_Ctrl);
	RegisterKey(&KEY_Tab);
	RegisterKey(&KEY_Space);
	RegisterKey(&KEY_BackSpace);
	RegisterKey(&KEY_PgUp);
	RegisterKey(&KEY_PgDown);

	for(int i=33;i<96;i++)
	{
		KEY_Array[i-33].SetCode(i);
		RegisterKey(KEY_Array+i-33);
	}
}
/*-------------------------------------------
*	EVENT_MainLoop()
*	Основной цикл этого модуля
*-------------------------------------------*/
void CInput::EVENT_MainLoop()
{
	if (Mouse.IsVisible())
	{
		Mouse.Update();
		Mouse.Draw();
	}
	
	if (Keys.size() == 0) return;
	Keys.top();
	do
	{
		Keys.cur()->d->Update();
	}
	while(Keys.next());
}

void CInput::RegisterKey(CKey* K)
{
	ItemKey* NewKey = new ItemKey(K);
	Keys.addAftLas(NewKey);
}

void Mouse1_KeyDown()
{
	POINT p = Mouse.GetPosition();
	MenuClick( p.x , p.y );
	KEY_Mouse1.DefaultOnKeyDown();
}

void KEY_Return_KeyDown()
{
	MenuEnter(NULL);
	KEY_Return.DefaultOnKeyDown();
}
void KEY_Up_KeyDown()
{
	MenuMoveUp(NULL);
	KEY_Up.DefaultOnKeyDown();
}
void KEY_Down_KeyDown()
{
	MenuMoveDown(NULL);
	KEY_Down.DefaultOnKeyDown();
}
void KEY_Left_KeyDown()
{
	MENU_CMD_MenuHide(NULL);
	KEY_Left.DefaultOnKeyDown();
}
void KEY_Right_KeyDown()
{
	MenuSubMenuShow();
	KEY_Right.DefaultOnKeyDown();
}



/*-------------------------------------------
* СПРАВКА

VK_LBUTTON      = 01H
VK_RBUTTON      = 02H
VK_CANCEL       = 03H
VK_BACK         = 08H
VK_TAB          = 09H
VK_CLEAR        = 0cH
VK_RETURN       = 0dH
VK_SHIFT        = 10H
VK_CONTROL      = 11H
VK_MENU         = 12H
VK_PAUSE        = 13H
VK_CAPITAL      = 14H
VK_ESCAPE       = 1bH
VK_SPACE        = 20H

VK_PRIOR        = 21H
VK_NEXT         = 22H
VK_END          = 23H
VK_HOME         = 24H
VK_LEFT         = 25H
VK_UP           = 26H
VK_RIGHT        = 27H
VK_DOWN         = 28H

[21:13:48] [_3d[Power]] mScrollUp : byte = 253;
[21:13:48] [_3d[Power]] mScrollDn : byte = 254;

-----------------------------------------------*/
