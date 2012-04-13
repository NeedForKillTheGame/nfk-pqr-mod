#ifndef _AI_H_
#define _AI_H_

#define __TIME_BETWEN_FIRE__ 5
#define __TIME_BEFORE_SELECT_NEXT_ITEM__ 50
#define __TIME_MOVE_THINK__ 3
#define __WAIT_ITEM_TIME__ 1
#define __CRITICAL_TIME_TO_MOVE__ 40
#define __DEBUG_show_variant_path_CHAR__ "."
#define __DEBUG_show_path_CHAR__ "*"
#define __MAX_TIME_NOT_MOVE__ 5

#include "pqrmod.h"
#include "player.h"
#include "map.h"

void AI_EVENT_BeginGame();
void AI_RegisterConsoleCommands();
bool AI_EVENT_ConsoleCommand(CList*);
void AI_ConfigSave(FILE*);
bool AI_ConfigLoad(CList*);

class CBot : public CPlayer {
private:
	int time_before_select_item;
	int time_move_think;
	int time_moving;
	BYTE time_fire;
	BYTE time_stoping;

	int last_x;
	int last_y;

	void* BLOC_GS_Current_item;
	void* AI_FIRE_Current_enemy;
	CPath* CurPath;
	bool isStoped;
	bool AI_FIRE_select_enemy;
	bool BLOC_find_best_item;
	bool BLOC_goal_score;
	bool BLOC_select_bloc;
	BYTE CurKeys;

	bool DEBUG_show_path;
	bool DEBUG_show_variant_path;

	void SetWeapon(unsigned int);
public:
	CBot(TPlayerEx);
	void ai_think();
	void ai_fire();
	void ai_goal_score(CItem*);
	bool ai_move_to_WP(int);
	bool ai_touchWP(int);
	bool isOnGround();

	void ai_reset();
	void ShowPath();
	void ShowVariantPath();
	void HidePath();
};

#endif