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

unit single;

interface
uses classes,windows;

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


type
//Типы процедур и функций для Single Interface
  TCallProc_SetArmorHealth = procedure(h,a: integer; DXID: WORD; password:shortstring);
  TCallProc_Teleport = procedure(x,y: real; DXID: WORD; password:shortstring);
	
  TCallProc_DLL_SINGLE_RegisterSetArmorHealth = procedure(AProc: TCallProc_SetArmorHealth);
  TCallProc_DLL_SINGLE_RegisterTeleport = procedure(AProc: TCallProc_Teleport);


procedure ProcHackInit;
procedure SetArmorHealth(h,a: integer; DXID: WORD; password:shortstring);
procedure Teleport(x,y: real; DXID: WORD; password:shortstring);

var
	PlayersBasePointer: p_arrTplayerO;

implementation

procedure ProcHackInit;
var s: string;
begin
	PlayersBasePointer := nil;
	s := GetSystemVariable('nfkversion');
	if s = '064' then PlayersBasePointer := ptr($007564CC);
end;

procedure Teleport(x,y: real; DXID: WORD; password:shortstring);
begin
if (PlayersBasePointer<>nil) then
	for j := 0 to 7 do
		if PlayersBasePointer^[j] <> nil then
			if PlayersBasePointer^[j].DXID = DXID then
			begin
				PlayersBasePointer^[j].x := x;	
				PlayersBasePointer^[j].y := y;
			end;
end;

procedure SetArmorHealth(h,a: integer; DXID: WORD; password:shortstring);
begin
if (PlayersBasePointer<>nil) then
	for j := 0 to 7 do
		if PlayersBasePointer^[j] <> nil then
			if PlayersBasePointer^[j].DXID = DXID then
			begin
				PlayersBasePointer^[j].health := h;	
				PlayersBasePointer^[j].armor := a;
			end;
end;

end.

