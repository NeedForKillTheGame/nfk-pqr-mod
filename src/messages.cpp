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
#include "messages.h"

#include "player.h"
#define DEFAULT_FRAGMSG_FONT 4

CMessages Messages; //Главный объект модуля
CTextMessage MessageYouFragged(200,100,100,1,"You fragged "); //Переменная - сообщение, инициализируется следующими значениями: x,y,время,font-не используется,префикс,суфикс(здесь нет)

//cf_fragmsg (default = 1)
//Font size of the "you fragged" messages on top of your HUD.

c_var_bool* ch_fragmsg; //Консольная переменная, выводить ли сообщение "You Fragged ... "
c_var_int* ch_fragmsg_x;
c_var_int* ch_fragmsg_y;
c_var_int* ch_fragmsg_time;
c_var_int* cf_fragmsg; //Консольная переменная, номер шрифта которым выводить сообщение "You Fragged ... "
c_var_string* ch_fragmsg_color;
c_var_bool* ch_fragmsg_colored_name;

struct team_msg_type
{
	char* text;
	int time;
};

typedef Item<team_msg_type> ItemTeamMsg; 
typedef List<ItemTeamMsg> ListTeamMsg;

ListTeamMsg TeamMessages;
c_var_int* ch_teamchat;
c_var_int* ch_teamchat_x;
c_var_int* ch_teamchat_y;
c_var_int* ch_teamchat_dy;
c_var_int* ch_teamchat_time;
c_var_bool* ch_teamchat_printname;
c_var_bool* ch_teamchat_printlocation;
c_var_bool* ch_teamchat_showmyself;
c_var_int* cf_teamchat;
c_var_string* ch_teamchat_color;

void ch_fragmsg_color_OnChange(char* new_val);
void ch_teamchat_color_OnChange(char* new_val);

CTextMessage::CTextMessage(int _x, int _y, int t, int fnt, char* _tp, char* _ts)
{
	SetSettings(_x, _y, t, fnt, _tp, _ts);
	TickCount = 0;
	visible = false;
	if (_tp==NULL) text_prefix = NULL;
	if (_ts==NULL) text_suffix = NULL;
}
void CTextMessage::Show(char* txt, int t)
{
	visible = true;
	if (t>0) TickCount = t;
	else TickCount = Time;
	if (txt)
	{
		if (text) delete[] text;
		text = new char[strlen(txt)+1];
		strcpy(text,txt);
	}
	Messages.SetCurrent(this);
}

void CTextMessage::SetSettings(int _x, int _y, int t, int fnt, char* _tp, char* _ts)
{
	if (_tp) 
	{
		if (text_prefix!=NULL) delete[] text_prefix;
		text_prefix = new char[strlen(_tp)+1];
		strcpy(text_prefix,_tp);
	}
	

	if (_ts)
	{
		if (text_suffix!=NULL) delete[] text_suffix;
		text_suffix = new char[strlen(_ts)+1];
		strcpy(text_suffix,_ts);
	}

	x = _x;
	y = _y;
	font = fnt;
	Time = t;
}
void CTextMessage::Draw()
{
	if (!visible) return;
	TickCount--;
	if (TickCount <= 0) visible=false;

	char str[400]="";
	if (text_prefix) strcat(str,text_prefix);
	if (text) strcat(str,text);
	if (text_suffix) strcat(str,text_suffix);
	fnP_DLL_ExtendedTextout(x,y,font,false,str);
}

CMessages::CMessages()
{
	cur = NULL;
}

void CMessages::SetCurrent(CTextMessage* c)
{
	cur = c;
}
void CMessages::RegisterConsoleCommands()
{
	cf_fragmsg = new c_var_int("cf_fragmsg","cf_fragmsg",DEFAULT_FRAGMSG_FONT,0,6);
	ch_fragmsg_x = new c_var_int("ch_fragmsg_x","ch_fragmsg_x",200,0,VID_WIDTH);
	ch_fragmsg_y = new c_var_int("ch_fragmsg_y","ch_fragmsg_y",100,0,VID_HEIGHT);
	ch_fragmsg_time = new c_var_int("ch_fragmsg_time","ch_fragmsg_time",100,0,1000);
	ch_fragmsg = new c_var_bool("ch_fragmsg","ch_fragmsg",false);
	ch_fragmsg_color = new c_var_string("ch_fragmsg_color","ch_fragmsg_color","7",ch_fragmsg_color_OnChange);
	ch_fragmsg_colored_name = new c_var_bool("ch_fragmsg_colored_name","ch_fragmsg_colored_name",1);

	ch_teamchat = new c_var_int("ch_teamchat","ch_teamchat",0,0,10);
	ch_teamchat_x = new c_var_int("ch_teamchat_x","ch_teamchat_x",5,0,VID_WIDTH);
	ch_teamchat_y = new c_var_int("ch_teamchat_y","ch_teamchat_y",425,0,VID_HEIGHT);
	ch_teamchat_dy = new c_var_int("ch_teamchat_dy","ch_teamchat_dy",12,0,50);
	ch_teamchat_time = new c_var_int("ch_teamchat_time","ch_teamchat_time",125,0,1000);
	cf_teamchat = new c_var_int("cf_teamchat","cf_teamchat",2,0,6);
	ch_teamchat_printname = new c_var_bool("ch_teamchat_printname","ch_teamchat_printname",true);
	ch_teamchat_printlocation = new c_var_bool("ch_teamchat_printlocation","ch_teamchat_printlocation",true);
	ch_teamchat_showmyself = new c_var_bool("ch_teamchat_showmyself","ch_teamchat_showmyself",false);
	ch_teamchat_color = new c_var_string("ch_teamchat_color","ch_teamchat_color","5",ch_teamchat_color_OnChange);

	fnP_DLL_RegisterConsoleCommand("cf_fragmsg");
	fnP_DLL_RegisterConsoleCommand("ch_fragmsg_x");
	fnP_DLL_RegisterConsoleCommand("ch_fragmsg_y");
	fnP_DLL_RegisterConsoleCommand("ch_fragmsg_time");
	fnP_DLL_RegisterConsoleCommand("ch_fragmsg");
	fnP_DLL_RegisterConsoleCommand("ch_fragmsg_color");
	fnP_DLL_RegisterConsoleCommand("ch_fragmsg_colored_name");

	fnP_DLL_RegisterConsoleCommand("ch_teamchat");
	fnP_DLL_RegisterConsoleCommand("ch_teamchat_x");
	fnP_DLL_RegisterConsoleCommand("ch_teamchat_y");
	fnP_DLL_RegisterConsoleCommand("ch_teamchat_dy");
	fnP_DLL_RegisterConsoleCommand("ch_teamchat_time");
	fnP_DLL_RegisterConsoleCommand("ch_teamchat_printname");
	fnP_DLL_RegisterConsoleCommand("ch_teamchat_printlocation");
	fnP_DLL_RegisterConsoleCommand("ch_teamchat_showmyself");
	fnP_DLL_RegisterConsoleCommand("cf_teamchat");
	fnP_DLL_RegisterConsoleCommand("ch_teamchat_color");
}
/*-------------------------------------------
*	EVENT_MainLoop()
*	Основной цикл этого модуля
*-------------------------------------------*/
void CMessages::EVENT_MainLoop()
{
	if (cur) cur->Draw(); //Выводит текущее сообщение
	if (TeamMessages.size())
	{
		//Выводим все записи по списку
		TeamMessages.top();
		if (ch_teamchat->value())
		{
			int i=ch_teamchat->value();
			do
			{
				fnP_DLL_ExtendedTextout(ch_teamchat_x->value(),ch_teamchat_y->value()-ch_teamchat_dy->value()*i,cf_teamchat->value(),0,TeamMessages.cur()->d.text);
				i--;
			}
			while(TeamMessages.next() && i>0);
		}


		//Удаляем устаревшие записи
		TeamMessages.top();
		do
		{
			TeamMessages.cur()->d.time++;
			if (TeamMessages.cur()->d.time > ch_teamchat_time->value())
			{
				delete[] TeamMessages.cur()->d.text;
				TeamMessages.delCur();
			}
		}
		while(TeamMessages.next());
	}
}

void CMessages::ChatReceived(WORD DXID, char* Text)
{
	if (!IsTeamplay) return;
	if (ch_teamchat->value()==0) return;
	if (DXID == LocalDXID && ch_teamchat_showmyself->value()==0) return;

	CPlayer* Pl; //Временная переменная
	Pl = FindPlayerByDXID(DXID);
	if (Pl == NULL) return;
	if (Pl->GetTeam() != LocalPlayer->GetTeam()) return;

	ItemTeamMsg* NewTeamMsg = new ItemTeamMsg;
	NewTeamMsg->d.time = 0;
	if (ch_teamchat_printname->value())
	{
		NewTeamMsg->d.text = new char[strlen(Text)+strlen(Pl->GetNetname())+6+80];
		strcpy(NewTeamMsg->d.text,Pl->GetNetname());
		if (ch_teamchat_printlocation->value()) strcat(NewTeamMsg->d.text,va(" ^7(%s^7): ",Pl->GetLocation()));
		strcat(NewTeamMsg->d.text,va("^%s",ch_teamchat_color->value()));
		strcat(NewTeamMsg->d.text,Text);
	}
	else
	{
		NewTeamMsg->d.text = new char[strlen(Text)+3+80];
		strcpy(NewTeamMsg->d.text,"\0");
		if (ch_teamchat_printlocation->value()) strcat(NewTeamMsg->d.text,va("^7(%s^7): ",Pl->GetLocation()));
		strcat(NewTeamMsg->d.text,va("^%s",ch_teamchat_color->value()));
		strcat(NewTeamMsg->d.text,Text);
	}

	TeamMessages.addBefFir(NewTeamMsg);
}

void ch_fragmsg_color_OnChange(char* new_val)
{
	if (new_val!=NULL)
	{
		if (ColorCheck(new_val))
		{
			ch_fragmsg_color->set_value_string(new_val);
			Console.printf( "ch_fragmsg_color changet to %s" , new_val );
			return;
		}
		else Console.print("Invalid color");
	}
	ch_fragmsg_color->print(); //Выводит на консоль текущее значение переменной
}

void ch_teamchat_color_OnChange(char* new_val)
{
	if (new_val!=NULL)
	{
		if (ColorCheck(new_val))
		{
			ch_teamchat_color->set_value_string(new_val);
			Console.printf( "ch_teamchat_color changet to %s" , new_val );
			return;
		}
		else Console.print("Invalid color");
	}
	ch_teamchat_color->print(); //Выводит на консоль текущее значение переменной
}