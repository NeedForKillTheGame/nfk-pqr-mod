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

#include "console.h"

#include "input.h"

CConsole Console;
void CMD_bind(char *str); //при вызове консольной команды bind
void CMD_alias(char *str); //при вызове консольной команды alias

c_cmd* EmptyCMD;
c_alias* EmptyAlias;
void CMD_EmptyCMD(char* s);

c_cmd::c_cmd(char* n, char* s, fnOnExecute fn)
{
	if (n!=NULL)
	{
		name = new char[strlen(n)+1];
		strcpy(name,n);
	}
	if (s!=NULL)
	{
		string = new char[strlen(s)+1];
		strcpy(string,s);
	}
	
	if (n!=NULL && s!=NULL)Console.RegisterCMD(this); //Если оба параметра NULL - Это альяс, не регистрируем
	OnExecute = fn;
}

bool c_cmd::cmp(char *str) //Возвращает TRUE если входной параметр эквивалентен полю string
{
	if (str==NULL || strlen(str)==0) return false;
	if (strcmp(string,str)==0) return true;
	return false;
}

void c_cmd::set_value(char *p)
{
	//Команда не может сменить своего значения, она его не имеет
	//просто вызываем внешний обработчик
	if (OnExecute!=NULL) OnExecute(p); 
}

void c_cmd::print2(FILE* f)
{
}

c_var_int::c_var_int(char* n, char* s,int d,int mn, int mx, fnOnExecute fn): c_cmd( n , s , fn )
{
	val = d;
	default_val = d;
	min_val = mn;
	max_val = mx;
}

void c_var_int::set_value_int(int new_val_i)
{
	val = new_val_i;
}

void c_var_int::set_value(char* new_val)
{
	if ( OnExecute != NULL ) {OnExecute(new_val);return;} //Внешняя функция обработки смены значения вместо стандартной
	if (new_val!=NULL && strlen(new_val)>0)
	{
		int new_val_i = atoi(new_val);
		if ( new_val_i>max_val || new_val_i<min_val )
		{
			Console.printf("Out of range [%i - %i]",min_val,max_val);
			return;
		}
		
		val = new_val_i;
		Console.printf( "%s changet to %i" , name, val );
		return;
	}
	print(); //Выводит на консоль текущее значение переменной
}

void c_var_int::print()
{
	Console.printf("%s is %i",name,val);
}

void c_var_int::print2(FILE* f)
{
	if (f==NULL) return;
	fputs(va("%s %d\n",string,val),f);
}

c_var_float::c_var_float(char* n, char* s,float d,float mn, float mx, fnOnExecute fn): c_cmd( n , s , fn )
{
	val = d;
	default_val = d;
	min_val = mn;
	max_val = mx;
}

void c_var_float::set_value_float(float new_val_f)
{
	val = new_val_f;
}

void c_var_float::set_value(char* new_val)
{
	if ( OnExecute != NULL ) {OnExecute(new_val);return;} //Внешняя функция обработки смены значения вместо стандартной
	if (new_val!=NULL && strlen(new_val)>0)
	{
		float new_val_f = atof(new_val);
		if ( new_val_f>max_val || new_val_f<min_val )
		{
			Console.printf("Out of range [%f - %f]",min_val,max_val);
			return;
		}
		val = new_val_f;
		Console.printf( "%s changet to %f" , name, val );
		return;
	}
	print(); //Выводит на консоль текущее значение переменной
}

void c_var_float::print()
{
	Console.printf( "%s is %f" , name, val );
}
void c_var_float::print2(FILE* f)
{
	if (f==NULL) return;
	fputs(va("%s %f\n",string,val),f);
}

c_var_string::c_var_string(char* n, char* s,const char* d, fnOnExecute fn): c_cmd( n , s , fn )
{
	if (d!=NULL)
	{
		val = new char[strlen(d)+1];
		strcpy(val,d);
		default_val = new char[strlen(d)+1];
		strcpy(default_val,d);
	}
	else
	{
		val = new char[1];
		val[0] = '\0';
	}
}

void c_var_string::set_value_string(char* new_val)
{
	if (val!=NULL) delete[] val;
	if (new_val!=NULL && strlen(new_val)>0)
	{
		val = new char[strlen(new_val)+1];
		strcpy(val,new_val);
	}
	else
	{
		val = new char[1];
		val[0] = '\0';
	}
}

void c_var_string::set_value(char* new_val)
{
	if ( OnExecute != NULL ) {OnExecute(new_val);return;} //Внешняя функция обработки смены значения вместо стандартной
	if (new_val!=NULL && strlen(new_val)>0)
	{
		if (val!=NULL) delete[] val;
		if (strcmp(new_val,"\"\""))
		{
			val = new char[strlen(new_val)+1];
			strcpy(val,new_val);
		}
		else
		{
			val = new char[1];
			val[0] = '\0';
		}
		Console.printf( "%s changet to \"%s\"" , name, val );
		return;
	}
	print(); //Выводит на консоль текущее значение переменной
}

void c_var_string::print()
{
	Console.printf( "%s is %s" , name, (val==NULL || strlen(val)==0)?"\"\"":val );
}
void c_var_string::print2(FILE* f)
{
	if (f==NULL) return;
	if (val==NULL || strlen(val)==0) return;
	fputs(va("%s %s\n",string,val),f);
}

c_var_enum::c_var_enum(char* n, char* s,int d, int c, const char** ev, fnOnExecute fn): c_cmd( n , s , fn )
{
}

void c_var_enum::set_value(char* new_val) //НЕ ДОДЕЛАНА
{

}

void c_var_enum::print()
{
}
void c_var_enum::print2(FILE* f)
{
	if (f==NULL) return;
}

c_var_bool::c_var_bool(char* n, char* s,bool d, fnOnExecute fn): c_cmd( n , s , fn )
{
	val = d;
	default_val = d;
}

void c_var_bool::set_value_bool(bool new_val_b) //Меняет значение переменной и возвращает новое значение
{
	val = new_val_b;
}

void c_var_bool::set_value(char* new_val)
{
	if ( OnExecute != NULL ) {OnExecute(new_val);return;} //Внешняя функция обработки смены значения вместо стандартной
	if (new_val!=NULL && strlen(new_val)>0)
	{
		bool new_val_bool;
		bool result = true;
		//Переводим строку new_val в переменную типа bool new_val_bool
		//Если перевод невозможен, то флаг result становится false
		if (strcmp(new_val,"true")==0 || strcmp(new_val,"on")==0 || strcmp(new_val,"0")) new_val_bool = true;
		else if (strcmp(new_val,"false")==0 || strcmp(new_val,"off")==0 || strcmp(new_val,"0")==0) new_val_bool = false;
		else result=false;

		if ( result )
		{
			val = new_val_bool;
			Console.printf("%s is turned %s",name,(val?"on":"off"));
			return;
		}
	}
	print();
}

bool c_var_bool::togle()
{
	if (val) set_value("0");
	else set_value("1");

	return val;
}

void c_var_bool::print()
{
	Console.printf("%s is turned %s",name,(val?"on":"off"));
}
void c_var_bool::print2(FILE* f)
{
	if (f==NULL) return;
	fputs(va("%s %d\n",string,(val?1:0)),f);
}

c_alias::c_alias(char* n, char* s,const char* d, fnOnExecute fn): c_cmd(NULL,NULL,fn)
{
	if (n!=NULL)
	{
		name = new char[strlen(n)+1];
		strcpy(name,n);
	}
	if (s!=NULL)
	{
		string = new char[strlen(s)+1];
		strcpy(string,s);
	}
	
	if (d!=NULL)
	{
		val = new char[strlen(d)+1];
		strcpy(val,d);
		default_val = new char[strlen(d)+1];
		strcpy(default_val,d);
	}
	else
	{
		val = new char[1];
		val[0] = '\0';
	}
	Console.RegisterAlias(this);
}

void c_alias::set_value_alias(char* new_val)
{
	if (val!=NULL) delete[] val;
	if (new_val!=NULL && strlen(new_val)>0)
	{
		val = new char[strlen(new_val)+1];
		strcpy(val,new_val);
	}
	else
	{
		val = new char[1];
		val[0] = '\0';
	}
}

void c_alias::set_value(char* new_val)
{
	if ( OnExecute != NULL ) {OnExecute(new_val);return;} //Внешняя функция обработки смены значения вместо стандартной
	if (new_val!=NULL && strlen(new_val)>0)
	{
		if (val!=NULL) delete[] val;
		if (strcmp(new_val,"\"\""))
		{
			val = new char[strlen(new_val)+1];
			strcpy(val,new_val);
		}
		else
		{
			val = new char[1];
			val[0] = '\0';
		}
		Console.printf( "Alias %s changet to \"%s\"" , name, val );
		return;
	}
	print(); //Выводит на консоль текущее значение переменной
}

void c_alias::print()
{
	Console.printf( "Alias %s is %s" , name, (val==NULL || strlen(val)==0)?"\"\"":val );
}
void c_alias::print2(FILE* f)
{
	if (f==NULL) return;
	if (val==NULL || strlen(val)==0) return;
	fputs(va("alias %s %s\n",string,val),f);
}

c_cmd* CConsole::FindVarByString(char* s)
{
	if (ListCMDs.size()!=0)
	{
		ListCMDs.top();
		do
			if (ListCMDs.cur()->d->cmp(s))
				return ListCMDs.cur()->d;
		while (ListCMDs.next());
	}

	return EmptyCMD;
}

c_alias* CConsole::FindAliasByString(char* s)
{
	if (ListAliases.size()!=0)
	{
		ListAliases.top();
		do
			if (ListAliases.cur()->d->cmp(s))
				return ListAliases.cur()->d;
		while (ListAliases.next());
	}

	return EmptyAlias;
}

void CConsole::RegisterConsoleCommands()
{
	c_cmd* bind = new c_cmd("bind","bind",CMD_bind);
	c_cmd* alias = new c_cmd("alias","alias",CMD_alias);
	c_cmd* EmptyCMD = new c_cmd("EmptyCMD","EmptyCMD",CMD_EmptyCMD);
	c_alias* EmptyAlias = new  c_alias("EmptyAlias","EmptyAlias");

	fnP_DLL_RegisterConsoleCommand("alias");
}

void CMD_EmptyCMD(char* s)
{
}

/*-------------------------------------------
*	EVENT_MainLoop()
*	Основной цикл этого модуля
*-------------------------------------------*/
void CConsole::EVENT_MainLoop()
{
	//Отсылаем в NFK.exe все команды из списка SendOnThisFrame
	if (SendOnThisFrame.size()) 
	{
		SendOnThisFrame.top();
		do
		{
			SendConsoleCommand(SendOnThisFrame.cur()->d);
			delete[] SendOnThisFrame.cur()->d;
		}
		while (SendOnThisFrame.next());
		SendOnThisFrame.delAll();
	}
	//Перемещаем все команды из списка SendOnNextFrame в список SendOnThisFrame,
	//таким образом в следующем кадре они будут отправлены в NFK.exe
	if (SendOnNextFrame.size())
		SendOnNextFrame.movAllAftLas(SendOnThisFrame);
}
bool CConsole::EVENT_ConsoleCommand(char *s1) //Обарабатывает введённую консольню команду s1
{
	if (s1==NULL) return false;
	if (strlen(s1)==0) return false;
	
	char* s = new char[strlen(s1)+1];
	
	strcpy(s,s1);
	char* p;
	p = strtok(s," "); //Вычленяем из принятой строки первое слово до пробела
	
	if (ListCMDs.size()!=0) //Ищем соответствующую команду
	{
		ListCMDs.top();
		do
		{
			if (ListCMDs.cur()->d->cmp(p))
			{
				p = strtok(NULL,"\n"); //Вычленяем из принятой строки всё после первого пробела
				ListCMDs.cur()->d->set_value(p); //set_value сначала пытается вызвать внешний обработчик OnExecute, в случае неудачи меняет значение переменной самостоятельно
				return true;
			}
		}
		while (ListCMDs.next());
	}

	if (ListAliases.size()!=0) //Ищем соответствующий альяс
	{
		ListAliases.top();
		do
		{
			if (ListAliases.cur()->d->cmp(p))
			{
				ParseAlias(ListAliases.cur()->d->value()); //Выполняем альяс
				return true;
			}
		}
		while (ListAliases.next());
	}

	return false;
}

void CConsole::SendConsoleCommand(char *str)
{
	if ( str==NULL || strlen(str)==0 ) return;
	fnP_DLL_SendConsoleCommand(str);	
}

void CConsole::SendConsoleHCommand(char *str)
{
	if ( str==NULL || strlen(str)==0 ) return;
	fnP_DLL_SendConsoleHCommand(str);	
}

void CConsole::SendConsoleCommandOnNextFrame(char *str)
{
	
	if ( str==NULL || strlen(str)==0 ) return;
	char* s = new char[strlen(str)+1];
	strcpy(s,str);
	//	Добавляем в список SendOnNextFrame команду char *str
	ItemString* NewIS = new ItemString(s);
	SendOnNextFrame.addAftLas(NewIS);
}

void CConsole::RegisterCMD(c_cmd* c)
{
	Item_c_cmd* NV = new Item_c_cmd;
	NV->d = c;
	ListCMDs.addAftLas(NV);
}

void CConsole::RegisterAlias(c_alias* a)
{
	Item_c_alias* NV = new Item_c_alias;
	NV->d = a;
	ListAliases.addAftLas(NV);
}

void CConsole::printf (const char *fmt, ...)
{
    va_list     argptr;
    char        msg[255];
   
    va_start (argptr,fmt);
    vsprintf (msg,fmt,argptr);
    va_end (argptr);
    
    print(msg);
}

void CConsole::print(char* str)
{
	fnP_DLL_AddMessage(str);
}

void CConsole::ParseAlias(char* str)
{
	//Здесь будет парсинг альясов - расчленение альяса на составляющие и отсылка их поочерёдно в EVENT_ConsoleCommand
	if (str==NULL || strlen(str)==0) return;
	char* s = new char[strlen(str)+1]; //Временный буфер
	strcpy(s,str);
	char *p,*s2;
	ListStrings SendCommands;
	ItemString* NewIS;

	//Расчленяем строку на составляющие через ; и заносим в список SendCommands
	p=strtok(s,";");
	do
	{
		s2 = new char[strlen(p)+1];
		strcpy(s2,p);
		//	Добавляем в список SendCommands команду char *s2
		NewIS = new ItemString(s2);
		SendCommands.addAftLas(NewIS);
	}
	while( (p = strtok(NULL,";")) != NULL );
	delete[] s;

	//Выполняем список команд SendCommands
	if (SendCommands.size()) 
	{
		SendCommands.top();
		do
		{
			SendConsoleHCommand(SendCommands.cur()->d);
			delete[] SendCommands.cur()->d;
		}
		while (SendCommands.next());
		SendOnThisFrame.delAll();
	}
}

void CMD_bind(char* str)
{
	if (str==NULL || strlen(str)==0)
	{
		Console.print("bind <key> [command]");
		return;
	}
	
	char* mkey;
	char* cmd;
	mkey = strtok(str," ");
	CKey* K=NULL;
	if (!strcmp(mkey,"mouse5") || !strcmp(mkey,"mbutton5")) K=&KEY_Mouse5;
	else
	if (!strcmp(mkey,"mouse6") || !strcmp(mkey,"mbutton6")) K=&KEY_Mouse6;
	else
	if (!strcmp(mkey,"mouse4") || !strcmp(mkey,"mbutton4")) K=&KEY_Mouse4;
	else
	if (!strcmp(mkey,"mouse7") || !strcmp(mkey,"mbutton7")) K=&KEY_Mouse7;
	else
	if (!strcmp(mkey,"mouse8") || !strcmp(mkey,"mbutton8")) K=&KEY_Mouse8;

	if (K!=NULL)
		if (cmd = strtok(NULL,"\n")) //Используем переменную cmd для взятия следующего параметра
			K->SetCmd(cmd); //Устанавливаем команду на нажатие соотв. кнопки
		else //Если следующего параметра нет (т.е. было набрано что-то вроде "bind mouse6") надо вывести сообщение о текущем бинде
			if (K->GetCmdDown()!=NULL)
				Console.printf("%s binded to %s",mkey,K->GetCmdDown());
			else
				Console.printf("%s is unbinded",mkey);
}

void CMD_alias(char* str)
{
	if (str==NULL || strlen(str)==0) return;
	
	char* s = new char[strlen(str)+1]; //Временный буфер
	strcpy(s,str);
	char* p;
	p = strtok(s," "); //Вычленяем из принятой строки первое слово до пробела
	if (p==NULL) {delete[] s;return;}
	
	
	while(strlen(p)==0) //Если в начале идёт несколько пробелов, то проходим их все в цикле
	{
		p = strtok(NULL," ");
		if (p==NULL) {delete[] s;return;}
	}
	
	//Добрались до значещего слова
	
	c_alias* CurAlias = Console.FindAliasByString(p);
	if (CurAlias==EmptyAlias)
	{
		char* alias_name = new char[strlen(p)+1];
		strcpy(alias_name,p);
		p = strtok(NULL,"\0");
		if (p!=NULL && strlen(p)!=0) //Далее идёт не пустое содержание
		{
			CurAlias = new c_alias(alias_name,alias_name); //Создаём новый альяс
			CurAlias->set_value(p);
		}
		else Console.print("Unknown alias");
	}
	else
	{
		p = strtok(NULL,"\0");
		if (p!=NULL && strlen(p)!=0) //Далее идёт не пустое содержание
		{
			CurAlias->set_value(p);
		}
		else CurAlias->print();
	}
	delete[] s;
}