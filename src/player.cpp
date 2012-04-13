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
#include "player.h"

// #include "map.h" подключается в player.h

ListPlayers players;
extern c_var_bool* cl_triggers;

CPlayer* LocalPlayer;
CPlayer* LocalPlayer2;
WORD LocalDXID;
WORD LocalDXID2;;

BYTE CheckTakenInerval(int bx, int by, int respawntime)
{
	TBrick* b = NULL;
	for(int i=-2;i<=2;i++)
	{
		b = fnP_DLL_GetBrickStruct(bx,by+i);
		
		//DEBUG PRINT
		//Console.printf("%d",b->respawntime);
		
		if (b->respawntime==respawntime) return b->img;
	}
	
	return IT_NONE;
}

CPlayer :: CPlayer(TPlayerEx player)
{
	UpdatePlayer(player);
};
void CPlayer::UpdatePlayer(TPlayerEx player)
{
	//Сбрасываем флаги событий
	event_death = false;
	event_flagdeath = false;
	event_respawn = false;
	event_weaponchange = IT_NONE;
	event_change_frags = false;
	
	//Старый способ обнаружения события took. Не работает на MP Client
	/*
	if (cl_triggers->value() && !FirstFrameAfterReset && this == LocalPlayer && !LocalPlayer->IsDead())
	{
		//Координаты игрока в бриках
		int bx, by;
		bx = (player.x) / 32;
		if (player.crouch) by = (player.y) / 16; 
		else by = (player.y) / 16;

		//DEBUG PRINT
		//Console.printf("%d %d",bx,by);

		BYTE brick_img=0;
		if (brick_img = CheckTakenInerval(bx,by,1000)) //Событие в зятии 20сек
		{
			if ((brick_img>=1 && brick_img<=3) || (brick_img>=8 && brick_img<=14) || brick_img==6 || brick_img==16 || brick_img==19 || brick_img==20) event_took = brick_img;
		}
		else if (brick_img = CheckTakenInerval(bx,by,1500)) //Событие о взятии предметов с респауном 30сек. 
		{
			if (brick_img==5 || brick_img==17 || brick_img==18 || brick_img==21) event_took = brick_img;
		}
		else if (brick_img = CheckTakenInerval(bx,by,2000)) //Событие взятии 40sec
		{
			if (brick_img==21 || brick_img==4) event_took = brick_img;
		}
		else if (brick_img = CheckTakenInerval(bx,by,3000)) //Событие взятии MH
		{
			if (brick_img==22 || brick_img==15) event_took = brick_img;
		}
		else if (brick_img = CheckTakenInerval(bx,by,5000)) //Событие взятии BFG
		{
			if (brick_img==7) event_took = brick_img;
		}
		else if (brick_img = CheckTakenInerval(bx,by,6000)) //Событие о взятии Powerups
		{
			if (brick_img>22) event_took = brick_img;
		}
	}
	*/

	//Событие о смерти event_death и event_flagdeath
	
	if (dead != player.dead)
		if (dead == false)
			if (flagcarrier) event_flagdeath = true;
			else event_death = true;
		else event_respawn = true;
	
	dead = player.dead;
	bot = player.bot;
	crouch = player.crouch;
	balloon = player.balloon;
	flagcarrier = player.flagcarrier;

	have_rl = player.have_rl;
	have_gl = player.have_gl;
	have_rg = player.have_rg;
	have_bfg = player.have_bfg;
	have_ssg = player.have_ssg;
	have_mg = player.have_mg;
	have_pg = player.have_pg;
	have_lg = player.have_lg;
	refire = player.refire;
	weapchg = player.weapchg;

	//Событие event_weaponchange
	if (weapon != player.weapon) event_weaponchange = player.weapon;
	weapon = player.weapon;

	threadweapon = player.threadweapon;
	dir = player.dir;
	gantl_state = player.gantl_state;
	air = player.air;
	team = player.team;

	item_quad = player.item_quad;
	item_regen = player.item_regen;
	item_battle = player.item_battle;
	item_flight = player.item_flight;
	item_haste = player.item_haste;
	item_invis = player.item_invis;

	ammo_mg = player.ammo_mg;
	ammo_ssg = player.ammo_ssg;
	ammo_gl = player.ammo_gl;
	ammo_rl = player.ammo_rl;
	ammo_lg = player.ammo_lg;
	ammo_rg = player.ammo_rg;
	ammo_pg = player.ammo_pg;
	ammo_bfg = player.ammo_bfg;

	health = player.health;
	armor = player.armor;

	if (frags != player.frags) event_change_frags = true;
	
	frags = player.frags;
	x = player.x;
	y = player.y;
	cx = player.cx;
	cy = player.cy;
	fangle = player.fangle;
	InertiaX = player.InertiaX;
	InertiaY = player.InertiaY;
	DXID = player.DXID;
}
void CPlayer::UpdateNetName(char* str)
{
	strcpy(netname,str);
}
void CPlayer::UpdateModel(char* str)
{
	strcpy(nfkmodel,str);
}
void CPlayer::UpdateLocation(char* str)
{
	strcpy(location,str);
}
void CPlayer::PrintHealth(int hx, int hy, int ax, int ay, int fnt, char color, bool camera) //Выводит жизни и броню в указанных координатах
{
	char str[15];
	char str2[15];
	char tmp[4];

	strcpy(str,"^7");
	itoa((health<0?0:health),tmp,10);
	strcat(str,tmp);
	if (health<=0) str[1]='0';
	else if (health<HEALTH_VALUE_WARNING_2) str[1]='1';
	else if (health<HEALTH_VALUE_WARNING_1) str[1]='3';
	//fnP_DLL_ExtendedTextout(hx,hy,fnt,camera,str);
	/*
	strcpy(str,"^7");
	itoa((armor<0?0:armor),tmp,10);
	strcat(str,tmp);
	if (health<=0) str[1]='0';
	else if (health<HEALTH_VALUE_WARNING_2) str[1]='1';
	else if (health<HEALTH_VALUE_WARNING_1) str[1]='3';
	fnP_DLL_ExtendedTextout(ax,ay,fnt,camera,str);
	*/
	strcpy(str2,"^7");
	itoa((armor<0?0:armor),tmp,10);
	strcat(str2,tmp);
	if (health<=0) str2[1]='0';
	else if (health<HEALTH_VALUE_WARNING_2) str2[1]='1';
	else if (health<HEALTH_VALUE_WARNING_1) str2[1]='3';
	fnP_DLL_ExtendedTextout(hx,hy,fnt,camera,va("%s / %s",str,str2));
}

void CPlayer::PrintHealth(int fnt, char color) //Выводит жизни и броню под моделью игрока
{
	PrintHealth(x-27,y+25,x+7,y+25,fnt,color,true);
	//fnP_DLL_FX_Line(x,y+25,x,y+35,0xFFFFFF,0x103,true);
}

BYTE CPlayer::GetTeam()
{
	return team;
}

WORD CPlayer::GetDXID()
{
	return DXID;
}
char* CPlayer::GetNetname()
{
	return netname;
}
char* CPlayer::GetLocation()
{
	if (m_Map.Locs.size() != 0) return m_Map.GetLoc(x,y);
	else return location;
}
bool CPlayer::IsDead()
{
	return dead;
}
int  CPlayer::Health()
{
	return health;
}
int  CPlayer::Armor()
{
	return armor;
}
double CPlayer::GetX()
{
	return x;
}
double CPlayer::GetY()
{
	return y;
}

bool CPlayer::have_weapon(BYTE wn)
{
	if (wn<=1) return true;
	switch(wn)
	{
	case 2: return have_ssg;
	case 3: return have_gl;
	case 4: return have_rl;
	case 5: return have_lg;
	case 6: return have_rg;
	case 7: return have_pg;
	case 8: return have_bfg;
	}
	return false;
}
BYTE CPlayer::GetAmmo(BYTE wn)
{
	if (wn<1) return 0;
	switch(wn)
	{
	case 1: return ammo_mg;
	case 2: return ammo_ssg;
	case 3: return ammo_gl;
	case 4: return ammo_rl;
	case 5: return ammo_lg;
	case 6: return ammo_rg;
	case 7: return ammo_pg;
	case 8: return ammo_bfg;
	}
	return 0;
}

BYTE CPlayer::GetWapon()
{
	return weapon;
}

bool CPlayer::have_item(BYTE item_code)
{
	if (item_code<1 || item_code>28) return false;
	switch(item_code)
	{
	case IT_SHOTGUN: return have_ssg;
	case IT_GRENADE: return have_gl;
	case IT_ROCKET: return have_rl;
	case IT_SHAFT: return have_lg;
	case IT_RAIL: return have_rg;
	case IT_PLASMA: return have_pg;
	case IT_BFG: return have_bfg;
	case IT_POWERUP_REGENERATION: return item_regen;
	case IT_POWERUP_BATTLESUIT: return item_battle;
	case IT_POWERUP_HASTE: return item_haste;
	case IT_POWERUP_QUAD: return item_quad;
	case IT_POWERUP_FLIGHT: return item_flight;
	case IT_POWERUP_INVISIBILITY: return item_invis;
	}
	return false;
}

bool CPlayer::TouchRegion(int bx,int by,int width,int height)
{
	//DEBUG PRINT
	//Console.printf("x:%d y:%d bx:%d by:%d",int(x),int(y),bx,by);

	if ((x + 9 >= bx*32) && (x - 8 <= bx*32+width*32))
	   if ((y + 23 >= by*16) && (y - 23 <= by*16+height*16))
			return true;
	return false;
}

int GetAngle(int x1,int y1,int x2,int y2)
{
	//DEBUG PRINT
	//Console.printf("x1:%d y1:%d x2:%d y2:%d",x1,y1,x2,y2);

	int dx = x2 - x1;
	int dy = y1 - y2; //Т.к. ось y в машинной графике направлена вниз

	if (dx == 0) return (y2<y1)?0:180; //Вертикальное направление
	double angle=atan2(double(dy),double(dx)); //Возвращает угол от -Pi до Pi

	angle = -angle + M_PI_2; //Виртуальный угол в NFK отсчитывается по часовой вектора +Pi/2. Преобразуем в виртуальному углу
	
	//после этого углы 2 четверти становятся отрицательными, исправим это
	if (angle<0) angle = 2*M_PI + angle;

	angle = angle/M_PI*180; //Переградуировка: в NFK углол Pi/2 измеряется дискретно значениями от 0 до 180
	
	return floor(angle);
}


CItem CPlayer::PointItem()
{
	CItem* result;
	double min_dist = 1000000;
	double dist;
	int angl_top,angl_bot;
	
	int correct_y;
	if (crouch) correct_y = 2;
	else correct_y = 0;

	double realy_fangle;
	if (fangle<30) realy_fangle = fangle-1;
	else if (fangle<60) realy_fangle = fangle-2;
	else if (fangle<120) realy_fangle = fangle-3;
	else if (fangle<150) realy_fangle = fangle-1;
	else if (fangle<210) realy_fangle = fangle;
	else if (fangle<240) realy_fangle = fangle+2;
	else if (fangle<300) realy_fangle = fangle+3;
	else if (fangle<330) realy_fangle = fangle+1;
	else realy_fangle = fangle;

	int top_x,top_y,bot_x,bot_y;
	top_x = x + sin((realy_fangle-90)/180*M_PI)*23;
	top_y = y + cos((realy_fangle+90)/180*M_PI)*23 + correct_y;
	bot_x = x + sin((realy_fangle+90)/180*M_PI)*20;
	bot_y = y + cos((realy_fangle-90)/180*M_PI)*20 + correct_y;

	bool OneScreenMap;
	if (m_Map.GetSizeX() <=20 && m_Map.GetSizeY() <=30) OneScreenMap = true;
	else OneScreenMap = false;

	//Перебираем стационарные предметы
	if (m_Map.MapItems.size()!=0) //Перебираем в цикле все предметы
	{
		m_Map.MapItems.top();
		do
		{
			
			if (!OneScreenMap)
				if (abs(x - m_Map.MapItems.cur()->d->brick_x*32+16) > 320 || abs(y - m_Map.MapItems.cur()->d->brick_y*16+8) > 240) continue;
			m_Map.MapItems.cur()->d->UpdateItem();
			if (m_Map.MapItems.cur()->d->respawntime != 0) continue;

			if ((fangle<90 || fangle>270) && ((top_x > m_Map.MapItems.cur()->d->brick_x*32+32 && dir==3) || (bot_x < m_Map.MapItems.cur()->d->brick_x*32 && dir==2))) continue; //игрок смотрит вверх, top правее правого при ходьбе направо, bot левее левого при ходьбе налево
			if ((fangle>90 && fangle<270) && ((top_x < m_Map.MapItems.cur()->d->brick_x*32 && dir==2) || (bot_x > m_Map.MapItems.cur()->d->brick_x*32+32 && dir==3))) continue; //игрок смотрит вниз, top левее левошл, bot правее правого

			angl_bot = GetAngle(bot_x,bot_y,m_Map.MapItems.cur()->d->brick_x*32+16,m_Map.MapItems.cur()->d->brick_y*16+8);
			angl_top = GetAngle(top_x,top_y,m_Map.MapItems.cur()->d->brick_x*32+16,m_Map.MapItems.cur()->d->brick_y*16+8);

			if (dir==2) //игрок смотрит влево, отрицательный угол надо преобразовать к положительному
			{
				if (angl_bot<90) angl_bot = 360 + angl_bot;
				if (angl_top<90) angl_top = 360 + angl_top;
			}
			else 
			{
				if (angl_bot>270) angl_bot = angl_bot - 360;
				if (angl_top>270) angl_top = angl_top - 360;
			}
			
			//DEBUG PRINT
			//Console.printf("fa:%d ab:%d at:%d",int(fangle),int(angl_bot),int(angl_top));

			if (angl_bot<=realy_fangle && angl_top>=realy_fangle)
			{
				
				dist = GetDist(x,y,m_Map.MapItems.cur()->d->brick_x*32+16,m_Map.MapItems.cur()->d->brick_y*16+8);
				if (dist < min_dist)
				{
					min_dist = dist;
					result = m_Map.MapItems.cur()->d;
				}
			}
		}
		while(m_Map.MapItems.next());
	}

	CItem resultItem; //Создаём новую переменную, которая будет возвращаться из этой функции
	if (result != NULL)
	{
		//Копинуем данные из переменной-указателя в новую перменную CItem
		resultItem.brick_x = result->brick_x;
		resultItem.brick_y = result->brick_y;
		resultItem.image = result->image;
	}

	//Перебираем выпавшее оружие
	//{

	//}

	return resultItem;
}

CPlayer* FindPlayerByDXID(WORD DXID)
{
	if (players.size()==0) return NULL;
	players.top();
	do
	{
		if (players.cur()->d->GetDXID() == DXID) return players.cur()->d;
	}
	while (players.next());
	return NULL;
}

