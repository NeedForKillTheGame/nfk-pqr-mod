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
#ifndef __CONSOE_H__
#define __CONSOE_H__
#include "pqrmod.h"

typedef void(*fnOnExecute)(char*);

class c_cmd//�������
{
protected:
	char* name;
	char* string;
public:
	c_cmd(char *n, char *s, fnOnExecute fn=NULL);
	bool cmp(char *str); //���������� TRUE, ���� ������� str ������������ ���� string
	virtual void set_value(char *new_val);
	fnOnExecute OnExecute; //��������� ����� ���������� �������� ���������� ��� ��� ������ �������
	//�� ������� ������� OnExecute ������ �������� set_value() � togle() ����� ��������� �������� � stack overflow
	virtual void print2(FILE* f);
};

class c_var_int : public c_cmd //���������� ���� int
{
	int val;
	int default_val;
	int min_val;
	int max_val;
public:
	c_var_int(char* n, char* s, int d, int mn, int mx, fnOnExecute fn=NULL);
	int value() {return val;}
	void set_value_int(int new_val_i); //��������������� ������ �������� ����������, ��� �������� � ������ ���������� ���������
	void set_value(char *new_val);
	void print();
	void print2(FILE* f);
};

class c_var_float : public c_cmd //���������� ���� float
{
	float val;
	float default_val;
	float min_val;
	float max_val;
public:
	c_var_float(char* n, char* s, float d, float mn, float mx, fnOnExecute fn=NULL);
	float value() {return val;}
	void set_value_float(float new_val_f); //��������������� ������ �������� ����������, ��� �������� � ������ ���������� ���������
	void set_value(char *new_val);
	void print();
	void print2(FILE* f);
};

class c_var_string : public c_cmd //���������� ���� string
{
	char* val;
	char* default_val;
public:
	c_var_string(char* n, char* s,const char* d=NULL, fnOnExecute fn=NULL);
	char* value() {return val;}
	void set_value_string(char *new_val); //��������������� ������ �������� ����������, ��� �������� � ������ ���������� ���������
	void set_value(char *new_val); //���������� TRUE, ���� �������� ���������� ������� ��������
	void print();
	void print2(FILE* f);
};

class c_var_enum : public c_cmd //���������� ���� enum
{
	int val;
	int default_val;
	char** enum_values;
	int count_enum_values;
public:
	c_var_enum(char* n, char* s, int d, int c, const char** ev, fnOnExecute fn=NULL);
	int value() {return val;}
	void set_value(char* new_val); //���������� TRUE, ���� �������� ���������� ������� ��������
	void print();
	void print2(FILE* f);
};

class c_var_bool : public c_cmd //���������� ���� bool
{
	bool val;
	bool default_val;
public:
	c_var_bool(char* n, char* s, bool d, fnOnExecute fn=NULL);
	bool value() {return val;}
	void set_value_bool(bool new_val); //��������������� ������ �������� ����������, ��� �������� � ������ ���������� ���������
	void set_value(char* new_val);
	bool togle(); //������ �������� ���������� � ���������� ����� ��������
	void print();
	void print2(FILE* f);
};

class c_alias : public c_cmd //���������� ���� string
{
	char* val;
	char* default_val;
public:
	c_alias(char* n, char* s,const char* d=NULL, fnOnExecute fn=NULL);
	char* value() {return val;}
	void set_value_alias(char *new_val); //��������������� ������ �������� ����������, ��� �������� � ������ ���������� ���������
	void set_value(char *new_val); //���������� TRUE, ���� �������� ���������� ������� ��������
	void print();
	void print2(FILE* f);
};

typedef Item<c_cmd*> Item_c_cmd;
typedef Item<c_alias*> Item_c_alias;

class CConsole
{
	
	ListStrings SendOnNextFrame;
	ListStrings SendOnThisFrame;
public:
	List<Item_c_cmd> ListCMDs;
	List<Item_c_alias> ListAliases;
	void RegisterCMD(c_cmd* c);
    void RegisterAlias(c_alias* a);

	c_cmd* FindVarByString(char* s); //����������� ��������� �� ������� �� ���� string
	c_alias* FindAliasByString(char* s);
	void ParseAlias(char* str);
	void RegisterConsoleCommands();
	void EVENT_MainLoop();
	bool EVENT_ConsoleCommand(char *s1); //��������� ���������� ������� �� NKF.exe
	void SendConsoleCommand(char *str); //���������� ���������� ������� � NFK.exe
	void SendConsoleHCommand(char *str); //���������� ���������� ������� � NFK.exe
	void SendConsoleCommandOnNextFrame(char *str); //���������� ���������� ������� � NFK.exe � ��������� �����
	void printf (const char *fmt, ...);
	void print (char* str);
};

extern CConsole Console;

#endif //__CONSOE_H__
