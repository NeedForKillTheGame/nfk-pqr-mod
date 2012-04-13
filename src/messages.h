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
*	messages.h
*	Модуль выводит на экран (не на консоль) сообщения.
*	Пока используется только для вывода сообщения "You fragged... "
*	Модуль единовременно выводит только одно сообщение - см. указатель CMessages::cur
*/

#ifndef __TEXT_MESSAGES_H__
#define __TEXT_MESSAGES_H__
#include "pqrmod.h"

class CTextMessage //Класс самого сообщения
{
	char* text_prefix;
	char* text;
	char* text_suffix;
	int Time; //Время в течении которого отображать сообщение
	int x;
	int y;
	int font; //Пока не используется, значение шрифта берётся из консольной переменной cf_Fragmsg
	int TickCount; //Счётчик кадров, используется в цикле отображения
	bool visible; //Флаг видимости
public:
	CTextMessage(int _x, int _y, int t=50, int fnt=1, char* _tp=NULL, char* _ts=NULL);
	void SetSettings(int _x, int _y, int t, int fnt, char* _tp=NULL, char* _ts=NULL);
	void Show(char* text=NULL, int t=-1); //принимает текст сообщения и время в течении которого отображать. По умолчанию (если параметр -1) сообщение отображается Time число кадров - приватная переменная объекта инициализируется при создании объекта
	void Draw(); //Выводит на экран сообщение если флаг visible==true;
};

class CMessages
{
	CTextMessage* cur; //Указатель на текущее сообщение, которое выводить на экран
public:
	CMessages();
	void SetCurrent(CTextMessage* c);
	void RegisterConsoleCommands();
	void ChatReceived(WORD DXID, char* Text);
	void EVENT_MainLoop();
};

extern CMessages Messages; //Главный объект модуля
extern CTextMessage MessageYouFragged; //Объект текстового сообщения "You fragged... "

extern c_var_bool* ch_fragmsg; //Консольная переменная, выводить ли сообщение "You Fragged ... "
extern c_var_int* ch_fragmsg_x;
extern c_var_int* ch_fragmsg_y;
extern c_var_int* ch_fragmsg_time;
extern c_var_int* cf_fragmsg; //Консольная переменная, номер шрифта которым выводить сообщение "You Fragged ... "
extern c_var_string* ch_fragmsg_color;
extern c_var_bool* ch_fragmsg_colored_name;

#endif //__TEXT_MESSAGES_H__