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
#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "pqrmod.h"
#include "map.h"

class CPlayer {
protected:
	bool dead;
	bool bot;
	bool crouch;
	bool balloon;
	bool flagcarrier;
	bool have_rl;
	bool have_gl;
	bool have_rg;
	bool have_bfg;
	bool have_ssg;
	bool have_mg;
	bool have_lg;
	bool have_pg;
	BYTE refire;
	BYTE weapchg;
	BYTE weapon;
	BYTE threadweapon;
	BYTE dir;
	BYTE gantl_state;
	BYTE air;
	BYTE team;
	BYTE item_quad;
	BYTE item_regen;
	BYTE item_battle;
	BYTE item_flight;
	BYTE item_haste;
	BYTE item_invis;
	BYTE ammo_mg;
	BYTE ammo_ssg;
	BYTE ammo_gl;
	BYTE ammo_rl;
	BYTE ammo_lg;
	BYTE ammo_rg;
	BYTE ammo_pg;
	BYTE ammo_bfg;
	double x;
	double y;
	double cx;
	double cy;
	double fangle;
	double InertiaX;
	double InertiaY;
	int health;
	int armor;
	int frags;
	char netname[32];
	char nfkmodel[32];
    char location[66];
	WORD DXID;

	bool event_death;
	bool event_flagdeath;
	bool event_respawn;
	BYTE event_weaponchange;
	bool event_change_frags;

public:
	CPlayer(TPlayerEx);
	WORD GetDXID();
	BYTE GetTeam();
	char* GetNetname();
	BYTE GetAmmo(BYTE wn);
	char* GetLocation();
	BYTE GetWapon();
	int GetFrags() {return frags;}
	bool IsDead();
	int Health();
	int Armor();
	double GetX();
	double GetY();
	void UpdatePlayer(TPlayerEx);
	void UpdateNetName(char* str);
	void UpdateModel(char* str);
	void UpdateLocation(char* str);
	void PrintHealth(int hx, int hy, int ax, int ay, int fnt=1, char color='7', bool camera=false); //Выводит жизни и броню в указанных координатах
	void PrintHealth(int fnt=1, char color='7'); //Выводит жизни и броню под моделью игрока

	bool is_event_death() {return event_death;}
	bool is_event_flagdeath() {return event_flagdeath;}
	bool is_event_respawn() {return event_respawn;}
	bool is_event_change_frags() {return event_change_frags;}
	BYTE is_event_weaponchange() {return event_weaponchange;}
	bool have_weapon(BYTE wn);
	bool have_item(BYTE item_code);
	bool TouchRegion(int bx,int by,int width,int height); //Вовращает true если игрок касается бриковой области с ВП углом x,y и размером width, height
	CItem PointItem(); //Возвращает код ближайшего предмета на который смотрит игрок
};

typedef Item<CPlayer*> ItemPlayer;
typedef List<ItemPlayer> ListPlayers;

extern ListPlayers players;
extern CPlayer* LocalPlayer;
extern CPlayer* LocalPlayer2;
extern WORD LocalDXID;
extern WORD LocalDXID2;;

CPlayer* FindPlayerByDXID(WORD DXID);

#endif //__PLAYER_H__