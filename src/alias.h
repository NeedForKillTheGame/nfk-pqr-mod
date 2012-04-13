#ifndef _ALIAS_H_
#define _ALIAS_H_
#include "list.h"
#include <stdio.h>

extern CKey KEY_Mouse5;
extern CKey KEY_Mouse6;
extern CKey KEY_Mouse4;
extern CKey KEY_Mouse7;
extern CKey KEY_Mouse8;

void ALIAS_ParseAlias(char*);
void ALIAS_RegisterConsoleCommands();
bool ALIAS_EVENT_ConsoleCommand(CList*); //Вызывается из EVENT_ConsoleCommand (console.cpp)
void ALIAS_ConfigSave(FILE*);
bool ALIAS_ConfigLoad(CList*);

#endif
