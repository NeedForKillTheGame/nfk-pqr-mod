#ifndef NO_CHEAT
#ifndef _TIME_HELP_H_
#define _TIME_HELP_H_

#include "pqrmod.h"
#include "map.h"

class CTimeHelp {
private:
	unsigned long ItemsToShow;
	//bool AutoShow;
	unsigned int AutoShow_StartTime;
	bool visible;

	bool IsVisible(CItem*);
	unsigned int GetItemTime(CItem*);
	void DrawItemTime(CItem*);
	
public:
	CTimeHelp();
	~CTimeHelp();
	void Draw();
	void Show() {visible=true;}
	void Hide() {visible=false;}
	void SetAutoShow(unsigned int _i) {AutoShow_StartTime=_i;}
	unsigned int GetAutoShow() {return AutoShow_StartTime;}
	void SetItems(unsigned long _i) {ItemsToShow = _i;}
	unsigned long GetItems() {return ItemsToShow;}
	void AddItem(unsigned long _i) {ItemsToShow = ItemsToShow | _i;}
	void DeleteItem (unsigned long _i) {ItemsToShow = ItemsToShow & (~_i);}
};

void TIME_HELP_EVENT_MainLoop();
void TIME_HELP_RegisterConsoleCommands();
bool TIME_HELP_EVENT_ConsoleCommand(CList*); //Вызывается из EVENT_ConsoleCommand (console.cpp)
void TIME_HELP_ConfigSave(FILE*);
bool TIME_HELP_ConfigLoad(CList*);

//extern CTimeHelp TimeHelp;

#endif
#endif