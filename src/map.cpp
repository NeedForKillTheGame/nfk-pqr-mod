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
    ������ �����. ����� ����� ������� ����, ������������ � �������� ����. ��������� ��
	���������� � ������.
*/
#include "map.h"
#include "config_manager.h"

CMap m_Map; //������ �����
ListMapItems MapItems; //������ ��������� �� �����

CMap::CMap()
{
	strcpy(CRC32,"");
}

void CMap::EVENT_MapChanged()
{
	char newCRC32[20];
	strcpy(newCRC32,fnP_DLL_GetSystemVariable("mapcrc32"));
	if (strcmp(CRC32,newCRC32)) //����� �����
	{
	LoadMapConfig(NULL);
	//��������� pqrmod\configs\maps\���_�����.cfg
	//��� ���� �������� �����. ������ ������ ��� ������� ����������
	//�� ������� C_DLL_EVENT_MapChanged() -> LoadMap() -> LoadMapConfig()
	//������ � ���������� ����� ���� ����������� �������� mb_off, � �������������
	//�� ���������� ��������� �� fnP_DLL_SetStatusOnOff(i,bool) (�.�. �� Multibot.EVENT_BeginGame() � ���������� ������)
		LoadLocs();
		strcpy(CRC32,newCRC32);
	}
	strcpy(name,fnP_DLL_GetSystemVariable("mapname"));
}

//============================================================
//	��������� ����� � ������������ � ��������� ���������� mapcrc32 ��� mapname
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
bool CItem::UpdateItem() //��������� ���������� ������� �� �������� ������� �����
{
	
	
	bool result = false;
	TBrick* b = NULL;	
	b = fnP_DLL_GetBrickStruct(brick_x,brick_y);

	respawntime = b->respawntime;
	return true;
	
	//���������� true, ���� ������� ��� ������ ��� ����
	//mod_respawntime--;
	/*
	if (respawntime == 0 || mod_respawntime<=0) //� ���������� ����� ������� ��� �� ����� ��� ����� ����� ��������� �� ��� �������
	{
		//DEBUG PRINT
		//Console.printf("%d",b->respawntime);
		if (b->respawntime >= 2)
			result = true;
		
		if (IsClient) //�� ������� ����� ������� ������������ ����� �������� ��������, ��� b->respawntime ������ = 2, ���� ������� ����
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
	//������� ������ ���������� � �������� �� �����
	if (Locs.size()!=0) //���������� � ����� ��� ��������
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
		//������� ������ ����� ������ �� ������
		p=NULL;
		p = strstr(str,"\r");
		if (p!=NULL) p[0] = '\0';

		//��������� ������ �� "����� ����� ���_�������"
		int x = 0;
		int y = 0;
		p = strtok(str," ");
		x = atoi(p);
		p = strtok(NULL," ");
		y = atoi(p);
		p = strtok(NULL,"\n");

		//������ ����� ������� � ������ �������
		CLocation* NewLoc = new CLocation(p,x,y);
		ItemLocation* NewItemLoc = new ItemLocation;
		NewItemLoc->d = NewLoc;
		Locs.addAftLas(NewItemLoc);
	}

	fclose(f);
	return true;
}

char* CMap::GetLoc(int x, int y) //���������� ��������� � ��������� ����������� �������
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
	//������� ������ ���������� � ��������� �� �����
	if (MapItems.size()!=0) //���������� � ����� ��� ��������
	{
		MapItems.top();
		do
		{
			delete MapItems.cur()->d;
			MapItems.delCur();
		}
		while(MapItems.size()!=0);
	}
	
	//��������� ������ � ������� �����	
	char* tmp_s;
	tmp_s = fnP_DLL_GetSystemVariable("bricks_x");

	sizex = atoi(tmp_s);
	tmp_s = fnP_DLL_GetSystemVariable("bricks_y");
	sizey = atoi(tmp_s);

    //��������� ����� � ���������� �������� � ����� �� 1 �� 28 - �������� ������� ����� �����
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

	//����� ���� �� ����� x,� �� ���������� WP
	//����������� �������� ��������� �� ������ ���� CPath, ������� �������� ������ WP � ����� ���� � ������
	CPath* P = new CPath;
	P->WP = new CList;
	int start_WP = FindNearestWP(from_x,from_y);
	int* pIntVar = new int;
	pIntVar[0] = start_WP;
	P->WP->AddBottom(pIntVar); //WP ��������� � x,y �������� ������ � ������ ����

	//���� ����� ��� ��������� ����� ��������� WP
	if (start_WP == fin_WP) {
		P->time = 1;
		return P;
	}

	int* trace_value = new int[1000]; //[number_WPs];
	int i,j,k,Ni,time;
	bool exit_flag = false;

	//DEBUG PRINT
	//fnP_DLL_AddMessageXY(start_WP,fin_WP,"^2(CMap::FindPath) Start WP; fin WP");

/* �������� �������� ������ ����.
1. ��a�a�a ���������� ����a�� �a����� �a���� R(MxN),�a���� �� �a����� �a����� �������� ���� P(MxN). 
2. �a����� �������� �a������ �a����a R(i,j) �����a��a���� ��������� ��a����� � �a��������� �� ������� �������a �������� ���� P(i,j) �� ��������� ��������: 
a. ���� ���� P(i,j) �����������, �� R(i,j):=255; 
b. ���� ���� P(i,j) ���������, �� R(i,j):=254; 
c. ���� ���� P(i,j) �������� ������� (��������) ��������, �� R(i,j):=0; 
d. ���� ���� P(i,j) �������� ��a������ ��������, �� R(i,j):=253. 
3. ��a� "�a�������a����� �����". ������ ���������� Ni - ������� ����a��� (����������) � �����a��a�� �� �a�a����� ��a����� 0. 
4. ������ �����a��� N�,������� ���a�a����a�� �a���� �a����a���� ���������� ����� ����a���. 
5. ��������� �����a����a�� �a����� �a���� R (�.�.���a������ ��a ��������� ����a: �� ������� �a����a i �� 1 �� �, �� ������� �a����a j �� 1 �� N). 
6. ���� R(i,j) �a��� Ni,�� �����a����a���� �������� �������� R(i+1,j), R(i-1,j), R(i,j+1), R(i,j-1) �� ���������� ��a���� (� �a������ ������a �a�������� R(i+1,j): 
a. E��� R(i+1,j)=253, �� ��������� � ������ 10; 
b. E��� R(i+1,j)=254, ����������� �����a��a��� R(i+1,j):=Ni+1; 
c. �� ���� ���a����� ����a�� R(i+1,j) ���a���� ��� ���������. 
A�a������� ������a�� � �������a�� R(i-1,j), R(i,j+1),R(i,j-1). 

7. �� �a�������� ����������� ��������a ����� �a����a ��������a�� Ni �a 1. 
8. ���� Ni>N�,�� ����� �a�����a ��������� ���������. ����� �� ���������. 
9. ��������� � ������ 5. 
10. ��a� ���������� �a�����a �����������. �����a��a�� ���������� � � Y ��a����� �������a� ��a������ �������. 
11. � ����������� ������� R(�,Y) ���� ������� � �a�������� ��a������ (�.�.��� ����� �����a����a�� R(�+1,Y), R(�-1,Y), R(�,Y+1), R(�,Y-1). �������a�� ����� �������a �a����� � ���������� X1 � Y1. 
12. ������a�� ����������� ������a (��� �a� � �a� ����� - �����, a��a�a��, �����-���) �� �������� ���� �� ������� [X,Y] � ������� [X1,Y1]. (�� ���a���, �� ������ �����a�������� �a������ �������a�� X1,Y1 � ��������� �a����, �, ������ �a������ ���������� ����� �a�����a,�a������ ������������ ����� �a ���a��). 
13. ���� R(X1,Y1)=0,�� ��������� � ������ 15. 
14. ��������� �����a��a��� X:=X1,Y:=Y1. ��������� � ������ 11. 
15. �� !!!
*/
  //������������� ���� ������ WP �������� 6001.
/*	for(i=0;i<number_WPs;i++)
	{
		trace_value[i]=6001;
	}
	trace_value[fin_WP]=0;
	trace_value[start_WP]=6000; //��������� �����

	for(Ni=0;Ni<__MAX_WAY_POINTS__;Ni++)
	{
		for(i=0;(i<number_WPs)&&(!exit_flag);i++)
		{
			if (trace_value[i]==Ni) //������� ����� � ������ t_v = �������� �������� Ni
				for(j=0;(j<WPs[i].number_InWPs)&&(!exit_flag);j++) {//��������� ��� ���� WP �� �������� ����� ����� �� �������� � Ni + ����� �������
					if (trace_value[WPs[i].InWPs[j]] == 6000) //���� �� �������� WP �������� ��������� ������!
						exit_flag=true;
					else {
						//����� ����� ������� � ������ WP
						time = 1;
						for(k=0;k<WPs[WPs[i].InWPs[j]].number_OutWPs;k++)
							if (WPs[WPs[i].InWPs[j]].OutWPs[k] == i) time=WPs[WPs[i].InWPs[j]].OutWPsTime[k];

						if (trace_value[WPs[i].InWPs[j]] == 6001) trace_value[WPs[i].InWPs[j]] = Ni + time;
					}
				}
		}
	}
	
	if (!exit_flag) { //���� ��������� ��������� �����������, �� ��� ������� �� ������ �����������
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
	#define __MAX_PATH_SEARCH_iterator_counter__ 1000 //������������ ���������� WP � ����.
	//���� ������� ��������� ����� ��������, �� ��������� ��� ����� ���������� => ������ � ����������� WP.


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
		for(j=0;j<WPs[i].number_OutWPs;j++) //����� ��������� WP � ����������� t_v
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

			//����������� ������ ������� ����������� ����������� ����
			free(P);
			return NULL;
		}
		P->time += WPs[i].OutWPsTime[min_j];
		i=min_WP;
		pIntVar = new int;
		pIntVar[0] = i;
		P->WP->AddBottom(pIntVar); //������� ��������� WP � ������ ����
	}
	while ((i!=fin_WP)&&(PATH_SEARCH_iterator_counter < __MAX_PATH_SEARCH_iterator_counter__));
	if (PATH_SEARCH_iterator_counter >= __MAX_PATH_SEARCH_iterator_counter__) {
		fnP_DLL_AddMessage("^1ERROR: ^2(CMap::FindPath) ^7Path NOT found because PATH_SEARCH_iterator_counter >= __MAX_PATH_SEARCH_iterator_counter__");
		//����������� ������ ������� ����������� ����������� ����
		free(P->WP);
		free(P);
		return NULL;
	}
	return P;
}

int CMap::FindNearestWP(int x,int y) //���� ��������� WP � ������ �����
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
	int TempInWPs[__MAX_WAY_POINTS__][255]; //255 �.�. ��� �������� � ��������� WP ���������� ��������� ��� BYTE
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
// ��������� ������������ � �������
// true ���� ������ �������� � x1,y1 �� x2,y2
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
���������� ����� 2�� �������
function Get2Ddist(x1,y1, x2, y2 : single): word; begin result := round(sqrt(sqr(x2 - x1)+sqr(y2 - y1))); end;

*/