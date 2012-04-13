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

#define PQRMOD_API extern "C" __declspec(dllexport)

#define MOD_VERSION "^3^nP^1^bQ^3^nR ^7Mod v0.75"

#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define VID_WIDTH 1024
#define VID_HEIGHT 768
#define HEALTH_VALUE_WARNING_1 99
#define HEALTH_VALUE_WARNING_2 30
#define ARMOR_VALUE_WARNING_1 99
#define ARMOR_VALUE_WARNING_2 30

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "ShevList.h"
#include "types.h"
#include "dll_register.h"
#include "console.h"
#include "common.h"

//Двунаправленый список из строк. Используется во многих модулях поэтому вынесен в pqrmod.h
typedef Item<char*> ItemString; 
typedef List<ItemString> ListStrings;

extern HINSTANCE hModule1;
extern char NFKRootDir[200];
extern char NFKGameDir[200];
extern char NFKVersion[50];
extern int NFKVersionI;
extern bool match_in_process;
extern bool IsClient;
extern bool IsTeamplay;