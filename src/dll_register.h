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
	dll_register.h
	ќбъ€вдение переменный функционального типа, которым будут сопоставлены функции
	динамически загружаемой библиотеки bot.dll - Delhi модуль
*/

#ifndef __DLL_REGISTER_H__
#define __DLL_REGISTER_H__
#include "types.h"
#include <windows.h> //дл€ типа POINT

extern void (*fnP_DLL_SetAngle)(WORD,WORD);
extern void (*fnP_DLL_AddMessage)(char*);
extern void (*fnP_DLL_RegisterConsoleCommand)(char*);
extern char* (*fnP_DLL_GetSystemVariable)(char*);
extern WORD (*fnP_DLL_sys_CreatePlayer)(char*,char*,BYTE);
extern void (*fnP_DLL_SetKeys)(WORD,BYTE);
extern void (*fnP_DLL_SetBalloon)(WORD,BYTE);
extern void (*fnP_DLL_SetWeapon)(WORD,BYTE);
extern void (*fnP_DLL_SendConsoleCommand)(char*);
extern void (*fnP_DLL_SendConsoleHCommand)(char*);
extern bool (*fnP_DLL_Test_Blocked)(WORD,WORD);
extern void (*fnP_DLL_debug_textout)(WORD,WORD,char*);
extern void (*fnP_DLL_debug_textoutc)(WORD,WORD,char*);
extern void (*fnP_DLL_ExtendedTextout)(WORD x,WORD y,BYTE fnt,bool camera,char* str);
extern TBrick* (*fnP_DLL_GetBrickStruct)(WORD,WORD);
extern TObj* (*fnP_DLL_GetObjStruct)(WORD);
extern TSpecObj* (*fnP_DLL_GetSpecObjStruct)(BYTE);
extern void (*fnP_DLL_RemoveBot)(WORD);
extern void (*fnP_DLL_PatchBot)(WORD dxid, BYTE hiparam, BYTE loparam);

extern void (*fnP_DLL_RegBotDlls)(void);
extern void (*fnP_DLL_SetActiveBot)(char);
extern void (*fnP_DLL_SetStatusOnOff)(char,char);
extern void (*fnP_DLL_EVENT_ExternMenuEnter)(BYTE mid,BYTE oid);

extern void (*fnP_DLL_FX_FillRect)(int,int,int,int,Cardinal,int,bool);
extern void (*fnP_DLL_FX_FillRectMap)(int,int,int,int,int,int,int,int,Cardinal,int,bool);
extern void (*fnP_DLL_FX_FillRectMapEx)(int,int,int,int,int,int,int,int,Cardinal,Cardinal,Cardinal,Cardinal,int,bool);
extern void (*fnP_DLL_FX_Rectangle)(int,int,int,int,Cardinal,Cardinal,int,bool);
extern void (*fnP_DLL_FX_Line)(int x1,int y1,int x2,int y2,Cardinal color,int effect,bool camera);

extern POINT* (*fnP_DLL_MouseDelta)();
extern bool (*fnP_DLL_KeyPressed)(BYTE key);


#endif //__DLL_REGISTER_H__