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


/* match_auto_record (Default: "0")
--------------------------------
Description : When set to 1 or 2, a temp demo will automatically be recorded
              when a match starts (usually when the countdown starts).
              match_auto_record 2 automatically saves the demo after the match
              is completed. 'match_auto_record 1' requires manual saving (see
              the "match_save" command below). */

/* match_auto_mintime (Default: "30")
----------------------------------
Description : When using 'match_auto_record 2', temp demo's auto recorded won't
              be saved automatically if they are shorter than the number of
              seconds 'match_auto_mintime' is set to.  If a temp demo is too
              short to autosave, you can still save it manually with
              "match_save" (see below).
*/

/* demo_dir (Default: "")
----------------------
Description: This is the default location easy recorded and saved auto recorded
             demos will be saved to.  It is relative to your basedir.  If it is
             "" then the demos will be saved to your current gamedir (default
             qw behaviour).  It's recommended setting this to something like
             "qw/demos/recording" to have your demos put into
             c:\quake\qw\demos\recording .  Note that this only affects easy
             recorded demos and saved auto recorded demos.  It does not affect
             demos recorded with "record" (these go into your gamedir).*/


#include "autorecord.h"
#include "player.h"



#define TEMP_DEMO_NAME "_!_temp_!_"

void CMD_MatchSave(char* s); //при вызове консольной команды match_save
void CMD_ForceStart(char* s);//при вызове консольной команды match_forcestart
void CMD_StopRecord(char* s);//при вызове консольной команды stoprecord
//Параметр s здесь не используется. Он содержит текст введённый в консоли после команды

CAutorecord Autorecord;
c_var_int* match_auto_record;

CAutorecord::CAutorecord()
{
	tmp_demo_recorded = false;
	record_in_process = false;
	filename[0]='\0';
}

void CAutorecord::EVENT_Warmup_5()
{
	if (match_auto_record->value() && !record_in_process)
	{
		Start();
	}
}

void CAutorecord::EVENT_ResetGame()
{
	//Когда флаг match_in_process == true, это значит, что ResetGame вызван командой наступлением warmup 0.
	//В этом случае запись демки продолжается, если она была начата в вармапе, т.е. не нужно сбрасывать record_in_process=false.
	if (!match_in_process) record_in_process=false;
}

void CAutorecord::RegisterConsoleCommands()
{
	//Регистрируем консольные команды внутри PQR Mod
	match_auto_record = new c_var_int("match_auto_record","match_auto_record",1,0,2);
	match_name_versus = new c_var_string("match_name_versus","match_name_versus","_vs_");
	match_name_on = new c_var_string("match_name_on","match_name_on","on");
	match_name_and = new c_var_string("match_name_and","match_name_and","_&_");
	c_cmd* match_save = new c_cmd("match_save","match_save",CMD_MatchSave);
	c_cmd* match_forcestart = new c_cmd("match_forcestart","match_forcestart",CMD_ForceStart);
	c_cmd* stoprecord = new c_cmd("stoprecord","stoprecord",CMD_StopRecord);

	//Регистрируем консольные команды в NFK.exe
	fnP_DLL_RegisterConsoleCommand("match_auto_record");
	fnP_DLL_RegisterConsoleCommand("match_name_versus");
	fnP_DLL_RegisterConsoleCommand("match_name_on");
	fnP_DLL_RegisterConsoleCommand("match_name_and");
	fnP_DLL_RegisterConsoleCommand("match_save");
	fnP_DLL_RegisterConsoleCommand("match_forcestart");
}

void CAutorecord::Save()
{
	if (!tmp_demo_recorded || !strlen(filename))
	{
		Console.print("Nothing to save");
		return;
	}
	if (record_in_process)
	{
		Console.print("Auto demo recording still in progress");
		return;
	}
	else
	{
		char ExistringFile[255];
		strcpy(ExistringFile,NFKRootDir);
		strcat(ExistringFile,"\\demos\\");
		strcat(ExistringFile,TEMP_DEMO_NAME);
		strcat(ExistringFile,".ndm");

		char NewFile[255];
		strcpy(NewFile,NFKRootDir);
		strcat(NewFile,"\\demos\\");
		strcat(NewFile,filename);
		strcat(NewFile,".ndm");
		
		//DEBUG PRINT
		//Console.print(ExistringFile);
		//Console.print(NewFile);
		
		if (MoveFileEx(ExistringFile,NewFile,MOVEFILE_REPLACE_EXISTING))
		{
			strcpy(NewFile,"Demo saved as ");
			strcat(NewFile,filename);
			strcat(NewFile,".ndm");
			Console.print(NewFile);
		}
		else
		{
			Console.print("^1Demo saving failed");
		}
		tmp_demo_recorded = false;
	}
}

char* toValidFilename(char* str)
{
	if (str==NULL) return NULL;
	char* newstr= new char[strlen(str)+1];
	int j=0;
	for(unsigned i=0;i<strlen(str);i++)
	{
		if (str[i]==':' || str[i]==' ') newstr[j]='_';
		else
		if (!(str[i]=='\\' || str[i]=='/' || str[i]=='*' || str[i]=='^' || str[i]=='%' || str[i]=='$' || str[i]=='#' || str[i]=='?' || str[i]=='`' || str[i]=='~' || str[i]=='?' || str[i]=='<' || str[i]=='>' || str[i]=='|')) newstr[j]=str[i];
		else j--;
		j++; //Указатель на текущую букву в массиве newstr
	}
	newstr[j]='\0';
	strcpy(str,newstr);
	delete[] newstr;
	return str;
}


void CAutorecord::Start()
{
	tmp_demo_recorded = false;
	if (!match_auto_record->value()) return;
	if (record_in_process) return;
	record_in_process = true;
	char gametype[30];
	char PlayersRed[255]="\0";
	char PlayersBlue[255]="\0";
	strcpy(filename,"\0");
	strcpy(gametype,fnP_DLL_GetSystemVariable("gametype"));

	CPlayer* tmpP;
	if (players.size()!=0) //Перебираем всех игроков и склеиваем их имена
	{	
		players.top();
		do
		{
			tmpP = players.cur()->d;
			if (IsTeamplay)
			{
				if (tmpP->GetTeam()==1)
				{
					if (strlen(PlayersRed)) strcat(PlayersRed,match_name_and->value());
					strcat(PlayersRed,StripColorName(tmpP->GetNetname()));
				}
				else
				{
					if (strlen(PlayersBlue)) strcat(PlayersBlue,match_name_and->value());
					strcat(PlayersBlue,StripColorName(tmpP->GetNetname()));
				}
			}
			else
			{
				if (strlen(filename)) strcat(filename,match_name_versus->value());
				strcat(filename,StripColorName(tmpP->GetNetname()));
			}
		}
		while(players.next());
	}
	
	if (IsTeamplay)
	{
		strcpy(filename,PlayersRed);
		strcat(filename,match_name_versus->value());
		strcat(filename,PlayersBlue);
	}

	//Карта
	strcat(filename,"_");
	strcat(filename,match_name_on->value());
	strcat(filename,"_");
	strcat(filename,fnP_DLL_GetSystemVariable("mapname"));
	

	//Тип игры
	strcat(filename,"_");
	strcat(filename,gametype);

	//Дата и время
	strcat(filename,"_");
	LPSYSTEMTIME dt = new SYSTEMTIME;
	GetLocalTime(dt);
	char tmpstr[100];
	itoa(dt->wDay,tmpstr,10);
	if (dt->wDay<10) strcat(filename,"0");
	strcat(filename,tmpstr);
	strcat(filename,".");
	itoa(dt->wMonth,tmpstr,10);
	if (dt->wMonth<10) strcat(filename,"0");
	strcat(filename,tmpstr);
	strcat(filename,".");
	itoa(dt->wYear,tmpstr,10);
	if (dt->wYear<10) strcat(filename,"0");
	strcat(filename,tmpstr);
	strcat(filename,"_");
	itoa(dt->wHour,tmpstr,10);
	strcat(filename,tmpstr);
	strcat(filename,"-");
	itoa(dt->wMinute,tmpstr,10);
	if (dt->wMinute<10) strcat(filename,"0");
	strcat(filename,tmpstr);
	strcat(filename,"-");
	itoa(dt->wSecond,tmpstr,10);
	if (dt->wSecond<10) strcat(filename,"0");
	strcat(filename,tmpstr);

	toValidFilename(filename);

	if (match_auto_record->value()==1)
	{
		char cmd[100]="record ";
		strcat(cmd,TEMP_DEMO_NAME);
		fnP_DLL_SendConsoleCommand(cmd);
		tmp_demo_recorded = true;
		//Console.printf("^b^3Recording %s",TEMP_DEMO_NAME);
	}
	else
	{
		char cmd[255];
		strcpy(cmd,"record ");
		strcat(cmd,filename);
		Console.print(cmd);
		fnP_DLL_SendConsoleCommand(cmd);
		//Console.printf("^b^3Recording %s",filename);
	}
}

void CMD_MatchSave(char* s) //при вызове консольной команды match_save
{
	Autorecord.Save();
}

void CMD_ForceStart(char* s) //при вызове консольной команды match_forcestart
{
	Autorecord.Start();
}

void CMD_StopRecord(char* s) //при вызове консольной команды stoprecord
{
	if (Autorecord.is_record_in_process()) {
		Console.print("Auto demo recording complite");
		Autorecord.stop_record(); //Устанавливаем флаг о прекращении записи
	}
}
