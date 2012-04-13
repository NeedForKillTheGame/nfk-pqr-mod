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
*	types.h
*	ќсновные типы дл€ совместимости с Delphi модулем
*/
#ifndef __TYPES_H__
#define __TYPES_H__

		//bot movement
#define BKEY_MOVERIGHT 1
#define BKEY_MOVELEFT 2
#define BKEY_MOVEUP 8
#define BKEY_MOVEDOWN 16
#define BKEY_FIRE 32

		// team
#define C_TEAMBLUE 0
#define C_TEAMRED 1
#define C_TEAMNON 2

		// weapon ID
#define C_WPN_GAUNTLET 0
#define C_WPN_MACHINE 1
#define C_WPN_SHOTGUN 2
#define C_WPN_GRENADE 3
#define C_WPN_ROCKET 4
#define C_WPN_SHAFT 5
#define C_WPN_RAIL 6
#define C_WPN_PLASMA 7
#define C_WPN_BFG 8

        // model direction & current animation status.
#define DIR_LW = 0 // walkin left
#define DIR_RW = 1 // walkin right
#define DIR_LS = 2 // standin left
#define DIR_RS = 3 // standin right

        // values returned by GetBrickStruct() ... (TBrick.image)
#define IT_NONE 0
#define IT_SHOTGUN 1
#define IT_GRENADE 2
#define IT_ROCKET 3
#define IT_SHAFT 4
#define IT_RAIL 5
#define IT_PLASMA 6
#define IT_BFG 7
#define IT_AMMO_MACHINEGUN 8
#define IT_AMMO_SHOTGUN 9
#define IT_AMMO_GRENADE 10
#define IT_AMMO_ROCKET 11
#define IT_AMMO_SHAFT 12
#define IT_AMMO_RAIL 13
#define IT_AMMO_PLASMA 14
#define IT_AMMO_BFG 15
#define IT_SHARD 16
#define IT_YELLOW_ARMOR 17
#define IT_RED_ARMOR 18
#define IT_HEALTH_5 19
#define IT_HEALTH_25 20
#define IT_HEALTH_50 21
#define IT_HEALTH_100 22 // megahealth
#define IT_POWERUP_REGENERATION 23
#define IT_POWERUP_BATTLESUIT 24
#define IT_POWERUP_HASTE 25
#define IT_POWERUP_QUAD 26
#define IT_POWERUP_FLIGHT 27
#define IT_POWERUP_INVISIBILITY 28
#define IT_TRIX_GRENADE 29 // will never used by bot.dll
#define IT_TRIX_ROCKET 30 // will never used by bot.dll
#define IT_LAVA 31
#define IT_WATER 32
#define IT_DEATH 33
#define IT_RESPAWN 34 // not used. use GetBrickStruct() to handle this
#define IT_RED_RESPAWN 35 // not used. use GetBrickStruct() to handle this
#define IT_BLUE_RESPAWN 36 // not used. use GetBrickStruct() to handle this
#define IT_EMPTY 37
#define IT_JUMPPAD 38
#define IT_JUMPPAD2 39 // strong
#define IT_BLUE_FLAG 40
#define IT_RED_FLAG 41
#define IT_DOMINATION_FLAG 42 // use GetBrickStruct() to get color.

// ==========================================
//BotPatch defines

#define BP_AIR 1
#define BP_HEALTH 2
#define BP_ARMOR 3
#define BP_DO_NOT_RESPAWN_BOTS 4
#define BP_HAVE_SHOTGUN 5
#define BP_HAVE_GRENADE 6
#define BP_HAVE_ROCKET 7
#define BP_HAVE_SHAFT 8
#define BP_HAVE_RAIL 9
#define BP_HAVE_PLASMA 10
#define BP_HAVE_BFG 11
#define BP_AMMO_MACHINEGUN 12
#define BP_AMMO_SHOTGUN 13
#define BP_AMMO_GRENADE 14
#define BP_AMMO_ROCKET 15
#define BP_AMMO_SHAFT 16
#define BP_AMMO_RAIL 17
#define BP_AMMO_PLASMA 18
#define BP_AMMO_BFG 19
#define BP_POWERUP_REGENERATION 20	// amount in seconds
#define BP_POWERUP_BATTLESUIT 21
#define BP_POWERUP_HASTE 22
#define BP_POWERUP_QUAD 23
#define BP_POWERUP_FLIGHT 24
#define BP_POWERUP_INVISIBILITY 25
#define BP_DEAD 26
#define DP_POS_X 27
#define DP_POS_Y 28
#define DP_INERTIA_X 29
#define DP_INERTIA_Y 30

//===============================================
//fnP_DLL_GetSpecObjStruct().objname_code

#define OC_flag 1
#define OC_weapon 2
#define OC_grenade 3
#define OC_rocket 4
#define OC_plasma 5
#define OC_shaft2 6
#define OC_blood 7
#define OC_smoke 8
#define OC_shotgun 9 
#define OC_gauntlet 10 
#define OC_machine 11 
#define OC_rail 12
#define OC_gib 13
#define OC_shots 14
#define OC_shots2 15
#define OC_bubble 16
#define OC_flash 17

#define MAX_BOTS 16
#define MAX_LEN 64

typedef struct {
	char FilesCount;
	bool NewStatusOnOff[MAX_BOTS];
	bool StatusOnOff[MAX_BOTS];
	char FilesArray[MAX_BOTS][MAX_LEN]; //«начени€ размера массива зав€заны на bot.dll Ќ≈ ћ≈Ќя“№
} TBotsDllArray;

typedef struct {
	int x1,y1,x2,y2;
} TRect;

typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef unsigned Cardinal;
typedef struct {
        BYTE dead;
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
        char netname[31];
		char nfkmodel[31];
        char Location[65];
        WORD DXID;
} TPlayerEx;

typedef struct {
        BYTE img;        // graphix index
        bool block;       // do this brick block player;
		int respawntime; // respawn time
        char y;
        BYTE dir;
        double oy;
        bool respawnable; // is this shit can respawn?
        BYTE scale;
} TBrick;

		// object structure. (eg Rockets; blood; everything!)
typedef struct {
        BYTE dead;
        BYTE speed;
		BYTE fallt;
		BYTE weapon;
		BYTE doublejump;
		BYTE refire;
        BYTE imageindex;
		BYTE dir;
		BYTE idd;
		short clippixel;
        WORD spawnerDXID;
        BYTE frame;
        short health;
        double x;
		double y;
		double cx;
		double cy;
		double fangle;
		double fspeed;
		BYTE objname_code;
        WORD DXID;
        double mass;
		double InertiaX;
		double InertiaY;
} TObj;

        // special object structure. (eg Doors; Buttons)
typedef struct {
        bool active;
        WORD x;
		WORD y;
		WORD lenght;
		WORD dir;
		WORD wait;
        WORD targetname;
		WORD target;
		WORD orient;
		WORD nowanim;
		WORD special;
        BYTE objtype;
} TSpecObj;


#endif //__TYPES_H__