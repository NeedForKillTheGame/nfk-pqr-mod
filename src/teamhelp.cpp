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
#include "teamhelp.h"

#include "map.h"
#include "player.h"
#include "tools_menu.h" //для TOOLS_MENU_Update();
//extern bool FirstFrameAfterReset;

#define _NFK_FONT_ 1

CTeamHelp TeamHelp;
bool PointItem();

c_var_int* tp_ftook_repeat;
c_var_bool* ch_teamhealth;
c_var_int *tp_weaponorder;
c_var_bool* cl_triggers;
c_var_bool* tp_forceTriggers;
c_var_string* tp_took;
c_var_bool* f_aliases;
c_var_string* f_death;
c_var_string* f_flagdeath;
c_var_string* f_mapend;
c_var_string* f_newmap;
c_var_string* f_respawn; 
c_var_string* f_spawn; 
c_var_string* f_took; 
c_var_string* f_weaponchange;

c_var_string* tp_name_gn;
c_var_string* tp_name_mg;
c_var_string* tp_name_ssg;
c_var_string* tp_name_gl;
c_var_string* tp_name_rl;
c_var_string* tp_name_lg;
c_var_string* tp_name_rg;
c_var_string* tp_name_pg;
c_var_string* tp_name_bfg;
c_var_string* tp_name_ammo_mg;
c_var_string* tp_name_ammo_ssg;
c_var_string* tp_name_ammo_gl;
c_var_string* tp_name_ammo_rl;
c_var_string* tp_name_ammo_lg;
c_var_string* tp_name_ammo_rg;
c_var_string* tp_name_ammo_pg;
c_var_string* tp_name_ammo_bfg;
c_var_string* tp_name_shard;
c_var_string* tp_name_ya;
c_var_string* tp_name_ra;
c_var_string* tp_name_health_5;
c_var_string* tp_name_health_25;
c_var_string* tp_name_health_50;
c_var_string* tp_name_health_100;
c_var_string* tp_name_regeneration;
c_var_string* tp_name_battlesuit;
c_var_string* tp_name_haste;
c_var_string* tp_name_quad;
c_var_string* tp_name_flight;
c_var_string* tp_name_invisibility; 
c_var_string* tp_name_health;
c_var_string* tp_name_armor;
c_var_string* tp_name_weapon;
c_var_string* tp_name_at;
c_var_string* tp_name_nothing;

c_var_bool* tp_need_rg;
c_var_int* tp_need_weapon;
c_var_int* tp_need_armor;
c_var_int* tp_need_health;
c_var_int* tp_need_ammo_mg;
c_var_int* tp_need_ammo_ssg;
c_var_int* tp_need_ammo_gl;
c_var_int* tp_need_ammo_rl;
c_var_int* tp_need_ammo_lg;
c_var_int* tp_need_ammo_rg;
c_var_int* tp_need_ammo_pg;
c_var_int* tp_need_ammo_bfg;

bool Check_tp_took(int item_code);
void AddRemoveItemsIn_tp_took_flags(char* str,bool add_remove);

void CMD_ShowTeamHealth(char* s);//при вызове консольной команды +team_health
void CMD_HideTeamHealth(char* s);//при вызове консольной команды -team_health
void CMD_say_team2(char* s);//при вызове консольной команды team_say2
//Параметр s не используется. В нём текст введённый в консоли после команды.

void ch_teamhealth_onChange(char* new_val); //Перед изменением переменной ch_teamhealth вызывается эта функция
void tp_took_onChange(char* new_val); //Перед изменением переменной tp_took вызывается эта функция
c_var_string* Get_tp_name_weapon(BYTE wn);
c_var_string* Get_tp_name_ammo(BYTE wn);
c_var_int* Get_tp_need_ammo(BYTE wn);

class CTookItem
{
public:
	CTookItem();
	bool tp_took_flag;
	char name[20];
	c_var_string* tp_name;
	bool cmp_tp_name(char* nm);
};

CTookItem TookItems[29];

CTookItem::CTookItem()
{
	tp_took_flag = false;
	strcpy(name,"");
	tp_name = NULL;
}
bool CTookItem::cmp_tp_name(char* nm)
{
	//Сравнивает параметр с именем преддмета указанным в консольной переменной tp_name_*
	//и с именем по умолчанию CTookItem::name
	//Возвращает true, если параметр совпал хотя бы с одним именем
	
	if (tp_name!=NULL)
		if (strcmp_color(nm,tp_name->value())) return true;

	if (strcmp_color(nm,name)) return true;

	return false;
}

CTeamHelp::CTeamHelp()
{
	loc_last_death=NULL;
	loc_last_took=NULL;
	item_last_took=NULL;
	loc_last_point=NULL;
	item_last_point=NULL;
	after_death_timer=0;
	TC_need_send = true;
}
/*-------------------------------------------
*	EVENT_MainLoop()
*	Основной цикл этого модуля
*-------------------------------------------*/
void CTeamHelp::EVENT_MainLoop()
{
	if (ch_teamhealth->value()) DrawHealth();

	if (LocalPlayer == NULL) return;

	if (after_death_timer) after_death_timer--; //Таймер отсчитывает 5 сек после смерти

	//Обновим информацию о всех предметах на карте.
	//В процессе этого, выявим предметы взятые только что
	//Проверим, не находится в этом месте LocalPlayer
	//Если да, то генерируем событие took
	/*
	if (m_Map.MapItems.size()!=0) //Перебираем в цикле все предметы
	{
		m_Map.MapItems.top();
		do
		{
			if (m_Map.MapItems.cur()->d->UpdateItem()) //Возвращает true, если предмет был только что взят
			{
				if (cl_triggers->value() && (IsTeamplay || tp_forceTriggers->value()))
				if (LocalPlayer->TouchRegion(m_Map.MapItems.cur()->d->brick_x,m_Map.MapItems.cur()->d->brick_y,1,1))
				{					
					if (Check_tp_took(m_Map.MapItems.cur()->d->image))
					{
						//Запоминаем локацию
						if (loc_last_took!=NULL) delete[] loc_last_took;
						loc_last_took = new char[strlen(LocalPlayer->GetLocation())+1];
						strcpy(loc_last_took,LocalPlayer->GetLocation());
						//Запоминаем предмет
						if (item_last_took!=NULL) delete[] item_last_took;
						item_last_took = new char[strlen(TookItems[m_Map.MapItems.cur()->d->image].tp_name->value())+1];
						strcpy(item_last_took,TookItems[m_Map.MapItems.cur()->d->image].tp_name->value());
						is_last_took = true; //Флаг, показывающий что последним действием было took а не point. Нужно для макроса %
						//Вызов команды f_took в последнюю очередь. Там могут присутствавать макросы % , которые используют loc_last_took и item_last_took
						for(int j=1;j<=tp_ftook_repeat->value();j++)
						{
							Console.SendConsoleHCommand(f_took->value());
						}
					}
				}
				break;
			}
		}
		while(m_Map.MapItems.next());
	}
	*/
	
	
	if (cl_triggers->value() && (IsTeamplay || tp_forceTriggers->value()))
	{
		if (LocalPlayer->is_event_death() || LocalPlayer->is_event_flagdeath())
		{
			//Запоминаем локацию смерти
			if (loc_last_death!=NULL) delete[] loc_last_death;
			loc_last_death = new char[strlen(LocalPlayer->GetLocation())+1];
			strcpy(loc_last_death,LocalPlayer->GetLocation());
			//Запускаем таймер после смерти на обратный отсчёт
			after_death_timer=50*5;
			//Вызываем альяс который хранится в соовт. переменной (f_death или f_flagdeath)
			if (LocalPlayer->is_event_flagdeath()) Console.ParseAlias(f_flagdeath->value());
			else if (LocalPlayer->is_event_death()) Console.ParseAlias(f_death->value());
		}
	}
}

void CTeamHelp::EVENT_ItemTaken(BYTE brickx, BYTE bricky, BYTE image, WORD dxid)
{
	if (LocalPlayer!=NULL && LocalPlayer->GetDXID() == dxid)
	if (cl_triggers->value() && (IsTeamplay || tp_forceTriggers->value()))
	{					
		if (Check_tp_took(image))
		{
			//Запоминаем локацию
			if (loc_last_took!=NULL) delete[] loc_last_took;
			loc_last_took = new char[strlen(LocalPlayer->GetLocation())+1];
			strcpy(loc_last_took,LocalPlayer->GetLocation());
			//Запоминаем предмет
			if (item_last_took!=NULL) delete[] item_last_took;
			item_last_took = new char[strlen(TookItems[image].tp_name->value())+1];
			strcpy(item_last_took,TookItems[image].tp_name->value());
			is_last_took = true; //Флаг, показывающий что последним действием было took а не point. Нужно для макроса %
			//Вызов команды f_took в последнюю очередь. Там могут присутствавать макросы % , которые используют loc_last_took и item_last_took
			for(int j=1;j<=tp_ftook_repeat->value();j++)
			{
				Console.ParseAlias(f_took->value());
			}
		}
	}
}
void CTeamHelp::EVENT_ResetGame()
{
	IsTeamplay=false; //Режим игры TDM (DOM, CTF) ?
	//Определяем режим игры
	char gametype[30]; //временная переменная
	strcpy(gametype,fnP_DLL_GetSystemVariable("gametype"));
	if (!strcmp(gametype,"TDM") || !strcmp(gametype,"DOM") || !strcmp(gametype,"CTF")) IsTeamplay=true;


	//Обнуляем локации и имена предметов took и point - если они заданы, значит это было в прошлой игре
	if (loc_last_death!=NULL) delete[] loc_last_death;
	loc_last_death = NULL;
	if (loc_last_took!=NULL) delete[] loc_last_took;
	loc_last_took = NULL;
	if (item_last_took!=NULL) delete[] item_last_took;
	item_last_took = NULL;
	if (loc_last_point!=NULL) delete[] loc_last_point;
	loc_last_point = NULL;
	if (item_last_point!=NULL) delete[] item_last_point;
	item_last_point = NULL;

	after_death_timer=0; //Таймер для отсчёта 5 секунд после смерти. Требуется в макросах %
}

//----------------------------------------
//	ChatReceived(WORD DXID, char* Text)
//  вызывается при получении чат сообщений
//----------------------------------------
void CTeamHelp::ChatReceived(WORD DXID, char* Text)
{

}

void CTeamHelp::RegisterConsoleCommands()
{
	//Регистрируем консольные команды внутри PQR Mod
//----------------------------------
//tp_weaponorder [default: 78564321]
//----------------------------------
//This allows you to define the order from best to worst weapon. The default value
//is "78564321", which means that the Rocket Launcher is the best weapon (impulse 7)
//then Lightning Gun (impulse 8), Super Nailgun (impulse 5), Grenade Launcher
//(impulse 6), Nailgun (impulse 4), Super Shotgun (impulse 3), Shotgun (impulse 2),
//Axe (impulse 1).

	tp_weaponorder = new c_var_int("tp_weaponorder","tp_weaponorder",867453210,0,876543210);

	cl_triggers = new c_var_bool("cl_triggers","cl_triggers",true);
	//then special aliases can be executed, or triggered, when something happens. Determines
	//whether special aliases can be executed/triggered when something happens during team 
	//games (ie. TF & TDM).

	tp_forceTriggers = new c_var_bool("tp_forceTriggers","tp_forceTriggers",false);
	//Controls whether f_took, f_death etc are forced to execute even if the game isn't a team game.

	f_aliases = new c_var_bool("f_aliases","f_aliases",true);
	//loads f_aliases at some situtation:  

	f_death = new c_var_string("f_death","f_death");
	//When you die. 

	f_flagdeath = new c_var_string("f_flagdeath","f_flagdeath");
	//When you die with the flag. Usually in CTF mod. 

	f_mapend = new c_var_string("f_mapend","f_mapend");
	//When a map ends and the final scoreboard is displayed. 

	f_newmap = new c_var_string("f_newmap","f_newmap");
	//When you connect to a server for a first time, or when the map on the server changes. 

	f_respawn = new c_var_string("f_respawn","f_respawn"); 
	//When you respawn after having been killed. 

	f_spawn = new c_var_string("f_spawn","f_spawn"); 
	//When you first spawn on a map. This happens after f_newmap. 

	f_took = new c_var_string("f_took","f_took");
	//When you take an item in tp_took (see tp_took).
	

	f_weaponchange = new c_var_string("f_weaponchange","f_weaponchange"); 
	//When you change your current weapon.

	tp_ftook_repeat = new c_var_int("tp_ftook_repeat","tp_ftook_repeat",1,1,4);
	ch_teamhealth = new c_var_bool("ch_teamhealth","ch_teamhealth",false,ch_teamhealth_onChange);
	c_cmd* s_team_health = new c_cmd("+team_health","+team_health",CMD_ShowTeamHealth);
	c_cmd* h_team_health = new c_cmd("-team_health","-team_health",CMD_HideTeamHealth);
	c_cmd* say_team2= new c_cmd("say_team2","say_team2",CMD_say_team2);

	tp_name_gn = new c_var_string("tp_name_gn","tp_name_gn","GN");
	tp_name_mg = new c_var_string("tp_name_mg","tp_name_mg","MG");
	tp_name_ssg = new c_var_string("tp_name_ssg","tp_name_ssg","SSG");
	tp_name_gl = new c_var_string("tp_name_gl","tp_name_gl","GL");
	tp_name_rl = new c_var_string("tp_name_rl","tp_name_rl","RL");
	tp_name_lg = new c_var_string("tp_name_lg","tp_name_lg","LG");
	tp_name_rg = new c_var_string("tp_name_rg","tp_name_rg","RG");
	tp_name_pg = new c_var_string("tp_name_pg","tp_name_pg","PG");
	tp_name_bfg = new c_var_string("tp_name_bfg","tp_name_bfg","^4BFG");
	tp_name_ammo_mg = new c_var_string("tp_name_ammo_mg","tp_name_ammo_mg","^!bullets");
	tp_name_ammo_ssg = new c_var_string("tp_name_ammo_ssg","tp_name_ammo_ssg","^!shels");
	tp_name_ammo_gl = new c_var_string("tp_name_ammo_gl","tp_name_ammo_gl","^!grenades");
	tp_name_ammo_rl = new c_var_string("tp_name_ammo_rl","tp_name_ammo_rl","^!rockets");
	tp_name_ammo_lg = new c_var_string("tp_name_ammo_lg","tp_name_ammo_lg","^!ammo_lg");
	tp_name_ammo_rg = new c_var_string("tp_name_ammo_rg","tp_name_ammo_rg","^!slugs");
	tp_name_ammo_pg = new c_var_string("tp_name_ammo_pg","tp_name_ammo_pg","^!cells");
	tp_name_ammo_bfg = new c_var_string("tp_name_ammo_bfg","tp_name_ammo_bfg","^!ammo_bfg");
	tp_name_shard = new c_var_string("tp_name_shard","tp_name_shard","shard");
	tp_name_ya = new c_var_string("tp_name_ya","tp_name_ya","^3YA");
	tp_name_ra = new c_var_string("tp_name_ra","tp_name_ra","^1RA");
	tp_name_health_5 = new c_var_string("tp_name_health_5","tp_name_health_5","H5");
	tp_name_health_25 = new c_var_string("tp_name_health_25","tp_name_health_25","^%H25");
	tp_name_health_50 = new c_var_string("tp_name_health_50","tp_name_health_50","^%H50");
	tp_name_health_100 = new c_var_string("tp_name_health_100","tp_name_health_100","^4MH");
	tp_name_regeneration = new c_var_string("tp_name_regeneration","tp_name_regeneration","^1Regen");
	tp_name_battlesuit = new c_var_string("tp_name_battlesuit","tp_name_battlesuit","^3BS");
	tp_name_haste = new c_var_string("tp_name_haste","tp_name_haste","^3HST");
	tp_name_quad = new c_var_string("tp_name_quad","tp_name_quad","^5QUAD");
	tp_name_flight = new c_var_string("tp_name_flight","tp_name_flight","^7Flight");
	tp_name_invisibility = new c_var_string("tp_name_invisibility","tp_name_invisibility","^7Invis"); 
	tp_name_health = new c_var_string("tp_name_health","tp_name_health","health");
	tp_name_armor = new c_var_string("tp_name_armor","tp_name_armor","armor");
	tp_name_weapon = new c_var_string("tp_name_weapon","tp_name_weapon","weapon");
	tp_name_at = new c_var_string("tp_name_at","tp_name_at"," ^7at ");
	tp_name_nothing = new c_var_string("tp_name_nothing","tp_name_nothing","nothing");
	
	tp_need_rg = new c_var_bool("tp_need_rg","tp_need_rg",true);
	tp_need_weapon = new c_var_int("tp_need_weapon","tp_need_weapon",234768,0,876543210);
	tp_need_armor = new c_var_int("tp_need_armor","tp_need_armor",50,0,200);
	tp_need_health = new c_var_int("tp_need_health","tp_need_health",50,0,200);
	tp_need_ammo_mg = new c_var_int("tp_need_ammo_mg","tp_need_ammo_mg",10,0,100);
	tp_need_ammo_ssg = new c_var_int("tp_need_ammo_ssg","tp_need_ammo_ssg",5,0,100);
	tp_need_ammo_gl = new c_var_int("tp_need_ammo_gl","tp_need_ammo_gl",5,0,100);
	tp_need_ammo_rl = new c_var_int("tp_need_ammo_rl","tp_need_ammo_rl",5,0,100);
	tp_need_ammo_lg = new c_var_int("tp_need_ammo_lg","tp_need_ammo_lg",15,0,100);
	tp_need_ammo_rg = new c_var_int("tp_need_ammo_rg","tp_need_ammo_rg",3,0,100);
	tp_need_ammo_pg = new c_var_int("tp_need_ammo_pg","tp_need_ammo_pg",10,0,100);
	tp_need_ammo_bfg = new c_var_int("tp_need_ammo_bfg","tp_need_ammo_bfg",5,0,100);

	//Инициализация tp_took обязана быть после всех tp_name_*
	tp_took = new c_var_string("tp_took","tp_took","default",tp_took_onChange);
	tp_took_onChange("default");//Устанавливаем значения по умолчанию
	
	//Регистрируем консольные команды в NFK.exe
	fnP_DLL_RegisterConsoleCommand("say_team2");
	fnP_DLL_RegisterConsoleCommand("tp_weaponorder");
	fnP_DLL_RegisterConsoleCommand("cl_triggers");
	fnP_DLL_RegisterConsoleCommand("tp_forceTriggers");
	fnP_DLL_RegisterConsoleCommand("tp_took");
	fnP_DLL_RegisterConsoleCommand("f_aliases");
	fnP_DLL_RegisterConsoleCommand("f_death");
	fnP_DLL_RegisterConsoleCommand("f_flagdeath");
	//fnP_DLL_RegisterConsoleCommand("f_mapend");
	//fnP_DLL_RegisterConsoleCommand("f_newmap");
	fnP_DLL_RegisterConsoleCommand("f_respawn");
	//fnP_DLL_RegisterConsoleCommand("f_spawn");
	fnP_DLL_RegisterConsoleCommand("f_took");
	fnP_DLL_RegisterConsoleCommand("f_weaponchange");

	fnP_DLL_RegisterConsoleCommand("tp_ftook_repeat");
	fnP_DLL_RegisterConsoleCommand("tp_name_gn");
	fnP_DLL_RegisterConsoleCommand("tp_name_mg");
	fnP_DLL_RegisterConsoleCommand("tp_name_ssg");
	fnP_DLL_RegisterConsoleCommand("tp_name_gl");
	fnP_DLL_RegisterConsoleCommand("tp_name_rl");
	fnP_DLL_RegisterConsoleCommand("tp_name_lg");
	fnP_DLL_RegisterConsoleCommand("tp_name_rg");
	fnP_DLL_RegisterConsoleCommand("tp_name_pg");
	fnP_DLL_RegisterConsoleCommand("tp_name_bfg");
	fnP_DLL_RegisterConsoleCommand("tp_name_ammo_mg");
	fnP_DLL_RegisterConsoleCommand("tp_name_ammo_ssg");
	fnP_DLL_RegisterConsoleCommand("tp_name_ammo_gl");
	fnP_DLL_RegisterConsoleCommand("tp_name_ammo_rl");
	fnP_DLL_RegisterConsoleCommand("tp_name_ammo_lg");
	fnP_DLL_RegisterConsoleCommand("tp_name_ammo_rg");
	fnP_DLL_RegisterConsoleCommand("tp_name_ammo_pg");
	fnP_DLL_RegisterConsoleCommand("tp_name_ammo_bfg");
	fnP_DLL_RegisterConsoleCommand("tp_name_shard");
	fnP_DLL_RegisterConsoleCommand("tp_name_ya");
	fnP_DLL_RegisterConsoleCommand("tp_name_ra");
	fnP_DLL_RegisterConsoleCommand("tp_name_health_5");
	fnP_DLL_RegisterConsoleCommand("tp_name_health_25");
	fnP_DLL_RegisterConsoleCommand("tp_name_health_50");
	fnP_DLL_RegisterConsoleCommand("tp_name_health_100");
	fnP_DLL_RegisterConsoleCommand("tp_name_regeneration");
	fnP_DLL_RegisterConsoleCommand("tp_name_battlesuit");
	fnP_DLL_RegisterConsoleCommand("tp_name_haste");
	fnP_DLL_RegisterConsoleCommand("tp_name_quad");
	fnP_DLL_RegisterConsoleCommand("tp_name_flight");
	fnP_DLL_RegisterConsoleCommand("tp_name_invisibility");
	fnP_DLL_RegisterConsoleCommand("tp_name_health");
	fnP_DLL_RegisterConsoleCommand("tp_name_armor");
	fnP_DLL_RegisterConsoleCommand("tp_name_weapon");
	fnP_DLL_RegisterConsoleCommand("tp_name_at");
	fnP_DLL_RegisterConsoleCommand("tp_name_nothing");

	fnP_DLL_RegisterConsoleCommand("tp_need_rg");
	fnP_DLL_RegisterConsoleCommand("tp_need_weapon");
	fnP_DLL_RegisterConsoleCommand("tp_need_armor");
	fnP_DLL_RegisterConsoleCommand("tp_need_health");
	fnP_DLL_RegisterConsoleCommand("tp_need_ammo_mg");
	fnP_DLL_RegisterConsoleCommand("tp_need_ammo_ssg");
	fnP_DLL_RegisterConsoleCommand("tp_need_ammo_gl");
	fnP_DLL_RegisterConsoleCommand("tp_need_ammo_rl");
	fnP_DLL_RegisterConsoleCommand("tp_need_ammo_lg");
	fnP_DLL_RegisterConsoleCommand("tp_need_ammo_rg");
	fnP_DLL_RegisterConsoleCommand("tp_need_ammo_pg");
	fnP_DLL_RegisterConsoleCommand("tp_need_ammo_bfg");

	fnP_DLL_RegisterConsoleCommand("ch_teamhealth");
	fnP_DLL_RegisterConsoleCommand("+team_health");
	fnP_DLL_RegisterConsoleCommand("-team_health");

	//Заполняем массив предметов TookItems
	//SSG
	strcpy(TookItems[1].name,"ssg");
	TookItems[1].tp_name = tp_name_ssg;
	//GL
	strcpy(TookItems[2].name,"gl");
	TookItems[2].tp_name = tp_name_gl;
	//RL
	strcpy(TookItems[3].name,"rl");
	TookItems[3].tp_name = tp_name_rl;
	//LG
	strcpy(TookItems[4].name,"lg");
	TookItems[4].tp_name = tp_name_lg;

	strcpy(TookItems[5].name,"rg");
	TookItems[5].tp_name = tp_name_rg;

	strcpy(TookItems[6].name,"pg");
	TookItems[6].tp_name = tp_name_pg;

	strcpy(TookItems[7].name,"bfg");
	TookItems[7].tp_name = tp_name_bfg;

	strcpy(TookItems[8].name,"bullets");
	TookItems[8].tp_name = tp_name_ammo_mg;

	strcpy(TookItems[9].name,"shels");
	TookItems[9].tp_name = tp_name_ammo_ssg;

	strcpy(TookItems[10].name,"grenades");
	TookItems[10].tp_name = tp_name_ammo_gl;

	strcpy(TookItems[11].name,"rockets");
	TookItems[11].tp_name = tp_name_ammo_rl;

	strcpy(TookItems[12].name,"ammo_lg");
	TookItems[12].tp_name = tp_name_ammo_lg;

	strcpy(TookItems[13].name,"slugs");
	TookItems[13].tp_name = tp_name_ammo_rg;

	strcpy(TookItems[14].name,"cells");
	TookItems[14].tp_name = tp_name_ammo_pg;

	strcpy(TookItems[15].name,"ammo_bfg");
	TookItems[15].tp_name = tp_name_ammo_bfg;

	strcpy(TookItems[16].name,"shard");
	TookItems[16].tp_name = tp_name_shard;

	strcpy(TookItems[17].name,"ya");
	TookItems[17].tp_name = tp_name_ya;

	strcpy(TookItems[18].name,"ra");
	TookItems[18].tp_name = tp_name_ra;

	strcpy(TookItems[19].name,"h5");
	TookItems[19].tp_name = tp_name_health_5;

	strcpy(TookItems[20].name,"h25");
	TookItems[20].tp_name = tp_name_health_25;

	strcpy(TookItems[21].name,"h50");
	TookItems[21].tp_name = tp_name_health_50;

	strcpy(TookItems[22].name,"mh");
	TookItems[22].tp_name = tp_name_health_100;

	strcpy(TookItems[23].name,"regen");
	TookItems[23].tp_name = tp_name_regeneration;

	strcpy(TookItems[24].name,"bs");
	TookItems[24].tp_name = tp_name_battlesuit;

	strcpy(TookItems[25].name,"haste");
	TookItems[25].tp_name = tp_name_haste;

	strcpy(TookItems[26].name,"quad");
	TookItems[26].tp_name = tp_name_quad;

	strcpy(TookItems[27].name,"flight");
	TookItems[27].tp_name = tp_name_flight;

	strcpy(TookItems[28].name,"invis");
	TookItems[28].tp_name = tp_name_invisibility;
}

void CTeamHelp::ShowHealth()
{
	ch_teamhealth->set_value("1");
}

void CTeamHelp::HideHealth()
{
	ch_teamhealth->set_value("0");
}

void CTeamHelp::DrawHealth()
{
	if (!IsTeamplay || !ch_teamhealth->value()) return; //Если режим не TDM (CTF,DOM) или консольная настройка ch_teamhealth 0 - выходим.
	if (LocalPlayer==NULL) return; //Если не инициализирован указатель на локального игрока, выходим (он должен инициализироваться в DLL_EVENT_BeginGame())
	BYTE MyTeam = LocalPlayer->GetTeam();
	
	if (players.size()==0) return; //Если списко игроков пуст, выходим.
	CPlayer* CurPl; //Временная переменная
	players.top();	//Проходим в цикле всех игроков и выводим жизни союзников
	do 
	{
		CurPl = players.cur()->d; //Берём игрока из глобального списка игроков
		if (LocalDXID==CurPl->GetDXID()) continue; //Игрок оказался локальным -> следующий шаг цикла (зачем выводить собственные жизни под моделью ?)
		if (CurPl->GetTeam()!=MyTeam) continue; //Игрок из команды противника -> следующий шаг цикла
		CurPl->PrintHealth();
	}
	while(players.next());
}

void CMD_ShowTeamHealth(char* s)//при вызове консольной команды +team_health
{
	ch_teamhealth->set_value_bool(true); //Непосредственно изменяем значение переменной без вывода на консоль сообщений типа "is turned on"	
}
void CMD_HideTeamHealth(char* s)//при вызове консольной команды -team_health
{
	ch_teamhealth->set_value_bool(false); //Непосредственно изменяем значение переменной без вывода на консоль сообщений типа "is turned on"	
}
void ch_teamhealth_onChange(char* new_val) //Перед изменением переменной ch_teamhealth вызывается эта функция
{
	//Тело этой функции практически повторяет стандартную set_value
	//Однако использован внешний обработчик, для дополнительного вызова TOOLS_MENU_Update()
	if (new_val!=NULL && strlen(new_val)>0)
	{
		bool new_val_bool;
		bool result = true;
		//Переводим строку new_val в переменную типа bool new_val_bool
		//Если перевод невозможен, то флаг result становится false
		if (strcmp(new_val,"true")==0 || strcmp(new_val,"on")==0 || strcmp(new_val,"0")) new_val_bool = true;
		else if (strcmp(new_val,"false")==0 || strcmp(new_val,"off")==0 || strcmp(new_val,"0")==0) new_val_bool = false;
		else result=false;

		if ( result )
		{
			ch_teamhealth->set_value_bool(new_val_bool);
			Console.printf("ch_teamhealth is turned %s",(new_val_bool?"on":"off"));
			TOOLS_MENU_Update();
			return;
		}
	}
	ch_teamhealth->print();
}

c_var_string* Get_tp_name_weapon(BYTE wn)
{
	switch(wn)
	{
	case 0: return tp_name_gn;
	case 1: return tp_name_mg;
	case 2: return tp_name_ssg;
	case 3: return tp_name_gl;
	case 4: return tp_name_rl;
	case 5: return tp_name_lg;
	case 6: return tp_name_rg;
	case 7: return tp_name_pg;
	case 8: return tp_name_bfg;
	}
	return NULL;
}

c_var_string* Get_tp_name_ammo(BYTE wn)
{
	switch(wn)
	{
	case 1: return tp_name_ammo_mg;
	case 2: return tp_name_ammo_ssg;
	case 3: return tp_name_ammo_gl;
	case 4: return tp_name_ammo_rl;
	case 5: return tp_name_ammo_lg;
	case 6: return tp_name_ammo_rg;
	case 7: return tp_name_ammo_pg;
	case 8: return tp_name_ammo_bfg;
	}
	return NULL;
}

c_var_int* Get_tp_need_ammo(BYTE wn)
{
	switch(wn)
	{
	case 1: return tp_need_ammo_mg;
	case 2: return tp_need_ammo_ssg;
	case 3: return tp_need_ammo_gl;
	case 4: return tp_need_ammo_rl;
	case 5: return tp_need_ammo_lg;
	case 6: return tp_need_ammo_rg;
	case 7: return tp_need_ammo_pg;
	case 8: return tp_need_ammo_bfg;
	}
	return NULL;
}

int CTeamHelp::GetSec()
{
	char tmp_str[5];
	int mn,sc;
	strcpy(tmp_str,fnP_DLL_GetSystemVariable("time_sec"));
	sc = atoi(tmp_str);
	/*if (dsec==0) return sc;

	strcpy(tmp_str,fnP_DLL_GetSystemVariable("time_min"));
	mn = atoi(tmp_str);
	
	sc += mn*60 - dsec;
	sc = sc%60;*/
	return sc;
}
int CTeamHelp::GetMin()
{
	char tmp_str[5];
	int mn,sc;
	strcpy(tmp_str,fnP_DLL_GetSystemVariable("time_min"));
	mn = atoi(tmp_str);
	/*if (dsec==0) return mn;
	
	strcpy(tmp_str,fnP_DLL_GetSystemVariable("time_sec"));
	sc = atoi(tmp_str);
	
	sc += mn*60 - dsec;
	mn = sc/60;*/
	return mn;
}

//----------------------------------
// CMD_say_team2 заменяет макро-коды
// и делает вызывает say_team
//----------------------------------
void CMD_say_team2(char* s)//при вызове консольной команды say_team2
{
	if (LocalPlayer == NULL) return;
	if (s==NULL || strlen(s)==0) return;
	char new_str[1000] = "";
	int l = strlen(s);
	bool is_macro = false;
	for(int i=0; i<l; i++) //перебераем параметр s по буквам
	{
		if (is_macro) {is_macro=false;continue;} //Если предыдущий символ был % то текущий выводить не надо (пример %a )
		if (s[i]=='%')
		{
			is_macro = true; //Текущий символ это %, установим флаг, чтобы не выводить следующий
			switch(s[i+1]) //Перебираем следующий символ
			{
			case 'a': //Amount of armor
				strcat(new_str,va("%d",LocalPlayer->Armor()));
			break;
			case 'A': //= Same as %a, but adds glowing braces if armor is low.
				{
				int h_color = 7;
				if (LocalPlayer->Armor()<=ARMOR_VALUE_WARNING_2) h_color = 1;
				else if (LocalPlayer->Armor()<=ARMOR_VALUE_WARNING_1) h_color = 3;
				strcat(new_str,va("^%d%d^7",h_color,LocalPlayer->Armor()));
				}
			break;
			case 'b': //Best weapon and amount of best weapon ammo
			{
				int wpn;
				for(unsigned j = 100000000; j>=1; j /= 10)
				{
					wpn = tp_weaponorder->value()/j - (tp_weaponorder->value()/(10*j))*10;
					if (wpn<1) continue;
					if (LocalPlayer->have_weapon(wpn))
					{
						strcat(new_str,va("%s:%d",Get_tp_name_weapon(wpn)->value(),LocalPlayer->GetAmmo(wpn)));
						break;
					}
				}
			}
			break;
			case 'c': //Amount of slugs
				strcat(new_str,va("%d",LocalPlayer->GetAmmo(6)));
			break;
			case 'd': //Location of last death
				if (TeamHelp.loc_last_death!=NULL) strcat(new_str,TeamHelp.loc_last_death);
			break;
			case 'e': //Number of enemies in your vicinity. Hidden for you
			break;
			case 'E': //Same as %e, but remembers the situtation 5seconds after death
			break;
			case 'h': //Health
				strcat(new_str,va("%d",LocalPlayer->Health()));
			break;
			case 'H': //= Same as %h, but adds glowing braces if health is low
				{
				int h_color = 7;
				if (LocalPlayer->Health()<=HEALTH_VALUE_WARNING_2) h_color = 1;
				else if (LocalPlayer->Health()<=HEALTH_VALUE_WARNING_1) h_color = 3;
				strcat(new_str,va("^%d%d^7",h_color,LocalPlayer->Health()));
				}
			break;
			case 'i': //Name and location of item you last picked up
				if (TeamHelp.item_last_took && TeamHelp.loc_last_took)
				{
					strcat(new_str,TeamHelp.item_last_took);
					if (tp_name_at->value()!=NULL) strcat(new_str,tp_name_at->value());
					strcat(new_str,TeamHelp.loc_last_took);
				}
			break;
			case 'j': //Name and location of item you last pointed
				if (TeamHelp.item_last_point)
				{
					strcat(new_str,TeamHelp.item_last_point); //Добавляем сообщение к формируемой строке
					if (TeamHelp.loc_last_point)
					{
						if (tp_name_at->value()!=NULL) strcat(new_str,tp_name_at->value());
						strcat(new_str,TeamHelp.loc_last_point); //Добавляем сообщение к формируемой строке	
					}
				}
			break;
			case 'k': //Name and location of item you last picked up or pointed to
				if (TeamHelp.is_last_took) //Последним было взятие предмета
					if (TeamHelp.item_last_took)
					{
						strcat(new_str,TeamHelp.item_last_took);
						if (TeamHelp.loc_last_took)
						{
							if (tp_name_at->value()!=NULL) strcat(new_str,tp_name_at->value());
							strcat(new_str,TeamHelp.loc_last_took);
						}
					}
				else //Последним было указание на предмет
					if (TeamHelp.item_last_point)
					{
						strcat(new_str,TeamHelp.item_last_point); //Добавляем сообщение к формируемой строке
						if (TeamHelp.loc_last_point)
						{
							if (tp_name_at->value()!=NULL) strcat(new_str,tp_name_at->value());
							strcat(new_str,TeamHelp.loc_last_point); //Добавляем сообщение к формируемой строке	
						}
					}
			break;
			case 'l': //Nearest location
				strcat(new_str,LocalPlayer->GetLocation());
			break;
			case 'L': //Same as %l, but remembers the situation 5 seconds after death
				if (TeamHelp.after_death_timer) {if (TeamHelp.loc_last_death) strcat(new_str,TeamHelp.loc_last_death);}
				else strcat(new_str,LocalPlayer->GetLocation());
			break;
			case 'm': //= %k if less than 5 secs ago, nearest item otherwise
				break;
			/*
			case 'M': //Current minute
				strcat(new_str,va("%d",TeamHelp.GetMin()));
			break;
			*/
			case 'o': //Number of teammates in vicinity. Hidden for you.  
			break;
			case 'O': //Same as %o, but remember the situtation 5seconds after death
			break;
			case 'p': //Powerups you have (quad, pent, ring, flag)
				if (LocalPlayer->have_item(IT_POWERUP_REGENERATION)) strcat(new_str,va("%s ",tp_name_regeneration->value()));
				if (LocalPlayer->have_item(IT_POWERUP_BATTLESUIT)) strcat(new_str,va("%s ",tp_name_battlesuit->value()));
				if (LocalPlayer->have_item(IT_POWERUP_HASTE)) strcat(new_str,va("%s ",tp_name_haste->value()));
				if (LocalPlayer->have_item(IT_POWERUP_QUAD)) strcat(new_str,va("%s ",tp_name_quad->value()));
				if (LocalPlayer->have_item(IT_POWERUP_FLIGHT)) strcat(new_str,va("%s ",tp_name_flight->value()));
				if (LocalPlayer->have_item(IT_POWERUP_INVISIBILITY)) strcat(new_str,va("%s ",tp_name_invisibility->value()));
				new_str[strlen(new_str)-1]='\0'; //Удаляем лишний пробел с конца
			break;
			case 'P': //Same as %p, but adds glowing braces if you have a powerup. 
				if (LocalPlayer->have_item(IT_POWERUP_REGENERATION)) strcat(new_str,va("^b^1%s ",tp_name_regeneration->value()));
				if (LocalPlayer->have_item(IT_POWERUP_BATTLESUIT)) strcat(new_str,va("^b^3^%s ",tp_name_battlesuit->value()));
				if (LocalPlayer->have_item(IT_POWERUP_HASTE)) strcat(new_str,va("^b^3%s ",tp_name_haste->value()));
				if (LocalPlayer->have_item(IT_POWERUP_QUAD)) strcat(new_str,va("^b^5%s ",tp_name_quad->value()));
				if (LocalPlayer->have_item(IT_POWERUP_FLIGHT)) strcat(new_str,va("^b^6%s ",tp_name_flight->value()));
				if (LocalPlayer->have_item(IT_POWERUP_INVISIBILITY)) strcat(new_str,va("^b^7%s ",tp_name_invisibility->value()));
				new_str[strlen(new_str)-1]='\0'; //Удаляем лишний пробел с конца
				strcat(new_str,"^n^7"); //добавляем код белого цвета ^7
			break;
			case 'q': //Powerups of last seen enemy
			break;
			case 'r': //Last reported location
			break;
			case 's': //This reports the status of how many teammates and enemies are in your vicinity
			//as a LED: if there are no enemies in vicinity then greenled. if %e <= %o then yellowled. 
			//if %e > %o then redled.
			break;
			/*
			case 'S':
			{
				int sc = TeamHelp.GetSec();
				if (sc<10) strcat(new_str,va("0%d",sc));
				else strcat(new_str,va("%d",sc));
			}
			*/
			break;
			case 't': //Name and location of object you are looking at
				if (PointItem())
				if (TeamHelp.item_last_point)
				{
					strcat(new_str,TeamHelp.item_last_point); //Добавляем сообщение к формируемой строке
					if (TeamHelp.loc_last_point)
					{
						if (tp_name_at->value()!=NULL) strcat(new_str,tp_name_at->value());
						strcat(new_str,TeamHelp.loc_last_point); //Добавляем сообщение к формируемой строке	
					}
				}
			break;
			/*
			case 'T': //Current time min:sec
			{
				int sc = TeamHelp.GetSec();
				if (sc<10) strcat(new_str,va("%d:0%d",TeamHelp.GetMin(),sc));
				else strcat(new_str,va("%d:%d",TeamHelp.GetMin(),sc));
			}
			break;
			*/
			case 'u': //What you need
			{
				if (LocalPlayer->IsDead()) break;
				char need_str[1000]="";
				if (LocalPlayer->Health() < tp_need_health->value())
					strcat(need_str,va("%s/",tp_name_health->value()));
				if (LocalPlayer->Armor() < tp_need_armor->value())
					strcat(need_str,va("%s/",tp_name_armor->value()));
				
				int wpn;
				int last_have_and_need_ammo = 0;
				bool need_wpn = true;
				bool need_amo = true;
				for(unsigned j = 100000000; j>=1; j /= 10) //перебираем цифры в списке tp_need_weapon
				{
					wpn = tp_need_weapon->value()/j - (tp_need_weapon->value()/(10*j))*10;
					if (wpn<1 || wpn>9) continue;
					if (LocalPlayer->have_weapon(wpn))
					{
						need_wpn = false; //Нашли хотя бы одно оружие из списка tp_need_weapon которое имеется на руках
						
						//Для каждого имеющегося на руках оружия их списка tp_need_wepon
						//проверяем наличие патрон. Если хватает хотя бы одному оружию,
						//нет необходимости в сообщении need ammo
						if (LocalPlayer->GetAmmo(wpn) >= Get_tp_need_ammo(wpn)->value()) need_amo = false;
						else last_have_and_need_ammo = wpn; //Запоминаем последнее по списку оружие, для которого нет патрон
					}
				}
				if (need_wpn) //Нужно оружие. Выводим сообщение из переменной tp_name_weapon
					strcat(need_str,va("%s/",tp_name_weapon->value()));
				else
				{
					//Если присутствует хотя бы одно оружие из списка tp_need_weapon,
					//проверим исключительное оружие - RG
					if (tp_need_rg->value() && !LocalPlayer->have_weapon(6)) //RG
						strcat(need_str,va("%s/",tp_name_rg->value()));

					//Если присутсвует хотя бы одно оружие из списка tp_need_weapon,
					//Но для всех присутствующих из tp_need_weapon нет патрон, то выводим сообщение
					//о нехватке патрон для последнего в списке
					if (need_amo && last_have_and_need_ammo)
						strcat(need_str,va("%s/",Get_tp_name_ammo(last_have_and_need_ammo)->value()));
				}

				if (strlen(need_str)==0) strcpy(need_str,tp_name_nothing->value());
				else need_str[strlen(need_str)+-1]='\0'; //Удаляем лишний / в конце

				strcat(new_str,need_str);
			}
			break;
			case 'v': //Current weapon and ammo
				strcat(new_str,va("%s:%d",Get_tp_name_weapon(LocalPlayer->GetWapon())->value(),LocalPlayer->GetAmmo(LocalPlayer->GetWapon())));
			break;
			case 'x': //Name of object you are looking at
				if (PointItem())
				if (TeamHelp.item_last_point)
					strcat(new_str,TeamHelp.item_last_point); //Добавляем сообщение к формируемой строке	
			break;
			case 'X': //Name of item you last picked up
				if (TeamHelp.item_last_took)
					strcat(new_str,TeamHelp.item_last_took);
			break;
			case 'y': //Location of object you are looking at
				if (PointItem())
				if (TeamHelp.loc_last_point)
					strcat(new_str,TeamHelp.loc_last_point); //Добавляем сообщение к формируемой строке	
			break;
			case 'Y': //Location of item you last picked up
				if (TeamHelp.loc_last_took)
					strcat(new_str,TeamHelp.loc_last_took);
			break;
			default: //Если после % идёт неправильный код, то это не макрос
				is_macro = false;
				strcat(new_str,"%");
			}
		}
		else //Текущий символ не %
		{
			//Добавляем текущий символ в конец
			new_str[strlen(new_str)+1]='\0'; 
			new_str[strlen(new_str)]=s[i];
		}
	} //for(int i=0; i<l; i++) //перебераем параметр s по буквам

	if (strlen(new_str)) //Строка сформировалась
		Console.SendConsoleHCommand(va("say_team ^7%s",new_str)); //отсылаем команду say_team и сформированую строку
}
bool PointItem()
{
	CItem PItem;

	PItem = LocalPlayer->PointItem();
	if (PItem.image!=IT_NONE)
	{
		//Очищаем старые данные
		if (TeamHelp.item_last_point!=NULL) delete[] TeamHelp.item_last_point;
		if (TeamHelp.loc_last_point!=NULL) delete[] TeamHelp.loc_last_point;

		//Запоминаем предмет и локацию
		TeamHelp.item_last_point = new char[strlen(TookItems[PItem.image].tp_name->value())+1];
		strcpy(TeamHelp.item_last_point,TookItems[PItem.image].tp_name->value());

		char* tmp_loc = m_Map.GetLoc(PItem.brick_x*32+16,PItem.brick_y*16+8); //Запоминаем локацию предмета (указатель на строку)
		if (tmp_loc != NULL) //Возможно для данной карты нет файла локаций
		{
			TeamHelp.loc_last_point = new char[strlen(tmp_loc)+1];
			strcpy(TeamHelp.loc_last_point,tmp_loc);
		}
		TeamHelp.is_last_took = false; //Последним было сообщение point, а не took
		return true;
	}
	return false;
}

void tp_took_onChange(char* new_val) //Перед изменением переменной tp_took вызывается эта функция
{
	/*tp_took [default: default]
Customizes the f_took trigger. 
Item can be: quad, pent, ring, suit, ra, ya, ga, mh, health, lg, 
rl, gl, sng, ng, ssg, pack, cells, rockets, nails, shells, flag, 
armor, weapons, powerups, ammo, all, default, none 
Item names can be customized with tp_name_item 
Examples: 
tp_took powerups ra ya lg rl mh flag (same as "default") 
tp_took all -ammo (report everything but ammo) 
tp_took +gl (add grenade launcher to pickup list) */
	
	//Раскладываем строку new_val по параметрам и обновляем массив tp_took_flags
	if (new_val == NULL || strlen(new_val) == 0)
	{		
			tp_took->print();
			return;
	}
	
	int i;
	char* p = strtok(new_val," "); //Первое слово до пробела
	if (!strcmp(p,"\"\""))
	{
		for(i=1;i<29;i++)
			TookItems[i].tp_took_flag = false; //Если tp_took "", обнуляем массив с флагами
		tp_took->set_value_string(NULL);
		tp_took->print();
		return;
	}
	bool first_not_plus_mins = false; //Флаг, устанавливается в true, как только в строке обнаружен параметр начинающийся не с + или -. В это случае список сбрасывается и начиная с этого параметра по умолчанию все считаются со знаком +.
	do
	{
		if (strlen(p)==0) continue;
		if (!first_not_plus_mins && p[0]!='+' && p[0]!='-')
		{
			first_not_plus_mins = true;
			for(i=1;i<29;i++)
				TookItems[i].tp_took_flag = false; //обнуляем массив с флагами
		}

		if (p[0]=='-') //Слово начинается со знака минус
		{
			if (strlen(p)!=1)
			{
				p++; //Убираем из строки '-'
				AddRemoveItemsIn_tp_took_flags(p,false);
			}
		}
		else if (p[0]=='+') //Слово начинается со знака плюс
		{
			if (strlen(p)!=1)
			{
				p++; //Убираем из строки '+'
				AddRemoveItemsIn_tp_took_flags(p,true);
			}
		}
		else //Слово без знака
		{
			if (!strcmp(p,"none")) //параметр содержит none
				for(i=1;i<29;i++)
					TookItems[i].tp_took_flag = false; //обнуляем массив с флагами
			else AddRemoveItemsIn_tp_took_flags(p,true);
		}
	}
	while ( (p = strtok(NULL," ")) != NULL);

	//Обновляем текстовое значение tp_took на основе флагов tp_took_flags
	//Проверка на значение all и none
	int realy_true = 0;
	for(i=1;i<29;i++)
	{
		if (TookItems[i].tp_took_flag) realy_true++;
	}
	if (realy_true==28) {tp_took->set_value_string("all");return;}
	else if (realy_true==28) {tp_took->set_value_string("none");return;}

	char new_str[1000] = "";

	//Проверка на wepons
	realy_true = 0;
	for(i=1;i<=7;i++)
	{
		if (TookItems[i].tp_took_flag) realy_true++;
	}
	if (realy_true==7)
	{
		strcpy(new_str,"weapons");
		strcat(new_str," ");
	}
	else if (realy_true!=0)
		for(i=1;i<=7;i++) //Конкатинируем в строке имена оружий
		{
			if (TookItems[i].tp_took_flag)
			{
				strcat(new_str,TookItems[i].name);
				strcat(new_str," ");
			}
		}

	//Проверка на ammo
	realy_true = 0;
	for(i=8;i<=15;i++)
	{
		if (TookItems[i].tp_took_flag) realy_true++;
	}
	if (realy_true==8)
	{
		strcat(new_str,"ammo");
		strcat(new_str," ");
	}
	else if (realy_true!=0)
		for(i=8;i<=15;i++) //Конкатинируем в строке патроны
		{
			if (TookItems[i].tp_took_flag)
			{
				strcat(new_str,TookItems[i].name);
				strcat(new_str," ");
			}
		}

	//Проверка на shards
	if (TookItems[16].tp_took_flag)
			{
				strcat(new_str,TookItems[16].name);
				strcat(new_str," ");
			}

	//Проверка на armor
	realy_true = 0;
	if (TookItems[17].tp_took_flag && TookItems[18].tp_took_flag)
	{
		strcat(new_str,"armor");
		strcat(new_str," ");
	}
	else
	{
		if (TookItems[17].tp_took_flag) //YA
		{
			strcat(new_str,TookItems[17].name);
			strcat(new_str," ");
		}
		if (TookItems[18].tp_took_flag) //RA
		{
			strcat(new_str,TookItems[18].name);
			strcat(new_str," ");
		}
	}

	//Проверка на +5 health
	if (TookItems[19].tp_took_flag)
			{
				strcat(new_str,TookItems[19].name);
				strcat(new_str," ");
			}

	//Проверка на health
	realy_true = 0;
	for(i=20;i<=22;i++)
	{
		if (TookItems[i].tp_took_flag) realy_true++;
	}
	if (realy_true==3)
	{
		strcat(new_str,"health");
		strcat(new_str," ");
	}
	else if (realy_true!=0)
		for(i=20;i<=22;i++) //Конкатинируем в строке имена аптечек
		{
			if (TookItems[i].tp_took_flag)
			{
				strcat(new_str,TookItems[i].name);
				strcat(new_str," ");
			}
		}

	//Проверка на powerups
	realy_true = 0;
	for(i=23;i<=28;i++)
	{
		if (TookItems[i].tp_took_flag) realy_true++;
	}
	if (realy_true==6)
	{
		strcat(new_str,"powerups");
		strcat(new_str," ");
	}
	else if (realy_true!=0)
		for(i=23;i<=28;i++) //Конкатинируем в строке названия поверапов
		{
			if (TookItems[i].tp_took_flag)
			{
				strcat(new_str,TookItems[i].name);
				strcat(new_str," ");
			}
		}

	if (new_str!=NULL && new_str[strlen(new_str)-1]==' ') new_str[strlen(new_str)-1] = '\0'; //Удаляем пробел с конца строки
	tp_took->set_value_string(new_str);
	
	//Выводим результат
	tp_took->print();
}

//-------------------------------
//  Check_tp_took(int item_code)
//  Возвращает true, если оружие с кодом item_code перечислено в tp_took
//-------------------------------
bool Check_tp_took(int item_code)
{
	return TookItems[item_code].tp_took_flag;
}
//-------------------------------
//  AddRemoveItemsIn_tp_took_flags(char* str, bool add_remove)
//  По текстовому обозначениею str вычиляет предмет(ы)
//  и устанавливает флаг(и) TookItems[i].tp_took_flag в значение add_remove
//-------------------------------
void AddRemoveItemsIn_tp_took_flags(char* str, bool add_remove)
{
	int i;

	for(i=1;i<29;i++) //Перебираем все предметы и устанавливаем флаг для явно указанного предмета (например str = "rl")
	{
		if (TookItems[i].cmp_tp_name(str)) TookItems[i].tp_took_flag = add_remove;
	}

	//Устанавливаем флаги для группы предметов, если было передано соответствующее слово (например str = "armor")
	if (strcmp_color(str,tp_name_health->value()) || strcmp_color(str,"health")) for(i = 20; i<=22; i++) TookItems[i].tp_took_flag = add_remove;
	else if (strcmp_color(str,tp_name_armor->value()) || strcmp_color(str,"armor")) for(i = 17; i<=18; i++) TookItems[i].tp_took_flag = add_remove;
	else if (strcmp_color(str,tp_name_weapon->value()) || strcmp_color(str,"weapons")) for(i = 1; i<=7; i++) TookItems[i].tp_took_flag = add_remove;
	else if (strcmp_color(str,"powerups")) for(i = 23; i<=28; i++) TookItems[i].tp_took_flag = add_remove;
	else if (strcmp_color(str,"ammo")) for(i = 8; i<=15; i++) TookItems[i].tp_took_flag = add_remove;
	else if (strcmp_color(str,"default")) //powerups ra ya rg mh
	{
			for(i = 23; i<=28; i++) TookItems[i].tp_took_flag = add_remove; //powerups
			TookItems[17].tp_took_flag = add_remove; //YA
			TookItems[18].tp_took_flag = add_remove; //RA
			TookItems[5].tp_took_flag = add_remove; //RG
			TookItems[22].tp_took_flag = add_remove; //MH
	}
	else if (strcmp_color(str,"all")) for(i = 0; i<=28; i++) TookItems[i].tp_took_flag = add_remove;
}

//Справка

//--------------------
//tp_name_* [default:]
//--------------------
//Customizes item name.
//tp_name_axe
//tp_name_sg
//tp_name_ssg
//tp_name_ng
//tp_name_sng
//tp_name_gl
//tp_name_rl
//tp_name_lg
//tp_name_ga
//tp_name_ya
//tp_name_ra
//tp_name_shells
//tp_name_nails
//tp_name_rockets
//tp_name_cells
//tp_name_quad
//tp_name_pent
//tp_name_ring
//tp_name_suit
//tp_name_mh
//tp_name_health
//tp_name_armor
//tp_name_weapon
//tp_name_backpack
//tp_name_flag
//tp_name_status_green
//tp_name_status_yellow
//tp_name_status_red
//tp_name_someplace
//tp_name_at
//tp_name_nothing
//tp_name_none
//tp_name_separator
//
//¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
//Item Need Amounts
//¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
//
//--------------------
//tp_need_* [default:]
//--------------------
//Customizes the %u macro.
//tp_need_rl
//tp_need_weapon
//tp_need_ga
//tp_need_ya
//tp_need_ra
//tp_need_shells
//tp_need_nails
//tp_need_rockets
//tp_need_cells
//tp_need_health

//Macro %-Codes
//%a = Amount of armor, same as $armor 
//%A = Type of your armor, same as $armortype 
//%[A] = Same as %A:%a, but adds glowing braces if armor is low. 
//%b = Best weapon and amount of best weapon ammo. 
//%c = Amount of cells, same as $cells. 
//%d = Location of last death, same as $deathloc. 
//%e = Number of enemies in your vicinity. Hidden for you. 
//%E = Same as %e, but remembers the situtation 5seconds after death. 
//%h = Health, same as $health 
//[%h] = Same as %h, but adds glowing braces if health is low 
//%i = Name and location of item you last picked up, same as $tookatloc. 
//%j = Name and location of item you last pointed. 
//%k = Name and location of item you last picked up or pointed to. 
//%l = Nearest location from .loc file (or 'someplace' if none found), same as $location. 
//%L = Same as %l, but remembers the situation 5 seconds after death. 
//%m = %k if less than 5 secs ago, nearest item otherwise. 
//%o = Number of teammates in vicinity. Hidden for you. 
//%O = Same as %o, but remember the situtation 5seconds after death. 
//%p = Powerups you have (quad, pent, ring, flag), same as $powerups. 
//[%p] = Same as %p, but adds glowing braces if you have a powerup. 
//%q = Powerups of last seen enemy (default: quad). 
//%r = Last reported location (%l). 
//%s = This reports the status of how many teammates and enemies are in your vicinity 
//as a LED: if there are no enemies in vicinity then greenled. if %e <= %o then yellowled. 
//if %e > %o then redled. 
//%S = Skin, same as $tf_skin. 
//%t = Name and location of object you are looking at, same as $pointatloc. 
//%u = What you need, same as $need. 
//%w = Current weapon and ammo. 
//%x = Name of object you are looking at, same as $point. 
//%X = Name of item you last picked up, same as $took 
//%y = Location of object you are looking at, same as $pointloc 
//%Y = Location of item you last picked up, same as $tookloc
//
