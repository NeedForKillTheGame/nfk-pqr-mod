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
#ifndef __AUTORECORD_H__
#define __AUTORECORD_H__
#include "pqrmod.h"

extern c_var_int* match_auto_record;

class CAutorecord
{
	int TickCountMainLoop;
	bool tmp_demo_recorded;
	bool record_in_process;
	bool match_in_process;
	char filename[255];

	c_var_string* match_name_versus;
	c_var_string* match_name_on;
	c_var_string* match_name_and;
public:
	CAutorecord();
	bool is_tmp_demo_recorded() {return tmp_demo_recorded;}
	bool is_record_in_process() {return record_in_process;}
	bool is_match_in_process() {return match_in_process;}
	BYTE auto_record() {return match_auto_record->value();}
	void stop_record() {record_in_process=false;} //Вызывается, когда пользователь принудительно даёт команду stoprecord
	void EVENT_Warmup_5();
	void EVENT_ResetGame();
	void RegisterConsoleCommands();

	void Start();
	void Save();
};

extern CAutorecord Autorecord; //Главный объект модуля

#endif //__AUTORECORD_H__