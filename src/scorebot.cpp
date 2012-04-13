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

#include "scorebot.h"
#include "player.h"
#include "teamhelp.h"

char* IrcColors(char *str);
char* StripColorName2(char* s);

CScorebot Scorebot;
c_var_bool* pm_scorebot;
c_var_bool* pm_scorebot_colored;
c_var_int* pm_scorebot_time;
c_var_int* pm_scorebot_bg;
c_var_string *pm_scorebot_teamred;
c_var_string *pm_scorebot_teamblue;


void CScorebot::PrintFile(char *prefix)
{
	if (players.size()==0) return;

	char text[300];
	strcpy(text,"");

	if (IsTeamplay)
	{
		strcat(text,va("%s",StripColorName2(pm_scorebot_teamblue->value())));
		if (pm_scorebot_bg->value()!=-1) strcat(text,va("%c0,%d",3,pm_scorebot_bg->value()));
		else if (pm_scorebot_colored->value()) strcat(text,va("%c",15));
		strcat(text,va(" %s:",fnP_DLL_GetSystemVariable("teamscore_blue")));
		strcat(text,va("%s %s",fnP_DLL_GetSystemVariable("teamscore_red"),StripColorName2(pm_scorebot_teamred->value())));
	}
	else
	{
		players.top();	//Проходим в цикле всех игроков и выводим фраги
		if (players.size()==2)
		{
			strcat(text,va("%s",StripColorName2(players.cur()->d->GetNetname())));
			if (pm_scorebot_bg->value()!=-1) strcat(text,va("%c0,%d",3,pm_scorebot_bg->value()));
			else if (pm_scorebot_colored->value()) strcat(text,va("%c",15));
			strcat(text,va(" %i:",players.cur()->d->GetFrags()));
			players.next();
			strcat(text,va("%i %s",players.cur()->d->GetFrags(),StripColorName2(players.cur()->d->GetNetname())));
		}
		else
		{
			do 
			{
				if (strlen(text)) strcat(text,", ");
				if (pm_scorebot_bg->value()!=-1) strcat(text,va("%c0,%d",3,pm_scorebot_bg->value()));
				strcat(text,va("%s",StripColorName2(players.cur()->d->GetNetname())));
				if (pm_scorebot_bg->value()!=-1) strcat(text,va("%c0,%d",3,pm_scorebot_bg->value()));
				else if (pm_scorebot_colored->value()) strcat(text,va("%c",15));
				strcat(text,va(" (%i)",players.cur()->d->GetFrags()));
			}
			while(players.next());
		}
	}
	if (pm_scorebot_bg->value()!=-1) strcat(text,va("%c0,%d",3,pm_scorebot_bg->value()));
	else if (pm_scorebot_colored->value()) strcat(text,va("%c",15));
	strcat(text,va(" @ %s [time: ",m_Map.name));

	int sc = TeamHelp.GetSec();
	if (sc<10) strcat(text,va("%d:0%d]",TeamHelp.GetMin(),sc));
	else strcat(text,va("%d:%d]",TeamHelp.GetMin(),sc));
	
	char path[300];
	strcpy(path,NFKRootDir);
	strcat(path,"\\score.txt");
	FILE* f = fopen(path,"wt");
	if (pm_scorebot_bg->value()!=-1)
	{
		fputs(va("%c0,%d",3,pm_scorebot_bg->value()),f);
	}
	if (prefix!=NULL && strlen(prefix)) fputs(prefix,f);
	fputs(text,f);
	fclose(f);
}

void CScorebot::EVENT_MainLoop()
{
	
	if (!pm_scorebot->value()) return;
	if (players.size()<=1) return;
	//Console.printf("%i",players.size());

	print_timer_tick++;
	if (print_timer_tick > 50*pm_scorebot_time->value()) print_timer_tick=0;

	if (players.size()!=2 && print_timer_tick!=0) return;
	
	players.top();	//Проходим в цикле всех игроков
	do 
	{
		if (players.cur()->d->is_event_change_frags() || players.size()!=2)
		{
			PrintFile(NULL);	
		}
	}
	while(players.next());
}

void CScorebot::EVENT_ConsoleMessage(char *s)
{
	if (s==NULL) return;
	if (!strcmp(s,"timelimit hit.") || !strcmp(s,"Timelimit hit.")) PrintFile("Timelimit hit: ");
	else if (!strcmp(s,"fraglimit hit.") || !strcmp(s,"Fraglimit hit.")) PrintFile("Fraglimit hit: ");
}

void CScorebot::RegisterConsoleCommands()
{
	pm_scorebot = new c_var_bool("pm_scorebot","pm_scorebot",false);
	pm_scorebot_colored = new c_var_bool("pm_scorebot_colored","pm_scorebot_colored",true);
	pm_scorebot_time = new c_var_int("pm_scorebot_time","pm_scorebot_time",60,5,600);
	pm_scorebot_bg = new c_var_int("pm_scorebot_bg","pm_scorebot_bg",14,-1,15);
	pm_scorebot_teamred = new c_var_string("pm_scorebot_teamred","pm_scorebot_teamred","red");
	pm_scorebot_teamblue = new c_var_string("pm_scorebot_teamblue","pm_scorebot_teamblue","blue");
	fnP_DLL_RegisterConsoleCommand("pm_scorebot_bg");
	fnP_DLL_RegisterConsoleCommand("pm_scorebot");
	fnP_DLL_RegisterConsoleCommand("pm_scorebot_colored");
	fnP_DLL_RegisterConsoleCommand("pm_scorebot_time");
	fnP_DLL_RegisterConsoleCommand("pm_scorebot_teamred");
	fnP_DLL_RegisterConsoleCommand("pm_scorebot_teamblue");
}

char* StripColorName2(char* s)
{
	if (pm_scorebot_colored->value()) return IrcColors(s);
	return StripColorName(s);
}

char* IrcColors(char *str)
{
	static char newstr[1024];
	if (str==NULL)
	{
		strcpy(newstr,"");	
	}
	bool readcolor=false;
	newstr[0]='\3';
	newstr[1]='0';
	int j=2;
	for(unsigned i=0;i<strlen(str);i++)
	{
		if (readcolor && str[i]!='^') {
			switch(str[i])
			{
			case '0':
				newstr[j]='\3';
				newstr[j+1]='1';
				j+=2;
				break;
			case '1':
				newstr[j]='\3';
				newstr[j+1]='4';
				j+=2;
				break;
			case '2':
				newstr[j]='\3';
				newstr[j+1]='9';
				j+=2;
				break;
			case '3':
				newstr[j]='\3';
				newstr[j+1]='8';
				j+=2;
				break;
			case '4':
				newstr[j]='\3';
				newstr[j+1]='2';
				j+=2;
				break;
			case '5':
				newstr[j]='\3';
				newstr[j+1]='1';
				newstr[j+2]='2';
				j+=3;
				break;
			case '6':
				newstr[j]='\3';
				newstr[j+1]='6';
				j+=2;
				break;
			case '7':
				newstr[j]='\3';
				newstr[j+1]='0';
				j+=2;
				break;
			case '!':
				newstr[j]='\3';
				newstr[j+1]='1';
				newstr[j+2]='4';
				j+=3;
				break;
			case '#':
				newstr[j]='\3';
				newstr[j+1]='1';
				newstr[j+2]='2';
				j+=3;
				break;
			case '%':
				newstr[j]='\3';
				newstr[j+1]='1';
				newstr[j+2]='1';
				j+=3;
				break;
			case '&':
				newstr[j]='\3';
				newstr[j+1]='1';
				newstr[j+2]='10';
				j+=3;
				break;
			}
			readcolor=false;
		}
		else
			if (!readcolor && str[i]=='^') readcolor=true;
			else 
			{
				newstr[j]=str[i];
				j++;
			}
	}
	newstr[j]='\0';
	return newstr;
}