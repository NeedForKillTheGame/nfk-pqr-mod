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
#include "lagometer.h"

#define PING_MIN 5
#define PING_GREEN 35
#define PING_YELLOW 110

CLagometer Lagometer;
c_var_int* cg_lagometer;
c_var_int* cg_drawping;
c_var_int* ch_ping_x;
c_var_int* ch_ping_y;
c_var_int* ch_ping_yellow;
c_var_int* ch_ping_green;
c_var_int* ch_minping;
c_var_int* ch_lagometer_x;
c_var_int* ch_lagometer_y;
c_var_int* ch_lagometer_alpha;


CLagometer::CLagometer()
{
	ItemPing* P;
	for(int i=0;i<11;i++)
	{
		P = new ItemPing(0);
		PingRing.addAftLas(P);
	}
	timer = 0;
}

void CLagometer::RegisterConsoleCommands()
{
	cg_lagometer = new c_var_int("cg_lagometer","cg_lagometer",0,0,2);
	cg_drawping = new c_var_int("cg_drawping","cg_drawping",0,0,2);
	ch_ping_x = new c_var_int("ch_ping_x","ch_ping_x",608,0,VID_WIDTH);
	ch_ping_y = new c_var_int("ch_ping_y","ch_ping_y",410,0,VID_HEIGHT);
	ch_ping_yellow = new c_var_int("ch_ping_yellow","ch_ping_yellow",PING_YELLOW,0,1000);
	ch_ping_green = new c_var_int("ch_ping_green","ch_ping_green",PING_GREEN,0,1000);
	ch_minping = new c_var_int("ch_minping","ch_minping",PING_MIN,0,1000);
	ch_lagometer_x = new c_var_int("ch_lagometer_x","ch_lagometer_x",608,0,VID_WIDTH);
	ch_lagometer_y = new c_var_int("ch_lagometer_y","ch_lagometer_y",428,0,VID_HEIGHT);
	ch_lagometer_alpha = new c_var_int("ch_lagometer_alpha","ch_lagometer_alpha",120,0,255);
	
	fnP_DLL_RegisterConsoleCommand("cg_lagometer");
	fnP_DLL_RegisterConsoleCommand("cg_drawping");
	fnP_DLL_RegisterConsoleCommand("ch_lagometer_x");
	fnP_DLL_RegisterConsoleCommand("ch_lagometer_y");
	fnP_DLL_RegisterConsoleCommand("ch_lagometer_alpha");
	fnP_DLL_RegisterConsoleCommand("ch_ping_x");
	fnP_DLL_RegisterConsoleCommand("ch_ping_y");
	fnP_DLL_RegisterConsoleCommand("ch_ping_yellow");
	fnP_DLL_RegisterConsoleCommand("ch_ping_green");
	fnP_DLL_RegisterConsoleCommand("ch_minping");
}

void CLagometer::EVENT_MainLoop()
{
	timer++;
	if (timer>=50) //Добавляем новое значение пинга
	{
		timer = 0;
		int cur_ping = 0;
		char *tmp_str = fnP_DLL_GetSystemVariable("localping");
		if (tmp_str!=NULL && strlen(tmp_str)!=0)
		{
			cur_ping = atoi(tmp_str);	
		}
		PingRing.cnext();
		PingRing.cur()->d = cur_ping;
	}

	if (cg_drawping->value()==1 || (cg_drawping->value()==2 && PingRing.cur()->d >= ch_minping->value())) //Выводим значение пинга
	{
		fnP_DLL_ExtendedTextout(ch_ping_x->value(),ch_ping_y->value(),1,false,va("%d ms",PingRing.cur()->d));
	}

	if (!cg_lagometer->value() || (cg_lagometer->value()==2 && PingRing.cur()->d<ch_minping->value())) return;

	Cardinal cl;
	if (PingRing.cur()->d < ch_ping_green->value()) cl = 0xFF00+(ch_lagometer_alpha->value()<<24);
	else if (PingRing.cur()->d < ch_ping_yellow->value()) cl = 0xFFFF+(ch_lagometer_alpha->value()<<24);
	else cl = 0xFF+(ch_lagometer_alpha->value()<<24);
	
	fnP_DLL_FX_Rectangle(ch_lagometer_x->value(),ch_lagometer_y->value(),32,32,cl,0x00000000,0x102,false); //Рамка лагометра
	
	PingRing.cnext();
	int y1,y2,x1,x2;
	y1 = ch_lagometer_y->value()+31-(PingRing.cur()->d/10); 
	if (y1 < (ch_lagometer_y->value()+1)) y1 = ch_lagometer_y->value() + 1;

	for(int i=1;i<=10;i++) //Отрисовываем все значения пинга
	{
		PingRing.cnext();
		  
		x2 = ch_lagometer_x->value() + i*3 + 1;
		x1 = x2-3; //Лево

		y2 = ch_lagometer_y->value()+31-(PingRing.cur()->d/10);
		if (y2 < (ch_lagometer_y->value()+1)) y2 = ch_lagometer_y->value() + 1;
		
		if (PingRing.cur()->d < ch_ping_green->value()) cl = 0xFF00+(ch_lagometer_alpha->value()<<24);
		else if (PingRing.cur()->d < ch_ping_yellow->value()) cl = 0xFFFF+(ch_lagometer_alpha->value()<<24);
		else cl = 0xFF+(ch_lagometer_alpha->value()<<24);

		fnP_DLL_FX_FillRectMap(x1,y1,x2,y2,x2,ch_lagometer_y->value()+31,x1,ch_lagometer_y->value()+31,cl,0x102,false);

		y1 = y2; //Запоминаем последнюю координату
	}
}