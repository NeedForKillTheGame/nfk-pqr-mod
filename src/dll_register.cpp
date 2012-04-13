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
#include "dll_register.h"

void (*fnP_DLL_SetAngle)(WORD,WORD);
void (*fnP_DLL_AddMessage)(char*);
void (*fnP_DLL_RegisterConsoleCommand)(char*);
char* (*fnP_DLL_GetSystemVariable)(char*);
WORD (*fnP_DLL_sys_CreatePlayer)(char*,char*,BYTE);
void (*fnP_DLL_SetKeys)(WORD,BYTE);
void (*fnP_DLL_SetBalloon)(WORD,BYTE);
void (*fnP_DLL_SetWeapon)(WORD,BYTE);
void (*fnP_DLL_SendConsoleCommand)(char*);
void (*fnP_DLL_SendConsoleHCommand)(char*);
bool (*fnP_DLL_Test_Blocked)(WORD,WORD);
void (*fnP_DLL_debug_textout)(WORD,WORD,char*);
void (*fnP_DLL_debug_textoutc)(WORD,WORD,char*);
void (*fnP_DLL_ExtendedTextout)(WORD,WORD,BYTE,bool,char*);
TBrick* (*fnP_DLL_GetBrickStruct)(WORD,WORD);
TObj* (*fnP_DLL_GetObjStruct)(WORD);
TSpecObj* (*fnP_DLL_GetSpecObjStruct)(BYTE);
void (*fnP_DLL_RemoveBot)(WORD);
void (*fnP_DLL_PatchBot)(WORD dxid, BYTE hiparam, BYTE loparam);

void (*fnP_DLL_RegBotDlls)(void);
void (*fnP_DLL_SetActiveBot)(char);
void (*fnP_DLL_SetStatusOnOff)(char,char);
void (*fnP_DLL_EVENT_ExternMenuEnter)(BYTE mid,BYTE oid);

void (*fnP_DLL_FX_FillRect)(int,int,int,int,Cardinal,int,bool);
void (*fnP_DLL_FX_FillRectMap)(int,int,int,int,int,int,int,int,Cardinal,int,bool);
void (*fnP_DLL_FX_FillRectMapEx)(int,int,int,int,int,int,int,int,Cardinal,Cardinal,Cardinal,Cardinal,int,bool);
void (*fnP_DLL_FX_Rectangle)(int x,int y,int w,int h,Cardinal ColorLine,Cardinal ColorFill,int effect,bool camera);
void (*fnP_DLL_FX_Line)(int x1,int y1,int x2,int y2,Cardinal color,int effect,bool camera);

POINT* (*fnP_DLL_MouseDelta)();
bool (*fnP_DLL_KeyPressed)(BYTE key);
