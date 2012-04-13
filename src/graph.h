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
#ifndef __GRAPH_H__
#define __GRAPH_H__
#include "pqrmod.h"

void GRAPH_EVENT_MainLoop();
void GRAPH_RegisterConsoleCommands();

class CImg
{
protected:
	bool visible;
	int Time;
	int TickCount;
public:
	CImg(int t): visible(false), Time(t), TickCount(0) {}
	void Show(int t) { Time=t; visible=true; }
	virtual void Draw();

};

class CLogo : public CImg
{
public:
	CLogo(int t): CImg(t) {}
	virtual void Draw();
};

extern CLogo Logo;

#endif //__GRAPH_H__