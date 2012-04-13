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

#include "graph.h"

CLogo Logo(75);
void CMD_ShowLogo(char* s); //при вызове консольной команды logo

void GRAPH_RegisterConsoleCommands()
{
	//Регистрируем консольные команды внутри PQR Mod
	c_cmd* s_logo = new c_cmd("logo","logo",CMD_ShowLogo);
	
	//Регистрируем консольные команды в NFK.exe
	fnP_DLL_RegisterConsoleCommand("logo");	
}

/*-------------------------------------------
*	EVENT_MainLoop()
*	Основной цикл этого модуля
*-------------------------------------------*/
void GRAPH_EVENT_MainLoop()
{
	Logo.Draw();	
}

void CImg::Draw()
{
	if (!visible)  return;
	
	TickCount++;
	if (TickCount>Time) visible=false;
}

void CLogo::Draw()
{
	CImg::Draw();
	if (!visible) return;
	
	fnP_DLL_FX_FillRect(100,100,440,100,0x990000FF,0x102,false);
	fnP_DLL_FX_FillRectMap(150,300,320,150,490,300,320,450,0x990000FF,0x102,false);
	fnP_DLL_FX_FillRectMap(200,300,320,200,440,300,320,400,0x77000000,0x102,false);
	fnP_DLL_FX_FillRectMapEx(330,310,331,309,420,450,520,350,0x440000FF,0x440000FF,0xFF0000FF,0xFF0000FF,0x102,false);

	fnP_DLL_ExtendedTextout(278,265,6,false,"PQR MOD");
}

void CMD_ShowLogo(char* s) //при вызове консольной команды logo
{
	int t;
	if (s==NULL || strlen(s)==0) t = 75;
	else t = atoi(s); //Параметр - количество кадров для отображения
	if (t<=0) t=75;
	Logo.Show(t);
}