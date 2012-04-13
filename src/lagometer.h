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
#ifndef __LAGOMETER_H__
#define __LAGOMETER_H__
#include "pqrmod.h"

typedef Item<int> ItemPing;
typedef List<ItemPing> ListPingRing;

class CLagometer
{
	int timer;
public:
	CLagometer();

	void EVENT_MainLoop();
	void RegisterConsoleCommands();

	ListPingRing PingRing;
};

extern CLagometer Lagometer;

extern c_var_int* cg_lagometer;
extern c_var_int* cg_drawping;

#endif //__LAGOMETER_H__