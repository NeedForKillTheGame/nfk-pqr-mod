#ifndef __PROC_HACK_H__
#define __PROC_HACK_H__
#include "list.h"
#include "types.h"
#include "pqrmod.h"

#define PH_X 356
#define PH_Y 364
#define PH_DIRECTION 12
#define PH_ANGLE 68
#define PH_HEALTH 72
#define PH_ARMOR 76
#define PH_AMMO_MG 28
#define PH_AMMO_SSG 29
#define PH_AMMO_GL 30
#define PH_AMMO_RL 31
#define PH_AMMO_LG 32
#define PH_AMMO_RG 33
#define PH_AMMO_PG 34
#define PH_AMMO_BFG 35

bool PH_IsActive();
BYTE PH_GetVar(int);

void PROC_HACK_EVENT_NFKStarted();

void* GetPlayersArrayPointer(); //Возвращает указатель на начало массива игроков в NFK.exe
bool PH_UpdatePlayerEX(WORD);
bool PH_SetPlayerAngle(WORD,double);
void* PH_GetPlayersArrayPointer();

typedef struct {
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
		WORD DXID;
} TPlayerEx2;

extern TPlayerEx2 PlayerEX; //Переменная, которая заполняется данными из памяти NFK.exe по вызову UpdatePlayerEX(DXID);
extern char* PlayerEX_nfkmodel;
extern char* PlayerEX_netname;
extern char* PlayerEX_Location;

#endif //__PROC_HACK_H__