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
#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include "pqrmod.h"

#define __MOD_DEFAULT_CONFIG__ "pqrmod"

void ConfigSave(char*); //��������� ������� PQR Mod
void ConfigLoad(char*);

void CMD_cfg_save(char*); //��������� ������� NFK + PQR Mod
void CMD_cfg_save_pm(char* s); //��������� ������� PQR Mod

void LoadMapConfig(char*);
void SaveMapConfig(char*);

void CFG_MAN_RegisterConsoleCommands();
void CFG_MAN_EVENT_NFKStarted();

#endif