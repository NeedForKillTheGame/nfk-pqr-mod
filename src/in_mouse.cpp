#include "in_mouse.h"

POINT Cursor;
bool CursorVisible;

void CursorDraw();

void MOUSE_EVENT_MainLoop()
{
	//if (CursorVisible) CursorDraw();
}

void MOUSE_Init()
{
	Cursor.x=320;
	Cursor.y=240;
}

void MOUSE_Show()
{
	CursorVisible=true;

}
void MOUSE_Hide()
{
	CursorVisible=false;
}

void CursorDraw()
{
	fnP_DLL_FX_FillRectMap(Cursor.x,Cursor.y,Cursor.x+11,Cursor.y+10,Cursor.x+5,Cursor.y+10,Cursor.x,Cursor.y+13,0x990000FF,0x102,false);
}