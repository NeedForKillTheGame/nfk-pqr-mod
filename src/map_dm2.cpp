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

void CMap::LoadMap_dm2()
{
	fnP_DLL_AddMessage("Loading map DM2...");
	
	number_WPs = 37;
	WPs = new CWayPoint[37];

	Items.DeleteAll();
	CItem* I;

	WPs[0].number = 0; //RA
	WPs[0].x = 4*32;
	WPs[0].y = 28*16-30;
	WPs[0].x2 = 4*32+32;
	WPs[0].y2 = 28*16+16;
	WPs[0].number_OutWPs = 1;
	WPs[0].OutWPs = new BYTE;
	WPs[0].OutWPsTime = new int;
	WPs[0].OutWPs[0] = 30;
	WPs[0].OutWPsTime[0] = 2;

	I = new CItem;
	I->type = IT_RED_ARMOR;
	I->WP = 0;
	I->score = 70;
	I->brick_x = 4;
	I->brick_y = 28;
	Items.AddBottom(I);

	WPs[1].number = 1; //Над RA
	WPs[1].x = 3*32;
	WPs[1].y = 23*16-6;
	WPs[1].x2 = 3*32+32;
	WPs[1].y2 = 23*16+17;
	WPs[1].number_OutWPs = 4;
	WPs[1].OutWPs = new BYTE[4];
	WPs[1].OutWPsTime = new int[4];
	WPs[1].OutWPs[0] = 30; //30
	WPs[1].OutWPsTime[0] = 5;
	WPs[1].OutWPs[1] = 31;
	WPs[1].OutWPsTime[1] = 2;
	WPs[1].OutWPs[2] = 3;
	WPs[1].OutWPsTime[2] = 6;
	WPs[1].OutWPs[3] = 22;
	WPs[1].OutWPsTime[3] = 4;


	WPs[2].number = 2; //DOWN RL
	WPs[2].x = 8*32;
	WPs[2].y = 21*16-16;
	WPs[2].x2 = 8*32+32;
	WPs[2].y2 = 21*16+16;
	WPs[2].number_OutWPs = 2;
	WPs[2].OutWPs = new BYTE[2];
	WPs[2].OutWPsTime = new int[2];
	WPs[2].OutWPs[0] = 31;
	WPs[2].OutWPsTime[0] = 3;
	WPs[2].OutWPs[1] = 3;
	WPs[2].OutWPsTime[1] = 7;

	I = new CItem;
	I->type = IT_ROCKET;
	I->WP = 2;
	I->score = 20;
	I->brick_x = 8;
	I->brick_y = 21;
	Items.AddBottom(I);

	WPs[3].number = 3; //under down rl HEALTH +25
	WPs[3].x = 7*32;
	WPs[3].y = 28*16-16;
	WPs[3].x2 = 7*32+32;
	WPs[3].y2 = 28*16+16;
	WPs[3].number_OutWPs = 3;
	WPs[3].OutWPs = new BYTE[3];
	WPs[3].OutWPsTime = new int[3];
	WPs[3].OutWPs[0] = 1;
	WPs[3].OutWPsTime[0] = 6;
	WPs[3].OutWPs[1] = 4;
	WPs[3].OutWPsTime[1] = 7;
	WPs[3].OutWPs[2] = 31;
	WPs[3].OutWPsTime[2] = 3;

	I = new CItem;
	I->type = IT_HEALTH_25;
	I->WP = 3;
	I->score = 10;
	I->brick_x = 7;
	I->brick_y = 28;
	Items.AddBottom(I);

	WPs[4].number = 4; //left over shotgun HEALTH +25
	WPs[4].x = 14*32;
	WPs[4].y = 26*16-16;
	WPs[4].x2 = 14*32+32;
	WPs[4].y2 = 26*16+16;
	WPs[4].number_OutWPs = 4;
	WPs[4].OutWPs = new BYTE[4];
	WPs[4].OutWPsTime = new int[4];
	WPs[4].OutWPs[0] = 3;
	WPs[4].OutWPsTime[0] = 7;
	WPs[4].OutWPs[1] = 5;
	WPs[4].OutWPsTime[1] = 4;
	WPs[4].OutWPs[2] = 28;
	WPs[4].OutWPsTime[2] = 5;
	WPs[4].OutWPs[3] = 14;
	WPs[4].OutWPsTime[3] = 9;

	I = new CItem;
	I->type = IT_HEALTH_25;
	I->WP = 4;
	I->score = 10;
	I->brick_x = 14;
	I->brick_y = 26;
	Items.AddBottom(I);

	WPs[5].number = 5; //SHOTGUN
	WPs[5].x = 18*32;
	WPs[5].y = 28*16-16;
	WPs[5].x2 = 18*32+32;
	WPs[5].y2 = 28*16+16;
	WPs[5].number_OutWPs = 1;
	WPs[5].OutWPs = new BYTE;
	WPs[5].OutWPsTime = new int;
	WPs[5].OutWPs[0] = 4;
	WPs[5].OutWPsTime[0] = 4;

	I = new CItem;
	I->type = IT_SHOTGUN;
	I->WP = 5;
	I->score = 12;
	I->brick_x = 18;
	I->brick_y = 28;
	Items.AddBottom(I);
	
	WPs[6].number = 6; //right over right respawn AMMO_SHAFT
	WPs[6].x = 18*32;
	WPs[6].y = 22*16-16;
	WPs[6].x2 = 18*32+32;
	WPs[6].y2 = 22*16+16;
	WPs[6].number_OutWPs = 1;
	WPs[6].OutWPs = new BYTE;
	WPs[6].OutWPsTime = new int;
	WPs[6].OutWPs[0] = 28;
	WPs[6].OutWPsTime[0] = 1;

	I = new CItem;
	I->type = IT_AMMO_SHAFT;
	I->WP = 6;
	I->score = 5;
	I->brick_x = 18;
	I->brick_y = 22;
	Items.AddBottom(I);

	WPs[7].number = 7; //before up rl HEALTH +25
	WPs[7].x = 16*32+10;
	WPs[7].y = 13*16-8;
	WPs[7].x2 = 16*32+32;
	WPs[7].y2 = 13*16+16;
	WPs[7].number_OutWPs = 4;
	WPs[7].OutWPs = new BYTE[4];
	WPs[7].OutWPsTime = new int[4];
	WPs[7].OutWPs[0] = 28;
	WPs[7].OutWPsTime[0] = 7;
	WPs[7].OutWPs[1] = 8;
	WPs[7].OutWPsTime[1] = 4;
	WPs[7].OutWPs[2] = 9;
	WPs[7].OutWPsTime[2] = 2;
	WPs[7].OutWPs[3] = 21;
	WPs[7].OutWPsTime[3] = 6;

	I = new CItem;
	I->type = IT_HEALTH_25;
	I->WP = 7;
	I->score = 10;
	I->brick_x = 16;
	I->brick_y = 23;
	Items.AddBottom(I);

	WPs[8].number = 8; //UP RL
	WPs[8].x = 18*32;
	WPs[8].y = 10*16-16;
	WPs[8].x2 = 18*32+32;
	WPs[8].y2 = 10*16+16;
	WPs[8].number_OutWPs = 3;
	WPs[8].OutWPs = new BYTE[3];
	WPs[8].OutWPsTime = new int[3];
	WPs[8].OutWPs[0] = 7;
	WPs[8].OutWPsTime[0] = 4;
	WPs[8].OutWPs[1] = 9;
	WPs[8].OutWPsTime[1] = 3;
	WPs[8].OutWPs[2] = 28;
	WPs[8].OutWPsTime[2] = 12;

	I = new CItem;
	I->type = IT_ROCKET;
	I->WP = 8;
	I->score = 20;
	I->brick_x = 18;
	I->brick_y = 10;
	Items.AddBottom(I);

	WPs[9].number = 9; //SHARD 1 (right)
	WPs[9].x = 15*32;
	WPs[9].y = 12*16-16;
	WPs[9].x2 = 15*32+32;
	WPs[9].y2 = 12*16+16;
	WPs[9].number_OutWPs = 4;
	WPs[9].OutWPs = new BYTE[4];
	WPs[9].OutWPsTime = new int[4];
	WPs[9].OutWPs[0] = 8;
	WPs[9].OutWPsTime[0] = 3;
	WPs[9].OutWPs[1] = 7;
	WPs[9].OutWPsTime[1] = 2;
	WPs[9].OutWPs[2] = 36;
	WPs[9].OutWPsTime[2] = 3;
	WPs[9].OutWPs[3] = 10;
	WPs[9].OutWPsTime[3] = 1;

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 9;
	I->score = 2;
	I->brick_x = 15;
	I->brick_y = 12;
	Items.AddBottom(I);

	WPs[10].number = 10; //SHARD 2 (right-center)
	WPs[10].x = 14*32;
	WPs[10].y = 12*16-16;
	WPs[10].x2 = 14*32+32;
	WPs[10].y2 = 12*16+16;
	WPs[10].number_OutWPs = 2;
	WPs[10].OutWPs = new BYTE[2];
	WPs[10].OutWPsTime = new int[2];
	WPs[10].OutWPs[0] = 9;
	WPs[10].OutWPsTime[0] = 1;
	WPs[10].OutWPs[1] = 11;
	WPs[10].OutWPsTime[1] = 1;

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 10;
	I->score = 1;
	I->brick_x = 14;
	I->brick_y = 12;
	Items.AddBottom(I);

	WPs[11].number = 11; //SHARD 3 (left-center)
	WPs[11].x = 13*32;
	WPs[11].y = 12*16-16;
	WPs[11].x2 = 13*32+32;
	WPs[11].y2 = 12*16+16;
	WPs[11].number_OutWPs = 2;
	WPs[11].OutWPs = new BYTE[2];
	WPs[11].OutWPsTime = new int[2];
	WPs[11].OutWPs[0] = 10;
	WPs[11].OutWPsTime[0] = 1;
	WPs[11].OutWPs[1] = 12;
	WPs[11].OutWPsTime[1] = 1;

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 11;
	I->score = 1;
	I->brick_x = 13;
	I->brick_y = 12;
	Items.AddBottom(I);

	WPs[12].number = 12; //SHARD 4 (left)
	WPs[12].x = 12*32;
	WPs[12].y = 12*16-16;
	WPs[12].x2 = 12*32+32;
	WPs[12].y2 = 12*16+16;
	WPs[12].number_OutWPs = 3;
	WPs[12].OutWPs = new BYTE[3];
	WPs[12].OutWPsTime = new int[3];
	WPs[12].OutWPs[0] = 11;
	WPs[12].OutWPsTime[0] = 1;
	WPs[12].OutWPs[1] = 13;
	WPs[12].OutWPsTime[1] = 1;
	WPs[12].OutWPs[2] = 14;
	WPs[12].OutWPsTime[2] = 3; //искуственное занижение времени пути для увеличения приоритета ветки

	I = new CItem;
	I->type = IT_SHARD;
	I->WP = 12;
	I->score = 1;
	I->brick_x = 12;
	I->brick_y = 12;
	Items.AddBottom(I);
	
	WPs[13].number = 13; //AMMO_SHOTGUN
	WPs[13].x = 11*32;
	WPs[13].y = 13*16-32;
	WPs[13].x2 = 11*32+32;
	WPs[13].y2 = 13*16+16;
	WPs[13].number_OutWPs = 2;
	WPs[13].OutWPs = new BYTE[2];
	WPs[13].OutWPsTime = new int[2];
	WPs[13].OutWPs[0] = 12;
	WPs[13].OutWPsTime[0] = 1;
	WPs[13].OutWPs[1] = 14;
	WPs[13].OutWPsTime[1] = 4;

	I = new CItem;
	I->type = IT_AMMO_SHOTGUN;
	I->WP = 13;
	I->score = 1;
	I->brick_x = 11;
	I->brick_y = 13;
	Items.AddBottom(I);

	WPs[14].number = 14; //CENTER
	WPs[14].x = 8*32;
	WPs[14].y = 17*16-32;
	WPs[14].x2 = 8*32+32;
	WPs[14].y2 = 17*16+16;
	WPs[14].number_OutWPs = 3;
	WPs[14].OutWPs = new BYTE[3];
	WPs[14].OutWPsTime = new int[3];
	WPs[14].OutWPs[0] = 13;
	WPs[14].OutWPsTime[0] = 4;
	WPs[14].OutWPs[1] = 5;
	WPs[14].OutWPsTime[1] = 18;
	WPs[14].OutWPs[2] = 15;
	WPs[14].OutWPsTime[2] = 4;

	WPs[15].number = 15; //under health +50
	WPs[15].x = 5*32;
	WPs[15].y = 15*16-16;
	WPs[15].x2 = 5*32+32;
	WPs[15].y2 = 15*16+16;
	WPs[15].number_OutWPs = 5;
	WPs[15].OutWPs = new BYTE[5];
	WPs[15].OutWPsTime = new int[5];
	WPs[15].OutWPs[0] = 14;
	WPs[15].OutWPsTime[0] = 4;
	WPs[15].OutWPs[1] = 32;
	WPs[15].OutWPsTime[1] = 4;
	WPs[15].OutWPs[2] = 23;
	WPs[15].OutWPsTime[2] = 1;
	WPs[15].OutWPs[3] = 1;
	WPs[15].OutWPsTime[3] = 9;
	WPs[15].OutWPs[4] = 24;
	WPs[15].OutWPsTime[4] = 5;

	WPs[16].number = 16; //HEALTH +50
	WPs[16].x = 6*32;
	WPs[16].y = 9*16-16;
	WPs[16].x2 = 6*32+32;
	WPs[16].y2 = 9*16+16;
	WPs[16].number_OutWPs = 1;
	WPs[16].OutWPs = new BYTE;
	WPs[16].OutWPsTime = new int;
	WPs[16].OutWPs[0] = 15;
	WPs[16].OutWPsTime[0] = 5;

	I = new CItem;
	I->type = IT_HEALTH_50;
	I->WP = 16;
	I->score = 20;
	I->brick_x = 6;
	I->brick_y = 10;
	Items.AddBottom(I);

	WPs[17].number = 17; //MEGA
	WPs[17].x = 3*32;
	WPs[17].y = 5*16-16;
	WPs[17].x2 = 3*32+16;
	WPs[17].y2 = 5*16+32;
	WPs[17].number_OutWPs = 2;
	WPs[17].OutWPs = new BYTE[2];
	WPs[17].OutWPsTime = new int[2];
	WPs[17].OutWPs[0] = 18;
	WPs[17].OutWPsTime[0] = 6;
	WPs[17].OutWPs[1] = 25;
	WPs[17].OutWPsTime[1] = 1;

	I = new CItem;
	I->type = IT_HEALTH_100;
	I->WP = 17;
	I->score = 30;
	I->brick_x = 3;
	I->brick_y = 5;
	Items.AddBottom(I);

	WPs[18].number = 18; //Над шафтом
	WPs[18].x = 7*32;
	WPs[18].y = 5*16-16*3;
	WPs[18].x2 = 7*32+32;
	WPs[18].y2 = 5*16+16;
	WPs[18].number_items = 1;
	WPs[18].items = new BYTE;
	WPs[18].items[0] = IT_SHAFT;
	WPs[18].number_OutWPs = 3;
	WPs[18].OutWPs = new BYTE[3];
	WPs[18].OutWPsTime = new int[3];
	WPs[18].OutWPs[0] = 33;
	WPs[18].OutWPsTime[0] = 1;
	WPs[18].OutWPs[1] = 19;
	WPs[18].OutWPsTime[1] = 5;
	WPs[18].OutWPs[2] = 17;
	WPs[18].OutWPsTime[2] = 6;

	I = new CItem;
	I->type = IT_SHAFT;
	I->WP = 33;
	I->score = 6;
	I->brick_x = 7;
	I->brick_y = 5;
	Items.AddBottom(I);

	WPs[19].number = 19; //between shaft and ya
	WPs[19].x = 12*32+16;
	WPs[19].y = 5*16-16;
	WPs[19].x2 = 12*32+42;
	WPs[19].y2 = 5*16+16;
	WPs[19].number_OutWPs = 3;
	WPs[19].OutWPs = new BYTE[3];
	WPs[19].OutWPsTime = new int[3];
	WPs[19].OutWPs[0] = 18;
	WPs[19].OutWPsTime[0] = 5;
	WPs[19].OutWPs[1] = 35;
	WPs[19].OutWPsTime[1] = 2;
	WPs[19].OutWPs[2] = 9;
	WPs[19].OutWPsTime[2] = 5;

	WPs[20].number = 20; //YA
	WPs[20].x = 17*32;
	WPs[20].y = 4*16-32;
	WPs[20].x2 = 17*32+32;
	WPs[20].y2 = 4*16;
	WPs[20].number_OutWPs = 2;
	WPs[20].OutWPs = new BYTE[2];
	WPs[20].OutWPsTime = new int[2];
	WPs[20].OutWPs[0] = 9;
	WPs[20].OutWPsTime[0] = 7;
	WPs[20].OutWPs[1] = 34;
	WPs[20].OutWPsTime[1] = 3;

	I = new CItem;
	I->type = IT_YELLOW_ARMOR;
	I->WP = 20;
	I->score = 35;
	I->brick_x = 17;
	I->brick_y = 4;
	Items.AddBottom(I);

	WPs[21].number = 21; //RESPAWN before ya
	WPs[21].x = 14*32-16;
	WPs[21].y = 8*16-16;
	WPs[21].x2 = 14*32+16;
	WPs[21].y2 = 8*16+16;
	WPs[21].number_OutWPs = 4;
	WPs[21].OutWPs = new BYTE[4];
	WPs[21].OutWPsTime = new int[4];
	WPs[21].OutWPs[0] = 18;
	WPs[21].OutWPsTime[0] = 8;
	WPs[21].OutWPs[1] = 20;
	WPs[21].OutWPsTime[1] = 5;
	WPs[21].OutWPs[2] = 9;
	WPs[21].OutWPsTime[2] = 4;
	WPs[21].OutWPs[3] = 7;
	WPs[21].OutWPsTime[3] = 6;

	I = new CItem;
	I->type = IT_RESPAWN;
	I->WP = 21;
	I->score = 10;
	I->brick_x = 14;
	I->brick_y = 8;
	Items.AddBottom(I);

	WPs[22].number = 22; //Первая ступенька (слева) от RA наверх
	WPs[22].x = 2*32;
	WPs[22].y = 20*16-16;
	WPs[22].x2 = 2*32+32;
	WPs[22].y2 = 20*16+16;
	WPs[22].number_OutWPs = 3;
	WPs[22].OutWPs = new BYTE[3];
	WPs[22].OutWPsTime = new int[3];
	WPs[22].OutWPs[0] = 1;
	WPs[22].OutWPsTime[0] = 4;
	WPs[22].OutWPs[1] = 23;
	WPs[22].OutWPsTime[1] = 4;
	WPs[22].OutWPs[2] = 15;
	WPs[22].OutWPsTime[2] = 5;

	WPs[23].number = 23; //LEFT RESPAWN вторая ступенька (справа) от RA наверх
	WPs[23].x = 4*32;
	WPs[23].y = 16*16-32;
	WPs[23].x2 = 4*32+32;
	WPs[23].y2 = 16*16+16;
	WPs[23].number_OutWPs = 3;
	WPs[23].OutWPs = new BYTE[3];
	WPs[23].OutWPsTime = new int[3];
	WPs[23].OutWPs[0] = 15;
	WPs[23].OutWPsTime[0] = 1;
	WPs[23].OutWPs[1] = 1;
	WPs[23].OutWPsTime[1] = 5;
	WPs[23].OutWPs[2] = 24;
	WPs[23].OutWPsTime[2] = 5;

	I = new CItem;
	I->type = IT_RESPAWN;
	I->WP = 23;
	I->score = 10;
	I->brick_x = 4;
	I->brick_y = 16;
	Items.AddBottom(I);

	WPs[24].number = 24;
	WPs[24].x = 1*32;
	WPs[24].y = 11*16-32;
	WPs[24].x2 = 1*32+32;
	WPs[24].y2 = 11*16+16;
	WPs[24].number_OutWPs = 3;
	WPs[24].OutWPs = new BYTE[3];
	WPs[24].OutWPsTime = new int[3];
	WPs[24].OutWPs[0] = 25;
	WPs[24].OutWPsTime[0] = 6;
	WPs[24].OutWPs[1] = 15;
	WPs[24].OutWPsTime[1] = 5;
	WPs[24].OutWPs[2] = 23;
	WPs[24].OutWPsTime[2] = 5;

	WPs[25].number = 25;
	WPs[25].x = 1*32;
	WPs[25].y = 4*16-16;
	WPs[25].x2 = 1*32+32;
	WPs[25].y2 = 4*16+16;
	WPs[25].number_OutWPs = 2;
	WPs[25].OutWPs = new BYTE[2];
	WPs[25].OutWPsTime = new int[2];
	WPs[25].OutWPs[0] = 17;
	WPs[25].OutWPsTime[0] = 1;
	WPs[25].OutWPs[1] = 26;
	WPs[25].OutWPsTime[1] = 3;

	WPs[26].number = 26;
	WPs[26].x = 1*32;
	WPs[26].y = 8*16-16;
	WPs[26].x2 = 1*32+32;
	WPs[26].y2 = 8*16+16;
	WPs[26].number_OutWPs = 2;
	WPs[26].OutWPs = new BYTE[2];
	WPs[26].OutWPsTime = new int[2];
	WPs[26].OutWPs[0] = 15;
	WPs[26].OutWPsTime[0] = 7;
	WPs[26].OutWPs[1] = 23;
	WPs[26].OutWPsTime[1] = 7;

	WPs[27].number = 27; //Клетка справа за стеной от RL. Чтобы в этом месте ближайшим вейпоинтом определялся не RL, а эта клетка
	WPs[27].x = 10*32;
	WPs[27].y = 20*16-16;
	WPs[27].x2 = 10*32+32;
	WPs[27].y2 = 20*16+16;
	WPs[27].number_OutWPs = 2;
	WPs[27].OutWPs = new BYTE[2];
	WPs[27].OutWPsTime = new int[2];
	WPs[27].OutWPs[0] = 14;
	WPs[27].OutWPsTime[0] = 3;
	WPs[27].OutWPs[1] = 4;
	WPs[27].OutWPsTime[1] = 4;

	WPs[28].number = 28; //RIGHT RESPAWN
	WPs[28].x = 17*32;
	WPs[28].y = 22*16-16;
	WPs[28].x2 = 17*32+32;
	WPs[28].y2 = 22*16+16;
	WPs[28].number_OutWPs = 4;
	WPs[28].OutWPs = new BYTE[4];
	WPs[28].OutWPsTime = new int[4];
	WPs[28].OutWPs[0] = 4;
	WPs[28].OutWPsTime[0] = 5;
	WPs[28].OutWPs[1] = 29;
	WPs[28].OutWPsTime[1] = 7;
	WPs[28].OutWPs[2] = 6;
	WPs[28].OutWPsTime[2] = 1;
	WPs[28].OutWPs[3] = 14;
	WPs[28].OutWPsTime[3] = 9;

	I = new CItem;
	I->type = IT_RESPAWN;
	I->WP = 28;
	I->score = 8;
	I->brick_x = 17;
	I->brick_y = 22;
	Items.AddBottom(I);

	WPs[29].number = 29; //UP OF shaft ammo
	WPs[29].x = 18*32;
	WPs[29].y = 18*16-16;
	WPs[29].x2 = 18*32+32;
	WPs[29].y2 = 18*16+16;
	WPs[29].number_OutWPs = 2;
	WPs[29].OutWPs = new BYTE[2];
	WPs[29].OutWPsTime = new int[2];
	WPs[29].OutWPs[0] = 7;
	WPs[29].OutWPsTime[0] = 4;
	WPs[29].OutWPs[1] = 28;
	WPs[29].OutWPsTime[1] = 4;

	WPs[30].number = 30; //left of RA
	WPs[30].x = 2*32-48;
	WPs[30].y = 25*16;
	WPs[30].x2 = 2*32+16;
	WPs[30].y2 = 25*16+16;
	WPs[30].number_OutWPs = 2;
	WPs[30].OutWPs = new BYTE[2];
	WPs[30].OutWPsTime = new int[2];
	WPs[30].OutWPs[0] = 0;
	WPs[30].OutWPsTime[0] = 2;
	WPs[30].OutWPs[1] = 1;
	WPs[30].OutWPsTime[1] = 5;

	WPs[31].number = 31; //Перед прыжком на RL
	WPs[31].x = 5*32;
	WPs[31].y = 24*16-16;
	WPs[31].x2 = 5*32+32;
	WPs[31].y2 = 24*16+16;
	WPs[31].number_OutWPs = 2;
	WPs[31].OutWPs = new BYTE[2];
	WPs[31].OutWPsTime = new int[2];
	WPs[31].OutWPs[0] = 1;
	WPs[31].OutWPsTime[0] = 2;
	WPs[31].OutWPs[1] = 2;
	WPs[31].OutWPsTime[1] = 3;

	WPs[32].number = 32; //Перед прыжком +50
	WPs[32].x = 5*32;
	WPs[32].y = 11*16-16;
	WPs[32].x2 = 5*32+32;
	WPs[32].y2 = 11*16+16;
	WPs[32].number_OutWPs = 1;
	WPs[32].OutWPs = new BYTE[1];
	WPs[32].OutWPsTime = new int[1];
	WPs[32].OutWPs[0] = 16;
	WPs[32].OutWPsTime[0] = 1;

	WPs[33].number = 33; //SHAFT
	WPs[33].x = 7*32;
	WPs[33].y = 5*16-16;
	WPs[33].x2 = 7*32+32;
	WPs[33].y2 = 5*16+16;
	WPs[33].number_OutWPs = 1;
	WPs[33].OutWPs = new BYTE;
	WPs[33].OutWPsTime = new int;
	WPs[33].OutWPs[0] = 18;
	WPs[33].OutWPsTime[0] = 1;

	WPs[34].number = 34; //From ya before jump to shaft
	WPs[34].x = 16*32-10;
	WPs[34].y = 4*16-16;
	WPs[34].x2 = 16*32+8;
	WPs[34].y2 = 4*16+16;
	WPs[34].number_OutWPs = 1;
	WPs[34].OutWPs = new BYTE;
	WPs[34].OutWPsTime = new int;
	WPs[34].OutWPs[0] = 35;
	WPs[34].OutWPsTime[0] = 2;

	WPs[35].number = 35; //From ya extremum in jump to shaft
	WPs[35].x = 14*32;
	WPs[35].y = 3*16-32;
	WPs[35].x2 = 14*32+32;
	WPs[35].y2 = 3*16;
	WPs[35].number_OutWPs = 2;
	WPs[35].OutWPs = new BYTE[2];
	WPs[35].OutWPsTime = new int[2];
	WPs[35].OutWPs[0] = 19;
	WPs[35].OutWPsTime[0] = 2;
	WPs[35].OutWPs[1] = 20;
	WPs[35].OutWPsTime[1] = 5;

	WPs[36].number = 36; //прыжок на респаун перед YA с полки где лежат шарды
	WPs[36].x = 15*32;
	WPs[36].y = 9*16-8;
	WPs[36].x2 = 15*32+48;
	WPs[36].y2 = 9*16+16;
	WPs[36].number_OutWPs = 1;
	WPs[36].OutWPs = new BYTE;
	WPs[36].OutWPsTime = new int;
	WPs[36].OutWPs[0] = 21;
	WPs[36].OutWPsTime[0] = 1;

	//WARNINIG!!! THIS FUNCTION MUST BE CALED AFTER ALL WayPoints INITIALISATION
	PutMapInWPs();
}
