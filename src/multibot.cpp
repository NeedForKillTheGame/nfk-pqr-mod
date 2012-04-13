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
#include "multibot.h"

#include "main_menu.h" //for m_MainMenu
#include "config_manager.h"


#define MULTIBOT_MENU_Y 144

CMultibot Multibot;
TBotsDllArray BotsDllArr;
c_var_int* mb_activebot;

void mb_activebot_OnChange(char* new_ab); //��� ��������� ���������� mb_activebot
void CMD_mb_list(char*); //��� ������ ���������� ������� mb_list. ������� �� ������� ������ �����.
//�������� char* �� ������������, �� ����� ��� ������������� � ����� fnOnExecute ������ Console

void CMD_mb_on(char* s); //��� ������ ���������� ������� mb_on.
void CMD_mb_off(char* s); //��� ������ ���������� ������� mb_off.

char CurrentBotSubmenu=0;
CModMenu m_MultibotSubMenu;
CModMenu m_MultibotMenu;

void MultibotSubMenu_Enter(unsigned char);
void MULTIBOT_MENU_Enter(unsigned char n);

CMultibot::CMultibot()
{
	BotsDllArr.FilesCount = 0;
}

void CMultibot::BeforeActiveBotChange(int i) //�������� �������� � Delphi ������ � ������������� MultibotMenu
{
	fnP_DLL_SetActiveBot(i);
	m_MultibotMenu.SetOptionColor(mb_activebot->value()+2,'7');
	m_MultibotMenu.SetOptionColor(i+2,LIGHT_OPTION_COLOR);
}

void CMultibot::EVENT_NFKStarted()
{
	memset(&BotsDllArr.FilesArray,0,sizeof(BotsDllArr.FilesArray));
	BotsDllArr.FilesCount = 0;
	fnP_DLL_RegBotDlls(); //�������� ������� ��������� � bot.dll �� Delphi,
	//������� ����� � ����� ��������� ��� ������� BOT2_API void C_DLL_RegisterBotDll(char* s)
	//����� ������� ���������� ��������� BotsDllArr ���������� ����� DLL ������ � ������
	if (BotsDllArr.FilesCount==0) mb_activebot->set_value_int(-1);
	else mb_activebot->set_value(0);
}

void CMultibot::EVENT_BeginGame()
{

	m_MultibotMenu.Hide();
	m_MultibotSubMenu.Hide();

	LoadMapConfig(NULL); //��������� pqrmod\configs\maps\���_�����.cfg
	//��� ���� �������� �����. ������ ������ ��� ������� ����������
	//�� ������� C_DLL_EVENT_MapChanged() -> LoadMap() -> LoadMapConfig()
	//������ � ���������� ����� ���� ����������� �������� mb_off, � �������������
	//�� ���������� ��������� �� fnP_DLL_SetStatusOnOff(i,bool) (�.�. �� Multibot.EVENT_BeginGame() � ���������� ������)

	//����� ����� ����� ��������� �������� �����.
	//��� ������ ������� ���� ����� ���� ��������� ������ �� �������,
	//������ ���� �������� CFG_MAN_EVENT_BeginGame() ������ ���� ���������.
	//��� ����������� ��������� ���� ���� ����� ���� ��������� ��� ��������,
	//��� � ������� ����������� ���������� ������������.
	for(int i=0;i<BotsDllArr.FilesCount;i++)
	{
		fnP_DLL_SetStatusOnOff(i,BotsDllArr.NewStatusOnOff[i]);
		BotsDllArr.StatusOnOff[i]=BotsDllArr.NewStatusOnOff[i];	
	}
	//��������, �������� �������� DLL ���� ���������, ����� ���������� ������� ����������� ����
	if (!BotsDllArr.StatusOnOff[mb_activebot->value()])
	{
		int ActiveBot=-1;
		for(int i=0;i<BotsDllArr.FilesCount;i++)
		{
			if (BotsDllArr.StatusOnOff[i]) {ActiveBot=i;break;}
		}
		mb_activebot->set_value_int(ActiveBot);
		fnP_DLL_SetActiveBot(mb_activebot->value());
	}

	MENU_Init(); //�������������� ������ ���� ���������� ��������� c ������ ����������� � ����������
}

void CMultibot::RegisterConsoleCommands()
{
	//������������ ���������� ������� ������ PQR Mod
	mb_activebot = new c_var_int("mb_activebot","mb_activebot",-1,-1,16,mb_activebot_OnChange);
	c_cmd* activebot = new c_cmd("activebot","activebot",mb_activebot_OnChange); //��������� ���������� mb_activebot ��������� ����� �������. ��� �������� ������� ��������� ����, ��� �������� ���������� mb_activebot �������������� ����� ������� ������� OnExecute = mb_activebot_OnChange
	c_cmd* mb_list = new c_cmd("mb_list","mb_list",CMD_mb_list);
	c_cmd* mb_on = new c_cmd("mb_on","mb_on",CMD_mb_on);
	c_cmd* mb_off = new c_cmd("mb_off","mb_off",CMD_mb_off);
	
	//������������ ���������� ������� � NFK.exe
	fnP_DLL_RegisterConsoleCommand("mb_activebot");
	fnP_DLL_RegisterConsoleCommand("mb_list");
	fnP_DLL_RegisterConsoleCommand("mb_on");
	fnP_DLL_RegisterConsoleCommand("mb_off");
}

void CMultibot::MENU_Init() //���������� �� ������� CMultibot::EVENT_BeginGame()
{
	m_MultibotMenu.RemoveAllOptions();
	m_MultibotMenu.AddOption("Select None");
	long SbMnMask=0;
	for(int i=0;i<BotsDllArr.FilesCount;i++)
	{
		SbMnMask = SbMnMask | 1;
		SbMnMask = SbMnMask<<1;

		char str[MAX_LEN+10];
		itoa(i,str,10);
		strcat(str,": ");
		strcat(str,BotsDllArr.FilesArray[i]);
		if (!BotsDllArr.StatusOnOff[i])
			strcat(str," (OFF)");
		m_MultibotMenu.AddOption(str);
		if (!BotsDllArr.StatusOnOff[i]) m_MultibotMenu.SetOptionColor(i+2,'!');	
	}
	//������� ��������� ����� ���� Activebot LIGHT_OPTION_COLOR ������
	int ActiveBot = ((c_var_int*)Console.FindVarByString("mb_activebot"))->value();
	if (ActiveBot<BotsDllArr.FilesCount) m_MultibotMenu.SetOptionColor(ActiveBot+2,LIGHT_OPTION_COLOR);

	m_MultibotMenu.AddOption("Return to Main Menu");
	m_MultibotMenu.AddOption("Hide Menu");
	m_MultibotMenu.SetPosition(50,MULTIBOT_MENU_Y);
	m_MultibotMenu.fnMenuEnter = MULTIBOT_MENU_Enter;
	m_MultibotMenu.SetWidth(177);

	m_MultibotMenu.SetSubMenuMask(SbMnMask);

	m_MultibotSubMenu.fnMenuEnter = MultibotSubMenu_Enter;
	m_MultibotSubMenu.SetWidth(90);

}
void MULTIBOT_MENU_Enter(unsigned char n)
{
	if (n==1) {
		mb_activebot->set_value("-1");
	}
	int ActiveBot = mb_activebot->value();
	if (n>1 && n<=1+BotsDllArr.FilesCount) {
		
		CurrentBotSubmenu = n-2;
		m_MultibotSubMenu.SetPosition(250,MULTIBOT_MENU_Y+(n-1)*VERTICAL_INTERVAL);
		m_MultibotSubMenu.RemoveAllOptions();
		if (CurrentBotSubmenu!=ActiveBot && BotsDllArr.StatusOnOff[CurrentBotSubmenu]) m_MultibotSubMenu.AddOption("Select");
		m_MultibotSubMenu.AddOption("Switch On");
		m_MultibotSubMenu.AddOption("Switch Off");
		m_MultibotSubMenu.AddOption("Back");
		m_MultibotSubMenu.SetOptionColor(((CurrentBotSubmenu!=ActiveBot && BotsDllArr.StatusOnOff[CurrentBotSubmenu])?1:0) - (BotsDllArr.NewStatusOnOff[CurrentBotSubmenu]?1:0) + 2, LIGHT_OPTION_COLOR);
		m_MultibotSubMenu.Show(); //���������� ������� ��� ���������� ���, ��� ��� ����������� ��������� �� last_active
	}
	else
	if (n==2+BotsDllArr.FilesCount)
	{
		m_MultibotMenu.Hide();
		m_MainMenu.Show();
	}
	else m_MultibotMenu.Hide(); //�������� ���� ������ bot.dll �� ��������� ��� ��������
}
void MultibotSubMenu_Enter(unsigned char n)
{
	c_var_int* ActiveBot = (c_var_int*)Console.FindVarByString("mb_activebot");
	if (n==1)
	{
		if (CurrentBotSubmenu!=ActiveBot->value() && BotsDllArr.StatusOnOff[CurrentBotSubmenu])
		{
			char s1[2];
			itoa(CurrentBotSubmenu,s1,10);
			ActiveBot->set_value(s1);	
		}
		else
		{
			char s1[2];
			itoa(CurrentBotSubmenu,s1,10);
			((c_cmd*)Console.FindVarByString("mb_on"))->OnExecute(s1);
		}
	}
	else
	if (n==2)
	{
		if (CurrentBotSubmenu!=ActiveBot->value() && BotsDllArr.StatusOnOff[CurrentBotSubmenu])
		{
			char s1[2];
			itoa(CurrentBotSubmenu,s1,10);
			((c_cmd*)Console.FindVarByString("mb_on"))->OnExecute(s1);
		}
		else
		{
			char s1[2];
			itoa(CurrentBotSubmenu,s1,10);
			((c_cmd*)Console.FindVarByString("mb_off"))->OnExecute(s1);
		}
	}
	else
	if (n==3)
	{
		if (CurrentBotSubmenu!=ActiveBot->value() && BotsDllArr.StatusOnOff[CurrentBotSubmenu])
		{
			char s1[2];
			itoa(CurrentBotSubmenu,s1,10);
			((c_cmd*)Console.FindVarByString("mb_off"))->OnExecute(s1);
		}
	}

		m_MultibotSubMenu.Hide();
}

void CMD_mb_list(char* s) //��� ������ ���������� ������� mb_list. ������� �� ������� ������ �����.
//�������� char* �� ������������, �� ����� ��� ������������� � ����� fnOnExecute ������ Console
{
	if (BotsDllArr.FilesCount == 0)
	{
		Console.print("no bot list loaded");
		return;
	}
	char str[MAX_LEN + 4];
	for(int i=0;i<BotsDllArr.FilesCount;i++) {
		itoa(i,str,10);
		strcat(str,": ");
		strcat(str,BotsDllArr.FilesArray[i]);
		if (!BotsDllArr.StatusOnOff[i])
			strcat(str," (OFF)");
		Console.print(str);
	}
}
void mb_activebot_OnChange(char* new_ab) //��� ��������� ���������� mb_activebot
{
	char str[200]; //��������������� ���������� ��� ������ ���������
	if (!(new_ab == NULL || strlen(new_ab) == 0)) //��� ��������� ��������� ����� ������� mb_activebot
	{
		int ActiveBot = atoi(new_ab);
		if (ActiveBot>=-1 && ActiveBot<BotsDllArr.FilesCount)
		{
			if (ActiveBot!=-1)
				if (!BotsDllArr.StatusOnOff[ActiveBot]) //���� ������������ ��� ��������
				{
					Console.print("This bot DLL is ^1OFF^7. Switch ^2ON^7 first.");
					return;
				}
			
			if (ActiveBot>=0 && ActiveBot<BotsDllArr.FilesCount) {
				strcpy(str, BotsDllArr.FilesArray[ActiveBot]);
			}
			else strcpy(str, "no one bot dll active"); //(ActiveBot==-1)

			Console.printf("mb_activebot sets to %i (%s)",ActiveBot,str);
			Multibot.BeforeActiveBotChange(ActiveBot); //������������� ���� � ������� ����� �������� � Delphi ������
			mb_activebot->set_value_int(ActiveBot); //��������������� ������ �������� 
			return;
		}
	}
	//�������� �������� �� ��������� ����������� ��� ����� �����������
	Console.print("mb_activebot <dll number>");
	if (mb_activebot->value()>=0 && mb_activebot->value()<BotsDllArr.FilesCount) {
		strcpy(str, BotsDllArr.FilesArray[mb_activebot->value()]);
	}
	else strcpy(str, "no one bot dll active"); //(ActiveBot==-1)
	Console.printf("current value: %i (%s)",mb_activebot->value(),str);
}

void CMD_mb_on(char* s) //��� ������ ���������� ������� mb_on.
{
	char* str = new char[100];
	if ( s != NULL && strlen(s)>0 )
	{
		int i=atoi(s);
		if (i>=0 && i<BotsDllArr.FilesCount)
		{
			strcpy(str, BotsDllArr.FilesArray[i]);
			if (BotsDllArr.StatusOnOff[i])
				strcat(str, " ^2ON");
			else
				strcat(str, " ^2ON^7 after LeaveArena and start new server");
			BotsDllArr.NewStatusOnOff[i] = true;
			Console.print(str);
		}
		else {
			Console.print("mb_on <dll number>");
		}
	}
	else {
		Console.print("mb_on <dll number>");
	}
}

void CMD_mb_off(char* s) //��� ������ ���������� ������� mb_off.
{
	char* str = new char[100];
	if ( s != NULL && strlen(s)>0 )
	{
		int i=atoi(s);
		if (i>=0 && i<BotsDllArr.FilesCount)
		{
			strcpy(str, BotsDllArr.FilesArray[i]);
			if (!BotsDllArr.StatusOnOff[i])
				strcat(str, " ^1OFF");
			else
				strcat(str, " ^1OFF^7 after LeaveArena and start new server");
			BotsDllArr.NewStatusOnOff[i] = false;
			Console.print(str);
		}
		else {
			Console.print("mb_off <dll number>");
		}
	}
	else {
		Console.print("mb_off <dll number>");
	}
}
/* ������� �� ������������, �.�. ����� ������ ���������� � bot.dll �� Delphi
void FindBots(char *sPath)
{
	if (BotsCount>MAX_BOTS) return;

	char *sStr;

	char sCurFullPath[MAX_LEN];
	strcpy(sCurFullPath,_BOTS_FOLDER_);

	char tmp[MAX_LEN];
	strcpy(tmp,sPath);
	strcat(sCurFullPath,tmp);
	strcat(sCurFullPath,"*");

	WIN32_FIND_DATA FindData;
	HANDLE hFindFiles=FindFirstFile(sCurFullPath,&FindData);

	if (hFindFiles==INVALID_HANDLE_VALUE) return;
	for(;;)
	{
	   if ((strcmp(FindData.cFileName,".")!=0) && (strcmp(FindData.cFileName,"..")!=0))
	   {
	      if (FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		  {
		  	strcpy(sStr,sPath);
		  	strcat(sStr,FindData.cFileName);
		  	strcat(sStr,"\\");
		  	FindBots(sStr);
		  }
		  else
		  {
		  	char *ptr=strrchr(FindData.cFileName,'.');
		  	if (ptr)
		  	{
		  		if (strlen(ptr)==4)
		  		{
		  			if (ptr[1]=='d' && ptr[2]=='l' && ptr[3]=='l')
		  			{
          				char sPath1[MAX_LEN];
		  				strcpy(sPath1,sPath);
		  				strcat(sPath1,FindData.cFileName);

		  				strcpy(BotsArr[BotsCount++],sPath1);
          			}
		  		}
		  	}
		  }
		}
		if (!FindNextFile(hFindFiles,&FindData)) break;
	}
	FindClose(hFindFiles);
}*/