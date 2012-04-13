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
	//Регистрируем консольные команды внутри PQR Mod
	c_cmd* cfg_save = new c_cmd("cfg_save","cfg_save",CMD_cfg_save);
	c_cmd* cfg_save_pm = new c_cmd("cfg_save_pm","cfg_save_pm",CMD_cfg_save_pm);
	c_cmd* cfg_save_map = new c_cmd("cfg_save_map","cfg_save_map",SaveMapConfig);
	c_cmd* cfg_load = new c_cmd("cfg_load","cfg_load",ConfigLoad);
	c_cmd* cfg_load_map = new c_cmd("cfg_load_map","cfg_load_map",LoadMapConfig);
	
	//Регистрируем консольные команды в NFK.exe
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
	//Очищаем файл
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
	
	//ЗДЕСЬ СДЕЛАТЬ СОХРАНЕНИЕ КОНФИГА - СДЕЛАНО
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

	if (KEY_Mouse1.GetCmdDown()!=NULL) fputs(va("bind2 mouse5 %s\n",KEY_Mouse5.GetCmdDown()),f);
	if (KEY_Mouse2.GetCmdDown()!=NULL) fputs(va("bind2 mouse6 %s\n",KEY_Mouse6.GetCmdDown()),f);
	if (KEY_Mouse3.GetCmdDown()!=NULL) fputs(va("bind2 mouse7 %s\n",KEY_Mouse7.GetCmdDown()),f);
	if (KEY_Mouse4.GetCmdDown()!=NULL) fputs(va("bind2 mouse8 %s\n",KEY_Mouse8.GetCmdDown()),f);
	if (KEY_Mouse5.GetCmdDown()!=NULL) fputs(va("bind2 mouse5 %s\n",KEY_Mouse5.GetCmdDown()),f);
	if (KEY_Mouse6.GetCmdDown()!=NULL) fputs(va("bind2 mouse6 %s\n",KEY_Mouse6.GetCmdDown()),f);
	if (KEY_Mouse7.GetCmdDown()!=NULL) fputs(va("bind2 mouse7 %s\n",KEY_Mouse7.GetCmdDown()),f);
	if (KEY_Mouse8.GetCmdDown()!=NULL) fputs(va("bind2 mouse8 %s\n",KEY_Mouse8.GetCmdDown()),f);

	if (KEY_Return.GetCmdDown()!=NULL) fputs(va("bind2 enter %s\n",KEY_Return.GetCmdDown()),f);
	if (KEY_Tab.GetCmdDown()!=NULL) fputs(va("bind2 tab %s\n",KEY_Tab.GetCmdDown()),f);
	if (KEY_Space.GetCmdDown()!=NULL) fputs(va("bind2 space %s\n",KEY_Space.GetCmdDown()),f);
	if (KEY_BackSpace.GetCmdDown()!=NULL) fputs(va("bind2 backspace %s\n",KEY_BackSpace.GetCmdDown()),f);
	if (KEY_Shift.GetCmdDown()!=NULL) fputs(va("bind2 shift %s\n",KEY_Shift.GetCmdDown()),f);
	if (KEY_Ctrl.GetCmdDown()!=NULL) fputs(va("bind2 ctrl %s\n",KEY_Ctrl.GetCmdDown()),f);
	if (KEY_Up.GetCmdDown()!=NULL) fputs(va("bind2 uparrow %s\n",KEY_Up.GetCmdDown()),f);
	if (KEY_Down.GetCmdDown()!=NULL) fputs(va("bind2 downarrow %s\n",KEY_Down.GetCmdDown()),f);
	if (KEY_Left.GetCmdDown()!=NULL) fputs(va("bind2 leftarrow %s\n",KEY_Left.GetCmdDown()),f);
	if (KEY_Right.GetCmdDown()!=NULL) fputs(va("bind2 rightarrow %s\n",KEY_Right.GetCmdDown()),f);
	if (KEY_Home.GetCmdDown()!=NULL) fputs(va("bind2 home %s\n",KEY_Home.GetCmdDown()),f);
	if (KEY_End.GetCmdDown()!=NULL) fputs(va("bind2 end %s\n",KEY_End.GetCmdDown()),f);
	if (KEY_PgUp.GetCmdDown()!=NULL) fputs(va("bind2 pgup %s\n",KEY_PgUp.GetCmdDown()),f);
	if (KEY_PgDown.GetCmdDown()!=NULL) fputs(va("bind2 pgdown %s\n",KEY_PgDown.GetCmdDown()),f);

	for(int i=33;i<96;i++)
	{
		if (KEY_Array[i-33].GetCmdDown()!=NULL) fputs(va("bind2 %c %s\n",i,KEY_Array[i-33].GetCmdDown()),f);;
	}

	fclose(f);
	Console.printf("Config saved to file %s.cfg",fn);
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
		//Ищем конфиг в basenfk
		strcpy(path,NFKRootDir);
		strcat(path,"\\");
		strcat(path,fn);
		strcat(path,".cfg");

		f = fopen(path,"rt");
		
		if (f==NULL)
		{
			Console.printf("^1Faild to open %s.cfg",fn);
			return;
		}
	}

	char str[1025];
	char* p;
	
	while(fgets(str,1024,f))
	{
		//Удаляем символ конца строки из текста команды
		p=NULL;
		p = strstr(str,"\n");
		if (p!=NULL) p[0] = '\0';
		p = strstr(str,"\r");
		if (p!=NULL) p[0] = '\0';

		//DEBUG PRINT
		//Console.print(str);

		Console.SendConsoleHCommand(str); //Не ParseAlias иначе невозможно будет загружать альясы
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
		//ЗДЕСЬ СДЕЛАТЬ СОХРАНЕНИЕ КОНФИГА ДЛЯ КАРТЫ - СДЕЛАНО
		fputs("\n\n//PQR Mod\n",f);
		if (Console.ListCMDs.size()!=0) //Ищем соответствующую команду
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