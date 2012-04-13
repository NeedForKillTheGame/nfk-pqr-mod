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


/*
*	animation.h
*	Модуль воспроизводит графическую анимацию. Существует нескоклько классов различных анимаций,
*	все являются потомками от CAnim.
*	В любом другом модуле можно создать объект анимации и инициализировать его параметрами.
*	Далее необходимо создать "Список анимаций" следующего типа:
*	typedef Item<CAnim*> ItemAnim;
*	typedef List<ItemAnim> ListAnim; //Эти типы уже описаны в animation.h
*	Объекты внутри списка воспроизводятся поочерёдно. Как только объет отиграл своё,
*	он удаляется из списка и начинает воспроизводиться следующий.
*
*	Наконец, чтобы запустить список анимации на воспроизведение, необходимо добавить его в
*	"Список воспроизводимых списков анимаций": AnimationListAdd(ListAnim* a).
*	Все добавленные списки воспроизводятся одновременно.
*/

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "pqrmod.h"

#define ANIM_RECT 1
#define FAST -1
#define EFFECT_RECT 0

class CAnim
{
protected:
	int type;
	int Effect;
	bool Progress;
public:
	CAnim(int _e=0);
	~CAnim();
	virtual void Draw();
	bool InProgress() {return Progress;} //Progress инициализируется в true при каждом вызове Draw
	void Start(); // {Progress=true;}
	void Stop(); // {Progress=false;}
	void (*fnAfterStop)();
	ShevList* inList;
};

class CAnimRect: public CAnim
{
	float x1,y1,x2,y2;
	int x1END,y1END,x2END,y2END;
	float dx1,dy1,dx2,dy2;
	Cardinal colorLine,colorFill;
	int FXeffect;
	int frames;
public:
	CAnimRect() {}
	CAnimRect(int _x1,int _y1,int _x2,int _y2,int _x1END, int _y1END, int _x2END, int _y2END, Cardinal _cL, Cardinal _cF, int _fr=FAST,int _FXe=0x102,int _e=0);
	void SetParams(int _x1,int _y1,int _x2,int _y2,int _x1END, int _y1END, int _x2END, int _y2END, Cardinal _cL, Cardinal _cF, int _fr=FAST,int _FXe=0x102,int _e=0);
	void SetParams(TRect R1,TRect R2, Cardinal _cL, Cardinal _cF, int _fr=FAST,int _FXe=0x102,int _e=0);
	void SetNewEnd(int _x1END, int _y1END, int _x2END, int _y2END,int _fr=FAST);
	void SetNewEnd(TRect R2,int _fr=FAST);
	void Draw();
};

typedef Item<CAnim*> ItemAnim;
typedef List<ItemAnim> ListAnim;

void AnimationListAdd(ListAnim *a);
bool AnimationListRemove(ListAnim *a);

void ANIMATION_EVENT_MainLoop();

#endif //__ANIMATION_H__