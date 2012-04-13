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
#include "map.h"

void CMap::LoadMap_tourney1()
{
	fnP_DLL_AddMessage("Loading map tourney1...");
	
	number_WPs = 0;
	//WPs = new CWayPoint[37];

	Items.DeleteAll();
	CItem* I;

	I = new CItem;
	I->type = IT_RED_ARMOR;
	I->WP = 0;
	I->score = 70;
	I->brick_x = 4;
	I->brick_y = 28;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_ROCKET;
	I->WP = 2;
	I->score = 20;
	I->brick_x = 4;
	I->brick_y = 22;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_HEALTH_25;
	I->WP = 3;
	I->score = 10;
	I->brick_x = 5;
	I->brick_y = 14;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_HEALTH_25;
	I->WP = 4;
	I->score = 10;
	I->brick_x = 15;
	I->brick_y = 10;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_HEALTH_25;
	I->WP = 4;
	I->score = 10;
	I->brick_x = 18;
	I->brick_y = 26;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_HEALTH_5;
	I->WP = 4;
	I->score = 1;
	I->brick_x = 10;
	I->brick_y = 5;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_HEALTH_5;
	I->WP = 4;
	I->score = 1;
	I->brick_x = 11;
	I->brick_y = 4;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_HEALTH_5;
	I->WP = 4;
	I->score = 1;
	I->brick_x = 12;
	I->brick_y = 5;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_HEALTH_5;
	I->WP = 4;
	I->score = 1;
	I->brick_x = 13;
	I->brick_y = 6;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHOTGUN;
	I->WP = 5;
	I->score = 12;
	I->brick_x = 4;
	I->brick_y = 6;
	Items.AddBottom(I);
	
	I = new CItem;
	I->type = IT_AMMO_SHAFT;
	I->WP = 6;
	I->score = 5;
	I->brick_x = 1;
	I->brick_y = 10;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_HEALTH_25;
	I->WP = 7;
	I->score = 10;
	I->brick_x = 3;
	I->brick_y = 14;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_ROCKET;
	I->WP = 8;
	I->score = 20;
	I->brick_x = 18;
	I->brick_y = 3;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 9;
	I->score = 2;
	I->brick_x = 3;
	I->brick_y = 6;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 10;
	I->score = 1;
	I->brick_x = 5;
	I->brick_y = 6;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 11;
	I->score = 1;
	I->brick_x = 6;
	I->brick_y = 6;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 12;
	I->score = 1;
	I->brick_x = 13;
	I->brick_y = 14;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 12;
	I->score = 1;
	I->brick_x = 14;
	I->brick_y = 14;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 12;
	I->score = 1;
	I->brick_x = 15;
	I->brick_y = 14;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 12;
	I->score = 1;
	I->brick_x = 16;
	I->brick_y = 14;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_GRENADE;
	I->WP = 13;
	I->score = 1;
	I->brick_x = 16;
	I->brick_y = 28;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 12;
	I->score = 1;
	I->brick_x = 11;
	I->brick_y = 23;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 12;
	I->score = 1;
	I->brick_x = 12;
	I->brick_y = 23;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 12;
	I->score = 1;
	I->brick_x = 13;
	I->brick_y = 23;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 17;
	I->score = 1;
	I->brick_x = 14;
	I->brick_y = 23;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_RAIL;
	I->WP = 33;
	I->score = 10;
	I->brick_x = 18;
	I->brick_y = 10;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_YELLOW_ARMOR;
	I->WP = 20;
	I->score = 35;
	I->brick_x = 16;
	I->brick_y = 7;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_RESPAWN;
	I->WP = 21;
	I->score = 10;
	I->brick_x = 1;
	I->brick_y = 18;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_RESPAWN;
	I->WP = 23;
	I->score = 10;
	I->brick_x = 13;
	I->brick_y = 28;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_RESPAWN;
	I->WP = 28;
	I->score = 8;
	I->brick_x = 8;
	I->brick_y = 10;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_RESPAWN;
	I->WP = 28;
	I->score = 8;
	I->brick_x = 17;
	I->brick_y = 5;
	Items.AddBottom(I);

	I = new CItem;
	I->type = IT_RESPAWN;
	I->WP = 28;
	I->score = 8;
	I->brick_x = 18;
	I->brick_y = 18;
	Items.AddBottom(I);

	//WARNINIG!!! THIS FUNCTION MUST BE CALED AFTER ALL WayPoints INITIALISATION
	//PutMapInWPs();
}
