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

#include "config_manager.h"
#include "input.h"


void CFG_MAN_EVENT_NFKStarted()
{
	ConfigLoad(__MOD_DEFAULT_CONFIG__);
}

void CFG_MAN_RegisterConsoleCommands()
{
	//������������ ���������� ������� ������ PQR Mod
	c_cmd* cfg_save = new c_cmd("cfg_save","cfg_save",CMD_cfg_save);
	c_cmd* cfg_save_pm = new c_cmd("cfg_save_pm","cfg_save_pm",CMD_cfg_save_pm);
	c_cmd* cfg_save_map = new c_cmd("cfg_save_map","cfg_save_map",SaveMapConfig);
	c_cmd* cfg_load = new c_cmd("cfg_load","cfg_load",ConfigLoad);
	c_cmd* cfg_load_map = new c_cmd("cfg_load_map","cfg_load_map",LoadMapConfig);
	
	//������������ ���������� ������� � NFK.exe
	fnP_DLL_RegisterConsoleCommand("cfg_save");
	fnP_DLL_RegisterConsoleCommand("cfg_save_pm");
	fnP_DLL_RegisterConsoleCommand("cfg_load");
	fnP_DLL_RegisterConsoleCommand("cfg_save_map");
	fnP_DLL_RegisterConsoleCommand("cfg_load_map");
}

void CMD_cfg_save(char* s)
{
	if (s==NULL || strlen(s)<1)
	{
		Console.print("USAGE: cfg_save <filename>");
		return;
	}
	Console.SendConsoleHCommand(va("writeconfig %s",s));
	ConfigSave(s);
}

void CMD_cfg_save_pm(char* s)
{
	if (s==NULL || strlen(s)<1)
	{
		Console.print("USAGE: cfg_save_pm <filename>");
		return;
	}
	//������� ����
	char* path = new char[255];
	strcpy(path,NFKGameDir);
	strcat(path,"\\");
	strcat(path,s);
	strcat(path,".cfg");
	FILE* f = fopen(path,"w");
	delete[] path;
	fclose(f);

	ConfigSave(s);
}

void ConfigSave(char* fn)
{
	if (fn==NULL || strlen(fn)<1)
	{
		return;
	}

	char* path = new char[255];
	strcpy(path,NFKGameDir);
	strcat(path,"\\");
	strcat(path,fn);
	strcat(path,".cfg");
		
	//DEBUG PRINT
	//Console.print(path);

	FILE* f = fopen(path,"a");
	delete[] path;
	if (f==NULL) 
	{
		Console.printf("Config save to file %s.cfg ^1failed",fn);
		return;
	}
	
	//����� ������� ���������� ������� - �������
	fputs("\n\n//PQR Mod\n",f);
	if (Console.ListCMDs.size()!=0) 
	{
		Console.ListCMDs.top();
		do
		{
			Console.ListCMDs.cur()->d->print2(f);
		}
		while (Console.ListCMDs.next());
	}

	if (Console.ListAliases.size()!=0)
	{
		Console.ListAliases.top();
		do
		{
			Console.ListAliases.cur()->d->print2(f);
		}
		while (Console.ListAliases.next());
	}

	if (KEY_Mouse5.GetCmdDown()!=NULL) fputs(va("bind mouse5 %s\n",KEY_Mouse5.GetCmdDown()),f);
	if (KEY_Mouse6.GetCmdDown()!=NULL) fputs(va("bind mouse6 %s\n",KEY_Mouse6.GetCmdDown()),f);
	if (KEY_Mouse7.GetCmdDown()!=NULL) fputs(va("bind mouse7 %s\n",KEY_Mouse7.GetCmdDown()),f);
	if (KEY_Mouse8.GetCmdDown()!=NULL) fputs(va("bind mouse8 %s\n",KEY_Mouse8.GetCmdDown()),f);

	fclose(f);
	Console.printf("Config saved to file %s",fn);
}

void ConfigLoad(char* fn)
{
	if (fn==NULL) return;
	if (strlen(fn)<1) return;

	char path[255];
	
	strcpy(path,NFKGameDir);
	strcat(path,"\\");
	strcat(path,fn);
	strcat(path,".cfg");

	//DEBUG PRINT
	//Console.print(path);

	FILE* f = fopen(path,"rt");
	if (f==NULL)
	{
		//���� ������ � basenfk
		strcpy(path,NFKRootDir);
		strcat(path,"\\");
		strcat(path,fn);
		strcat(path,".cfg");
		
		Console.printf("^1Faild to open %s.cfg",fn);
		return;
	}

	char str[1025];
	char* p;
	
	while(fgets(str,1024,f))
	{
		//������� ������ ����� ������ �� ������ �������
		p=NULL;
		p = strstr(str,"\n");
		if (p!=NULL) p[0] = '\0';
		p = strstr(str,"\r");
		if (p!=NULL) p[0] = '\0';

		//DEBUG PRINT
		//Console.print(str);

		Console.SendConsoleHCommand(str); //�� ParseAlias ����� ���������� ����� ��������� ������
	}

	fclose(f);
	
	//Console.SendConsoleCommand(va("exec %s",fn));
}

void LoadMapConfig(char* s)
{
	char str[255];
	strcpy(str,"configs\\maps\\");
	strcat(str,fnP_DLL_GetSystemVariable("mapname"));
	ConfigLoad(str);
}

void SaveMapConfig(char* s)
{
	char path[255];
	char s1[255];
	strcpy(path,NFKGameDir);
	strcat(path,"\\configs\\maps\\");
	strcat(s1,fnP_DLL_GetSystemVariable("mapname"));
	strcat(s1,".cfg");
	strcat(path,s1);

	FILE* f = fopen(path,"wt");
	if (f!=NULL)
	{
		//����� ������� ���������� ������� ��� ����� - �������
		fputs("\n\n//PQR Mod\n",f);
		if (Console.ListCMDs.size()!=0) //���� ��������������� �������
		{
			Console.ListCMDs.top();
			do
			{
				Console.ListCMDs.cur()->d->print2(f);
			}
			while (Console.ListCMDs.next());
		}
		fclose(f);
		Console.printf("^2Config saved to file pqrmod\\configs\\maps\\%s.cfg",s1);
	}
	else
	{
		Console.printf("^1Config save to file pqrmod\\configs\\maps\\%s.cfg failed",s1);
	}
}