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
	map.cpp
    Модуль карты. Здесь много лишнего кода, относящегося к зачаткам бота. Навигация по
	вейпоинтам и прочее.
*/
#include "map.h"
#include "config_manager.h"

CMap m_Map; //Объект карты
ListMapItems MapItems; //Список предметов на карте

CMap::CMap()
{
	strcpy(CRC32,"");
}

void CMap::EVENT_MapChanged()
{
	char newCRC32[20];
	strcpy(newCRC32,fnP_DLL_GetSystemVariable("mapcrc32"));
	if (strcmp(CRC32,newCRC32)) //Новая карта
	{
	LoadMapConfig(NULL);
	//Загружает pqrmod\configs\maps\имя_карты.cfg
	//Ещё один обходной манёвр. Вообще говоря эта функция вызывается
	//по цепочке C_DLL_EVENT_MapChanged() -> LoadMap() -> LoadMapConfig()
	//Однако в настройках карты могу содержаться указания mb_off, и следовательно
	//их необходимо выполнить до fnP_DLL_SetStatusOnOff(i,bool) (т.е. до Multibot.EVENT_BeginGame() в глобальном смысле)
		LoadLocs();
		strcpy(CRC32,newCRC32);
	}
	strcpy(name,fnP_DLL_GetSystemVariable("mapname"));
}

//============================================================
//	Загружает карту в соответствии с системной переменной mapcrc32 или mapname
//============================================================

/*
void LoadMap()
{
	char* crc = fnP_DLL_GetSystemVariable("mapcrc32");
	if (m_Map != NULL) free(m_Map);
	if (!strcmp(MAP_DM2_CRC32,crc)) m_Map = new CMap(1);
	else
	if (!strcmp(MAP_TOURNEY1_CRC32,crc)) m_Map = new CMap(2);
	else
	if (!strcmp(MAP_TOURNEY4_CRC32,crc)) m_Map = new CMap(3);
	else
	if (!strcmp("tourney1",fnP_DLL_GetSystemVariable("mapname"))) m_Map = new CMap(2);
	else if (!strcmp("dm2",fnP_DLL_GetSystemVariable("mapname"))) m_Map = new CMap(1);
	else m_Map = new CMap(-1);
	LoadMapConfig(NULL);
}
*/

CLocation::CLocation()
{
	text = NULL;
}

CLocation::CLocation(char* str, int x1, int y1)
{
	x = x1;
	y = y1;
	if (str!=NULL)
	{
		text = new char[strlen(str)+1];
		strcpy(text,str);
	}
	else
	{
		text = new char[1];
		text = "\0";
	}
}

CLocation::~CLocation()
{
	if (text!=NULL) delete[] text; 
}

CItem::CItem()
{
	respawntime = 0;
	mod_respawntime = 0;
	brick_x = 0;
	brick_y = 0;
	image = IT_NONE;
}

CItem::CItem(int x, int y, int img)
{
	respawntime = 0;
	mod_respawntime = 0;
	brick_x = x;
	brick_y = y;
	image = img;

	//DEBUG PRINT
	//Console.printf("%d %d %d",brick_x,brick_y,img);
}
bool CItem::UpdateItem() //Обновляет переменную времени до респауна данного итема
{
	
	
	bool result = false;
	TBrick* b = NULL;	
	b = fnP_DLL_GetBrickStruct(brick_x,brick_y);

	respawntime = b->respawntime;
	return true;
	
	//Возвращает true, если предмет был только что взят
	//mod_respawntime--;
	/*
	if (respawntime == 0 || mod_respawntime<=0) //В предыдущем кадре предмет был на карте или вышло время отводимое на его респаун
	{
		//DEBUG PRINT
		//Console.printf("%d",b->respawntime);
		if (b->respawntime >= 2)
			result = true;
		
		if (IsClient) //На клиенте нужно вручную подсчитывать время респауна предмета, ибо b->respawntime всегда = 2, если предмет взят
		{
			if ((b->img>=1 && b->img<=3) || (b->img>=8 && b->img<=14) || b->img==6 || b->img==16 || b->img==19 || b->img==20) mod_respawntime = 1000;
			else if (b->img==5 || b->img==17 || b->img==18 || b->img==21) mod_respawntime = 1500;
			else if (b->img==21 || b->img==4) mod_respawntime = 2000;
			else if (b->img==22 || b->img==15) mod_respawntime = 3000;
			else if (b->img==7) mod_respawntime = 5000;
			else if (b->img>22) mod_respawntime = 6000;
		}
	}
	respawntime = b->respawntime;

	if (!IsClient) mod_respawntime = respawntime;

	return result;*/
}

bool CMap::LoadLocs()
{
	//Удаляем старую информацию о локациях на карте
	if (Locs.size()!=0) //Перебираем в цикле все предметы
	{
		Locs.top();
		do
		{
			delete Locs.cur()->d;
			Locs.delCur();
		}
		while(Locs.size()!=0);
	}

	char path[255];
	strcpy(path,NFKGameDir);
	strcat(path,"\\locs\\");
	strcat(path,fnP_DLL_GetSystemVariable("mapname"));
	strcat(path,".loc");

	//DEBUG PRINT
	//Console.print(path);

	FILE* f = fopen(path,"rt");
	if (f==NULL) return false;
	
	char str[1024];
	char* p;
	while(fgets(str,1024,f))
	{
		//Удаляем символ конца строки из текста
		p=NULL;
		p = strstr(str,"\r");
		if (p!=NULL) p[0] = '\0';

		//Разбиваем строку на "число число имя_локации"
		int x = 0;
		int y = 0;
		p = strtok(str," ");
		x = atoi(p);
		p = strtok(NULL," ");
		y = atoi(p);
		p = strtok(NULL,"\n");

		//Создаём новый элемент в списке локаций
		CLocation* NewLoc = new CLocation(p,x,y);
		ItemLocation* NewItemLoc = new ItemLocation;
		NewItemLoc->d = NewLoc;
		Locs.addAftLas(NewItemLoc);
	}

	fclose(f);
	return true;
}

char* CMap::GetLoc(int x, int y) //Возвращает ближайшую к указанным координатам локацию
{
	if (Locs.size() == 0) return NULL;
	char* result = NULL;
	double min_dist = 1000;
	double dist;

	
	Locs.top();
	do
	{
		dist = GetDist(Locs.cur()->d->x,Locs.cur()->d->y,x,y);
		if (dist<min_dist)
		{
			min_dist = dist;
			result = Locs.cur()->d->text;
		}
	}
	while(Locs.next());
	return result;
	
}

void CMap::ScanItems()
{	
	//Удаляем старую информацию о предметах на карте
	if (MapItems.size()!=0) //Перебираем в цикле все предметы
	{
		MapItems.top();
		do
		{
			delete MapItems.cur()->d;
			MapItems.delCur();
		}
		while(MapItems.size()!=0);
	}
	
	//Обновляем данные о размере карты	
	char* tmp_s;
	tmp_s = fnP_DLL_GetSystemVariable("bricks_x");

	sizex = atoi(tmp_s);
	tmp_s = fnP_DLL_GetSystemVariable("bricks_y");
	sizey = atoi(tmp_s);

    //Сканируем карту и запоминаем предметы с кодом от 1 до 28 - предметы которые можно взять
	TBrick* b = NULL;
	int tmp_img;
	for(int i = 0; i<sizex; i++)
		for(int j = 0; j<sizey; j++)
		{
			b = fnP_DLL_GetBrickStruct(i,j);
			tmp_img = b->img;
			if (tmp_img >= 1 && tmp_img <= 28)
			{
				CItem* NewItem = new CItem(i,j,tmp_img);
				ItemMapItem* NewMapItem = new ItemMapItem;
				NewMapItem->d = NewItem;
				MapItems.addAftLas(NewMapItem);
			}
		}
}
/*
CPath* CMap::FindPath(int from_x, int from_y, int fin_WP)
{
	//DEBUG PRINT
	//fnP_DLL_AddMessageXY(from_x,from_y,"Player x y");

	//Поиск пути от точки x,у до указанного WP
	//Результатом является указатель на объект типа CPath, который включает список WP и длина пути в бриках
	CPath* P = new CPath;
	P->WP = new CList;
	int start_WP = FindNearestWP(from_x,from_y);
	int* pIntVar = new int;
	pIntVar[0] = start_WP;
	P->WP->AddBottom(pIntVar); //WP ближайший к x,y является первым в списке пути

	//Если игрок уже находится возле финишного WP
	if (start_WP == fin_WP) {
		P->time = 1;
		return P;
	}

	int* trace_value = new int[1000]; //[number_WPs];
	int i,j,k,Ni,time;
	bool exit_flag = false;

	//DEBUG PRINT
	//fnP_DLL_AddMessageXY(start_WP,fin_WP,"^2(CMap::FindPath) Start WP; fin WP");

/* Волновой алгоритм поиска пути.
1. Снaчaлa необходимо создaть рaбочий мaссив R(MxN),рaвный по рaзмеру мaссиву игрового поля P(MxN). 
2. Кaждому элементу рaбочего мaссивa R(i,j) присвaивaется некоторое знaчение в зaвисимости от свойств элементa игрового поля P(i,j) по следующим правилам: 
a. Если поле P(i,j) непроходимо, то R(i,j):=255; 
b. Если поле P(i,j) проходимо, то R(i,j):=254; 
c. Если поле P(i,j) является целевой (финишной) позицией, то R(i,j):=0; 
d. Если поле P(i,j) является стaртовой позицией, то R(i,j):=253. 
3. Этaп "Рaспрострaнения волны". Вводим переменную Ni - счётчик итерaций (повторений) и присвaивaем ей нaчaльное знaчение 0. 
4. Вводим констaнту Nк,которую устaнaвливaем рaвной мaксимaльно возможному числу итерaций. 
5. Построчно просмaтривaем рaбочий мaссив R (т.е.оргaнизуем двa вложенных циклa: по индексу мaссивa i от 1 до М, по индексу мaссивa j от 1 до N). 
6. Если R(i,j) рaвен Ni,то просмaтривaются соседние элементы R(i+1,j), R(i-1,j), R(i,j+1), R(i,j-1) по следующему прaвилу (в кaчестве примерa рaссмотрим R(i+1,j): 
a. Eсли R(i+1,j)=253, то переходим к пункту 10; 
b. Eсли R(i+1,j)=254, выполняется присвaивaние R(i+1,j):=Ni+1; 
c. Во всех остaльных случaях R(i+1,j) остaётся без изменений. 
Aнaлогично поступaем с элементaми R(i-1,j), R(i,j+1),R(i,j-1). 

7. По зaвершению построчного просмотрa всего мaссивa увеличивaем Ni нa 1. 
8. Если Ni>Nк,то поиск мaршрутa признаётся неудачным. Выход из программы. 
9. Переходим к пункту 5. 
10. Этaп построения мaршрутa перемещения. Присвaивaем переменным Х и Y знaчения координaт стaртовой позиции. 
11. В окрестности позиции R(Х,Y) ищем элемент с нaименьшим знaчением (т.е.для этого просмaтривaем R(Х+1,Y), R(Х-1,Y), R(Х,Y+1), R(Х,Y-1). Координaты этого элементa зaносим в переменные X1 и Y1. 
12. Совершaем перемещение объектa (кто тaм у вaс будет - робот, aквaнaвт, Винни-Пух) по игровому полю из позиции [X,Y] в позицию [X1,Y1]. (По желaнию, вы можете предвaрительно зaносить координaты X1,Y1 в некоторый мaссив, и, только зaкончив построение всего мaршрутa,зaняться перемещением героя нa экрaне). 
13. Если R(X1,Y1)=0,то переходим к пункту 15. 
14. Выполняем присвaивaние X:=X1,Y:=Y1. Переходим к пункту 11. 
15. Всё !!!
*/
  //Устанавливаем всем точкам WP значение 6001.
/*	for(i=0;i<number_WPs;i++)
	{
		trace_value[i]=6001;
	}
	trace_value[fin_WP]=0;
	trace_value[start_WP]=6000; //Стартовая точка

	for(Ni=0;Ni<__MAX_WAY_POINTS__;Ni++)
	{
		for(i=0;(i<number_WPs)&&(!exit_flag);i++)
		{
			if (trace_value[i]==Ni) //Находим точку у кторой t_v = текущему значению Ni
				for(j=0;(j<WPs[i].number_InWPs)&&(!exit_flag);j++) {//Установим для всех WP из которыйх можно дойти до текущего в Ni + время прохода
					if (trace_value[WPs[i].InWPs[j]] == 6000) //Один из соседних WP является стартовой точкой!
						exit_flag=true;
					else {
						//Найдём время прихода в данный WP
						time = 1;
						for(k=0;k<WPs[WPs[i].InWPs[j]].number_OutWPs;k++)
							if (WPs[WPs[i].InWPs[j]].OutWPs[k] == i) time=WPs[WPs[i].InWPs[j]].OutWPsTime[k];

						if (trace_value[WPs[i].InWPs[j]] == 6001) trace_value[WPs[i].InWPs[j]] = Ni + time;
					}
				}
		}
	}
	
	if (!exit_flag) { //Если вейпоинты правильно расставлены, то это никогда не должно выполниться
		fnP_DLL_AddMessage("^1ERROR: ^2(CMap::FindPath) ^7Path NOT found on first etap");
		fnP_DLL_AddMessageXY(start_WP,fin_WP,"Start WP; fin WP");
		return NULL;
	}
	i=start_WP;
	int min=6001;
	int min_WP=start_WP;
	int min_j=0;
	P->time = 0;
	int PATH_SEARCH_iterator_counter = 0;
	#define __MAX_PATH_SEARCH_iterator_counter__ 1000 //Максимальное количество WP в пути.
	//Если счётчик достигнет этого значения, то считается что поиск зациклился => ошибка в простановке WP.


	//DEBUG PRINT
	//fnP_DLL_AddMessageXY(start_WP,fin_WP,"Start WP; fin WP");
	//DEBUG PRINT ALL TRACE VALUES
	/*for(j=0;j<number_WPs;j++) {
		fnP_DLL_AddMessageXY(j,trace_value[j],"WP; trace value");
	}*/
	
/*	do {
		PATH_SEARCH_iterator_counter++;
		//DEBUG PRINT
		//fnP_DLL_AddMessageXY("min; min_WP",min,min_WP);
		for(j=0;j<WPs[i].number_OutWPs;j++) //Поиск соседнего WP с минимальным t_v
			if (trace_value[WPs[i].OutWPs[j]]<min) {
				min = trace_value[WPs[i].OutWPs[j]];

				//DEBUG PRINT
				//fnP_DLL_AddMessageXY(i,j,"WP; i");

				min_WP = WPs[i].OutWPs[j];
				min_j = j;
			}
		if (i==min_WP) {
			fnP_DLL_AddMessage("^1ERROR: ^2(CMap::FindPath) ^7Path NOT found by trace values");
			fnP_DLL_AddMessageXY(start_WP,fin_WP,"Start WP; fin WP");
			//fnP_DLL_AddMessageXY(i,min,"i,min");
			//fnP_DLL_AddMessageXY(j,0,"j,0");

			//Освобождаем память занятую фрагментами ненйденного пути
			free(P);
			return NULL;
		}
		P->time += WPs[i].OutWPsTime[min_j];
		i=min_WP;
		pIntVar = new int;
		pIntVar[0] = i;
		P->WP->AddBottom(pIntVar); //Заносим найденный WP в список пути
	}
	while ((i!=fin_WP)&&(PATH_SEARCH_iterator_counter < __MAX_PATH_SEARCH_iterator_counter__));
	if (PATH_SEARCH_iterator_counter >= __MAX_PATH_SEARCH_iterator_counter__) {
		fnP_DLL_AddMessage("^1ERROR: ^2(CMap::FindPath) ^7Path NOT found because PATH_SEARCH_iterator_counter >= __MAX_PATH_SEARCH_iterator_counter__");
		//Освобождаем память занятую фрагментами ненйденного пути
		free(P->WP);
		free(P);
		return NULL;
	}
	return P;
}

int CMap::FindNearestWP(int x,int y) //Ищет ближайший WP к данной точке
{
	double min=10000;
	int min_i=1;
	double xc,yc;
	double direct_path;
	for(int i=0;i<number_WPs;i++) {
		xc = WPs[i].GetCenterX();
		yc = WPs[i].GetCenterY();
		direct_path=sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc));
		if (direct_path < min) {
			min = direct_path;
			min_i = i;
		}
	}

	//DEBUG PRINT
	//fnP_DLL_AddMessageXY(min_i,(int)direct_path,"min_i,(int)direct_path");

	return min_i;
}

void CMap::PutMapInWPs()
{
	int TempInWPs[__MAX_WAY_POINTS__][255]; //255 т.к. для входящих и исходящих WP количество опредлено как BYTE
	BYTE TempCountInWPs[__MAX_WAY_POINTS__];
	int i,j;

	for(i=0; i<__MAX_WAY_POINTS__; i++) {
		TempCountInWPs[i] = 0;
	}

	for(i=0; i<number_WPs; i++) {
		for(j=0; j<WPs[i].number_OutWPs; j++) {
			TempInWPs[WPs[i].OutWPs[j]][TempCountInWPs[WPs[i].OutWPs[j]]] = i;
			TempCountInWPs[WPs[i].OutWPs[j]]++;
		}
	}

	for(i=0;i<number_WPs; i++) {
		WPs[i].number_InWPs = TempCountInWPs[i];
		WPs[i].InWPs = new int[WPs[i].number_InWPs];
		for(j=0;j<TempCountInWPs[i];j++) {
			WPs[i].InWPs[j] = TempInWPs[i][j];
			//DEBUG PRINT
			//fnP_DLL_AddMessageXY(i,TempInWPs[i][j],"WPs[i].InWP");
		}
	}
}
int CMap::GetTimeMoving(int start_WP,int fin_WP)
{
	if (start_WP == fin_WP) return 0;
	for(int j=0;j<WPs[start_WP].number_OutWPs;j++) {
		if (WPs[start_WP].OutWPs[j] == fin_WP) return WPs[start_WP].OutWPsTime[j];
	}
	
	fnP_DLL_AddMessageXY(start_WP,fin_WP,"^1ERROR: ^2(CMap::GetTimeMoving) ^7Way points not neberhoods. Start_WP, Fin_WP:");
	return 999;
}
CPath::CPath()
{
	WP = new CList();
	time = 0;
}

CPath::~CPath()
{
	if (WP!=NULL) free(WP);
}

bool CMap::IsOnWP(int point_x, int point_y, int WP)
{
	//DEBUG IF
	/*
	if (m_Map->number_WPs - 1 < WP) {
		fnP_DLL_AddMessage("^1ERROR: ^2(CMap::IsOnWp) ^7 WP out of range. number_WPs -1 < WP");
		return false;
	}*/
	//END DEBUG IF
/*
	if ((WPs[WP].x<=point_x)&&(point_x<=WPs[WP].x2)&&(WPs[WP].y<=point_y)&&(point_y<=WPs[WP].y2)) return true;
	return false;
}
*/
bool InIs(double i,double m1,double m2)
{
	if (m1<m2) return ((m1<=i)&&(i<=m2));
	else return ((m2<=i)&&(i<=m1));
}

double GetDist(int x1,int y1,int x2,int y2)
{
	return sqrt(double((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
}
/*
bool CMap::TraceVector(int x1,int y1,int x2,int y2)
{
// ==================
// Проверяет столкновение с бриками
// true если вектор проводим с x1,y1 до x2,y2
// ==================
	WORD bounds[4];
	WORD start[4]; //  Left, Up, Right, Down
    int i,j;
    double zA,zB,nX,nY; // Y:=zA*X+zB;

    if ((x2==x1)&&(y2==y1)) return true;

    if(x1==x2) zA=0; else zA=(y2-y1)/(x2-x1);
	zB=y2-zA*x2;
    if (x1<=x2) {
		start[0] = x1/32;
		start[2] = x2/32;
	}
    else {
		start[0] = x2/32;
		start[2] = x1/32;
	}
    if (y1<=y2) {
		start[1] = y1/16;
		start[3] = y2/16;
	}
    else {
		start[1] = y2/16;
		start[3] = y1/16;
	}
    
	for(i=start[0];i<=start[2];i++)
		for(j=start[1];j<=start[3];j++)
			if (fnP_DLL_GetBrickStruct(i,j)->block) {
				bounds[0] = i*32;
                bounds[1] = j*16;
                bounds[2] = bounds[0]+32;
                bounds[3] = bounds[1]+16;
                // calculating X
                if (x2!=x1) {
					if (x1<x2) nX = bounds[0]; else nX = bounds[2];
                    if (InIs(nX, x1, x2)) {
						nY=zA*nX+zB;
                        if (InIs(nY, bounds[1], bounds[3])) return false;
					}
                }
                // calculating Y
                if (y2!=y1) {
					if (y2>y1) nY=bounds[1]; else nY=bounds[3];
					if (InIs(nY, y1, y2)) {
						if (zA==0) nX=x1; else nX=(nY-zB)/zA;
						if (InIs(nX, bounds[0], bounds[2])) return false;
					}
                }
			}
     return true;
}
*/
/*
Расстояние между 2мя точками
function Get2Ddist(x1,y1, x2, y2 : single): word; begin result := round(sqrt(sqr(x2 - x1)+sqr(y2 - y1))); end;

*/