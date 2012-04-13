#ifndef __AIM_H__
#define __AIM_H__
#include "pqrmod.h"

class CAim
{
private:
	bool visible;
	char* s;
public:
	CAim();
	~CAim();
	void Show() {visible=true;}
	void Hide() {visible=false;}
	void SetSymbol(char*);
	char* GetSymbol() {return s;}
	bool IsVisible() {return visible;}
	void Draw();
	void AutoAim();
};

void AIM_EVENT_MainLoop();
void AIM_RegisterConsoleCommands();
void AIM_ConfigSave(FILE*);
bool AIM_ConfigLoad(CList*);

#endif //__AIM_H__
