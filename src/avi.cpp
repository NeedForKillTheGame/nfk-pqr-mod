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

#include "avi.h"

void CMD_Avi_start(char* s);
void CMD_Avi_stop(char* s);

CAvi AVI;

CAvi::CAvi()
{
}

void CAvi::RegisterConsoleCommands()
{
	//Регистрируем консольные команды внутри PQR Mod
	c_cmd* avi_start = new c_cmd("avi_start","avi_start",CMD_Avi_start);
	c_cmd* avi_stop = new c_cmd("avi_stop","avi_stop",CMD_Avi_stop);

	//Регистрируем консольные команды в NFK.exe
	fnP_DLL_RegisterConsoleCommand("avi_start");
	fnP_DLL_RegisterConsoleCommand("avi_stop");
}

void CMD_Avi_start(char* s)
{
	Console.print("CMD_Avi_start");
	Console.SendConsoleHCommand("hidep2statusbar");
	Console.SendConsoleHCommand("drawtime 0");
    Console.SendConsoleHCommand("cl_avidemo");
}
void CMD_Avi_stop(char* s)
{
	Console.print("CMD_Avi_stop");
	Console.SendConsoleHCommand("cl_avidemo");
	Console.SendConsoleHCommand("drawtime 1");
}