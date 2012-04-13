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
*	Модуль Ruleset устанавливает правила игры по умолчанию как только наступает
*	0 < warmup < 5
*	Допустимые значения переменной Ruleset:
*	0 - выключено
*	1 - dm или duel (timelimit 10, forcerespawn 10, warmup 180)
*	2 - tdm или team (timelimi 15, forcerespawn 5, warmup 180)
*
*/

#ifndef __RULESET_H__
#define __RULESET_H__

#include "pqrmod.h"

class CRuleset
{
public:
	CRuleset();
	void RegisterConsoleCommands();

	enum {
		DUEL_TIMELIMIT = 10,
		DUEL_FORCERESPAWN = 10,
		DUEL_WARMUP = 180,
		TDM_TIMELIMIT = 15,
		TDM_SV_OVERTIME = 2,
		TDM_FORCERESPAWN = 5,
		TDM_WARMUP = 180,
		FRAGLIMIT = 0
	};
};

extern CRuleset Ruleset; //Главный объект модуля
extern c_var_int* ruleset; //консольная переменная

#endif //__RULESET_H__