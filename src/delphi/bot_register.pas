{

	BOT.DLL for Need For Kill
	(c) 3d[Power]
	http://www.3dpower.org

        unit: bot_register
        purpose: system types and vars.

        warning: do not modify this unit.
}

{
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
} 

unit bot_register;

interface
uses classes,windows;

type pTPoint = ^TPoint;
        // brick structure
type    TBrick = record // do not modify
        image : byte;          // graphix index
        block : boolean;       // do this brick block player;
        respawntime : integer; // respawn time
        y           : shortint;
        dir         : byte;
        oy          : real;
        respawnable : boolean; // is this shit can respawn?
        scale       : byte;
        end;

type pTBrick = ^TBrick;

        // object structure. (eg Rockets, blood, everything!)
type    TObj = record // do not modify
        dead : byte;
        speed,fallt,weapon,doublejump,refire : byte;
        imageindex,dir,idd : byte;
        clippixel : smallint;
        spawnerDXID : word;
        frame : byte;
        health : smallint;
        x,y,cx,cy,fangle,fspeed : real;
        objname : string[30];
        DXID : word;
        mass, InertiaX,InertiaY : real;
        end;
        
type    TObj2 = record // do not modify
        dead : byte;
        speed,fallt,weapon,doublejump,refire : byte;
        imageindex,dir,idd : byte;
        clippixel : smallint;
        spawnerDXID : word;
        frame : byte;
        health : smallint;
        x,y,cx,cy,fangle,fspeed : real;
        objname_code : byte;
        DXID : word;
        mass, InertiaX,InertiaY : real;
        end;


type pTObj2 = ^TObj2;

        // special object structure. (eg Doors, Buttons)
type    TSpecObj = record  // do not modify
        active : boolean;
        x,y,lenght,dir,wait : word;
        targetname,target,orient,nowanim,special:word;
        objtype : byte;
        end;

type pTSpecObj = ^TSpecObj;


type TPlayerEx = record //class copy. DO NOT MODIFY. This record used by NFK CODE.
        dead,bot,crouch,balloon,flagcarrier,have_rl, have_gl, have_rg, have_bfg, have_sg, have_mg, have_sh, have_pl : boolean;
        refire,weapchg,weapon,threadweapon,dir,gantl_state,air,team,item_quad, item_regen, item_battle, item_flight, item_haste, item_invis,ammo_mg, ammo_sg, ammo_gl, ammo_rl, ammo_sh, ammo_rg, ammo_pl, ammo_bfg : byte;
        x, y, cx, cy, fangle,InertiaX, InertiaY : real;
        health, armor, frags : integer;
        netname,nfkmodel : string[30];
        Location : string[64];
        DXID : word;
        end;

type TBot = class
	DXID: word;
	dll_num: Shortint;
	end;

type
//Типы процедур и функций описанных в NFK.exe
  TCallProcSTR = function(text:shortstring):shortstring;
  TCallTextProc = procedure(text:shortstring);
  //TCallProcCreatePlayer = procedure(name, model: shortstring; team : byte); //Версия до 065
  TCallProcCreatePlayer = function(name, model: shortstring; team : byte):word; //Начиная с 065 - возвращает DXID
  TCallProcWordByteFunc = procedure(DXID : word ; value: byte);
  TCallProcWordWordFunc = procedure(DXID : word ; value: word);
  TCallProcWordWord_Bool = function(x, y : word):boolean;
  TCallProcWordWordString = procedure(x, y : word; text : shortstring);
  TCallProcBrickStruct          = function(x, y : word):TBrick;
  TCallProcObjectsStruct        = function(ID : word):TObj;
  TCallProcSpecailObjectsStruct = function(ID : byte):TSpecObj;
  TCallProcChat = procedure(DXID:word; text : shortstring; teamchat: boolean);
  TCallProcWord = procedure(par : WORD);
  TCallProcExtendedTextout = procedure (x,y: word; text : shortstring; font: byte; camera:boolean);

  TCallProcFX_FillRect  = procedure (X, Y, Width, Height: Integer; Color: Cardinal; Effect: Integer; Camera: boolean);
  TCallProcFX_FillRectMap = procedure (X1, Y1, X2, Y2, X3, Y3, X4, Y4: Integer; Color: Cardinal; Effect: Integer; Camera: boolean);
  TCallProcFX_FillRectMapEx = procedure (X1, Y1, X2, Y2, X3, Y3, X4, Y4: Integer; C1, C2, C3, C4: Cardinal; Effect: Integer; Camera: boolean);
  TCallProcFX_Rectangle = procedure (X, Y, Width, Height: Integer; ColorLine, ColorFill: Cardinal; Effect: Integer; Camera: boolean);
  TCallProcFX_Line = procedure (X1, Y1, X2, Y2: Integer; Color: Cardinal; Effect: Integer; Camera: boolean);
  TCallProcKeyPressed = function(key : byte):boolean;
  TCallProcMouseDelta = function : TPoint;
  
  TCallProcPatch = procedure (DXID : word; hiparam : single; loparam : single);

//Типы процедур и функций описанных в других bot.dll (такие же функции, как и в этой dll)
TCallProc_DLL_RegisterProc1 = procedure(AProc : TCallProcWordWordFunc);
TCallProc_DLL_RegisterProc2 = procedure(AProc : TCallTextProc; ProcID:byte);
TCallProc_DLL_RegisterProc3 = procedure(AProc : TCallProcSTR; ProcID:byte);
TCallProc_DLL_RegisterProc4 = procedure(AProc : TCallProcCreatePlayer; ProcID:byte);
TCallProc_DLL_RegisterProc5 = procedure(AProc : TCallProcWordByteFunc; ProcID:byte);
TCallProc_DLL_RegisterProc6 = procedure(AProc : TCallProcWordWord_Bool; ProcID:byte);
TCallProc_DLL_RegisterProc7 = procedure(AProc : TCallProcWordWordString; ProcID:byte);
TCallProc_DLL_RegisterProc8 = procedure(AProc : TCallProcBrickStruct);
TCallProc_DLL_RegisterProc9 = procedure(AProc : TCallProcObjectsStruct);
TCallProc_DLL_RegisterProc10 = procedure(AProc : TCallProcSpecailObjectsStruct);
TCallProc_DLL_RegisterProc11 = procedure(AProc : TCallProcWord);
TCallProc_DLL_RegisterProc12 = procedure(AProc : TCallProcChat);
TCallProc_DLL_RegisterProc13 = procedure(AProc : TCallProcExtendedTextout);

TCallProc_DLL_RegisterProcFX_FillRect  = procedure(AProc : TCallProcFX_FillRect);
TCallProc_DLL_RegisterProcFX_FillRectMap  = procedure(AProc : TCallProcFX_FillRectMap);
TCallProc_DLL_RegisterProcFX_FillRectMapEx  = procedure(AProc : TCallProcFX_FillRectMapEx);
TCallProc_DLL_RegisterProcFX_Rectangle  = procedure(AProc : TCallProcFX_Rectangle);
TCallProc_DLL_RegisterProcFX_Line  = procedure(AProc : TCallProcFX_Line);
TCallProc_DLL_RegisterMouseDelta  = procedure(AProc : TCallProcMouseDelta);
TCallProc_DLL_RegisterKeyPressed  = procedure(AProc : TCallProcKeyPressed);

TCallProc_DLL_RegisterProcPatch  = procedure(AProc : TCallProcPatch);

TCallProc_DLL_MainLoop = procedure();
TCallProc_DLL_EVENT_BeginGame = procedure();
TCallProc_DLL_EVENT_MapChanged = procedure();
TCallProc_DLL_EVENT_ResetGame = procedure();
TCallProc_DLL_QUERY_VERSION = function():shortstring;
TCallProc_DLL_SYSTEM_AddPlayer = procedure(Player : TPlayerEx);
TCallProc_DLL_SYSTEM_RemoveAllPlayers = procedure();
TCallProc_DLL_SYSTEM_RemovePlayer = procedure(DXID:word);
TCallProc_DLL_DMGReceived = procedure(TargetDXID, AttackerDXID:Word; dmg : word);
TCallProc_DLL_ChatReceived = procedure(DXID:Word; Text : shortstring);
TCallProc_DLL_AddModel = procedure(s : shortstring);
TCallProc_DLL_SYSTEM_UpdatePlayer = procedure(Player : TPlayerEx);
TCallProc_DLL_EVENT_ConsoleMessage = procedure(s : shortstring);
TCallProc_DLL_CMD = procedure(s : string);
TCallProc_DLL_EVENT_ItemTaken = procedure ( brickx, bricky, image : byte; dxid : word );
TCallProc_DLL_EVENT_DroppableWeaponTaken = procedure (wpndxid, playerdxid : word );

//Типы процедур и функций описанных в PQR Mod на MS Visual C++
TCallProc_C_DLL_EVENT_BeginGame = procedure(); cdecl;
TCallProc_C_DLL_EVENT_ResetGame = procedure(); cdecl;
TCallProc_C_DLL_EVENT_MapChanged = procedure(); cdecl;
TCallProc_C_DLL_MainLoop = procedure(); cdecl;
TCallProc_C_DLL_SYSTEM_UpdatePlayer = procedure(Player : TPlayerEx); cdecl;
TCallProc_C_DLL_SYSTEM_UpdatePlayerString = procedure(code:Integer;DXID:word;str:PChar); cdecl;
TCallProc_C_DLL_SYSTEM_AddPlayer = procedure(Player : TPlayerEx); cdecl;
TCallProc_C_DLL_SYSTEM_RemoveAllPlayers = procedure(); cdecl;
TCallProc_C_DLL_SYSTEM_RemovePlayer = procedure(DXID:word); cdecl;
TCallProc_C_DLL_AddModel = procedure(s: PChar); cdecl;
TCallProc_C_DLL_DMGReceived = procedure(TargetDXID, AttackerDXID:Word; dmg : word); cdecl;
TCallProc_C_DLL_ChatReceived = procedure(DXID:Word; Text : PChar); cdecl;
TCallProc_C_DLL_EVENT_ConsoleMessage = procedure(Text : PChar); cdecl;
TCallProc_C_DLL_CMD = procedure(s: PChar); cdecl;
TCallProc_C_DLL_QUERY_VERSION = function(): PChar; cdecl;
TCallProc_C_DLL_RegisterBotDll = procedure(s: PChar); cdecl;
TCallProc_C_DLL_NFKStarted = procedure(); cdecl;
TCallProc_C_DLL_RegisterMainBotDll = procedure(s: PChar); cdecl;
TCallProc_C_DLL_EVENT_ItemTaken = procedure ( brickx, bricky, image : byte; dxid : word ); cdecl;
TCallProc_C_DLL_EVENT_DroppableWeaponTaken = procedure (wpndxid, playerdxid : word ); cdecl;

TCallProc_DLL_PMA_EVENT_MenuEnter = procedure(mid: byte; oid: byte);

type PMA_TMenu = record
	x,y,Width : integer;
	SubMenuMask : integer;
	end;

type

//Типы процедур и функций PQR MOD API описанных в PQR Mod на MS Visual C++
TCallProc_PMA_MenuCreate = function(m: PMA_TMenu): integer; cdecl;
TCallProc_PMA_MenuSets = procedure(mid: integer; m: PMA_TMenu); cdecl;
TCallProc_PMA_MenuAddOption = procedure(mid: integer; s: PChar); cdecl;
TCallProc_PMA_MenuSetOption = procedure(mid: integer; oid: integer; s: PChar); cdecl;
TCallProc_PMA_DeleteAllOptions = procedure(min: integer); cdecl;
TCallProc_PMA_MenuShowHide = procedure(mid: integer; visible: boolean); cdecl;

TCallProc_DLL_PMA_RegisterMenuCreate = procedure(AProc: TCallProc_PMA_MenuCreate); cdecl;
TCallProc_DLL_PMA_RegisterMenuSets = procedure(AProc: TCallProc_PMA_MenuSets); cdecl;
TCallProc_DLL_PMA_RegisterMenuAddOption = procedure(AProc: TCallProc_PMA_MenuAddOption); cdecl;
TCallProc_DLL_PMA_RegisterMenuSetOption = procedure(AProc: TCallProc_PMA_MenuSetOption); cdecl;
TCallProc_DLL_PMA_RegisterDeleteAllOptions = procedure(AProc: TCallProc_PMA_DeleteAllOptions); cdecl;
TCallProc_DLL_PMA_RegisterMenuShowHide = procedure(AProc: TCallProc_PMA_MenuShowHide); cdecl;

VAR
  AddMessage    : TCallTextProc;
  GetSystemVariable : TCallProcSTR;
  RegisterConsoleCommand : TCallTextProc;
  sys_CreatePlayer : TCallProcCreatePlayer;
  SetAngle      : TCallProcWordWordFunc;
  SetKeys       : TCallProcWordByteFunc;
  SetWeapon     : TCallProcWordByteFunc;
  SendConsoleCommand : TCallTextProc;
  SendConsoleHCommand : TCallTextProc;
  SendAuthorization : TCallTextProc;
  Test_Blocked  : TCallProcWordWord_Bool;
  SetBalloon    : TCallProcWordByteFunc;
  SendBotChat   : TCallProcChat;
  debug_textout : TCallProcWordWordString;
  debug_textoutc : TCallProcWordWordString;
  ExtendedTextout : TCallProcExtendedTextout;
  GetBrickStruct : TCallProcBrickStruct;
  GetObjStruct   : TCallProcObjectsStruct;
  GetSpecObjStruct : TCallProcSpecailObjectsStruct;
  RemoveBot      : TCallProcWord;
  PatchBot : TCallProcPatch;

	FX_FillRect : TCallProcFX_FillRect;
	FX_FillRectMap: TCallProcFX_FillRectMap;
	FX_FillRectMapEx : TCallProcFX_FillRectMapEx;
	FX_Rectangle : TCallProcFX_Rectangle;
	FX_Line : TCallProcFX_Line;
	
	MouseDelta : TCallProcMouseDelta;
	KeyPressed : TCallProcKeyPressed;

  bots : array[0..7] of TBot;

implementation


end.
