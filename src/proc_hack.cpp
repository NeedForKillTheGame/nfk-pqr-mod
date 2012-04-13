#include "proc_hack.h"
#include "dll_register.h"

#define NFK065_BASE 0x007564CC

void* p=NULL;
TPlayerEx2 PlayerEX;
char* PlayerEX_nfkmodel;
char* PlayerEX_netname;
char* PlayerEX_Location;

void* PH_GetPlayersArrayPointer()
{
	return p;
}

void PROC_HACK_EVENT_NFKStarted()
{
	PlayerEX_nfkmodel = new char[31];
	PlayerEX_netname = new char[31];
    PlayerEX_Location = new char[65];
	
	p = NULL;
	if (!strcmp(NFKVersion,"064")) p = (void*)NFK065_BASE;
	else p = NULL;

	if (p==NULL)
	{
		char* path = new char[255];
		char* p1;
	
		strcpy(path,NFKGameDir);
		//p1 = strstr(path,"basenfk");
		//p1[0] = '\0';
		strcat(path,"\\ph.dat");

		//DEBUG PRINT
		//fnP_DLL_AddMessage(path);

		FILE* f = fopen(path,"rt");
		delete path;
		if (f!=NULL)
		{	
			char str[1024];
			unsigned BA=0;
			while(fgets(str,1024,f))
			{
				p1=NULL;
				p1 = strstr(str,"\n");
				if (p1!=NULL) p1[0] = '\0';
				p1 = strstr(str,"\r");
				if (p1!=NULL) p1[0] = '\0';

				fnP_DLL_AddMessage("str");
				fnP_DLL_AddMessage(str);

				if (strlen(str)>=10) //Минимальное необходимое условие для строки с настройкой
				{
					//Разбираем прочитанную строку по словам через пробел и сохраняем в списке Params
					p1 = strtok(str," ");
					
					//DEBUG PRINT
					//fnP_DLL_AddMessage("p1");
					//fnP_DLL_AddMessage(p1);

					if (!strcmp(p1,NFKVersion))
					{
						p1 = strtok(NULL," ");
						//DEBUG PRINT
						fnP_DLL_AddMessage("p1");
						fnP_DLL_AddMessage(p1);
						BA=atoi(p1);
						if (BA!=0) {p = (void*)BA; break;}
					}
				}
			}
		}
	}

	if (p==NULL)
		if (!strcmp(NFKVersion,"064")) p = (void*)NFK065_BASE;

	if (p!=NULL) fnP_DLL_SetBasePointer(p);
}

bool PH_IsActive()
{
	if (p==NULL) return false;
	return true;
}


BYTE PH_GetVar(int sm) //Возвращает значение перменной типа BYTE по смещению внутри первого игрока
{
	if (p==NULL) return 0;
	void* p2 = NULL;
	BYTE h=0;
	unsigned long l[1];
	l[0]=4;

	p2 = (void*)((unsigned*)p)[0];
	return ((char*)p2+sm)[0];
}

bool PH_UpdatePlayerEX(WORD _DXID)
{
	return fnP_DLL_UpdatePlayerEX(_DXID,0);
}
bool PH_SetPlayerAngle(WORD _DXID,double a)
{
	if (p==NULL) return false;
	return fnP_DLL_SetAngleEX(_DXID,0,a);
}
PQRMOD_API void C_DLL_UpdatePlayerEX(TPlayerEx2 player,char* s1,char* s2,char* s3) //Получает полную информацию об игроке путём хака памяти
{
	PlayerEX.dead = player.dead;
	PlayerEX.bot = player.bot;
	PlayerEX.crouch = player.crouch;
	PlayerEX.balloon = player.balloon;
	PlayerEX.flagcarrier = player.flagcarrier;
	PlayerEX.have_rl = player.have_rl;
	PlayerEX.have_gl = player.have_gl;
	PlayerEX.have_rg = player.have_rg;
	PlayerEX.have_bfg = player.have_bfg;
	PlayerEX.have_ssg = player.have_ssg;
	PlayerEX.have_mg = player.have_mg;
	PlayerEX.have_pg = player.have_pg;
	PlayerEX.have_lg = player.have_lg;
	PlayerEX.refire = player.refire;
	PlayerEX.weapchg = player.weapchg;
	PlayerEX.weapon = player.weapon;
	PlayerEX.threadweapon = player.threadweapon;
	PlayerEX.dir = player.dir;
	PlayerEX.gantl_state = player.gantl_state;
	PlayerEX.air = player.air;
	PlayerEX.team = player.team;
	PlayerEX.item_quad = player.item_quad;
	PlayerEX.item_regen = player.item_regen;
	PlayerEX.item_battle = player.item_battle;
	PlayerEX.item_flight = player.item_flight;
	PlayerEX.item_haste = player.item_haste;
	PlayerEX.item_invis = player.item_invis;
	PlayerEX.ammo_mg = player.ammo_mg;
	PlayerEX.ammo_ssg = player.ammo_ssg;
	PlayerEX.ammo_gl = player.ammo_gl;
	PlayerEX.ammo_rl = player.ammo_rl;
	PlayerEX.ammo_lg = player.ammo_lg;
	PlayerEX.ammo_rg = player.ammo_rg;
	PlayerEX.ammo_pg = player.ammo_pg;
	PlayerEX.ammo_bfg = player.ammo_bfg;
	PlayerEX.health = player.health;
	PlayerEX.armor = player.armor;
	PlayerEX.frags = player.frags;
	PlayerEX.x = player.x;
	PlayerEX.y = player.y;
	PlayerEX.cx = player.cx;
	PlayerEX.cy = player.cy;
	PlayerEX.fangle = player.fangle;
	PlayerEX.InertiaX = player.InertiaX;
	PlayerEX.InertiaY = player.InertiaY;
	if (s1!=NULL) strcpy(PlayerEX_netname,s1); else PlayerEX_netname[0]='\0';
	if (s2!=NULL) strcpy(PlayerEX_nfkmodel,s2); else PlayerEX_nfkmodel[0]='\0';
    if (s3!=NULL) strcpy(PlayerEX_Location,s3); else PlayerEX_Location[0]='\0';
	PlayerEX.DXID = player.DXID;
}