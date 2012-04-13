{

	BOT.DLL for Need For Kill
	(c) 3d[Power]
	http://www.3dpower.org

        unit: bot_register
        purpose: system types and vars.

        warning: do not modify this unit.
}

unit bot_register;


interface
uses classes;

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

type pTObj = ^TObj;

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

type TPlayerEx2 = record //For Proc Hack
        dead:byte;
		bot,crouch,balloon,flagcarrier,have_rl, have_gl, have_rg, have_bfg, have_sg, have_mg, have_sh, have_pl : boolean;
        refire,weapchg,weapon,threadweapon,dir,gantl_state,air,team,item_quad, item_regen, item_battle, item_flight, item_haste, item_invis,ammo_mg, ammo_sg, ammo_gl, ammo_rl, ammo_sh, ammo_rg, ammo_pl, ammo_bfg : byte;
        x, y, cx, cy, fangle,InertiaX, InertiaY : real;
        health, armor, frags : integer;
		{
		netname,nfkmodel:string[30];
		Location:string[64];
		}
		//netname,nfkmodel,Location :PChar;
		
        DXID : word;
        end;

type TBot = class
	DXID: word;
	dll_num: Shortint;
	end;

type
//���� �������� � ������� ��������� � NFK.exe
  TCallProcSTR = function(text:shortstring):shortstring;
  TCallTextProc = procedure(text:shortstring);
  //TCallProcCreatePlayer = procedure(name, model: shortstring; team : byte); //������ �� 065
  TCallProcCreatePlayer = function(name, model: shortstring; team : byte):word; //������� � 065 - ���������� DXID
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


//���� �������� � ������� ��������� � ������ bot.dll (����� �� �������, ��� � � ���� dll)
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

//���� �������� � ������� ��������� � PQR Mod �� MS Visual C++
TCallProc_C_DLL_EVENT_BeginGame = procedure(); cdecl;
TCallProc_C_DLL_EVENT_ResetGame = procedure(); cdecl;
TCallProc_C_DLL_EVENT_MapChanged = procedure(); cdecl;
TCallProc_C_DLL_MainLoop = procedure(); cdecl;
TCallProc_C_DLL_SYSTEM_UpdatePlayer = procedure(Player : TPlayerEx); cdecl;
TCallProc_C_DLL_UpdatePlayerEX = procedure(Player : TPlayerEx2; s1: PChar; s2: PChar; s3: PChar); cdecl;
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
TCallProc_C_DLL_SetActiveBotDll = procedure(i: Integer); cdecl;
TCallProc_C_DLL_RegisterMainBotDll = procedure(s: PChar); cdecl;

TCallProc_DLL_PMA_EVENT_MenuEnter = procedure(mid: byte; oid: byte);

type PMA_TMenu = record
	x,y,Width : integer;
	SubMenuMask : integer;
	end;

type

//���� �������� � ������� PQR MOD API ��������� � PQR Mod �� MS Visual C++
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

type TPlayerStats = record
        stat_suicide : word;
        stat_kills : word;
        stat_deaths : word;
        stat_dmggiven : integer;
        stat_dmgrecvd : integer;
        stat_impressives : word;
        stat_excellents : word;
        stat_humiliations : word;
        gaun_hits : word;
        mach_kills : word;
        mach_hits : word;
        mach_fire : word;
        shot_kills  : word;
        shot_hits : word;
        shot_fire : word;
        gren_fire : word;
        gren_hits : word;
        gren_kills : word;
        rocket_fire : word;
        rocket_kills : word;
        rocket_hits : word;
        shaft_fire : word;
        shaft_hits : word;
        shaft_kills : word;
        plasma_fire : word;
        plasma_hits : word;
        plasma_kills : word;
        rail_kills : word;
        rail_hits : word;
        rail_fire : word;
        bfg_fire : word;
        bfg_kills : word;
        bfg_hits : word;
end;


type TplayerO = class
        public
        dead : byte;
        frame,nextframe : byte;
        refire,doublejump,weapchg,weapon,threadweapon : byte;
        dir,idd,control,shaftframe,shaftsttime,inlava,paintime,hitsnd, justrespawned, justrespawned2 : byte;
        gantl_s,gauntl_s_order, gantl_refire, gantl_state : byte;
        shaft_state : byte;
        ammo_snd,ammo_mg,ammo_sg,ammo_gl,ammo_rl,ammo_sh,ammo_rg,ammo_pl,ammo_bfg : byte;

        loadframe : byte;
        air, team : byte;

        walk_index, die_index, crouch_index, power_index,cpower_index : word;
        SND_death1,SND_death2,SND_death3,SND_Jump,SND_Pain100,SND_Pain75,SND_Pain50,SND_Pain25:word;//fmod index.

        clippixel : integer;
        health, armor, frags : integer;
        objname,netname,soundmodel,nfkmodel, realmodel : string[30];


        keys : word;

        crouch,balloon,flagcarrier : boolean;

        NETUpdateD : boolean;
        NETNoSignal : word;
        Location : string[64];

        IPAddress : ShortString;
        Port : word;
        PLAYERISHOST : boolean;

        Vote:byte;

        botangle : real;
        botrailcolor : byte;

        TESTPREDICT_X, TESTPREDICT_Y : Real; // interpolate players coordinates for multiplayer (if lag). outdated.

//        TST_X, TST_Y : Real; // interpolate players coordinates for multiplayer (if lag).
//        TEN_X, TEN_Y : Real; // interpolate players coordinates for multiplayer (if lag).
//        TMT, CTI : byte;

        {
                TST_X, TST_Y - start position
                TEN_X, TEN_Y - end position
                TMT - max ticks available
                CTI - current tick process.
        }

        walkframes,dieframes,modelsizex,walkstartframe,framerefreshtime,dieframerefreshtime,diesizey,crouchsizex,crouchsizey,crouchframes,crouchrefreshtime,crouchstartframe : byte;
        netobject : boolean;
        item_quad,item_regen,item_battle,item_flight,item_haste,item_invis : byte;
        item_quad_time, item_haste_time,item_regen_time,item_battle_time, item_flight_time: byte;
        impressive, excellent, rewardtype, rewardtime : byte;
        have_rl,have_gl,have_rg,have_bfg,have_sg,have_mg,have_sh,have_pl : boolean;
        DXID,respawn : word;
        x,y,cx,cy,fangle : real;
        InertiaX,InertiaY : real;
        stats : TPlayerstats;

        // for demo!
        Lx,Ly : word;
        LInertiaX,LInertiaY : Single;
        Lcrouch, Lballoon:boolean;
        Ldir,Ldead,Lwpn,Lwpnang,LArmor : byte;
        LHealth,Lfrags : smallint;
        NETHealth, NETFrags,NETArmor,NETLastammo,NETAmmo:smallint;
        NET_LastInertiaY, NET_LastPosY : real;
        clientrespawntimeout : cardinal; // for avoiding cant respawn bug..

        olspx, olspy : byte;
        // net;
        ping : word;
end;


type arrTplayerO = array[0..7] of TplayerO;
type p_arrTplayerO = ^arrTplayerO;

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

	FX_FillRect : TCallProcFX_FillRect;
	FX_FillRectMap: TCallProcFX_FillRectMap;
	FX_FillRectMapEx : TCallProcFX_FillRectMapEx;
	FX_Rectangle : TCallProcFX_Rectangle;
	FX_Line : TCallProcFX_Line;

  bots : array[0..7] of TBot;

implementation


end.
