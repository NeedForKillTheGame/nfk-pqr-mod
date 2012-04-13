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

#include "ruleset.h"

#include "tools_menu.h" //��� TOOLS_MENU_Update()

CRuleset Ruleset;
c_var_int* ruleset;

void ruleset_OnChange(char* new_val);
void CMD_timelimit(char* str); //��� ������ ���������� ������� timelimit
void CMD_fraglimit(char* str); //��� ������ ���������� ������� fraglimit
void CMD_sv_overtime(char* str); //��� ������ ���������� ������� sv_overtime
void CMD_sv_maxplayers(char* str);
void CMD_forcerespawn(char* str); //��� ������ ���������� ������� forcerespawn
void CMD_warmup(char* str); //��� ������ ���������� ������� warmup

CRuleset::CRuleset()
{
}

void CRuleset::RegisterConsoleCommands()
{
	//������������ ���������� ������� ������ PQR Mod
	ruleset = new c_var_int("Ruleset","ruleset",0,0,2,ruleset_OnChange);
	c_cmd* timelimit = new c_cmd("timelimit","timelimit",CMD_timelimit);
	c_cmd* sv_overtime = new c_cmd("sv_overtime","sv_overtime",CMD_sv_overtime);
	c_cmd* fraglimit = new c_cmd("fraglimit","fraglimit",CMD_fraglimit);
	c_cmd* forcerespawn = new c_cmd("forcerespawn","forcerespawn",CMD_forcerespawn);
	c_cmd* warmup = new c_cmd("warmup","warmup",CMD_warmup);

	//������������ ���������� ������� � NFK.exe
	fnP_DLL_RegisterConsoleCommand("ruleset");
}


void ruleset_OnChange(char* new_val) //��� ����� � ������� "ruleset <...>"
{
	if ( new_val!=NULL && strlen(new_val) ) //������������ �������� �� ����
	{
		bool result = true;
		int new_val_int;
		//��������� ������ new_val � ���������� ���� int new_val_int
		//���� ������� ����������, �� ���� result ���������� false
		if (strcmp(new_val,"tdm")==0 || strcmp(new_val,"team")==0 || strcmp(new_val,"2")==0) new_val_int = 2;
		else if (strcmp(new_val,"duel")==0 || strcmp(new_val,"dm")==0 || strcmp(new_val,"1")==0) new_val_int = 1;
		else if (strcmp(new_val,"off")==0 || strcmp(new_val,"0")==0) new_val_int = 0;
		else result=false;
		
		if ( result )
		{	
			ruleset->set_value_int(new_val_int); //������������� ����� �������� ����������
			//��������� ������� ������������� ��������� �������� ���������� ��� ������ �������� ruleset
			CMD_warmup("-1");
			CMD_sv_overtime("-1");
			CMD_sv_maxplayers("-1");
			CMD_fraglimit("-1");
			CMD_forcerespawn("-1");
			CMD_timelimit("-1");
			
			TOOLS_MENU_Update(); //������������� ������ ����
		}
	}

	//������� ������� �������� ���������� ruleset
	switch(ruleset->value())
	{
	case 0: //off
		Console.print("Ruleset is OFF");
			break;
	case 1: //duel
		Console.printf("Ruleset 1 - duel mode (timelimit %i, fraglimit %i, forcerespawn %i, sv_maxplayers 2, warmup %i)",CRuleset::DUEL_TIMELIMIT, CRuleset::FRAGLIMIT, CRuleset::DUEL_FORCERESPAWN, CRuleset::DUEL_WARMUP);
			break;
	case 2: //team
		Console.printf("Ruleset 2 - tdm mode (timelimit %i, fraglimit %i, forcerespawn %i, sv_overtime %i, warmup %i)",CRuleset::TDM_TIMELIMIT, CRuleset::FRAGLIMIT, CRuleset::TDM_FORCERESPAWN, CRuleset::TDM_SV_OVERTIME, CRuleset::TDM_WARMUP);
			break;
	default:
		ruleset->set_value_int(0); //����������� ������������� � 0
		Console.print("Ruleset is OFF");
	}
}
/*--------------------------------------------------
*	��������� ������ ������� ���������� ��� ����� � ������� ������ timelimit, forcerespawn ��� warmup
*	���� ���������� ��������� ruleset ����������� � 1(duel) ��� 2(tdm), �� ����� �������
*	����� ����������� ��������� timelimit, forcespawn ��� warmup ����������� ���� ��������������
*	��������� ��������� ��������
---------------------------------------------------*/
void CMD_timelimit(char* str)
{
	if ( str==NULL || strlen(str)==0) return;

	char tmpstr[30];
	char tmpstr2[4];
	strcpy(tmpstr,"timelimit ");
	switch(ruleset->value())
	{
	case 1: //duel
		itoa(CRuleset::DUEL_TIMELIMIT,tmpstr2,10); //��������� ����� � ������
		if (strcmp(str,tmpstr2)) //�������� ����� �������� ���������� �� ����������
		{
			strcat(tmpstr,tmpstr2);
			Console.SendConsoleCommandOnNextFrame(tmpstr);
		}
		break;
	case 2: //team
		itoa(CRuleset::TDM_TIMELIMIT,tmpstr2,10); //��������� ����� � ������
		if (strcmp(str,tmpstr2)) //�������� ����� �������� ���������� �� ����������
		{
			strcat(tmpstr,tmpstr2);
			Console.SendConsoleCommandOnNextFrame(tmpstr);
		}
		break;
	}
}
void CMD_fraglimit(char* str)
{
	if ( str==NULL || strlen(str)==0) return;

	char tmpstr[30];
	char tmpstr2[4];
	strcpy(tmpstr,"fraglimit ");
	if (ruleset->value())
	{
		itoa(CRuleset::FRAGLIMIT,tmpstr2,10); //��������� ����� � ������
		if (strcmp(str,tmpstr2)) //�������� ����� �������� ���������� �� ����������
		{
			strcat(tmpstr,tmpstr2);
			Console.SendConsoleCommandOnNextFrame(tmpstr);
		}
	}
}
void CMD_sv_overtime(char* str)
{
	if ( str==NULL || strlen(str)==0) return;

	char tmpstr[30];
	char tmpstr2[4];
	strcpy(tmpstr,"sv_overtime ");
	switch(ruleset->value())
	{
	case 2: //team
		itoa(CRuleset::TDM_SV_OVERTIME,tmpstr2,10); //��������� ����� � ������
		if (strcmp(str,tmpstr2)) //�������� ����� �������� ���������� �� ����������
		{
			strcat(tmpstr,tmpstr2);
			Console.SendConsoleCommandOnNextFrame(tmpstr);
		}
		break;
	}
}
void CMD_forcerespawn(char* str)
{
	char tmpstr[30];
	char tmpstr2[4];
	strcpy(tmpstr,"forcerespawn ");
	switch(ruleset->value())
	{
	case 1: //duel
		itoa(CRuleset::DUEL_FORCERESPAWN,tmpstr2,10); //��������� ����� � ������
		if (strcmp(str,tmpstr2)) //�������� ����� �������� ���������� �� ����������
		{
			strcat(tmpstr,tmpstr2);
			Console.SendConsoleCommandOnNextFrame(tmpstr);
		}
		break;
	case 2: //team
		itoa(CRuleset::TDM_FORCERESPAWN,tmpstr2,10); //��������� ����� � ������
		if (strcmp(str,tmpstr2)) //�������� ����� �������� ���������� �� ����������
		{
			strcat(tmpstr,tmpstr2);
			Console.SendConsoleCommandOnNextFrame(tmpstr);
		}
		break;
	}
}
void CMD_warmup(char* str)
{
	char tmpstr[30];
	char tmpstr2[4];
	strcpy(tmpstr,"warmup ");
	switch(ruleset->value())
	{
	case 1: //duel
		itoa(CRuleset::DUEL_WARMUP,tmpstr2,10); //��������� ����� � ������
		if (strcmp(str,tmpstr2)) //�������� ����� �������� ���������� �� ����������
		{
			strcat(tmpstr,tmpstr2);
			Console.SendConsoleCommandOnNextFrame(tmpstr);
		}
		break;
	case 2: //team
		itoa(CRuleset::TDM_WARMUP,tmpstr2,10); //��������� ����� � ������
		if (strcmp(str,tmpstr2)) //�������� ����� �������� ���������� �� ����������
		{
			strcat(tmpstr,tmpstr2);
			Console.SendConsoleCommandOnNextFrame(tmpstr);
		}
		break;
	}
}

void CMD_sv_maxplayers(char* str) //��� ��������� �� ���������������� ��� ������� �� ����� ��������  sv_maxplayers �.�. sv_maxplayer �� ������������ ��������� ��������.
{
	char tmpstr[30];
	char tmpstr2[4];
	strcpy(tmpstr,"sv_maxplayers ");
	switch(ruleset->value())
	{
	case 1: //duel
		itoa(2,tmpstr2,10); //��������� ����� � ������
		if (strcmp(str,tmpstr2)) //�������� ����� �������� ���������� �� ����������
		{
			strcat(tmpstr,tmpstr2);
			Console.SendConsoleCommandOnNextFrame(tmpstr);
		}
		break;
	}	
}