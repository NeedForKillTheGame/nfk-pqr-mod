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
	map.h
    Модуль карты. Здесь много лишнего кода, относящегося к зачаткам бота. Навигация по
	вейпоинтам и прочее.
*/

#ifndef __MAP_H__
#define __MAP_H__

#include "pqrmod.h"
#define __MAX_WAY_POINTS__ 500

// crc32
#define MAP_DM2_CRC32 "2461749679"
#define MAP_TOURNEY1_CRC32 "30"
#define MAP_TOURNEY4_CRC32 "3229379975"
#define MAP_CTF1_CRC32 "775708255"

class CLocation
{
public:
	CLocation();
	CLocation(char* str,int x,int y);
	char* text;
	int x;
	int y;
};

class CItem
{
public:
	CItem();
	CItem(int x, int y, int img);
	int mod_respawntime;
	int respawntime;
	int brick_x;
	int brick_y;
	int image;
	bool UpdateItem(); //Возвращает true, если предмет был только что взят
};

typedef Item<CItem*> ItemMapItem; 
typedef List<ItemMapItem> ListMapItems;
typedef Item<CLocation*> ItemLocation; 
typedef List<ItemLocation> ListLocations;

class CMap {
private:
	unsigned sizex;
	unsigned sizey;
public:
	CMap();
	char CRC32[20];

	void EVENT_MapChanged();
	void ScanItems();
	char name[50];
//	bool TraceVector(int,int,int,int);
	unsigned GetSizeX() {return sizex;}
	unsigned GetSizeY() {return sizey;}

	ListMapItems MapItems;
	ListLocations Locs;

	bool LoadLocs();
	char* GetLoc(int x, int y); //Возвращает ближайшую к указанным координатам (не брики) локацию
};

extern CMap m_Map;

double GetDist(int x1,int y1,int x2,int y2); //Возвращает расстояние между точками

#endif //__MAP_H__