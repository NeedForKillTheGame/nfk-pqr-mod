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
	common.cpp
	Несколько вспомагательных функций написаных на pure c
	Взято из исходников Quake
*/

#include <ctype.h>
#include "common.h"

bool ColorCheck(char* cl)
{
	if (cl==NULL) return false;
	if (strlen(cl)!=1) return false;

	if (!strcmp(cl,"1")||
		!strcmp(cl,"2")||
		!strcmp(cl,"3")||
		!strcmp(cl,"4")||
		!strcmp(cl,"5")||
		!strcmp(cl,"6")||
		!strcmp(cl,"7")||
		!strcmp(cl,"0")||
		!strcmp(cl,"!")||
		!strcmp(cl,"#")||
		!strcmp(cl,"&")||
		!strcmp(cl,"%"))
		return true;
	return false;
}

bool strcmp_color(char* s1, char* s2)
{
	if (s1 == NULL || s2 == NULL) return false;
	char* new_s1 = new char[strlen(s1)+1];
	strcpy(new_s1,s1);
	char* new_s2 = new char[strlen(s2)+1];
	strcpy(new_s2,s2);

	new_s1 = StripColorName(new_s1);
	new_s2 = StripColorName(new_s2);

	bool res;
	if (strcmp(new_s1,new_s2) == 0) res = true;
	else res = false;

	delete[] new_s1;
	delete[] new_s2;

	return res;
}

char* StripColorName(char* str)
{
	if (str==NULL) return NULL;
	char* newstr= new char[strlen(str)+1];
	bool readcolor=false;
	int j=0;
	for(unsigned i=0;i<strlen(str);i++)
	{
		if (readcolor && str[i]!='^') readcolor=false;
		else
			if (!readcolor && str[i]=='^') readcolor=true;
			else 
			{
				newstr[j]=str[i];
				j++;
			}
	}
	newstr[j]='\0';
	strcpy(str,newstr);
	delete[] newstr;
	return str;
}

/*
============================================================================

                    LIBRARY REPLACEMENT FUNCTIONS

============================================================================
*/

#if 0
void Q_memset (void *dest, int fill, int count)
{
    int     i;
    
    if ( (((long)dest | count) & 3) == 0)
    {
        count >>= 2;
        fill = fill | (fill<<8) | (fill<<16) | (fill<<24);
        for (i=0 ; i<count ; i++)
            ((int *)dest)[i] = fill;
    }
    else
        for (i=0 ; i<count ; i++)
            ((byte *)dest)[i] = fill;
}

void Q_memcpy (void *dest, void *src, int count)
{
    int     i;
    
    if (( ( (long)dest | (long)src | count) & 3) == 0 )
    {
        count>>=2;
        for (i=0 ; i<count ; i++)
            ((int *)dest)[i] = ((int *)src)[i];
    }
    else
        for (i=0 ; i<count ; i++)
            ((byte *)dest)[i] = ((byte *)src)[i];
}

int Q_memcmp (void *m1, void *m2, int count)
{
    while(count)
    {
        count--;
        if (((byte *)m1)[count] != ((byte *)m2)[count])
            return -1;
    }
    return 0;
}

void Q_strcpy (char *dest, char *src)
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest++ = 0;
}

void Q_strncpy (char *dest, char *src, int count)
{
    while (*src && count--)
    {
        *dest++ = *src++;
    }
    if (count)
        *dest++ = 0;
}

int Q_strlen (char *str)
{
    int     count;
    
    count = 0;
    while (str[count])
        count++;

    return count;
}

char *Q_strrchr(char *s, char c)
{
    int len = Q_strlen(s);
    s += len;
    while (len--)
        if (*--s == c) return s;
    return 0;
}

void Q_strcat (char *dest, char *src)
{
    dest += Q_strlen(dest);
    Q_strcpy (dest, src);
}

int Q_strcmp (char *s1, char *s2)
{
    while (1)
    {
        if (*s1 != *s2)
            return -1;      // strings not equal    
        if (!*s1)
            return 0;       // strings are equal
        s1++;
        s2++;
    }
    
    return -1;
}

int Q_strncmp (char *s1, char *s2, int count)
{
    while (1)
    {
        if (!count--)
            return 0;
        if (*s1 != *s2)
            return -1;      // strings not equal    
        if (!*s1)
            return 0;       // strings are equal
        s1++;
        s2++;
    }
    
    return -1;
}

int Q_strncasecmp (char *s1, char *s2, int n)
{
    int     c1, c2;
    
    while (1)
    {
        c1 = *s1++;
        c2 = *s2++;

        if (!n--)
            return 0;       // strings are equal until end point
        
        if (c1 != c2)
        {
            if (c1 >= 'a' && c1 <= 'z')
                c1 -= ('a' - 'A');
            if (c2 >= 'a' && c2 <= 'z')
                c2 -= ('a' - 'A');
            if (c1 != c2)
                return -1;      // strings not equal
        }
        if (!c1)
            return 0;       // strings are equal
//      s1++;
//      s2++;
    }
    
    return -1;
}

int Q_strcasecmp (char *s1, char *s2)
{
    return Q_strncasecmp (s1, s2, 99999);
}

#endif

int Q_atoi (char *str)
{
    int     val;
    int     sign;
    int     c;
    
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else
        sign = 1;
        
    val = 0;

//
// check for hex
//
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X') )
    {
        str += 2;
        while (1)
        {
            c = *str++;
            if (c >= '0' && c <= '9')
                val = (val<<4) + c - '0';
            else if (c >= 'a' && c <= 'f')
                val = (val<<4) + c - 'a' + 10;
            else if (c >= 'A' && c <= 'F')
                val = (val<<4) + c - 'A' + 10;
            else
                return val*sign;
        }
    }
    
//
// check for character
//
    if (str[0] == '\'')
    {
        return sign * str[1];
    }
    
//
// assume decimal
//
    while (1)
    {
        c = *str++;
        if (c <'0' || c > '9')
            return val*sign;
        val = val*10 + c - '0';
    }
    
    return 0;
}


float Q_atof (char *str)
{
    double  val;
    int     sign;
    int     c;
    int     decimal, total;
    
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else
        sign = 1;
        
    val = 0;

//
// check for hex
//
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X') )
    {
        str += 2;
        while (1)
        {
            c = *str++;
            if (c >= '0' && c <= '9')
                val = (val*16) + c - '0';
            else if (c >= 'a' && c <= 'f')
                val = (val*16) + c - 'a' + 10;
            else if (c >= 'A' && c <= 'F')
                val = (val*16) + c - 'A' + 10;
            else
                return val*sign;
        }
    }
    
//
// check for character
//
    if (str[0] == '\'')
    {
        return sign * str[1];
    }
    
//
// assume decimal
//
    decimal = -1;
    total = 0;
    while (1)
    {
        c = *str++;
        if (c == '.')
        {
            decimal = total;
            continue;
        }
        if (c <'0' || c > '9')
            break;
        val = val*10 + c - '0';
        total++;
    }

    if (decimal == -1)
        return val*sign;
    while (total > decimal)
    {
        val /= 10;
        total--;
    }
    
    return val*sign;
}


/*
============
COM_SkipPath
============
*/
char *COM_SkipPath (char *pathname)
{
    char    *last;
    
    last = pathname;
    while (*pathname)
    {
        if (*pathname=='/')
            last = pathname+1;
        pathname++;
    }
    return last;
}

/*
============
COM_StripExtension
============
*/
void COM_StripExtension (char *in, char *out)
{
    while (*in && *in != '.')
        *out++ = *in++;
    *out = 0;
}

/*
============
COM_FileExtension
============
*/
char *COM_FileExtension (char *in)
{
    static char exten[8];
    int     i;

    while (*in && *in != '.')
        in++;
    if (!*in)
        return "";
    in++;
    for (i=0 ; i<7 && *in ; i++,in++)
        exten[i] = *in;
    exten[i] = 0;
    return exten;
}

/*
============
COM_FileBase
============
*/
void COM_FileBase (char *in, char *out)
{
    char *s, *s2;
    
    s = in + strlen(in) - 1;
    
    while (s != in && *s != '.')
        s--;
    
    for (s2 = s ; *s2 && *s2 != '/' ; s2--)
    ;
    
    if (s-s2 < 2)
        strcpy (out,"?model?");
    else
    {
        s--;
        strncpy (out,s2+1, s-s2);
        out[s-s2] = 0;
    }
}


/*
==================
COM_DefaultExtension
==================
*/
void COM_DefaultExtension (char *path, char *extension)
{
    char    *src;
//
// if path doesn't have a .EXT, append extension
// (extension should include the .)
//
    src = path + strlen(path) - 1;

    while (*src != '/' && src != path)
    {
        if (*src == '.')
            return;                 // it has an extension
        src--;
    }

    strcat (path, extension);
}

//============================================================================

char        com_token[1024];
int     com_argc;
char    **com_argv;
char    *com_args_original;  // kazik


/*
==============
COM_Parse

Parse a token out of a string
==============
*/
char *COM_Parse (char *data)
{
    int     c;
    int     len;
    
    len = 0;
    com_token[0] = 0;
    
    if (!data)
        return NULL;

// skip whitespace
skipwhite:
    //while ( (c = *data) <= ' ')
    while ( c=*data, (c != 13) && (isspace2(c) || c==0) )    // kazik
    {
        if (c == 0)
            return NULL;            // end of file;
        data++;
    }
    
// skip // comments
    if (c=='/' && data[1] == '/')
    {
        while (*data && *data != '\n')
            data++;
        goto skipwhite;
    }

// handle quoted strings specially
    if (c == '\"')
    {
        data++;
        while (1)
        {
            c = *data++;
            if (c=='\"' || !c)
            {
                com_token[len] = 0;
                return data;
            }
            com_token[len] = c;
            len++;
        }
    }

// parse a regular word
    do
    {
        com_token[len] = c;
        data++;
        len++;
        c = *data;
    } while (c && !isspace2(c));  // kazik
    //} while (c>32);
    
    com_token[len] = 0;
    return data;
}


/*
============
va

does a varargs printf into a temp buffer, so I don't need to have
varargs versions of all text functions.
FIXME: make this buffer size safe someday
============
*/
char    *va(char *format, ...)
{
    va_list     argptr;
    static char     string[8192];   // kazik: was 1024, mayby more?
    
    va_start (argptr, format);
    vsprintf (string, format,argptr);
    va_end (argptr);

    return string;  
}

long strhex2long(char *s)
{
		return strtoul(s,NULL,16);
}

/// just for debugging
int memsearch (byte *start, int count, int search)
{
    int     i;
    
    for (i=0 ; i<count ; i++)
        if (start[i] == search)
            return i;
    return -1;
}


static char *date = __DATE__ ;
static char *mon[12] = 
{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static char mond[12] = 
{ 31,    28,    31,    30,    31,    30,    31,    31,    30,    31,    30,    31 };

// returns days since 1 March 2004
int build_number( void )
{
    int m = 0; 
    int d = 0;
    int y = 0;
    static float b = 0;

    if (b != 0)
        return b;

    for (m = 0; m < 11; m++)
    {
        if (Q_strncasecmp( &date[0], mon[m], 3 ) == 0)
            break;
        d += mond[m];
    }

    d += atoi( &date[4] ) - 1;

    y = atoi( &date[7] ) - 1900;

    b = d + (int)((y - 1) * 365.25);

    if (((y % 4) == 0) && m > 1)
    {
        b += 1;
    }

	//В дробную часть добавляем часы
	/*
	time = strtok(time,":");
	h = atoi(time);
	b += 0.01*h;
	*/

    b -= 37679; //1 March 2004

	

    return b;
}

// kazik -->
int isspace2(int c)
{
    if (c == 0x09 || c == 0x0D || c == 0x0A || c == 0x20)
        return 1;
    return 0;
}
// kazik <--


