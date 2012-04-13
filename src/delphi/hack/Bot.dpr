{

	BOT.DLL for Need For Kill
	(c) 3d[Power]
	http://www.3dpower.org

        unit: bot.dpr
        purpose: system procedures.

        warning: do not modify this unit.
}

library bot;
{$S-}
uses
  SysUtils,
  Classes,
  Windows,
  WinProcs,
  WinTypes,
  bot_register in 'bot_register.pas';

type    TBotDllArray = record
        FilesCount: Shortint;
		FilesArray : array[0..15] of string[64];
		StatusOnOff : array[0..15] of boolean;
        end;

// ==========================================
var
	//Массивы переменных процедурного типа для динамической связи с внешними модулями bot.dll
EXTERNAL_DLL_RegisterProc1 : array[0..15] of TCallProc_DLL_RegisterProc1;
EXTERNAL_DLL_RegisterProc2 : array[0..15] of TCallProc_DLL_RegisterProc2;
EXTERNAL_DLL_RegisterProc3 : array[0..15] of TCallProc_DLL_RegisterProc3;
EXTERNAL_DLL_RegisterProc4 : array[0..15] of TCallProc_DLL_RegisterProc4;
EXTERNAL_DLL_RegisterProc5 : array[0..15] of TCallProc_DLL_RegisterProc5;
EXTERNAL_DLL_RegisterProc6 : array[0..15] of TCallProc_DLL_RegisterProc6;
EXTERNAL_DLL_RegisterProc7 : array[0..15] of TCallProc_DLL_RegisterProc7;
EXTERNAL_DLL_RegisterProc8 : array[0..15] of TCallProc_DLL_RegisterProc8;
EXTERNAL_DLL_RegisterProc9 : array[0..15] of TCallProc_DLL_RegisterProc9;
EXTERNAL_DLL_RegisterProc10 : array[0..15] of TCallProc_DLL_RegisterProc10;
EXTERNAL_DLL_RegisterProc11 : array[0..15] of TCallProc_DLL_RegisterProc11;
EXTERNAL_DLL_RegisterProc12 : array[0..15] of TCallProc_DLL_RegisterProc12;
EXTERNAL_DLL_RegisterProc13 : array[0..15] of TCallProc_DLL_RegisterProc13;
EXTERNAL_DLL_MainLoop : array[0..15] of TCallProc_DLL_MainLoop;
EXTERNAL_DLL_EVENT_BeginGame : array[0..15] of TCallProc_DLL_EVENT_BeginGame;
EXTERNAL_DLL_EVENT_MapChanged : array[0..15] of TCallProc_DLL_EVENT_MapChanged;
EXTERNAL_DLL_EVENT_ResetGame : array[0..15] of TCallProc_DLL_EVENT_ResetGame;
EXTERNAL_DLL_QUERY_VERSION : array[0..15] of TCallProc_DLL_QUERY_VERSION;
EXTERNAL_DLL_SYSTEM_AddPlayer : array[0..15] of TCallProc_DLL_SYSTEM_AddPlayer;
EXTERNAL_DLL_SYSTEM_RemoveAllPlayers : array[0..15] of TCallProc_DLL_SYSTEM_RemoveAllPlayers;
EXTERNAL_DLL_SYSTEM_RemovePlayer : array[0..15] of TCallProc_DLL_SYSTEM_RemovePlayer;
EXTERNAL_DLL_DMGReceived : array[0..15] of TCallProc_DLL_DMGReceived;
EXTERNAL_DLL_ChatReceived : array[0..15] of TCallProc_DLL_ChatReceived;
EXTERNAL_DLL_AddModel : array[0..15] of TCallProc_DLL_AddModel;
EXTERNAL_DLL_SYSTEM_UpdatePlayer : array[0..15] of TCallProc_DLL_SYSTEM_UpdatePlayer;
EXTERNAL_DLL_EVENT_ConsoleMessage : array[0..15] of TCallProc_DLL_EVENT_ConsoleMessage;
EXTERNAL_DLL_CMD : array[0..15] of TCallProc_DLL_CMD;


EXTERNAL_DLL_RegisterProcFX_FillRect : array[0..15] of TCallProc_DLL_RegisterProcFX_FillRect;
EXTERNAL_DLL_RegisterProcFX_FillRectMap : array[0..15] of TCallProc_DLL_RegisterProcFX_FillRectMap;
EXTERNAL_DLL_RegisterProcFX_FillRectMapEx : array[0..15] of TCallProc_DLL_RegisterProcFX_FillRectMapEx;
EXTERNAL_DLL_RegisterProcFX_Rectangle : array[0..15] of TCallProc_DLL_RegisterProcFX_Rectangle;
EXTERNAL_DLL_RegisterProcFX_Line : array[0..15] of TCallProc_DLL_RegisterProcFX_Line;

EXTERNAL_DLL_PMA_RegisterMenuCreate : array[0..15] of TCallProc_DLL_PMA_RegisterMenuCreate;
EXTERNAL_DLL_PMA_RegisterMenuSets : array[0..15] of TCallProc_DLL_PMA_RegisterMenuSets;
EXTERNAL_DLL_PMA_RegisterMenuAddOption : array[0..15] of TCallProc_DLL_PMA_RegisterMenuAddOption;
EXTERNAL_DLL_PMA_RegisterMenuSetOption : array[0..15] of TCallProc_DLL_PMA_RegisterMenuSetOption;
EXTERNAL_DLL_PMA_RegisterDeleteAllOptions : array[0..15] of TCallProc_DLL_PMA_RegisterDeleteAllOptions;
EXTERNAL_DLL_PMA_RegisterMenuShowHide : array[0..15] of TCallProc_DLL_PMA_RegisterMenuShowHide;
EXTERNAL_DLL_PMA_EVENT_MenuEnter : array[0..15] of TCallProc_DLL_PMA_EVENT_MenuEnter;


	//Переменные процедурного типа, соответствующие процедурам и функциям описанным в PQR Mod на C++
	C_DLL_EVENT_BeginGame : TCallProc_C_DLL_EVENT_BeginGame;
	C_DLL_EVENT_ResetGame : TCallProc_C_DLL_EVENT_ResetGame;
	C_DLL_EVENT_MapChanged : TCallProc_C_DLL_EVENT_MapChanged;
	C_DLL_MainLoop : TCallProc_C_DLL_MainLoop;
	C_DLL_SYSTEM_UpdatePlayer : TCallProc_C_DLL_SYSTEM_UpdatePlayer;
	C_DLL_UpdatePlayerEX : TCallProc_C_DLL_UpdatePlayerEX;
	C_DLL_SYSTEM_AddPlayer : TCallProc_C_DLL_SYSTEM_AddPlayer;
	C_DLL_SYSTEM_RemoveAllPlayers : TCallProc_C_DLL_SYSTEM_RemoveAllPlayers;
	C_DLL_SYSTEM_RemovePlayer : TCallProc_C_DLL_SYSTEM_RemovePlayer;
	C_DLL_AddModel : TCallProc_C_DLL_AddModel;
	C_DLL_DMGReceived : TCallProc_C_DLL_DMGReceived;
	C_DLL_ChatReceived : TCallProc_C_DLL_ChatReceived;
	C_DLL_EVENT_ConsoleMessage : TCallProc_C_DLL_EVENT_ConsoleMessage;
	C_DLL_CMD : TCallProc_C_DLL_CMD;
	C_DLL_QUERY_VERSION : TCallProc_C_DLL_QUERY_VERSION;
	C_DLL_RegisterBotDll : TCallProc_C_DLL_RegisterBotDll;
	C_DLL_NFKStarted : TCallProc_C_DLL_NFKStarted;
	C_DLL_SetActiveBotDll : TCallProc_C_DLL_SetActiveBotDll;
	C_DLL_RegisterMainBotDll : TCallProc_C_DLL_RegisterMainBotDll;
	
	C_DLL_PMA_MenuCreate : TCallProc_PMA_MenuCreate;
	C_DLL_PMA_MenuSets : TCallProc_PMA_MenuSets;
	C_DLL_PMA_MenuAddOption : TCallProc_PMA_MenuAddOption;
	C_DLL_PMA_MenuSetOption : TCallProc_PMA_MenuSetOption;
	C_DLL_PMA_DeleteAllOptions : TCallProc_PMA_DeleteAllOptions;
	C_DLL_PMA_MenuShowHide : TCallProc_PMA_MenuShowHide;
	


ActiveBot: Shortint;
BotsDllArray : TBotDllArray;
PQRModDllHandle : THAndle;
BotDllHandle: array[0..15] of THAndle;
PlayersBasePointer : p_arrTplayerO;
SaveExit: Pointer;
AddMessageOff: boolean;
NFKStarted: boolean;
//tmp_str: string;
SendFlag: boolean;
my_menu_id: byte;

//-------------------------------------------------------------------
procedure RegisterPQRModDLL(path : PChar);
var Handle: THAndle;
mainpath: string;
i: integer;
begin
	if FileExists(path) then begin
		Handle := LoadLibrary(path);
		//Handle := LoadLibrary('c:\Games\nfk\nfk057\pqrmod.dll');
		if Handle >=32 then
		begin
			PQRModDllHandle := Handle;

			@C_DLL_EVENT_BeginGame := GetProcAddress(PQRModDllHandle,'C_DLL_EVENT_BeginGame');
			@C_DLL_EVENT_ResetGame := GetProcAddress(PQRModDllHandle,'C_DLL_EVENT_ResetGame');
			@C_DLL_EVENT_MapChanged := GetProcAddress(PQRModDllHandle,'C_DLL_EVENT_MapChanged');
			@C_DLL_MainLoop	:= GetProcAddress(PQRModDllHandle,'C_DLL_MainLoop');
			@C_DLL_SYSTEM_UpdatePlayer := GetProcAddress(PQRModDllHandle,'C_DLL_SYSTEM_UpdatePlayer');
			@C_DLL_UpdatePlayerEX := GetProcAddress(PQRModDllHandle,'C_DLL_UpdatePlayerEX');
			@C_DLL_SYSTEM_AddPlayer := GetProcAddress(PQRModDllHandle,'C_DLL_SYSTEM_AddPlayer');
			@C_DLL_SYSTEM_RemoveAllPlayers := GetProcAddress(PQRModDllHandle,'C_DLL_SYSTEM_RemoveAllPlayers');
			@C_DLL_SYSTEM_RemovePlayer := GetProcAddress(PQRModDllHandle,'C_DLL_SYSTEM_RemovePlayer');
			@C_DLL_AddModel := GetProcAddress(PQRModDllHandle,'C_DLL_AddModel');
			@C_DLL_DMGReceived := GetProcAddress(PQRModDllHandle,'C_DLL_DMGReceived');
			@C_DLL_ChatReceived := GetProcAddress(PQRModDllHandle,'C_DLL_ChatReceived');
			@C_DLL_EVENT_ConsoleMessage := GetProcAddress(PQRModDllHandle,'C_DLL_EVENT_ConsoleMessage');
			@C_DLL_CMD := GetProcAddress(PQRModDllHandle,'C_DLL_CMD');
			@C_DLL_QUERY_VERSION := GetProcAddress(PQRModDllHandle,'C_DLL_QUERY_VERSION');
			@C_DLL_RegisterBotDll := GetProcAddress(PQRModDllHandle,'C_DLL_RegisterBotDll');
			@C_DLL_NFKStarted := GetProcAddress(PQRModDllHandle,'C_DLL_NFKStarted');
			@C_DLL_SetActiveBotDll := GetProcAddress(PQRModDllHandle,'C_DLL_SetActiveBotDll');
			@C_DLL_RegisterMainBotDll := GetProcAddress(PQRModDllHandle,'C_DLL_RegisterMainBotDll');
			
			@C_DLL_PMA_MenuCreate := GetProcAddress(PQRModDllHandle,'C_DLL_PMA_MenuCreate');
			@C_DLL_PMA_MenuSets := GetProcAddress(PQRModDllHandle,'C_DLL_PMA_MenuSets');
			@C_DLL_PMA_MenuAddOption := GetProcAddress(PQRModDllHandle,'C_DLL_PMA_MenuAddOption');
			@C_DLL_PMA_MenuSetOption := GetProcAddress(PQRModDllHandle,'C_DLL_PMA_MenuSetOption');
			@C_DLL_PMA_DeleteAllOptions := GetProcAddress(PQRModDllHandle,'C_DLL_PMA_DeleteAllOptions');
			@C_DLL_PMA_MenuShowHide := GetProcAddress(PQRModDllHandle,'C_DLL_PMA_MenuShowHide');
			
			if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_PMA_RegisterMenuCreate[i]<>nil) then EXTERNAL_DLL_PMA_RegisterMenuCreate[i](@C_DLL_PMA_MenuCreate);
			if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_PMA_RegisterMenuSets[i]<>nil) then EXTERNAL_DLL_PMA_RegisterMenuSets[i](@C_DLL_PMA_MenuSets);
			if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_PMA_RegisterMenuAddOption[i]<>nil) then EXTERNAL_DLL_PMA_RegisterMenuAddOption[i](@C_DLL_PMA_MenuAddOption);
			if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_PMA_RegisterMenuSetOption[i]<>nil) then EXTERNAL_DLL_PMA_RegisterMenuSetOption[i](@C_DLL_PMA_MenuSetOption);
			if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_PMA_RegisterDeleteAllOptions[i]<>nil) then EXTERNAL_DLL_PMA_RegisterDeleteAllOptions[i](@C_DLL_PMA_DeleteAllOptions);
			if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_PMA_RegisterMenuShowHide[i]<>nil) then EXTERNAL_DLL_PMA_RegisterMenuShowHide[i](@C_DLL_PMA_MenuShowHide);


			mainpath:=GetSystemVariable('gamedir');
			if mainpath='' then mainpath:=GetSystemVariable('rootdir');
			mainpath:=mainpath+'\bot.dll';
			mainpath:='bot.dll';
			if @C_DLL_RegisterMainBotDll<>nil then C_DLL_RegisterMainBotDll(PChar(mainpath));
		end
		else AddMessage('Handle < 32');
	end
	else AddMessage('^1'+path+' not found!');
end;

procedure RegisterBotDLL(path : PChar);
var Handle: THAndle;
begin
	if FileExists(path) then begin
		Handle := LoadLibrary(path);
		
		if Handle >= 32 then
		begin
		

		if BotDllHandle[BotsDllArray.FilesCount]<>0 then freelibrary(BotDllHandle[BotsDllArray.FilesCount]);
		BotDllHandle[BotsDllArray.FilesCount] := Handle;

		@EXTERNAL_DLL_RegisterProc1[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc1');
		@EXTERNAL_DLL_RegisterProc2[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc2');
		@EXTERNAL_DLL_RegisterProc3[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc3');
		@EXTERNAL_DLL_RegisterProc4[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc4');
		@EXTERNAL_DLL_RegisterProc5[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc5');
		@EXTERNAL_DLL_RegisterProc6[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc6');
		@EXTERNAL_DLL_RegisterProc7[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc7');
		@EXTERNAL_DLL_RegisterProc8[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc8');
		@EXTERNAL_DLL_RegisterProc9[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc9');
		@EXTERNAL_DLL_RegisterProc10[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc10');
		@EXTERNAL_DLL_RegisterProc11[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc11');
		@EXTERNAL_DLL_RegisterProc12[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc12');
		@EXTERNAL_DLL_RegisterProc13[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProc13');
		@EXTERNAL_DLL_MainLoop[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_MainLoop');
		@EXTERNAL_DLL_EVENT_BeginGame[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_EVENT_BeginGame');
		@EXTERNAL_DLL_EVENT_MapChanged[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_EVENT_MapChanged');
		@EXTERNAL_DLL_EVENT_ResetGame[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_EVENT_ResetGame');
		@EXTERNAL_DLL_QUERY_VERSION[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_QUERY_VERSION');
		@EXTERNAL_DLL_SYSTEM_AddPlayer[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_SYSTEM_AddPlayer');
		@EXTERNAL_DLL_SYSTEM_RemoveAllPlayers[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_SYSTEM_RemoveAllPlayers');
		@EXTERNAL_DLL_SYSTEM_RemovePlayer[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_SYSTEM_RemovePlayer');
		@EXTERNAL_DLL_DMGReceived[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_DMGReceived');
		@EXTERNAL_DLL_ChatReceived[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_ChatReceived');
		@EXTERNAL_DLL_AddModel[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_AddModel');
		@EXTERNAL_DLL_SYSTEM_UpdatePlayer[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_SYSTEM_UpdatePlayer');
		@EXTERNAL_DLL_EVENT_ConsoleMessage[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_EVENT_ConsoleMessage');
		@EXTERNAL_DLL_CMD[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_CMD');

		@EXTERNAL_DLL_RegisterProcFX_FillRect[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProcFX_FillRect');
		@EXTERNAL_DLL_RegisterProcFX_FillRectMap[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProcFX_FillRectMap');
		@EXTERNAL_DLL_RegisterProcFX_FillRectMapEx[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProcFX_FillRectMapEx');
		@EXTERNAL_DLL_RegisterProcFX_Rectangle[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProcFX_Rectangle');
		@EXTERNAL_DLL_RegisterProcFX_Line[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_RegisterProcFX_Line');
		
		@EXTERNAL_DLL_PMA_RegisterMenuCreate[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_PMA_RegisterMenuCreate');
		@EXTERNAL_DLL_PMA_RegisterMenuSets[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_PMA_RegisterMenuSets');
		@EXTERNAL_DLL_PMA_RegisterMenuAddOption[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_PMA_RegisterMenuAddOption');
		@EXTERNAL_DLL_PMA_RegisterMenuSetOption[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_PMA_RegisterMenuSetOption');
		@EXTERNAL_DLL_PMA_RegisterDeleteAllOptions[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_PMA_RegisterDeleteAllOptions');
		@EXTERNAL_DLL_PMA_RegisterMenuShowHide[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_PMA_RegisterMenuShowHide');
		@EXTERNAL_DLL_PMA_EVENT_MenuEnter[BotsDllArray.FilesCount] := GetProcAddress(BotDllHandle[BotsDllArray.FilesCount], 'DLL_PMA_EVENT_MenuEnter');
		
		BotsDllArray.FilesArray[BotsDllArray.FilesCount] := path;
		BotsDllArray.StatusOnOff[BotsDllArray.FilesCount] := true;
		BotsDllArray.FilesCount := BotsDllArray.FilesCount+1;
		end;

	end;
end;

procedure FindBotDlls;
var sr: TSearchRec;
begin
	if FindFirst('*.dll',faAnyFile,sr) = 0 then
	begin
		RegisterBotDLL(PChar(sr.Name));

		while FindNext(sr) = 0 do
		begin
			if BotsDllArray.FilesCount = 16 then break;
			RegisterBotDLL(PChar(sr.Name));
		end;
		FindClose(sr.FindHandle);
	end
	else ActiveBot:=-1;

end;

//---------------------------------------------------------------

function IsClient:boolean;
begin
	result:=false;
	if GetSystemVariable('netstatus')='2' then result:=true;
end;

//----------------------------------------------------------------
procedure DLL_RegisterProc1(AProc : TCallProcWordWordFunc);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc1[i]<>nil) then EXTERNAL_DLL_RegisterProc1[i](AProc);
	SetAngle := AProc;
end;

procedure DLL_RegisterProc2(AProc : TCallTextProc; ProcID:byte);
var i: shortint;
begin
		if BotsDllArray.FilesCount>0 then for i := 0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc2[i]<>nil) then EXTERNAL_DLL_RegisterProc2[i](AProc,ProcID);
        case ProcID of
        1 : AddMessage := AProc;
        2 : RegisterConsoleCommand := AProc;
		3 : SendConsoleCommand := AProc;
		4 : SendConsoleHCommand := AProc;
		255: SendAuthorization := AProc;
        end;
end;

procedure DLL_RegisterProc3(AProc : TCallProcSTR; ProcID:byte);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc3[i]<>nil) then EXTERNAL_DLL_RegisterProc3[i](AProc,ProcID);
	case ProcID of
	1 : GetSystemVariable := AProc;
	end;
end;

procedure DLL_RegisterProc4(AProc : TCallProcCreatePlayer; ProcID:byte);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc4[i]<>nil) then EXTERNAL_DLL_RegisterProc4[i](AProc,ProcID);
	sys_CreatePlayer := AProc;
end;

procedure DLL_RegisterProc5(AProc : TCallProcWordByteFunc; ProcID:byte);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc5[i]<>nil) then EXTERNAL_DLL_RegisterProc5[i](AProc,ProcID);
	case ProcID of
	1 : SetKeys := AProc;
	3 : SetWeapon := AProc;
	4 : SetBalloon := AProc;
	end;
end;

procedure DLL_RegisterProc6(AProc : TCallProcWordWord_Bool; ProcID:byte);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc6[i]<>nil) then EXTERNAL_DLL_RegisterProc6[i](AProc,ProcID);
	case ProcID of
	1 : Test_Blocked := AProc;
	end;
end;

procedure DLL_RegisterProc7(AProc : TCallProcWordWordString; ProcID:byte);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc7[i]<>nil) then EXTERNAL_DLL_RegisterProc7[i](AProc,ProcID);
	case ProcID of
	1 : debug_textout := AProc;
	2 : debug_textoutc := AProc;
	end;
end;

procedure DLL_RegisterProc8(AProc : TCallProcBrickStruct);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc8[i]<>nil) then EXTERNAL_DLL_RegisterProc8[i](AProc);
	GetBrickStruct := AProc;
end;
procedure DLL_RegisterProc9(AProc : TCallProcObjectsStruct);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc9[i]<>nil) then EXTERNAL_DLL_RegisterProc9[i](AProc);
	GetObjStruct := AProc;
end;
procedure DLL_RegisterProc10(AProc : TCallProcSpecailObjectsStruct);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc10[i]<>nil) then EXTERNAL_DLL_RegisterProc10[i](AProc);
	GetSpecObjStruct := AProc;
end;
procedure DLL_RegisterProc11(AProc : TCallProcWord);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc11[i]<>nil) then EXTERNAL_DLL_RegisterProc11[i](AProc);
	RemoveBot := AProc;
end;
procedure DLL_RegisterProc12(AProc : TCallProcChat);
var i: shortint;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc12[i]<>nil) then EXTERNAL_DLL_RegisterProc12[i](AProc);
	SendBotChat := AProc;
end;

procedure DLL_RegisterProc13(AProc : TCallProcExtendedTextout);
var i:integer;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProc13[i]<>nil) then EXTERNAL_DLL_RegisterProc13[i](AProc);
	ExtendedTextout := AProc;
end;

procedure DLL_RegisterProcFX_FillRect(AProc : TCallProcFX_FillRect);
var i:integer;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProcFX_FillRect[i]<>nil) then EXTERNAL_DLL_RegisterProcFX_FillRect[i](AProc);
	FX_FillRect := AProc;
end;

procedure DLL_RegisterProcFX_FillRectMap(AProc : TCallProcFX_FillRectMap);
var i:integer;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProcFX_FillRectMap[i]<>nil) then EXTERNAL_DLL_RegisterProcFX_FillRectMap[i](AProc);
	FX_FillRectMap := AProc;
		
end;

procedure DLL_RegisterProcFX_FillRectMapEx(AProc : TCallProcFX_FillRectMapEx);
var i:integer;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProcFX_FillRectMapEx[i]<>nil) then EXTERNAL_DLL_RegisterProcFX_FillRectMapEx[i](AProc);
	FX_FillRectMapEx := AProc;
	SendFlag:=true;
end;

procedure DLL_RegisterProcFX_Rectangle(AProc : TCallProcFX_Rectangle);
var i:integer;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProcFX_Rectangle[i]<>nil) then EXTERNAL_DLL_RegisterProcFX_Rectangle[i](AProc);
	FX_Rectangle := AProc;
end;

procedure DLL_RegisterProcFX_Line(AProc : TCallProcFX_Line);
var i:integer;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_RegisterProcFX_Line[i]<>nil) then EXTERNAL_DLL_RegisterProcFX_Line[i](AProc);
	FX_Line := AProc;

end;


//-------------------------------------------------
// Основные экспортируемые процедуры BOT.dll
//-------------------------------------------------

// MainLoop. NFK.EXE calls this procedure 50 times per second. (in game)
procedure DLL_MainLoop();
var i: shortint;
begin
	//DEBUG PRINT
	//AddMessage('DLL_MainLoop');
	
	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_MainLoop[i]<>nil) AND BotsDllArray.StatusOnOff[i] then EXTERNAL_DLL_MainLoop[i]();
	if @C_DLL_MainLoop <> nil then C_DLL_MainLoop();
	 
	//DEBUG PRINT
	//if (SendFlag) then AddMessage('true')
	//else AddMessage('false');
end;

// NFK loads a map. and begin game. It should be warmup time for now..
procedure DLL_EVENT_BeginGame;
var i: shortint;
NewMenu: PMA_TMenu;
s: string;
begin

	//DEBUG PRINT
	//AddMessage('DLL_EVENT_BeginGame BEFORE C++');

	//Важно! C_DLL_EVENT_BeginGame должна вызываться раньше всех,
	//она отключает ненужных ботов (mb_off <dll_number>)
	
	AddMessageOff := true; //Во время C_DLL_EVENT_BeginGame; идёт куча флуда на fnP_DLL_AddMessage.
	//Устанавливаем флаги и тем самым отключаем fnP_DLL_AddMessage
	
	if @C_DLL_EVENT_BeginGame <> nil then C_DLL_EVENT_BeginGame();

	AddMessageOff := false; 

	//if (SendFlag) then AddMessage('true')
	//else AddMessage('false');

	//DEBUG PRINT
	//AddMessage(tmp_str);
	//AddMessage('DLL_EVENT_BeginGame AFTER C++');
	//C_DLL_PrnPtr(@sys_CreatePlayer);
	//C_DLL_PrnPtr(@AddMessage);
	//C_DLL_PrnPtr(@RegisterConsoleCommand);

	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_EVENT_BeginGame[i]<>nil) AND BotsDllArray.StatusOnOff[i] then EXTERNAL_DLL_EVENT_BeginGame[i]();
	{
		NewMenu.x:=100;
		NewMenu.y:=100;
		NewMenu.Width:=200;
		NewMenu.SubMenuMask:=0;
		my_menu_id := C_DLL_PMA_MenuCreate(NewMenu);
		AddMessage('MENU ID '+IntToStr(my_menu_id));
		s := 'option 1';
		C_DLL_PMA_MenuAddOption(my_menu_id,PChar(s));
		
		C_DLL_PMA_MenuAddOption(my_menu_id,PChar('option 2'));
		C_DLL_PMA_MenuAddOption(my_menu_id,PChar('option 3'));
		
		C_DLL_PMA_MenuShowHide(my_menu_id,true);
	}	
end;

// Game Reborn. It happends at game start, or at map restart. (it also called after DLL_EVENT_BeginGame (once));
procedure DLL_EVENT_MapChanged;
var i: shortint;
begin
	//DEBUG PRINT
	AddMessage('DLL_EVENT_MapChanged');
	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_EVENT_MapChanged[i]<>nil) AND BotsDllArray.StatusOnOff[i] then EXTERNAL_DLL_EVENT_MapChanged[i]();
	if @C_DLL_EVENT_MapChanged <> nil then C_DLL_EVENT_MapChanged;
end;

// Map changed by "map" command, or by callvote
procedure DLL_EVENT_ResetGame;
var i: shortint;
begin
	//DEBUG PRINT
	AddMessage('DLL_EVENT_ResetGame');
	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_EVENT_ResetGame[i]<>nil) AND BotsDllArray.StatusOnOff[i] then EXTERNAL_DLL_EVENT_ResetGame[i]();
	if @C_DLL_EVENT_ResetGame <> nil then C_DLL_EVENT_ResetGame();
end;

// NFK.EXE query bot.dll version.
function DLL_QUERY_VERSION:shortstring;
var i: shortint;
path: shortstring;
begin
	//DEBUG PRINT
	AddMessage('DLL_QUERY_VERSION');
	
	//if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_QUERY_VERSION[i]<>nil) then AddMessage(EXTERNAL_DLL_QUERY_VERSION[i]);
	
	if Not NFKStarted Then
	begin
		//AddMessageOff := true;//Во время инициализации идёт куча флуда на fnP_DLL_AddMessage.
		//Устанавливаем флаги и тем самым отключаем fnP_DLL_AddMessage
		//Этот флаг true с самого старта dll

		SendAuthorization('authorize');
		
		path:=GetSystemVariable('gamedir');
		if path='' then path:=GetSystemVariable('rootdir');
		SetCurrentDir(path);
		
		//Подгружаем библиотеку PQR Mod
		RegisterPQRModDLL(PChar(path+'\pqrmod.dll'));
		
		
		//Подгрузка других ботов в DllEntirePoint
		//Подгружаем библиотеки других ботов
		//path:=GetSystemVariable('rootdir');
		//SetCurrentDir(path);
		//if SetCurrentDir('bots') then FindBotDlls
		//else ActiveBot:=-1;
		
		RegisterConsoleCommand('version');
		RegisterConsoleCommand('activebot');
		
		if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_QUERY_VERSION[i]<>nil) then AddMessage(EXTERNAL_DLL_QUERY_VERSION[i]);
		if @C_DLL_NFKStarted <> nil then C_DLL_NFKStarted();
		
		NFKStarted:=true;
		AddMessageOff := false;
	end;
		
	if @C_DLL_QUERY_VERSION <> nil then result := C_DLL_QUERY_VERSION
	else result :='PQR Mod not loaded';
end;

procedure DLL_SYSTEM_AddPlayer(Player : TPlayerEx);
var i: shortint;
begin
	//DEBUG PRINT
	AddMessage('DLL_SYSTEM_AddPlayer');	
	if NOT IsClient then if ActiveBot>=0 then
		if (@EXTERNAL_DLL_SYSTEM_AddPlayer[ActiveBot]<>nil) AND BotsDllArray.StatusOnOff[ActiveBot] then
			EXTERNAL_DLL_SYSTEM_AddPlayer[ActiveBot](Player);

	if Player.bot Then
	begin
		if ActiveBot=16 then if @C_DLL_SYSTEM_AddPlayer <> nil then C_DLL_SYSTEM_AddPlayer(Player); //in pqrmod.dll
		for i := 0 to 7 do
		if bots[i] = nil then // free cell
        begin
			bots[i] := TBot.Create;
			bots[i].DXID := Player.DXID;
			bots[i].dll_num := ActiveBot;
			break;
		end;
		Player.bot := false;
		if ActiveBot<>16 then if @C_DLL_SYSTEM_AddPlayer <> nil then C_DLL_SYSTEM_AddPlayer(Player);
	end
	else if @C_DLL_SYSTEM_AddPlayer <> nil then C_DLL_SYSTEM_AddPlayer(Player);
		
	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do
		if i<>ActiveBot Then			
			if (@EXTERNAL_DLL_SYSTEM_AddPlayer[i]<>nil) AND BotsDllArray.StatusOnOff[i] Then
				EXTERNAL_DLL_SYSTEM_AddPlayer[i](Player);
end;

procedure DLL_SYSTEM_RemoveAllPlayers();
var i: shortint;
begin
	//DEBUG PRINT
	AddMessage('DLL_SYSTEM_RemoveAllPlayers');

	for i := 0 to 7 do bots[i]:=nil; // free cell
	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_SYSTEM_RemoveAllPlayers[i]<>nil) AND BotsDllArray.StatusOnOff[i] then EXTERNAL_DLL_SYSTEM_RemoveAllPlayers[i]();
	if @C_DLL_SYSTEM_RemoveAllPlayers <> nil then C_DLL_SYSTEM_RemoveAllPlayers(); //in pqrmod.dll
end;

procedure DLL_SYSTEM_RemovePlayer(DXID:word);
var i: shortint;
begin
	//DEBUG PRINT
	AddMessage('DLL_SYSTEM_RemovePlayer');

	for i := 0 to 7 do if bots[i] <> nil then
		if bots[i].DXID = DXID then bots[i]:=nil; // free cell 
	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_SYSTEM_RemovePlayer[i]<>nil) AND BotsDllArray.StatusOnOff[i] then EXTERNAL_DLL_SYSTEM_RemovePlayer[i](DXID);
	if @C_DLL_SYSTEM_RemovePlayer <> nil then C_DLL_SYSTEM_RemovePlayer(DXID); //in pqrmod.dll
end;

procedure DLL_DMGReceived(TargetDXID, AttackerDXID:Word; dmg : word);
var i: shortint;
begin
	//DEBUG PRINT
	AddMessage('DLL_DMGReceived');

	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_DMGReceived[i]<>nil) AND BotsDllArray.StatusOnOff[i] then EXTERNAL_DLL_DMGReceived[i](TargetDXID,AttackerDXID,dmg);
	if @C_DLL_DMGReceived <> nil then C_DLL_DMGReceived(TargetDXID,AttackerDXID,dmg); //in pqrmod.dll
end;

procedure DLL_ChatReceived(DXID:Word; Text : shortstring);
var s1: string;
i: shortint;
begin
	//DEBUG PRINT
	AddMessage('DLL_ChatReceived');

	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_ChatReceived[i]<>nil) AND BotsDllArray.StatusOnOff[i] then EXTERNAL_DLL_ChatReceived[i](DXID,Text);
	s1 := Text;
	if @C_DLL_ChatReceived <> nil then C_DLL_ChatReceived(DXID,PChar(s1)); //in pqrmod.dll
end;

procedure DLL_AddModel(s : shortstring);
var s1: string;
i: shortint;
begin
	//DEBUG PRINT
	//AddMessage('DLL_AddModel');

	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_AddModel[i]<>nil) AND BotsDllArray.StatusOnOff[i] AND BotsDllArray.StatusOnOff[i] then EXTERNAL_DLL_AddModel[i](s);
	s1 := s;
   if @C_DLL_AddModel <> nil then C_DLL_AddModel(PChar(s1)); //in pqrmod.dll
end;

procedure DLL_SYSTEM_UpdatePlayer(Player : TPlayerEx);
var i,j: shortint;
botflag: bool;
begin
	//DEBUG PRINT
	//AddMessage('^2DLL_SYSTEM_UpdatePlayer');

      // update all player info
	if Player.bot Then botflag:=true
	else botflag:=false;
	Player.bot := false;

	if botflag then
		for j := 0 to 7 do if bots[j] <> nil then
			if bots[j].DXID = Player.DXID then
			begin
				if bots[j].dll_num = 16 then Player.bot := true;
				break;
			end;

	if @C_DLL_SYSTEM_UpdatePlayer <> nil then C_DLL_SYSTEM_UpdatePlayer(Player); //in pqrmod.dll

	
	for i := 0 to BotsDllArray.FilesCount-1 do
	begin
		if botflag then
		begin
			Player.bot := false;

			for j := 0 to 7 do if bots[j] <> nil then
				if bots[j].DXID = Player.DXID then
					begin
					if bots[j].dll_num = i then Player.bot := true;
					break;
					end;
		end;
			
		if (@EXTERNAL_DLL_SYSTEM_UpdatePlayer[i]<>nil) AND BotsDllArray.StatusOnOff[i] Then
			EXTERNAL_DLL_SYSTEM_UpdatePlayer[i](Player);
	end;
end;

procedure DLL_EVENT_ConsoleMessage(s : shortstring);
var s1:string;
i:integer;
begin
	//DEBUG PRINT
	//Из этой процедуры нельзя деать AddMessage(s); !!!Рекурсия и переполнение стека
	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_EVENT_ConsoleMessage[i]<>nil) AND BotsDllArray.StatusOnOff[i] then EXTERNAL_DLL_EVENT_ConsoleMessage[i](s);
	s1 := s;
	if @C_DLL_EVENT_ConsoleMessage <> nil then C_DLL_EVENT_ConsoleMessage(PChar(s1)); //in pqrmod.dll

end;

function strpar(s:string; pos : word):string;
var     counter, del1 : byte;
        len, i : word;
const   delimeter : char = ' ';
begin
        result := ''; len := length(s);  del1 := 1;
        if len = 0 then exit; counter := 0;

        for i := 1 to len do
        if (s[i]=delimeter) or (i=len) then begin
        if counter = pos then begin
                if (pos=0) and (s[i]<>delimeter) then result := copy(s, del1, i-del1+1) else
                if (pos=0) then result := copy(s, del1, i-del1) else
                if (i=len) and (s[i]<>delimeter) then result := copy(s, del1+1, i-del1+2) else
                result := copy(s, del1+1, i-del1);
                exit;
                end;
                del1 := i;
                inc(counter);
        end;
end;

procedure DLL_CMD(s : string);
var s1: string;
i: integer;
var   ls, par1, par2, par3: string;
begin
	s1 := s;
	if NOT IsClient then if ActiveBot>=0 Then
		if ActiveBot<16 Then
			if (@EXTERNAL_DLL_CMD[ActiveBot]<>nil) AND BotsDllArray.StatusOnOff[ActiveBot] then
				EXTERNAL_DLL_CMD[ActiveBot](s1);
	if @C_DLL_CMD <> nil then C_DLL_CMD(PChar(s1)); //in pqrmod.dll

	if (s = 'version') OR (s = 'version ') then
	begin
		AddMessage('NFK version '+GetSystemVariable('nfkversion'));
		if @C_DLL_QUERY_VERSION <> nil then
		begin
			AddMessage(C_DLL_QUERY_VERSION);
			if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do
				if (@EXTERNAL_DLL_QUERY_VERSION[i]<>nil) AND BotsDllArray.StatusOnOff[i] then
					AddMessage(IntToStr(i)+'.'+BotsDllArray.FilesArray[i]+': '+EXTERNAL_DLL_QUERY_VERSION[i]);
		end;

	end
	else
	begin
	    ls := lowercase(s);
        par1 := strpar(ls,0);
        par2 := strpar(ls,1);
        par3 := strpar(ls,2);

        // -------------------------------------
        if par1 = 'activebot' then
        begin
			if @C_DLL_CMD = nil then //Если нет pqrmod.dll надо обработать самостоятельно
			begin
				i:=-2;
				if par2 <> '' then i:=StrToIntDef(par2,-2);
				if (i >= -1) AND (i < BotsDllArray.FilesCount) Then
				begin
					if (i<>-1) then
						if (BotsDllArray.StatusOnOff[i]=false) then//Если активируемый бот выключен
						begin
							AddMessage('This bot DLL is ^1OFF^7. Switch ^2ON^7 first.');
							exit;
						end;
					ActiveBot:=i;
					s1:='activebot sets to '+IntToStr(ActiveBot)+' (';
					if (ActiveBot>=0) then s1:=s1+BotsDllArray.FilesArray[ActiveBot]
					else s1:=s1+'no one bot dll active';
					s1:=s1+')';
					AddMessage(s1);
				end
				else
				begin
					AddMessage('activebot <dll number>');
					s1:='current value: '+IntToStr(ActiveBot)+' (';;
					if (ActiveBot>0) then s1:=s1+BotsDllArray.FilesArray[ActiveBot]
					else s1:=s1+'no one bot dll active';
					s1:=s1+')';
					AddMessage(s1);
        		end;
        	end;
        end;
	end;
end;
//----------------------------------------------------------------------------------
//		PROC HACK
//-----------------------------------------------------------------------------------
procedure P_DLL_SetBasePointer(BasePointer : p_arrTplayerO); cdecl;
begin
	PlayersBasePointer := BasePointer;
end;

function P_DLL_SetAngleEX(DXID : WORD; PlNumber : Byte; Angle : real):boolean cdecl;
var j: integer;
begin
	result:=false;
	if (PlayersBasePointer<>nil) then
	begin
		if DXID=0 then
		begin
			if PlayersBasePointer^[PlNumber]<>nil then
			begin
				PlayersBasePointer^[PlNumber].fangle := Angle;
				result:=true;
			end;
		end
		else
			for j := 0 to 7 do
				if PlayersBasePointer^[j] <> nil then
					if PlayersBasePointer^[j].DXID = DXID then
					begin
						PlayersBasePointer^[j].fangle := Angle;
						result:=true;
					end;
	end;
end;
function P_DLL_UpdatePlayerEX(DXID : WORD; PlNumber : Byte):boolean cdecl;
var j: integer;
s1,s2,s3: string;
PlayerEX : TPlayerEx2;
//f: procedure(s: string);
begin
	//DEBUG PRINT
	//AddMessage(IntToStr(DXID));
	
	result:=false;
	if (PlayersBasePointer<>nil) then
	begin

		if DXID=0 then
			if PlayersBasePointer^[PlNumber]<>nil then DXID:=PlayersBasePointer^[PlNumber].DXID;
		if DXID<>0 then
			for j := 0 to 7 do
				if PlayersBasePointer^[j] <> nil then
					if PlayersBasePointer^[j].DXID = DXID then
					begin
						//DEBUG PRINT
						//AddMessage(PlayersBasePointer^[j].netname);

						PlayerEX.dead := PlayersBasePointer^[j].dead;
						PlayerEX.bot := false;
						PlayerEX.crouch := PlayersBasePointer^[j].crouch;
						PlayerEX.balloon := PlayersBasePointer^[j].balloon;
						PlayerEX.flagcarrier := PlayersBasePointer^[j].flagcarrier;
						PlayerEX.have_rl := PlayersBasePointer^[j].have_rl;
						PlayerEX.have_gl := PlayersBasePointer^[j].have_gl;
						PlayerEX.have_rg := PlayersBasePointer^[j].have_rg;
						PlayerEX.have_bfg := PlayersBasePointer^[j].have_bfg;
						PlayerEX.have_sg := PlayersBasePointer^[j].have_sg;
						PlayerEX.have_mg := PlayersBasePointer^[j].have_mg;
						PlayerEX.have_pl := PlayersBasePointer^[j].have_pl;
						PlayerEX.have_sh := PlayersBasePointer^[j].have_sh;
						PlayerEX.refire := PlayersBasePointer^[j].refire;
						PlayerEX.weapchg := PlayersBasePointer^[j].weapchg;
						PlayerEX.weapon := PlayersBasePointer^[j].weapon;
						PlayerEX.threadweapon := PlayersBasePointer^[j].threadweapon;
						PlayerEX.dir := PlayersBasePointer^[j].dir;
						PlayerEX.gantl_state := PlayersBasePointer^[j].gantl_state;
						PlayerEX.air := PlayersBasePointer^[j].air;
						PlayerEX.team := PlayersBasePointer^[j].team;
						PlayerEX.item_quad := PlayersBasePointer^[j].item_quad;
						PlayerEX.item_regen := PlayersBasePointer^[j].item_regen;
						PlayerEX.item_battle := PlayersBasePointer^[j].item_battle;
						PlayerEX.item_flight := PlayersBasePointer^[j].item_flight;
						PlayerEX.item_haste := PlayersBasePointer^[j].item_haste;
						PlayerEX.item_invis := PlayersBasePointer^[j].item_invis;
						PlayerEX.ammo_mg := PlayersBasePointer^[j].ammo_mg;
						PlayerEX.ammo_sg := PlayersBasePointer^[j].ammo_sg;
						PlayerEX.ammo_gl := PlayersBasePointer^[j].ammo_gl;
						PlayerEX.ammo_rl := PlayersBasePointer^[j].ammo_rl;
						PlayerEX.ammo_sh := PlayersBasePointer^[j].ammo_sh;
						PlayerEX.ammo_rg := PlayersBasePointer^[j].ammo_rg;
						PlayerEX.ammo_pl := PlayersBasePointer^[j].ammo_pl;
						PlayerEX.ammo_bfg := PlayersBasePointer^[j].ammo_bfg;
						PlayerEX.health := PlayersBasePointer^[j].health;
						PlayerEX.armor := PlayersBasePointer^[j].armor;
						PlayerEX.frags := PlayersBasePointer^[j].frags;
						PlayerEX.x := PlayersBasePointer^[j].x;
						PlayerEX.y := PlayersBasePointer^[j].y;
						PlayerEX.cx := PlayersBasePointer^[j].cx;
						PlayerEX.cy := PlayersBasePointer^[j].cy;
						PlayerEX.fangle := PlayersBasePointer^[j].fangle;
						PlayerEX.InertiaX := PlayersBasePointer^[j].InertiaX;
						PlayerEX.InertiaY := PlayersBasePointer^[j].InertiaY;

						s1 := PlayersBasePointer^[j].netname;
						//PlayerEX.netname := PChar(s1);

						s2 := PlayersBasePointer^[j].nfkmodel;
						//PlayerEX.nfkmodel := PChar(s2);

						s3 := PlayersBasePointer^[j].Location;
						//PlayerEX.Location := PChar(s3);
												
{
						PlayerEX.netname := PlayersBasePointer^[j].netname;
						PlayerEX.nfkmodel := PlayersBasePointer^[j].nfkmodel;
						PlayerEX.Location := PlayersBasePointer^[j].Location;
}
						PlayerEX.DXID := PlayersBasePointer^[j].DXID;
						
						if @C_DLL_UpdatePlayerEX <> nil then C_DLL_UpdatePlayerEX(PlayerEX,PChar(s1),PChar(s2),PChar(s3));
						result:=true;
						break;
					end;
	end;

	
	//AddMessage(IntToStr(Round(Player^[0].x)));
	//AddMessage(IntToStr(Round(Player^[0].y)));
	//Player := Player + 1;
	//AddMessage(Player^[1].netname);
	//AddMessage(IntToStr(Round(Player^.y)));
	//AddMessage(IntToStr(sizeof(ttt)));
	//AddMessage('start3');


	//@f := ptr($508070); //Похоже на вывод на консоль
	//f('test1');

	//	@f := ptr($40495C); //Похоже на вызов команды
	//	f('fraglimit 10');

	//	@f := ptr($404898); //Похоже на tab на консоли
	//	f('test3');

	//	@f := ptr($54DB90); //Похоже на вызов команды
	//	f('timelimit 10');
end;

//---------------------------------------------
// Процедуры рисования FX
//---------------------------------------------
//Effect := 2 or $100 это чтобы была поддержка прозрачности
//типа если в color укажешь $77FFFFFF
//цвет в формате ABGR
//чтобы получить аддитивный эффект юзай 3 or $100

procedure P_DLL_FX_FillRect(X, Y, Width, Height: Integer; Color: Cardinal; Effect: Integer; Camera: boolean); cdecl;
begin
	FX_FillRect(X,Y,Width,Height,Color,Effect,Camera);
end;
procedure P_DLL_FX_FillRectMap(X1, Y1, X2, Y2, X3, Y3, X4, Y4: Integer; Color: Cardinal; Effect: Integer; Camera: boolean); cdecl;
begin
	FX_FillRectMap(X1, Y1, X2, Y2, X3, Y3, X4, Y4,Color,Effect,Camera);
end;

//будет градиент если $102 укажешь
//Порядрк вершин 1-2 4-3
procedure P_DLL_FX_FillRectMapEx(X1, Y1, X2, Y2, X3, Y3, X4, Y4: Integer; C1, C2, C3, C4: Cardinal; Effect: Integer; Camera: boolean); cdecl;
begin
	FX_FillRectMapEx(X1, Y1, X2, Y2, X3, Y3, X4, Y4,C1, C2, C3, C4,Effect,Camera);
end;
procedure P_DLL_FX_Rectangle(X, Y, Width, Height: Integer; ColorLine, ColorFill: Cardinal; Effect: Integer; Camera: boolean); cdecl;
begin
	FX_Rectangle(X, Y, Width, Height,ColorLine,ColorFill,Effect,Camera);
end;
procedure P_DLL_FX_Line(X1, Y1, X2, Y2: Integer; Color: Cardinal; Effect: Integer; Camera: boolean); cdecl;
begin
	FX_Line(X1, Y1, X2, Y2,Color,Effect,Camera);
end;


//---------------------------------------------
// Стандартные процедуры NFK.exe
//---------------------------------------------
procedure P_DLL_SetAngle(DXID: word; angle : word); cdecl;
begin
	SetAngle(DXID,angle);
end;

procedure P_DLL_AddMessage(s: PChar); cdecl;
begin
	if NOT AddMessageOff then AddMessage(string(s));
end;

procedure P_DLL_RegisterConsoleCommand(s: PChar); cdecl;
begin
	RegisterConsoleCommand(string(s));
end;

function P_DLL_GetSystemVariable(Text: PChar): PChar; cdecl;
var s : string;
begin

	//AddMessage(Text);	
	s := GetSystemVariable(string(Text));
	result:= PChar(s);
end;

function P_DLL_sys_CreatePlayer(name,model: PChar; team: byte):word cdecl;
begin
	result:=sys_CreatePlayer(string(name),string(model),team);
end;

procedure P_DLL_SetKeys(DXID: word; keys : byte); cdecl;
begin
	SetKeys(DXID,keys);
end;

procedure P_DLL_SetBalloon(DXID: word; value : byte); cdecl;
begin
	SetBalloon(DXID,value);
end;

procedure P_DLL_SetWeapon(DXID: word; value : byte); cdecl;
begin
	SetWeapon(DXID,value);
end;

procedure P_DLL_SendConsoleCommand(s: PChar); cdecl;
begin
	SendConsoleCommand(string(s));
end;

procedure P_DLL_SendConsoleHCommand(s: PChar); cdecl;
begin
	SendConsoleHCommand(string(s));
end;

procedure P_DLL_ExtendedTextout(x,y: word; font: byte; camera:boolean; s: PChar); cdecl;
begin
	ExtendedTextout(x,y,string(s),font,camera);
end;

function P_DLL_Test_Blocked(X, Y : word):Boolean; cdecl;
begin
	result:=Test_Blocked(X,Y);
end;

procedure P_DLL_debug_textout(x, y : word; text : PChar); cdecl;
begin
	debug_textout(x,y,string(text));
end;

procedure P_DLL_debug_textout_int(x, y : word; i : integer); cdecl;
begin
	debug_textout(x,y,IntToStr(i));
end;

procedure P_DLL_debug_textoutc(x, y : word; text : PChar); cdecl;
begin
	debug_textoutc(x,y,string(text));
end;

function P_DLL_GetBrickStruct(x, y : word): pTBrick; cdecl;
var b: TBrick;
begin
	b := GetBrickStruct(x,y);
	result:=Addr(b);
end;

function P_DLL_GetObjStruct(ID : word): pTObj; cdecl;
var o: TObj;
begin
	o := GetObjStruct(ID);
	result:= Addr(o);
end;

function P_DLL_GetSpecObjStruct(ID : byte): pTSpecObj; cdecl;
var so: TSpecObj;
begin
	so := GetSpecObjStruct(ID);
	result:= Addr(so);
end;

procedure P_DLL_RemoveBot(DXID : word); cdecl;
begin
	RemoveBot(DXID);
end;

procedure P_DLL_AddMessageXY(x, y: integer; s: PChar); cdecl;
begin
	if NOT AddMessageOff then AddMessage(string(s)+': ^2'+IntToStr(x)+' ^7; ^3'+IntToStr(y));
end;

procedure P_DLL_AddMessageX(x: integer; s: PChar); cdecl;
begin
	if NOT AddMessageOff then AddMessage(string(s)+IntToStr(x));
end;

procedure P_DLL_RegBotDlls(); cdecl;
var i:byte;
s: string;
begin
	if NOT IsClient then if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do
	begin
		s := BotsDllArray.FilesArray[i];
		if @C_DLL_RegisterBotDll <> nil then C_DLL_RegisterBotDll(PChar(s))
	end;
	
	//DEBUG PRINT
	//AddMessage('C_DLL_RegisterBotDllFinish');
	
end;

procedure P_DLL_SetActiveBot(n: shortint); cdecl;
begin
	ActiveBot := n;
	
	//DEBUG PRINT
	//AddMessage('P_DLL_SetActiveBot: ActiveBot = '+IntToStr(n));
end;
//Эту процедуру нужно вызывать осторжно (из pqrmod.dll)
//Т.к. если во время игры отключить какую-то bot.dll то
//она может не освободить память или ещё что.
procedure P_DLL_SetStatusOnOff(i: shortint;st: boolean); cdecl;
begin
	BotsDllArray.StatusOnOff[i]:=st;

	//DEBUG PRINT
	//AddMessage('P_DLL_SetStatusOnOff '+IntToStr(i));

end;



procedure P_DLL_EVENT_ExternMenuEnter(mid: BYTE; oid: BYTE); cdecl;
var i:integer;
begin
	if BotsDllArray.FilesCount>0 then for i:=0 to BotsDllArray.FilesCount-1 do if (@EXTERNAL_DLL_PMA_EVENT_MenuEnter[i]<>nil) then EXTERNAL_DLL_PMA_EVENT_MenuEnter[i](mid,oid);
	AddMessage('mid: '+IntToStr(mid)+' ;oid: '+IntToStr(oid));
end;

exports DLL_RegisterProc1,DLL_RegisterProc2, DLL_RegisterProc3,
        DLL_RegisterProc4, DLL_RegisterProc5,DLL_RegisterProc6,
        DLL_RegisterProc7, DLL_RegisterProc8,DLL_RegisterProc9,
        DLL_RegisterProc10,DLL_RegisterProc11,DLL_RegisterProc12,DLL_RegisterProc13,
		DLL_RegisterProcFX_FillRect,DLL_RegisterProcFX_FillRectMap, DLL_RegisterProcFX_FillRectMapEx, 
		DLL_RegisterProcFX_Rectangle, DLL_RegisterProcFX_Line,
        DLL_EVENT_BeginGame, DLL_EVENT_ResetGame, DLL_EVENT_MapChanged,
        DLL_MainLoop, DLL_SYSTEM_UpdatePlayer, DLL_SYSTEM_AddPlayer,
        DLL_SYSTEM_RemoveAllPlayers, DLL_SYSTEM_RemovePlayer, DLL_AddModel,
        DLL_DMGReceived, DLL_ChatReceived, DLL_CMD, DLL_QUERY_VERSION,DLL_EVENT_ConsoleMessage,
		P_DLL_SetAngle,P_DLL_AddMessage,P_DLL_RegisterConsoleCommand,
		P_DLL_GetSystemVariable,P_DLL_sys_CreatePlayer,P_DLL_SetKeys,
		P_DLL_SetBalloon,P_DLL_SetWeapon,P_DLL_Test_Blocked,P_DLL_debug_textout,P_DLL_debug_textout_int,P_DLL_debug_textoutc,
		P_DLL_GetBrickStruct,P_DLL_GetObjStruct,P_DLL_GetSpecObjStruct,P_DLL_RemoveBot,
		P_DLL_AddMessageXY,P_DLL_AddMessageX,P_DLL_RegBotDlls,P_DLL_SetActiveBot,P_DLL_SetStatusOnOff,
		P_DLL_UpdatePlayerEX,P_DLL_SetAngleEX,P_DLL_SendConsoleCommand,P_DLL_SetBasePointer,
		P_DLL_ExtendedTextout,P_DLL_SendConsoleHCommand,
		P_DLL_FX_FillRect,P_DLL_FX_FillRectMap,P_DLL_FX_FillRectMapEx,P_DLL_FX_Rectangle,P_DLL_FX_Line,
		P_DLL_EVENT_ExternMenuEnter;


procedure LibExit; far;
var i: shortint;
begin
//	if ExitCode = wep_System_Exit then
//    begin
	{ выполняется завершение работы системы }
//    end else
//	begin
	{ разгружается DLL }
	for i := 0 to BotsDllArray.FilesCount-1 do FreeLibrary(BotDllHandle[i]);
	FreeLibrary(PQRModDllHandle);
//	end;
	ExitProc := SaveExit;
end;

begin
	{ выполнить инициализацию DLL }
	NFKStarted:=false;
	//AddMessageOff:=true;
	BotsDllArray.FilesCount := 0;
	ActiveBot:=0;
	PQRModDllHandle := 0;

	SetCurrentDir('basenfk');
	//Подгружаем библиотеки других ботов
	if SetCurrentDir('bots') then FindBotDlls
	else ActiveBot:=-1;

	//tmp_str:=GetCurrentDir;

	SaveExit := ExitProc;   { сохранить старый указатель процедуры выхода }
	ExitProc := @LibExit;   { установка процедуры выхода LibExit }
end.