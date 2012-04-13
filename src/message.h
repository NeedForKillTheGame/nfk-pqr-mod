#ifndef __MESSAGE_H__
#define __MESSAGE_H__
#include "pqrmod.h"

CTextMessage
{
	char* text_prefix;
	char* text;
	char* text_suffix;
	int Time;
	int x;
	int y;
	int font;
	int TikcCount;
	bool visible;
public:
	CTextMessage(int _x, int _y, int t=50, int fnt=1, char* _tp=NULL, char* _ts=NULL);
	void Show(char* text=NULL, int t=-1);
	void Draw();
};

CMessage
{
	CTextMessage* cur;
public:
	CMessage();
	void EVENT_MainLoop();
};

extern CMessage Message;
extern CTextMessage YouFragged;

#endif //__MESSAGE_H__