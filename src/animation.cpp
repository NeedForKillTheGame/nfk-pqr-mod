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
#include "animation.h"

typedef Item<ListAnim*> ItemListAnim;

List<ItemListAnim> AnimationLists;
/*-------------------------------------------
*	EVENT_MainLoop()
*	Основной цикл этого модуля
*-------------------------------------------*/
void ANIMATION_EVENT_MainLoop()
{
	if (AnimationLists.size()==0) return; //Если список пуст, выходим
	
	AnimationLists.top(); //Переходим к началу списка анимационных листов
	do {
		if (AnimationLists.cur()->d->size()!=0) //Если список анимаций содержит анимации, то рисуем
		{
			AnimationLists.cur()->d->cur()->d->Draw();
		}
	}
	while (AnimationLists.next());
}

void AnimationListAdd(ListAnim *a)
{
	ItemListAnim* NewL = new ItemListAnim;
	NewL->d = a;
	AnimationLists.addAftLas(NewL);
}

bool AnimationListRemove(ListAnim* a)
{
	if (AnimationLists.size()==0) return false; //Если список пуст, выходим
	
	AnimationLists.top(); //Переходим к началу списка анимационных листов
	do {
		if (AnimationLists.cur()->d == a) //Если содержит иcмый список анимаций, то рисуем
		{
			AnimationLists.delCur();
			return true;
		}
	}
	while (AnimationLists.next());
	return false;
	
}

CAnim::CAnim(int _e)
{
	type=0;
	Effect=_e;
	Progress=false;
	fnAfterStop=NULL;
}

CAnim::~CAnim()
{
}

void CAnim::Draw()
{
}
void CAnim::Start()
{
	Progress = false;
	if (inList!=NULL)
	{
		ItemAnim* A = new ItemAnim(this);
		inList->addAftLas(A);
	}
}
void CAnim::Stop()
{
	Progress = false;
	if (inList!=NULL) inList->delCur();
	if (fnAfterStop!=NULL) fnAfterStop();		
}

CAnimRect::CAnimRect(int _x1,int _y1,int _x2,int _y2,int _x1END, int _y1END, int _x2END, int _y2END, Cardinal _cL, Cardinal _cF, int _fr, int _FXe, int _e)
{
	SetParams( _x1, _y1, _x2, _y2, _x1END,  _y1END,  _x2END,  _y2END, _cL, _cF,  _fr, _FXe,_e);
}

void CAnimRect::SetParams(int _x1,int _y1,int _x2,int _y2,int _x1END, int _y1END, int _x2END, int _y2END, Cardinal _cL, Cardinal _cF, int _fr, int _FXe, int _e)
{
	type=ANIM_RECT;
	x1=_x1;
	x2=_x2;
	y1=_y1;
	y2=_y2;
	x1END=_x1END;
	x2END=_x2END;
	y1END=_y1END;
	y2END=_y2END;
	colorLine = _cL;
	colorFill = _cF;
	FXeffect = _FXe;
	Effect = _e;

	if (_fr>0) frames=_fr;
	else
	{
		if (_fr==FAST)
		{
			int minFrames=1000;
			if (abs(x1END-x1)!=0 && abs(x1END-x1)<minFrames) minFrames=abs(x1END-x1);
			if (abs(y1END-y1)!=0 && abs(y1END-y1)<minFrames) minFrames=abs(y1END-y1);
			if (abs(x2END-x2)!=0 && abs(x2END-x2)<minFrames) minFrames=abs(x2END-x2);
			if (abs(y2END-y2)!=0 && abs(y2END-y2)<minFrames) minFrames=abs(y2END-y2);
			frames=minFrames;
		}
		else
		{
			int maxFrames=0;
			if (abs(x1END-x1)>maxFrames) maxFrames=abs(x1END-x1);
			if (abs(y1END-y1)>maxFrames) maxFrames=abs(y1END-y1);
			if (abs(x2END-x2)>maxFrames) maxFrames=abs(x2END-x2);
			if (abs(y2END-y2)>maxFrames) maxFrames=abs(y2END-y2);
			frames=maxFrames;
		}
	}

	if (frames!=0)
	{
		dx1=(x1END-x1)/frames;
		dy1=(y1END-y1)/frames;
		dx2=(x2END-x2)/frames;
		dy2=(y2END-y2)/frames;
	}

	//DEBUG PRINT
	//fnP_DLL_AddMessageX(y1,"y1: ");
	//fnP_DLL_AddMessageX(y2,"y2: ");
}
void CAnimRect::SetParams(TRect R1, TRect R2, Cardinal _cL, Cardinal _cF, int _fr, int _FXe, int _e)
{
	SetParams(R1.x1,R1.y1,R1.x2,R1.y2,R2.x1,R2.y1,R2.x2,R2.y2, _cL, _cF, _fr, _FXe, _e);
}

void CAnimRect::SetNewEnd(int _x1END, int _y1END, int _x2END, int _y2END,int _fr)
{
	SetParams(x1,y1,x2,y2,_x1END,_y1END,_x2END,_y2END, colorLine, colorFill, _fr, FXeffect, Effect);
}
void CAnimRect::SetNewEnd(TRect R2,int _fr)
{
	SetParams(x1,y1,x2,y2,R2.x1,R2.y1,R2.x2,R2.y2, colorLine, colorFill, _fr, FXeffect, Effect);
}

void CAnimRect::Draw()
{
	if (frames>0)
	{
		Progress = true;
		frames--;
		x1	+=	dx1;	
		y1	+=	dy1;	
		x2	+=	dx2;	
		y2	+=	dy2;
		switch(Effect)
		{
		case EFFECT_RECT:
			/*int tx=(x1<x2)?x1:x2;
			int ty=(y1>`y2)?y1:y2;
			fnP_DLL_FX_Rectangle(tx,ty,abs(x2-x1),abs(y2-y1),colorLine,colorFill,FXeffect,false);*/
			fnP_DLL_FX_Rectangle(x1,y1,x2-x1,y2-y1,colorLine,colorFill,FXeffect,false);
		break;
		}	
	}
	else Stop();
}