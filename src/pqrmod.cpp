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

/*
	pqrmod.cpp : Defines the entry point for the DLL afnPlication.
	Основной файл PQR Mod.
	Он же содердит в себе все вызваемые из NFK функции, типа EVENT_MainLoop

	*Порядок выозва функций из NFK:
	DLL_QueryVersion
	DLL_AddModel <много раз>

	*При старте игры:
	DLL_SYSTEM_AddPlayer
	DLL_EVENT_BeginGame
	DLL_EVENT_ResetGame
	DLL_EVENT_MapChanged
	DLL_EVENT_ResetGame

	*При LeaveArena
	DLL_RemoveAllPlayers
*/

#include "pqrmod.h"

#include "avi.h"
#include "animation.h"
#include "autorecord.h"
#include "config_manager.h"
#include "console.h"
#include "graph.h"
#include "teamhelp.h"
#include "hud.h"
#include "input.h"
#include "lagometer.h"
#include "map.h"
#include "menu.h"
#include "main_menu.h"
#include "messages.h"
#include "multibot.h"
#include "mw_hook.h"
#include "pm_api.h"
#include "player.h"
#include "ruleset.h"
#include "scorebot.h"
#include "tools_menu.h"

HMODULE hLib;
char NFKRootDir[200];
char NFKGameDir[200];
char NFKVersion[50];
int NFKVersionI;
bool BeginGameInProcess;
bool match_in_process;
bool FlagMainLoop = false; //Основной цикл программы переодически выполняется
bool FirstFrameAfterReset = false;
bool IsClient = false;
bool IsTeamplay = false;
int TickCountMainLoop = 0;
HINSTANCE hModule1;
HWND NFKwind = NULL;
WORD CheckDeadOnNextFrame;

char* ModVersion()
{
	static char ver[200];
	strcpy(ver,MOD_VERSION);
	strcat(ver," (build ");
	char tmp[30];
	itoa(build_number(),tmp,10);
	strcat(ver,tmp);
//	strcat(ver," ");
//	strcat(ver,__TIME__);
	strcat(ver,")");
	return ver;
}

void PQRMOD_init()
{
	MENU_Init();
	Input.Init();
	//Почему-то не работает загрузка файла с настройками мода в самом начале
	//ConfigLoad(__MOD_DEFAULT_CONFIG__); //Функция описана в модуле config (config.cpp)
}



BOOL APIENTRY DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			PQRMOD_init();
			hModule1=hModule;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			//Почему-то не работает сохранение файла с настройками мода в самом начале
			//ConfigSave(__MOD_DEFAULT_CONFIG__); //Функция описана в модуле config (config.cpp)
			MW_HOOK_ShutDown();
			FreeLibrary(hLib);
			break;
    }
    return TRUE;
}
PQRMOD_API void C_DLL_RegisterMainBotDll(char* path)
{
	
	hLib = LoadLibrary(path);
	(FARPROC &)fnP_DLL_SetAngle = GetProcAddress(hLib, "P_DLL_SetAngle");
	(FARPROC &)fnP_DLL_AddMessage = GetProcAddress(hLib, "P_DLL_AddMessage");
	(FARPROC &)fnP_DLL_RegisterConsoleCommand = GetProcAddress(hLib, "P_DLL_RegisterConsoleCommand");
	(FARPROC &)fnP_DLL_GetSystemVariable = GetProcAddress(hLib, "P_DLL_GetSystemVariable");
	(FARPROC &)fnP_DLL_sys_CreatePlayer = GetProcAddress(hLib, "P_DLL_sys_CreatePlayer");
	
	(FARPROC &)fnP_DLL_SetKeys = GetProcAddress(hLib, "P_DLL_SetKeys");
	(FARPROC &)fnP_DLL_SetBalloon = GetProcAddress(hLib, "P_DLL_SetBalloon");
	(FARPROC &)fnP_DLL_SetWeapon = GetProcAddress(hLib, "P_DLL_SetWeapon");
	(FARPROC &)fnP_DLL_SendConsoleCommand = GetProcAddress(hLib, "P_DLL_SendConsoleCommand");
	(FARPROC &)fnP_DLL_SendConsoleHCommand = GetProcAddress(hLib, "P_DLL_SendConsoleHCommand");
	(FARPROC &)fnP_DLL_Test_Blocked = GetProcAddress(hLib, "P_DLL_Test_Blocked");
	(FARPROC &)fnP_DLL_debug_textout = GetProcAddress(hLib, "P_DLL_debug_textout");
	(FARPROC &)fnP_DLL_debug_textoutc = GetProcAddress(hLib, "P_DLL_debug_textoutc");
	(FARPROC &)fnP_DLL_ExtendedTextout = GetProcAddress(hLib, "P_DLL_ExtendedTextout");

	(FARPROC &)fnP_DLL_FX_FillRect = GetProcAddress(hLib, "P_DLL_FX_FillRect");
	(FARPROC &)fnP_DLL_FX_FillRectMap = GetProcAddress(hLib, "P_DLL_FX_FillRectMap");
	(FARPROC &)fnP_DLL_FX_FillRectMapEx = GetProcAddress(hLib, "P_DLL_FX_FillRectMapEx");
	(FARPROC &)fnP_DLL_FX_Rectangle = GetProcAddress(hLib, "P_DLL_FX_Rectangle");
	(FARPROC &)fnP_DLL_FX_Line = GetProcAddress(hLib, "P_DLL_FX_Line");
	(FARPROC &)fnP_DLL_MouseDelta = GetProcAddress(hLib, "P_DLL_MouseDelta");
	(FARPROC &)fnP_DLL_KeyPressed = GetProcAddress(hLib, "P_DLL_KeyPressed");
	(FARPROC &)fnP_DLL_PatchBot = GetProcAddress(hLib, "P_DLL_PatchBot");

	(FARPROC &)fnP_DLL_GetBrickStruct = GetProcAddress(hLib, "P_DLL_GetBrickStruct");
	(FARPROC &)fnP_DLL_GetObjStruct = GetProcAddress(hLib, "P_DLL_GetObjStruct");
	(FARPROC &)fnP_DLL_GetSpecObjStruct = GetProcAddress(hLib, "P_DLL_GetSpecObjStruct");

	(FARPROC &)fnP_DLL_RemoveBot = GetProcAddress(hLib, "P_DLL_RemoveBot");
	(FARPROC &)fnP_DLL_EVENT_ExternMenuEnter = GetProcAddress(hLib, "P_DLL_EVENT_ExternMenuEnter");
	(FARPROC &)fnP_DLL_RegBotDlls = GetProcAddress(hLib, "P_DLL_RegBotDlls");
	(FARPROC &)fnP_DLL_SetActiveBot = GetProcAddress(hLib, "P_DLL_SetActiveBot");
	(FARPROC &)fnP_DLL_SetStatusOnOff = GetProcAddress(hLib, "P_DLL_SetStatusOnOff");

}


PQRMOD_API void C_DLL_NFKStarted()
{
	strcpy(NFKRootDir,fnP_DLL_GetSystemVariable("rootdir"));
	strcpy(NFKGameDir,fnP_DLL_GetSystemVariable("gamedir"));
	if (!strlen(NFKGameDir)) strcpy(NFKGameDir,NFKRootDir);
	strcpy(NFKVersion,fnP_DLL_GetSystemVariable("nfkversion"));
	NFKVersionI=atoi(NFKVersion);

	//DEBUG PRINT
	//fnP_DLL_AddMessage(NFKVersion);

	NFKwind = FindWindow(NULL,"Need For Kill");

	MW_HOOK_RegisterConsoleCommands(); //Регистрируем команды модуля mw_hook
	MENU_RegisterConsoleCommands(); //Регистрируем команды модуля menu

	CFG_MAN_RegisterConsoleCommands(); //Регистрируем команды модуля config_manager
	Multibot.RegisterConsoleCommands(); //Регистрируем команды модуля multibot

	Autorecord.RegisterConsoleCommands();
	Ruleset.RegisterConsoleCommands();
	TeamHelp.RegisterConsoleCommands();
	GRAPH_RegisterConsoleCommands();
	Console.RegisterConsoleCommands();
	Messages.RegisterConsoleCommands();
	AVI.RegisterConsoleCommands();
	HUD.RegisterConsoleCommands();
	Lagometer.RegisterConsoleCommands();
	Scorebot.RegisterConsoleCommands();

	CFG_MAN_EVENT_NFKStarted();//Важно! Должен стоять перед Multibot.EVENT_NFKStarted()
	//чтобы из конфига при первом запуске игры
	//можно было отключать ненужных ботов командами mb_off <dll_number>
	Multibot.EVENT_NFKStarted();

	//Инициализируем меню
	//Инициализация должна происходить после регистрации всех команд,
	//т.к. некоторые меню используют консольные переменные при инициализациии своих пунктов
	MAIN_MENU_Init();
	TOOLS_MENU_Init();
}
PQRMOD_API void C_DLL_SYSTEM_RemoveAllPlayers()
{
	if (players.size()!=0) //Перебираем в цикле всех игроков
	{
		players.top();
		do
		{
			delete players.cur()->d;
			players.delCur();
		}
		while(players.size()!=0);
	}
}
PQRMOD_API void C_DLL_SYSTEM_RemovePlayer(WORD DXID)
{
	if (players.size()!=0) //Перебираем в цикле всех игроков
	{
		players.top();
		do
			if (players.cur()->d->GetDXID() == DXID)
			{
				delete players.cur()->d;
				players.delCur();
				break;
			}
		while(players.next());
	}
}
PQRMOD_API void C_DLL_EVENT_BeginGame()
{
	if (!strcmp(fnP_DLL_GetSystemVariable("netstatus"),"2")) IsClient = true;
	else IsClient = false;
	
	//DEBUG PRINT
	//if (IsClient) fnP_DLL_AddMessage("Client");
	//else fnP_DLL_AddMessage("Server");

	//DEBUG PRINT
	//fnP_DLL_AddMessage("^2C_DLL_EVENT_BeginGame");
	LocalDXID = 0;
	LocalDXID2 = 0;
	LocalPlayer = NULL;
	LocalPlayer2 = NULL;

	LocalDXID = atoi(fnP_DLL_GetSystemVariable("localdxid"));
	LocalPlayer = FindPlayerByDXID(LocalDXID);
	LocalDXID2 = atoi(fnP_DLL_GetSystemVariable("localdxid2"));
	LocalPlayer2 = FindPlayerByDXID(LocalDXID2);
	
	Multibot.EVENT_BeginGame();
	Logo.Show(75); //Отображает лого в течении 75 кадров (graph.cpp)
	
}
PQRMOD_API void C_DLL_EVENT_ResetGame()
{
	//DEBUG PRINT
	//fnP_DLL_AddMessage("^2C_DLL_EVENT_ResetGame");

	match_in_process=false;
	FirstFrameAfterReset = true;
	TickCountMainLoop = 0;

	IsTeamplay=false;
	char gametype[30];
	strcpy(gametype,fnP_DLL_GetSystemVariable("gametype"));
	if (!strcmp(gametype,"TDM") || !strcmp(gametype,"DOM") || !strcmp(gametype,"CTF")) IsTeamplay=true;

	char tmpstr[10];
	strcpy(tmpstr,fnP_DLL_GetSystemVariable("warmupleft"));
	if (!strcmp(tmpstr,"0")) //При ResetGame warmup == 0 - Началась игра.
	{
		match_in_process=true;
		if (IsClient) Autorecord.EVENT_Warmup_5();
	}

	Autorecord.EVENT_ResetGame();
	TeamHelp.EVENT_ResetGame();
	//m_Map.EVENT_ResetGame();
}
PQRMOD_API void C_DLL_EVENT_MapChanged()
{
	//DEBUG PRINT
	//fnP_DLL_AddMessage("^2C_DLL_EVENT_MapChanged");

	m_Map.EVENT_MapChanged();
}

PQRMOD_API void C_DLL_MainLoop()
{
	if (!match_in_process) //Если матч ещё не начался (warmup)
	{
		TickCountMainLoop++;
		if (TickCountMainLoop>50)
		{
			TickCountMainLoop = 0;
			if (FirstFrameAfterReset) { m_Map.ScanItems(); FirstFrameAfterReset=false;}
			//Раз в секунду проверяем значение warmup, и если оно от 0 до 5, отсылаем сообщение неоторым модулям
			
			char tmpstr[10];
			strcpy(tmpstr,fnP_DLL_GetSystemVariable("warmupleft"));
			int warmupleft = atoi(tmpstr);
			if (warmupleft<=5 && warmupleft>0) Autorecord.EVENT_Warmup_5();
			
			//Временная мера, т.к. на клиенте не вызывается EVENT_ResetGame
			//невозможно определить начало матча, иначе чем постоянно проверяя это в MainLoop
			else
				if (!match_in_process) //Матч как бы ещё не начался
				if (warmupleft	== 0) //Но warmup оказывается = 0
				if (IsClient) //и это клиент
				{
					Autorecord.EVENT_Warmup_5(); //На клиенте warmup изменяется дискретно. Либо 999 либо 0, поэтому невозможно обнаружить значение warmup = 5
					match_in_process = true;
				} 
		}
	}

	GRAPH_EVENT_MainLoop(); //Отображает на экране различную графику, например, лого PQR Mod
	TeamHelp.EVENT_MainLoop(); 
	HUD.EVENT_MainLoop();
	Lagometer.EVENT_MainLoop();
	MW_HOOK_EVENT_MainLoop();
	ANIMATION_EVENT_MainLoop();
	MENU_EVENT_MainLoop();
	Messages.EVENT_MainLoop();
	Input.EVENT_MainLoop();
	Scorebot.EVENT_MainLoop();

	Console.EVENT_MainLoop(); //Лучше вызывать после всех модулей
}
PQRMOD_API void C_DLL_SYSTEM_AddPlayer(TPlayerEx Player)
{
	//DEBUG PRINT
	//Console.print("^2C_DLL_SYSTEM_AddPlayer");

	CPlayer* NewPlayer = new CPlayer(Player);
	ItemPlayer* NewItemPlayer = new ItemPlayer;
	NewItemPlayer->d = NewPlayer;
	players.addAftLas(NewItemPlayer);

	if (LocalPlayer == NULL) 
	{
		LocalDXID = atoi(fnP_DLL_GetSystemVariable("localdxid"));
		LocalPlayer = FindPlayerByDXID(LocalDXID);
	}
}
PQRMOD_API void C_DLL_SYSTEM_UpdatePlayer(TPlayerEx Player)
{
	
	if (players.size()!=0) //Перебираем в цикле всех игроков
	{
		players.top();
		do
			if (players.cur()->d->GetDXID() == Player.DXID)
			{
				players.cur()->d->UpdatePlayer(Player);
				return;
			}
		while(players.next());
	}
	//Если игрок для которого вызвана процедура обновления не найден, создаём его:
	//C_DLL_SYSTEM_AddPlayer(Player);
}

//Вся информация об игроке обновляется через C_DLL_SYSTEM_UpdatePlayer
//Но только 3 поля: NetName, Model и Location обновляются через отдельную процедуру C_DLL_SYSTEM_UpdatePlayerString
//Это связано с различным предствлением строковых типов в Delphi и C
PQRMOD_API void C_DLL_SYSTEM_UpdatePlayerString(int code, WORD DXID,char* str)
{
	CPlayer* UP = FindPlayerByDXID(DXID);
	if (UP)
	{
		switch(code)
		{
		case 1:UP->UpdateNetName(str);
			break;
		case 2:UP->UpdateModel(str);
			break;
		case 3:UP->UpdateLocation(str);
			break;
		}
	}
}
PQRMOD_API void C_DLL_AddModel(char* s)
{
}
PQRMOD_API void C_DLL_DMGReceived(WORD TargetDXID, WORD AttackerDXID, WORD dmg)
{
	if (ch_fragmsg->value() && LocalDXID && LocalDXID==AttackerDXID && TargetDXID!=LocalDXID)
	{
		CPlayer* Target = NULL;
		Target = FindPlayerByDXID(TargetDXID);
		if ( Target!=NULL )
		{
			float da = Target->Armor() - dmg*0.66; //Склько останется армора после выстрела
			if (da > 0) da = 0; //Если армор ещё есть, то все 66% дамага поглатились армором
			//Если армор получается отрицательным, то эта отрицательная величина свидетельствует о дамаге, который нужно отнять от хелся, помимо стандартных 34% ушедших на хелс
			if (Target->Health() - ((dmg - dmg*0.66) - da) <=0)
			{
				MessageYouFragged.SetSettings(ch_fragmsg_x->value(),ch_fragmsg_y->value(),ch_fragmsg_time->value(),cf_fragmsg->value(),va("^%sYou fragged%s",ch_fragmsg_color->value(),(ch_fragmsg_colored_name->value()?" ^7":" ")));
				MessageYouFragged.Show((ch_fragmsg_colored_name->value()?Target->GetNetname():StripColorName(Target->GetNetname())));
			}
		}
	} 
}
PQRMOD_API void C_DLL_ChatReceived(WORD DXID, char* Text)
{
	TeamHelp.ChatReceived(DXID, Text);
	Messages.ChatReceived(DXID, Text);
}
PQRMOD_API void C_DLL_EVENT_ConsoleMessage(char *s)
{
	Scorebot.EVENT_ConsoleMessage(s);
	//Из этой функции нельзя вызывать fnP_DLL_AddMessage - рекурсия, переполнение стека.
}
PQRMOD_API void C_DLL_CMD(char* s)
{
	Console.EVENT_ConsoleCommand(s);
}
PQRMOD_API char* C_DLL_QUERY_VERSION()
{
	return ModVersion();
}
PQRMOD_API void C_DLL_RegisterBotDll(char* s) //Delphi модуль вызывает эту процедуру столько раз, сколько загружено внешних bot.dll Таким образом внутри С++ модуля еммется вся информация о внешних bot.dll
{
	strcpy(BotsDllArr.FilesArray[BotsDllArr.FilesCount],s);
	BotsDllArr.StatusOnOff[BotsDllArr.FilesCount]=true;
	BotsDllArr.NewStatusOnOff[BotsDllArr.FilesCount]=true;
	BotsDllArr.FilesCount++;
}

PQRMOD_API void C_DLL_SetActiveBotDll(int a) //НЕ ДОЛЖНА ВЫЗВАТЬСЯ
{
	Console.print("C_DLL_SetActiveBotDll");
	int ActiveBot = mb_activebot->value();
	m_MultibotMenu.SetOptionColor(ActiveBot+2,'7');	
	//mb_activebot->set_value(a);
	m_MultibotMenu.SetOptionColor(ActiveBot+2,LIGHT_OPTION_COLOR);
}
PQRMOD_API void C_DLL_EVENT_ItemTaken(BYTE brickx, BYTE bricky, BYTE image, WORD dxid)
{
	TeamHelp.EVENT_ItemTaken(brickx,bricky,image,dxid);
}
PQRMOD_API void C_DLL_EVENT_DroppableWeaponTaken(WORD wpndxid, WORD playerdxid)
{

}

