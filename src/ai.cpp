#include "ai.h"

#include "debug_utils.h"

void CMD_Addbot(CList* Params)
{
	char* name = "Thresh";
	char* model = "sarge+blue";
	Params->MoveTop();
	if (Params->MoveNext()) {
		if (strlen((char*)Params->GetCur())<=30 && strlen((char*)Params->GetCur())>0) strcpy(name, (char*)Params->GetCur());
		if (Params->MoveNext())
			if (strlen((char*)Params->GetCur())<=30 && strlen((char*)Params->GetCur())>0) strcpy(model, (char*)Params->GetCur());
	}
	fnP_DLL_sys_CreatePlayer(name,model,0);
}
void CMD_Removebot()
{
	if (m_Bots.IsEmpty()) return;
	m_Bots.MoveBottom();
	if (m_Bots.GetCur()!=NULL) fnP_DLL_RemoveBot(((CBot*)m_Bots.GetCur())->GetDXID());
	free(m_Bots.GetCur());
	m_Bots.Delete();
}

void CMD_ShowPath(CList* Params)
{
	if (m_Bots.IsEmpty()) return;

	Params->MoveTop();
	bool show_variant_path = false;
	if (Params->MoveNext()) show_variant_path = true;

	m_Bots.MoveTop();
	do {
		((CBot*)m_Bots.GetCur())->ShowPath();
		if (show_variant_path) ((CBot*)m_Bots.GetCur())->ShowVariantPath();

	}
	while (m_Bots.MoveNext());
}

void CMD_HidePath()
{
	if (m_Bots.IsEmpty()) return;
	m_Bots.MoveTop();
	do ((CBot*)m_Bots.GetCur())->HidePath();
	while (m_Bots.MoveNext());
}



bool AI_EVENT_ConsoleCommand(CList* _Params)
{
	if (!strcmp((char*)_Params->GetCur(),"addbot")) CMD_Addbot(_Params);
	else
	if (!strcmp((char*)_Params->GetCur(),"removebot")) CMD_Removebot();
	else
	if (!strcmp((char*)_Params->GetCur(),"b_show_path")) CMD_ShowPath(_Params);
	else
	if (!strcmp((char*)_Params->GetCur(),"b_hide_path")) CMD_HidePath();
	else return false;
	return true; //Возвращаем true, чтобы основной обработчик консольных команд EVENT_ConsoleCommand понял, что команда обработана модулем time_help
}

void AI_RegisterConsoleCommands()
{
	fnP_DLL_RegisterConsoleCommand("addbot");
	fnP_DLL_RegisterConsoleCommand("removebot");

	fnP_DLL_RegisterConsoleCommand("b_show_path");
	fnP_DLL_RegisterConsoleCommand("b_hide_path");
}

void AI_ConfigSave(FILE* _f)
{
}

bool AI_ConfigLoad(CList* __Params)
{
	return AI_EVENT_ConsoleCommand(__Params);
}


CBot :: CBot(TPlayerEx player) : CPlayer(player)
{
	ai_reset();
};

void CBot :: ai_think()
{
	if (isStoped) return;
	if (dead) {
		BLOC_goal_score = true;
		m_Map->Items.MoveTop();
		return;
	}
	else
	if (m_Map==NULL) {
		fnP_DLL_AddMessage("^1ERROR: ^2(ai_think) ^7 Map not loaded. m_Map == NULL");
		isStoped = true;
		return;
	}
	else if (m_Map->number_WPs==0) {
		fnP_DLL_AddMessage("^1ERROR: ^2(ai_think) ^7 Map have not WP's. m_Map->number_WPs==0");
		isStoped = true;
		return;
	}
	else if (m_Map->Items.IsEmpty()) {
		fnP_DLL_AddMessage("^1ERROR: ^2(ai_think) ^7 Map Items list is empty. m_Map->Items.IsEmpty");
		isStoped = true;
		return;
	}

	if (time_before_select_item<=0) BLOC_select_bloc = true;
	
	if (BLOC_select_bloc) {
		if ((!BLOC_find_best_item) && (!BLOC_goal_score)) {//Запустить подсчёт goal score, если (нет текущего пути или закончилось время ожидания перед подсчётом goal score) и не выполняется блок выбора лучшего итема, 

			//DEBUG PRINT
			//fnP_DLL_AddMessage("^2ai_think: ^7BLOC_select_bloc");


			BLOC_goal_score = true;
			m_Map->Items.MoveTop();
			BLOC_GS_Current_item = m_Map->Items.GetCur();
		}
		BLOC_select_bloc = false;
	}
		
	if (BLOC_goal_score) {

		//DEBUG PRINT
		//fnP_DLL_AddMessage("^2ai_think: ^7BLOC_goal_score");


		BLOC_find_best_item = false;
		//Проходим все итемы за несколько вызовов ai_think();
		if (!m_Map->Items.MoveTo(BLOC_GS_Current_item)) {
			fnP_DLL_AddMessage("^1ERROR: ^2(CBot::ai_think) Can't set current element in list of elements");
		}
		ai_goal_score((CItem*)m_Map->Items.GetCur()); //Считаем goal score для текущего итема из записываем в Item.cur_score
		if (!m_Map->Items.MoveNext()) {
			BLOC_find_best_item = true; //Закончили перебор итемов с целью подсчёта goal score
			BLOC_goal_score = false;
		}
		else BLOC_GS_Current_item = m_Map->Items.GetCur();
	}
	else time_before_select_item--;
	
	if (BLOC_find_best_item) { //Закончили подсчёт goal score для всех предметов. Теперь выберем лучший.
		
		//DEBUG PRINT
		//fnP_DLL_AddMessage("^2ai_think: ^7BLOC_find_best_item");
		
		CItem* I = NULL;
		CItem* best_I = NULL;
		double max = 0 ;
		m_Map->Items.MoveTop();
		do { //Перебираем предметы в цикле с целью поиска ЛУЧШЕГО
			I = (CItem*)m_Map->Items.GetCur();
			if ((I->cur_score >= max) && I->availble) {
				max = I->cur_score;
				best_I = I;
				//DEBUG PRINT
				//fnP_DLL_AddMessageXY(I->cur_score,I->type,"I->cur_score,I->type");
			}
		}
		while (m_Map->Items.MoveNext());

		//DEBUG PRINT
		///*
		if (best_I == NULL) {
			fnP_DLL_AddMessage("^1ERROR: ^2(CBot::ai_think) ^7Cant find best item in BLOC_find_best_item");
			fnP_DLL_AddMessage("BLOC_find_best_item ^1terminated");
			BLOC_find_best_item = false;
			return;
		}//*/

		//DEBUG PRINT
		//fnP_DLL_AddMessageXY(best_I->x,best_I->y,"^2ai_think: ^7best_I->x; best_I->y");
		
		//Находим путь до выбранного ЛУЧШЕГО предмета
		CPath* NewPath = NULL;
		NewPath = m_Map->FindPath(x,y+19,best_I->WP);

		if (NewPath == NULL) {
			fnP_DLL_AddMessage("^1ERROR: ^2(CBot::ai_think) ^7 m_Map->FindPath returns NewPath == NULL");
			fnP_DLL_AddMessage("BLOC_find_best_item ^1terminated");
			BLOC_find_best_item = false;
			return;
		}
		else {
			//Проверим, может предмета уже нет и он появится слишком поздно?
			if (best_I->GetRespawnTime() >= NewPath->time*__CRITICAL_TIME_TO_MOVE__/50 + best_I->wait_time) best_I->availble = false;
			else {
				//Проверим, может текущий путь уже ведёт к этому итему?
				bool FLAG_set_new_path = false;
				if (CurPath != NULL)
					if (CurPath->WP!=NULL)
						if (!CurPath->WP->IsEmpty()) {
							CurPath->WP->MoveBottom();
							NewPath->WP->MoveBottom();
							if (*((int*)NewPath->WP->GetCur()) != *((int*)CurPath->WP->GetCur())) FLAG_set_new_path=true;
							else if (time_moving<=0) FLAG_set_new_path=true;
						}
						else FLAG_set_new_path = true;
					else FLAG_set_new_path = true;
				else FLAG_set_new_path = true;
					
				if (FLAG_set_new_path) {
					//Удаляем старый путь
					if (CurPath != NULL) free(CurPath);
	
					CurPath = NewPath;
					CurPath->WP->MoveTop(); //Устанавливаем указатель в списке пути на перый WP
				}
				else delete NewPath;

				BLOC_find_best_item = false; //Нашли путь, сделали его текущим, теперь не нужно выполнять этот блок.
				time_before_select_item = __TIME_BEFORE_SELECT_NEXT_ITEM__;
				time_move_think = 0;
	
				//DEBUG PRINT
				/*int c = 0;
				do{
					c++;
					fnP_DLL_AddMessageXY(c,*((int*)CurPath->WP->GetCur()),"WP");
				}
				while (CurPath->WP->MoveNext());
				
				isStoped = true;*/
				//END DEBUG PRINT
			}
		}
	} //BLOC_find_best_item


	if (CurPath!=NULL)
		if (CurPath->WP!=NULL)
			if (!CurPath->WP->IsEmpty())
			{

			  //DEBUG PRINT - SHOW PATH

			  if (DEBUG_show_path)
			  {
				int wx1,wy1,wx2,wy2,wpN;
				wx2 = m_Map->WPs[*((int*)CurPath->WP->GetCur())].GetCenterX();
				wy2 = m_Map->WPs[*((int*)CurPath->WP->GetCur())].GetCenterY();
				wpN = *((int*)CurPath->WP->GetCur());
				fnP_DLL_debug_textout_int(wx2,wy2,wpN);
				while(CurPath->WP->MoveNext())
				{
					wx1 = wx2;
					wy1 = wy2;
					wx2	= m_Map->WPs[*((int*)CurPath->WP->GetCur())].GetCenterX();
					wy2 = m_Map->WPs[*((int*)CurPath->WP->GetCur())].GetCenterY();
					wpN = *((int*)CurPath->WP->GetCur());
					draw_line(wx1,wy1,wx2,wy2,__DEBUG_show_path_CHAR__);
					fnP_DLL_debug_textout_int(wx2+16,wy2,wpN);
				}
			  }
			  //END DEBUG PRINT

				if (time_move_think<=0) {
					CurPath->WP->MoveTop();
					int *nowWP = (int*)CurPath->WP->GetCur();
					if (!ai_move_to_WP(*nowWP)) BLOC_select_bloc = true;
					if (ai_touchWP(*nowWP)) {
						CurPath->WP->Delete();
						if (!CurPath->WP->IsEmpty()) {
							time_moving = m_Map->GetTimeMoving(*nowWP,*((int*)CurPath->WP->GetCur()));
							time_moving *= __CRITICAL_TIME_TO_MOVE__;
						}
					}
					time_move_think = __TIME_MOVE_THINK__;
				}
				else time_move_think--;
				time_moving--;
				if (time_moving<=0)
					BLOC_select_bloc = true;
			
			}
			else BLOC_select_bloc = true;//CurPath->WP->IsEmpty()
		else BLOC_select_bloc = true; //CurPath->WP==NULL
	else BLOC_select_bloc = true; //CurPath==NULL
	/*
			else fnP_DLL_AddMessage("think3");
		else fnP_DLL_AddMessage("think2");
	else fnP_DLL_AddMessage("think1");*/

	ai_fire();
	fnP_DLL_SetKeys(DXID,CurKeys);
}

void CBot::ai_goal_score(CItem* I)
{
	if (I == NULL) {
		fnP_DLL_AddMessage("^1ERROR: ^2(CBot::ai_goal_score) ^7Incoming parametr CItem* I == NULL");
		return;
	}
	
	CPath* P = NULL;
	P = m_Map->FindPath(x,y+19,I->WP);
	if (P == NULL) {
		fnP_DLL_AddMessage("^1ERROR: ^2(CBot::ai_goal_score) ^7 Find Path returns P == NULL");
		I->cur_score = 0;
		return;
	}
	if (P->WP == NULL) {
		fnP_DLL_AddMessage("^1ERROR: ^2(CBot::ai_goal_score) ^7 Find Path returns P->WP == NULL");
		I->cur_score = 0;
		return;
	}
	if (P->WP->IsEmpty()) {
		fnP_DLL_AddMessage("^1ERROR: ^2(CBot::ai_goal_score) ^7 Find Path returns P->WP->IsEmpty() == true");
		I->cur_score = 0;
		return;
	}
	
	//DEBUG PRINT
	//fnP_DLL_AddMessageXY(I->type,I->WP,"^2ai_goal_score: ^7I->type , WP");

	I->availble = true;
	I->cur_score = I->score; //- P->time / 2;
	//if (I->cur_score<0) I->cur_score = 1;
	double k;
	switch(I->type) {
	case IT_RED_ARMOR: k = ((double) ((200 - (armor+100<200)?(armor+100):200) * (200-armor))/40000);
						if (k>0.01) k = sqrt(k);
						I->cur_score *= k * 100 / P->time;
		break;
	case IT_YELLOW_ARMOR: k = ((double) ((200 - (armor+50<200)?(armor+50):200) * (200-armor))/40000);
						if (k>0.01) k = sqrt(k);
						I->cur_score *= k * 100 / P->time;
		break;
	case IT_SHARD: k= ((double) ((200 - (armor+5<200)?(armor+5):200) * (200-armor))/40000);
					if (k>0.01) k = sqrt(k);
					I->cur_score *= k / (P->time*P->time);
		break;

	case IT_HEALTH_100: k = ((double) ((200 - (health+100<200)?(health+100):200) * (200-health))/10000);
						if (k>0.01) k = sqrt(k);
						I->cur_score *= k * 100/ P->time;
		break;
	case IT_HEALTH_50:	if (health>=100) {I->cur_score = -100; break;}
						k = ( (double) ((100 - (health+50<100)?(health+50):100) * (99-health))/10000);
						if (k>0.1) k = sqrt(k);
						I->cur_score *= k *100 / P->time;
		break;
	case IT_HEALTH_25: if (health>=100) {I->cur_score = -100; break;}
						k = ((double) ((100 - (health+25<100)?(health+25):100)) * (99-health)/10000);
						if (k>0.1) k = sqrt(k);
						I->cur_score *= k *100 / P->time;
		break;
	case IT_HEALTH_5: if (health>=100) {I->cur_score = -100; break;}
						k = ((double) ((100 - (health+5<100)?(health+5):100)) * (99-health)/10000);
						if (k>0.1) k = sqrt(k);
						I->cur_score *= k *100 / P->time;
		break;

	case IT_SHOTGUN: I->cur_score *= (have_ssg)?(((ammo_ssg<10)?(10-ammo_ssg):1)*(99-ammo_ssg)/1000):1;
		break;
	case IT_GRENADE: I->cur_score *= (have_gl)?(((ammo_gl<10)?(10-ammo_gl):1)*(99-ammo_gl)/1000):1;
		break;
	case IT_ROCKET: I->cur_score *= (have_rl)?(((ammo_rl<10)?(10-ammo_rl):1)*(99-ammo_rl)/1000):1;
		break;
	case IT_SHAFT: I->cur_score *= (have_lg)?(((ammo_lg<130)?(130-ammo_lg):1)*(199-ammo_lg)/26000):1;
		break;
	case IT_RAIL: I->cur_score *= (have_rg)?(((ammo_rg<10)?(10-ammo_rg):1)*(99-ammo_rg)/1000):1;
		break;
	case IT_PLASMA: I->cur_score *= (have_pg)?(((ammo_pg<50)?(50-ammo_pg):1)*(99-ammo_pg)/5000):1;
		break;
	case IT_BFG: I->cur_score *= (have_bfg)?(((ammo_bfg<10)?(10-ammo_bfg):1)*(99-ammo_bfg)/1000):1;
		break;

	case IT_AMMO_MACHINEGUN: if (ammo_mg>=200) {I->cur_score = -100; break;}
							I->cur_score *= ((ammo_mg<50)?(50-ammo_mg):1)/50;
							I->cur_score *= 100/P->time;
		break;
	case IT_AMMO_SHOTGUN: if (ammo_ssg>=100) {I->cur_score = -100; break;}
							I->cur_score *= (have_ssg)?(((ammo_ssg<10)?(10-ammo_ssg):1)/10):0.1;
							I->cur_score *= (99-ammo_ssg)/100;
							I->cur_score *= 100/P->time;
		break;
	case IT_AMMO_GRENADE: if (ammo_gl>=100) {I->cur_score = -100; break;}
							I->cur_score *= (have_gl)?(((ammo_gl<10)?(10-ammo_gl):1)/10):0.1;
							I->cur_score *= (99-ammo_gl)/100;
							I->cur_score *= 100/P->time;
		break;
	case IT_AMMO_ROCKET: if (ammo_rl>=100) {I->cur_score = -100; break;}
							I->cur_score *= (have_rl)?(((ammo_rl<10)?(10-ammo_rl):1)/10):0.1;
							I->cur_score *= (99-ammo_rl)/100;
							I->cur_score *= 100/P->time;
		break;
	case IT_AMMO_SHAFT: if (ammo_lg>=200) {I->cur_score = -100; break;}
							I->cur_score *= (have_lg)?(((ammo_lg<130)?(130-ammo_lg):1)/130):0.1;
							I->cur_score *= (199-ammo_lg)/200;
							I->cur_score *= 100/P->time;
		break;
	case IT_AMMO_RAIL: if (ammo_rg>=100) {I->cur_score = -100; break;}
							I->cur_score *= (have_rg)?(((ammo_rg<10)?(10-ammo_rg):1)/10):0.1;
							I->cur_score *= (99-ammo_rg)/100;
							I->cur_score *= 100/P->time;
		break;
	case IT_AMMO_PLASMA: if (ammo_pg>=100) {I->cur_score = -100; break;}
							I->cur_score *= (have_pg)?(((ammo_pg<50)?(50-ammo_pg):1)/50):0.1;
							I->cur_score *= (99-ammo_pg)/100;
							I->cur_score *= 100/P->time;
		break;
	case IT_AMMO_BFG: if (ammo_bfg>=100) {I->cur_score = -100; break;}
							I->cur_score *= (have_bfg)?(((ammo_bfg<10)?(10-ammo_bfg):1)/10):0.1;
							I->cur_score *= (99-ammo_bfg)/100;
							I->cur_score *= 100/P->time;
		break;

	case IT_RESPAWN: I->cur_score = 0;
		break;
	}

	//DEBUG PRINT - SHOW PATH
	if (DEBUG_show_variant_path) {
		P->WP->MoveTop();
		int wx1,wy1,wx2,wy2;
		wx2 = m_Map->WPs[*((int*)P->WP->GetCur())].GetCenterX();
		wy2	= m_Map->WPs[*((int*)P->WP->GetCur())].GetCenterY();
		while(P->WP->MoveNext()) {
			wx1 = wx2;
			wy1 = wy2;
			wx2 = m_Map->WPs[*((int*)P->WP->GetCur())].GetCenterX();
			wy2 = m_Map->WPs[*((int*)P->WP->GetCur())].GetCenterY();
			draw_line(wx1,wy1,wx2,wy2,__DEBUG_show_variant_path_CHAR__);
		}
	}
	//END DEBUG PRINT

	//DEBUG PRINT
	//fnP_DLL_AddMessageXY(I->type,I->cur_score,"type score");
	//Освобождаем память.
	delete P;
}

bool CBot::ai_move_to_WP(int WP)
{
	//DEBUG PRINT
	//fnP_DLL_AddMessageXY(WP,0,"^2CBot::ai_move_to_WP: ^7moving to WP");

	if ((last_x == x)&&(last_y == y)) time_stoping++;
	else time_stoping = 0;
	if (time_stoping > __MAX_TIME_NOT_MOVE__) {time_stoping = 0;return false;}
	last_x = x;
	last_y = y;
	
	BYTE Keys = 0;
	TBrick* b = NULL;

	int y_bottom;
	if (crouch) y_bottom = y + 19;
	else y_bottom = y + 23;

	//DEBUG PRINT
	//fnP_DLL_AddMessageXY(x,m_Map->WPs[WP].x,"^2CBot::ai_move_to_WP: ^7Player.x ; WP.x");
	//fnP_DLL_AddMessageXY(y,m_Map->WPs[WP].y,"^2CBot::ai_move_to_WP: ^7Player.y ; WP.y");
	
	if (m_Map->WPs[WP].x2 < x) {
		Keys = BKEY_MOVELEFT;
		b = fnP_DLL_GetBrickStruct((x - 32)/32, (y_bottom-1)/16);

		//DEBUG IF
		//if (b != NULL)

		if (b->block) Keys = Keys | BKEY_MOVEUP;
		else {
			b = fnP_DLL_GetBrickStruct((x - 32)/32, (y_bottom-17)/16);
			if (b->block) Keys = Keys | BKEY_MOVEUP;
			else {
				b = fnP_DLL_GetBrickStruct((x - 32)/32, (y_bottom-33)/16);
				if (b->block) Keys = Keys | BKEY_MOVEUP;
				else {
				//	b = fnP_DLL_GetBrickStruct((x - 32)/32, ((y_bottom-49))/16);
				//	if (b->block) Keys = Keys | BKEY_MOVEUP;
				}
			}
		}
	}
	else if (x < m_Map->WPs[WP].x) {
		Keys = BKEY_MOVERIGHT;
		b = fnP_DLL_GetBrickStruct((x + 32)/32, (y_bottom-1)/16);

		//DEBUG IF
		//if (b != NULL)

		if (b->block) Keys = Keys | BKEY_MOVEUP;
		else {
			b = fnP_DLL_GetBrickStruct((x + 32)/32, (y_bottom-17)/16);
			if (b->block) Keys = Keys | BKEY_MOVEUP;
			else {
				b = fnP_DLL_GetBrickStruct((x + 32)/32, (y_bottom-33)/16);
				if (b->block) Keys = Keys | BKEY_MOVEUP;
				else {
				//	b = fnP_DLL_GetBrickStruct((x + 32)/32, ((y_bottom-49))/16);
				//	if (b->block) Keys = Keys | BKEY_MOVEUP;
				}
			}
		}
	}
	
	if ((y_bottom > m_Map->WPs[WP].y2)) Keys = Keys | BKEY_MOVEUP;

	if (!isOnGround()) Keys = Keys & (~BKEY_MOVEUP);
	//if (CurKeys!=Keys) {

		CurKeys=Keys;
	//}

	return true;
}

bool CBot::isOnGround()
{
	int y_bottom;
	if (crouch) y_bottom = y + 20;
	else y_bottom = y + 24;
	
	TBrick* b = NULL;
	b = fnP_DLL_GetBrickStruct((x-9)/32,(y_bottom+1)/16);
	if (b->block) return true;
	b = fnP_DLL_GetBrickStruct((x+9)/32,(y_bottom+1)/16);
	if (b->block) return true;
	return false;
}

bool CBot::ai_touchWP(int WP)
{
	//int y_bottom;
	//if (crouch) y_bottom = y + 20;
	//else y_bottom = y + 24;
	return m_Map->IsOnWP(x,y,WP);
}

void CBot::ai_reset()
{
	//DEBUG PRINT
	//fnP_DLL_AddMessage("^2CBot::ai_reset()");

	//if (CurPath != NULL) {
	//	free(CurPath);
	//}
	BLOC_GS_Current_item = NULL;
	BLOC_goal_score = true;
	BLOC_find_best_item = false;
	BLOC_select_bloc = false;
	time_before_select_item = 0;
	time_moving = 0;
	time_stoping = 0;
	time_fire = 0;
	isStoped = false;
	CurKeys = 0;
	DEBUG_show_path = false;
	DEBUG_show_variant_path = false;
	
}

void CBot::ShowPath()
{
	DEBUG_show_path = true;
}
void CBot::ShowVariantPath()
{
	DEBUG_show_variant_path = true;
}
void CBot::HidePath()
{
	DEBUG_show_path = false;
	DEBUG_show_variant_path = false;
}
void CBot::ai_fire()
{
/*
	if (!(CurKeys & BKEY_FIRE)) //Если не ведётся стрельба, то между проверками на возможность
		//пальнуть должна быть задержка, чтобы не тормозило
	{
		if (time_fire>0) {time_fire--;return;}
	}
	else //если стрельба велась*/
		CurKeys &= ~BKEY_FIRE; //Отжимаем кнопку стрельбы
		//И в переборе игроков, всё на том же игроке будет проверена TraceVector

	BYTE MoveKeys = 0;

	if (!m_Players.IsEmpty())
	{
		if (!AI_FIRE_select_enemy)
		{
			AI_FIRE_select_enemy=true;
			m_Players.MoveTop();
		}
		else
		{
			m_Players.MoveTo(AI_FIRE_Current_enemy);
		}
	
		CPlayer *P;
		int weapon_x,weapon_y;
		weapon_x = x;
		if (crouch) weapon_y = y+3;
		else weapon_y = y-5;
		
		P = (CPlayer*)m_Players.GetCur();
		if (!P->IsDead())
		{
			if (weapon_x!=P->GetX()||weapon_y!=P->GetY())
				if (!m_Map->TraceVector(weapon_x,weapon_y,P->GetX()-3,P->GetY()-4) || !m_Map->TraceVector(weapon_x,weapon_y,P->GetX()+3,P->GetY()+4))
					return;
			
			unsigned int range = sqrt((P->GetX()-weapon_x)*(P->GetX()-weapon_x)+(P->GetY()-weapon_y)*(P->GetY()-weapon_y));
			SetWeapon(range);	
			//fnP_DLL_SetWeapon(DXID,C_WPN_GAUNTLET);
			
			//DEBUG PRINT
			//fnP_DLL_AddMessage("fire");
			fnP_DLL_AddMessageX(weapchg,"weapchg ");

			double angle=atan2(P->GetX()-weapon_x,P->GetY()-weapon_y);
			//if (angle<0) angle+=M_PI;
			angle = M_PI - angle;
			if (angle<0) angle+=2*M_PI;

			int degree=angle*180/M_PI;
			//fnP_DLL_AddMessageX(degree,"degree ");
			fnP_DLL_SetAngle(DXID,degree);
			
			CurKeys |= BKEY_FIRE; //Зажимаем кнопку стрельбы

			//Повернёмбота в сторону игрока
			if ( P->GetX() < weapon_x) {CurKeys = CurKeys& (~BKEY_MOVERIGHT); CurKeys = CurKeys | BKEY_MOVELEFT;}
			else if( P->GetX() > weapon_x) {CurKeys = CurKeys | BKEY_MOVERIGHT; CurKeys = CurKeys& (~BKEY_MOVELEFT);}

				
		}
		if (!(CurKeys & BKEY_FIRE)) //Перебираем игроков для выстрела, когда не ведётся стрельба
		if (!m_Players.MoveNext())
		{
			AI_FIRE_select_enemy=false;
			time_fire=__TIME_BETWEN_FIRE__;
		}
		else AI_FIRE_Current_enemy = m_Players.GetCur();
		else AI_FIRE_Current_enemy = m_Players.GetCur();
	} //(!m_Players.IsEmpty())

}

				// cмотреть в указанную точку (нужно ограничить чтобы пушка не перекручивалась)
//procedure LookAt(i : byte; x, y :real);
//var angle : integer;
//begin
//if players=nil then exit;
//if players.bot = false then exit;
//angle := (round(RadToDeg(ArcTan2(players.y - y + 5,players.x-x))-90) mod 360);
//if angle < 0 then angle := 360+angle;
//players.fangle := angle;
//end;

void CBot::SetWeapon(unsigned int r)
{
 if (r<32) 
 {
   fnP_DLL_SetWeapon(DXID,C_WPN_GAUNTLET);
   return;
 }

 if (r<176)
 {
 //{===SHAFT===}
	if ((have_lg)&&(ammo_lg!=0))
    {
		fnP_DLL_SetWeapon(DXID,C_WPN_SHAFT);
		return;
    }
   //{===PLASMA===}
	if ((have_pg)&&(ammo_pg!=0))
    {
		fnP_DLL_SetWeapon(DXID,C_WPN_PLASMA);
		return;
    }
   //{===SHOTGUN===}
	if ((have_ssg)&&(ammo_ssg!=0))
    {
		fnP_DLL_SetWeapon(DXID,C_WPN_SHOTGUN);
		return;
    }
   //{===GRENADE LAUCHER===}
	if ((have_gl)&&(ammo_gl!=0))
    {
		fnP_DLL_SetWeapon(DXID,C_WPN_GRENADE);
		return;
    }
 }

 if (r>100)
 {
   //{===BFG===}
	if ((have_bfg)&&(ammo_bfg!=0))
    {
		fnP_DLL_SetWeapon(DXID,C_WPN_BFG);
		return;
    }
   //{===PLASMA===}
	if ((have_pg)&&(ammo_pg!=0))
    {
		fnP_DLL_SetWeapon(DXID,C_WPN_PLASMA);
		return;
    }
   //{===ROCKET LAUCHER===}
	if ((have_rl)&&(ammo_rl!=0))
    {
		fnP_DLL_SetWeapon(DXID,C_WPN_ROCKET);
		return;
    }
   //{===SHOTGUN===}
	if ((have_ssg)&&(ammo_ssg!=0))
    {
		fnP_DLL_SetWeapon(DXID,C_WPN_SHOTGUN);
		return;
    }
 }

  //{===RAILGUN===}
 if ((have_rg)&&(ammo_rg!=0))
 {
	fnP_DLL_SetWeapon(DXID,C_WPN_RAIL);
	return;
 }
  //{===PLASMA===}
 if ((have_pg)&&(ammo_pg!=0))
 {
	fnP_DLL_SetWeapon(DXID,C_WPN_PLASMA);
	return;
 }
  //{===SHOTGUN===}
 if ((have_ssg)&&(ammo_ssg!=0))
 {
	fnP_DLL_SetWeapon(DXID,C_WPN_SHOTGUN);
	return;
 }
  //{===MACHINE GUN===}
 if ((have_mg)&&(ammo_mg!=0))
 {
	fnP_DLL_SetWeapon(DXID,C_WPN_MACHINE);
	return;
 }

fnP_DLL_SetWeapon(DXID,C_WPN_GAUNTLET);
}
