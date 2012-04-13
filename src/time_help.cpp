#include "time_help.h"

#define TH_AUTO_DEFAULT 0

#define TH_SHOTGUN 1
#define TH_GRENADE 2
#define TH_ROCKET 4
#define TH_SHAFT 8
#define TH_RAIL 16
#define TH_PLASMA 32
#define TH_BFG 64
#define TH_YELLOW_ARMOR 128
#define TH_RED_ARMOR 256
#define TH_HEALTH_25 512
#define TH_HEALTH_50 1024
#define TH_HEALTH_100 2048 // megahealth
#define TH_POWERUP_REGENERATION 4096
#define TH_POWERUP_BATTLESUIT 8192 
#define TH_POWERUP_HASTE 16384
#define TH_POWERUP_QUAD 32768
#define TH_POWERUP_FLIGHT 65536
#define TH_POWERUP_INVISIBILITY 131072

const unsigned long DefaultItems = TH_ROCKET | TH_RAIL | TH_YELLOW_ARMOR | TH_RED_ARMOR | TH_HEALTH_50 | TH_HEALTH_100 | TH_POWERUP_QUAD | TH_POWERUP_BATTLESUIT | TH_POWERUP_REGENERATION | TH_POWERUP_FLIGHT | TH_POWERUP_INVISIBILITY | TH_POWERUP_HASTE;
CTimeHelp TimeHelp;

void TIME_HELP_EVENT_MainLoop()
{
	TimeHelp.Draw();
}

bool TIME_HELP_EVENT_ConsoleCommand(CList* _Params)
{
	if (!strcmp((char*)_Params->GetCur(),"th_auto"))
		if (_Params->MoveNext()) {
			int i=atoi((char*)_Params->GetCur());
			if (i>=0 && i<=255) {
				TimeHelp.SetAutoShow(i*50);
				fnP_DLL_AddMessageX(i,"th_auto sets to ");
			}
			else {fnP_DLL_AddMessageXY((TimeHelp.GetAutoShow()/50),TH_AUTO_DEFAULT,"th_auto is ;default");fnP_DLL_AddMessage("Time left for respawn when timer start be visible");}
		}
		else {fnP_DLL_AddMessageXY((TimeHelp.GetAutoShow()/50),TH_AUTO_DEFAULT,"th_auto is ;default");fnP_DLL_AddMessage("Time left for respawn when timer start be visible");}
	else

	if (!strcmp((char*)_Params->GetCur(),"th_items"))
		if (_Params->MoveNext()) {
			unsigned long i=atol((char*)_Params->GetCur());
			TimeHelp.SetItems(i);
			fnP_DLL_AddMessageX(i,"th_items sets to ");
		}
		else {fnP_DLL_AddMessageXY(TimeHelp.GetItems(),DefaultItems,"th_items is ;default");fnP_DLL_AddMessage("th_items 0 - display all items time");}

	else
	if (strstr((char*)_Params->GetCur(),"+th_show")!=NULL)
	{
		TimeHelp.Show();
	}
	else
	if (strstr((char*)_Params->GetCur(),"-th_show")!=NULL) 
	{
		TimeHelp.Hide();
	}
	else return false; //Возвращеённое значение false указывает основному обработчику консольных команд на то, что модуль time_help не может обработать данную команду
	return true; //Возвращаем true, чтобы основной обработчик консольных команд EVENT_ConsoleCommand понял, что команда обработана модулем time_help
}

void TIME_HELP_RegisterConsoleCommands()
{
	fnP_DLL_RegisterConsoleCommand("+th_show");
	fnP_DLL_RegisterConsoleCommand("-th_show");
	fnP_DLL_RegisterConsoleCommand("th_auto");
	fnP_DLL_RegisterConsoleCommand("th_items");
}

void TIME_HELP_ConfigSave(FILE* _f)
{
	fprintf(_f,"//th_auto <value>   Time left to respawn iten, when timer will be displayed. Value 0 - off.\n");
	fprintf(_f,"th_auto %d\n\n",TimeHelp.GetAutoShow()/50);
	fprintf(_f,"//th_items <bitmask>   (default = %u). \n//Define items that timers will be displayed. \n//The parameter for this setting is a bitmask:",DefaultItems);
	fprintf(_f,"\n// All Items: th_items 0");
	fprintf(_f,"\n// TH_SHOTGUN %u",TH_SHOTGUN);
	fprintf(_f,"\n// TH_GRENADE %u",TH_GRENADE);
	fprintf(_f,"\n// TH_ROCKET %u",TH_ROCKET);
	fprintf(_f,"\n// TH_SHAFT %u",TH_SHAFT);
	fprintf(_f,"\n// TH_RAIL %u",TH_RAIL);
	fprintf(_f,"\n// TH_PLASMA %u",TH_PLASMA);
	fprintf(_f,"\n// TH_BFG %u",TH_BFG);
	fprintf(_f,"\n// TH_YELLOW_ARMOR %u",TH_YELLOW_ARMOR);
	fprintf(_f,"\n// TH_RED_ARMOR %u",TH_RED_ARMOR);
	fprintf(_f,"\n// TH_HEALTH_25 %u",TH_HEALTH_25);
	fprintf(_f,"\n// TH_HEALTH_50 %u",TH_HEALTH_50);
	fprintf(_f,"\n// TH_HEALTH_100 %u",TH_HEALTH_100);
	fprintf(_f,"\n// TH_POWERUP_REGENERATION %u",TH_POWERUP_REGENERATION);
	fprintf(_f,"\n// TH_POWERUP_BATTLESUIT %u",TH_POWERUP_BATTLESUIT);
	fprintf(_f,"\n// TH_POWERUP_HASTE %u",TH_POWERUP_HASTE);
	fprintf(_f,"\n// TH_POWERUP_QUAD %u",TH_POWERUP_QUAD);
	fprintf(_f,"\n// TH_POWERUP_FLIGHT %u",TH_POWERUP_FLIGHT);
	fprintf(_f,"\n// TH_POWERUP_INVISIBILITY %u",TH_POWERUP_INVISIBILITY);
	fprintf(_f,"\nth_items %d\n\n",TimeHelp.GetItems());
}

bool TIME_HELP_ConfigLoad(CList* __Params)
{
	return TIME_HELP_EVENT_ConsoleCommand(__Params);
}

CTimeHelp::CTimeHelp()
{
	ItemsToShow = DefaultItems;
	AutoShow_StartTime = 0;
}
CTimeHelp::~CTimeHelp()
{
}

void CTimeHelp::Draw()
{
	if (m_Map->Items.IsEmpty()) return; //Если список предметов карты пуст, выходим
	m_Map->Items.MoveTop();
	CItem* CurIt;
	do
	{
		CurIt = (CItem*)m_Map->Items.GetCur();
		if (IsVisible(CurIt)) //Нужно ли отображать текущий предмет?
			if (visible || GetItemTime(CurIt)<=AutoShow_StartTime) DrawItemTime(CurIt);
	}
	while (m_Map->Items.MoveNext());
}

bool CTimeHelp::IsVisible(CItem* It)
{
	if (ItemsToShow==0)
		if (It->type!=IT_RESPAWN) return true;
		else return false; 
	unsigned long tp=0;
	switch(It->type) {
	case IT_SHOTGUN: tp=TH_SHOTGUN; break;
	case IT_GRENADE: tp=TH_GRENADE; break;
	case IT_ROCKET: tp=TH_ROCKET; break;
	case IT_SHAFT: tp=TH_SHAFT; break;
	case IT_RAIL: tp=TH_RAIL; break;
	case IT_PLASMA: tp=TH_PLASMA; break;
	case IT_BFG: tp=TH_BFG; break;
	case IT_YELLOW_ARMOR: tp=TH_YELLOW_ARMOR; break;
	case IT_RED_ARMOR: tp=TH_RED_ARMOR; break;
	case IT_HEALTH_25: tp=TH_HEALTH_25; break;
	case IT_HEALTH_50: tp=TH_HEALTH_50; break;
	case IT_HEALTH_100: tp=TH_HEALTH_100; break;
	case IT_POWERUP_REGENERATION: tp=TH_POWERUP_REGENERATION; break;
	case IT_POWERUP_BATTLESUIT: tp=TH_POWERUP_BATTLESUIT; break;
	case IT_POWERUP_HASTE: tp=TH_POWERUP_HASTE; break;
	case IT_POWERUP_QUAD: tp=TH_POWERUP_QUAD; break;
	case IT_POWERUP_FLIGHT: tp=TH_POWERUP_FLIGHT; break;
	case IT_POWERUP_INVISIBILITY: tp=TH_POWERUP_INVISIBILITY; break;
	}
	if ( (tp&ItemsToShow) !=0) return true;
	return false;
}

unsigned int CTimeHelp::GetItemTime(CItem* _It)
{
	TBrick* Br = fnP_DLL_GetBrickStruct(_It->brick_x,_It->brick_y);
	return Br->respawntime;
}

void CTimeHelp::DrawItemTime(CItem* It)
{
	int t = GetItemTime(It);
	char s[5];
	itoa(t/50+1,s+2,10);
	s[0]='^';
	if ((t/50+1)<5) s[1]='1';
	else if ((t/50+1)<10) s[1]='3';
	else s[1]='7';
	if (t!=0) fnP_DLL_debug_textoutc(It->brick_x*32+8,It->brick_y*16,s);
}