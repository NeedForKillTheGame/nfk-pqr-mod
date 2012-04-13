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

#ifndef __TEAMHELP_H__
#define __TEAMHELP_H__
#include "pqrmod.h"

class CTeamHelp
{
public:
	CTeamHelp();
	void EVENT_MainLoop();
	void EVENT_ResetGame();
	void EVENT_ItemTaken(BYTE brickx, BYTE bricky, BYTE image, WORD dxid);
	void ChatReceived(WORD DXID, char* Text);
	void RegisterConsoleCommands();

	void DrawHealth();
	void ShowHealth();
	void HideHealth();

	int GetSec();
	int GetMin();

	char* loc_last_death;
	char* loc_last_took;
	char* item_last_took;
	char* loc_last_point;
	char* item_last_point;
	int dsec;
	bool is_last_took;
	bool TC_need_send;
	int after_death_timer;
};

extern CTeamHelp TeamHelp;
extern c_var_bool* ch_teamhealth;

#endif //__TEAMHELP_H__