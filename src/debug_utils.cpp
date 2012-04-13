#include "debug_utils.h"
#include "dll_register.h"
#define __LINE_INTERVAL_X__ 40
#define __LINE_INTERVAL_Y__ 25

void draw_line(int x1,int y1,int x2,int y2,char* c)
{
	
	int dx,dy,adx,ady;
	int x=0;
	int y=0;
	int delta = 0;
	int xmin, ymin;

	dx = x2-x1;
	if (dx<0) {adx = -1*dx; xmin=x2;}
	else {adx = dx; xmin=x1;}
	dy = y2-y1;
	if (dy<0) {ady = -1*dy; ymin=y2;}
	else {ady = dy; ymin=y1;}
	
	if ((adx<__LINE_INTERVAL_X__)&&(ady<__LINE_INTERVAL_Y__)) return;

	bool by_x;
	y = ymin;
	x = xmin;
	if (dy == 0) by_x = true;
	else if (adx > ady) by_x = true;
		else by_x = false;
	if (!by_x)
		do {
			y += __LINE_INTERVAL_Y__;
			x = ((double)dx/dy)*(y - y1) + x1;
			fnP_DLL_debug_textout(x,y,c);
		}
		while (y < ady+ymin);
	else
		do {
			x += __LINE_INTERVAL_X__;
			y = ((double)dy/dx)*(x - x1) + y1;
			fnP_DLL_debug_textout(x,y,c);
		}
		while (x < adx+xmin);
}