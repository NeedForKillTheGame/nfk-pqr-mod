/*
Copyright (C) 1996-1997 Id Software, Inc.

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
	common.h
	Несколько вспомагательных функций написаных на pure c
	Взято из исходников Quake
*/
#ifndef __COMMON_H__
#define __COMMON_H__

#include "pqrmod.h"

bool ColorCheck(char* cl); //Проверяет строку на соответсвие допустимому коду цвета в NFK
bool strcmp_color(char* s1, char* s2); //Сравнивает сроки без цветовых кодов. Возвращает true если строки идентичны
char* StripColorName(char* str); //Осторожно, функия меняет *str и возвращает его

//============================================================================

#define Q_memset(d, f, c) memset((d), (f), (c))
#define Q_memcpy(d, s, c) memcpy((d), (s), (c))
#define Q_memcmp(m1, m2, c) memcmp((m1), (m2), (c))
#define Q_strcpy(d, s) strcpy((d), (s))
#define Q_strncpy(d, s, n) strncpy((d), (s), (n))
#define Q_strlen(s) ((int)strlen(s))
#define Q_strrchr(s, c) strrchr((s), (c))
#define Q_strcat(d, s) strcat((d), (s))
#define Q_strcmp(s1, s2) strcmp((s1), (s2))
#define Q_strncmp(s1, s2, n) strncmp((s1), (s2), (n))


#define Q_strcasecmp(s1, s2) _stricmp((s1), (s2))
#define Q_strncasecmp(s1, s2, n) _strnicmp((s1), (s2), (n))

int Q_atoi (char *str);
float Q_atof (char *str);


char    *va(char *format, ...);
// does a varargs printf into a temp buffer

long strhex2long(char *s);

int build_number( void );


// kazik -->
int isspace2(int c);
// kazik <--

#endif //__COMMON_H__