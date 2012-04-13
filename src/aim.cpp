#include "aim.h"

#include "ai.h"
#include "proc_hack.h"


CAim Aim;
void AIM_EVENT_MainLoop()
{
	//DEBUG PRINT
	//fnP_DLL_AddMessage("AIM_EVENT_MainLoop()");

	if (Aim.IsVisible()) Aim.Draw();
}

bool AIM_EVENT_ConsoleCommand(CList* _Params)
{
#ifndef NO_CHEAT
	if (strstr((char*)_Params->GetCur(),"+aim")!=NULL)
	{
		Aim.Show();
	}
	else
	if (strstr((char*)_Params->GetCur(),"-aim")!=NULL) 
	{
		Aim.Hide();
	}
	else
	if (!strcmp((char*)_Params->GetCur(),"autoaim")!=NULL) 
	{
		Aim.AutoAim();
	}
	else return false; //������������ �������� false ��������� ��������� ����������� ���������� ������ �� ��, ��� ������ AIM �� ����� ���������� ������ �������
#endif //NO_CHEAT

#ifdef NO_CHEAT
	return false;
#endif //NO CHEAT

	return true; //���������� true, ����� �������� ���������� ���������� ������ EVENT_ConsoleCommand �����, ��� ������� ���������� ������� AIM
}

void AIM_RegisterConsoleCommands()
{
#ifndef NO_CHEAT
	fnP_DLL_RegisterConsoleCommand("autoaim");
	fnP_DLL_RegisterConsoleCommand("+aim");
#endif //NO_CHEAT
}

void AIM_ConfigSave(FILE* _f)
{
}

bool AIM_ConfigLoad(CList* __Params)
{
	return AIM_EVENT_ConsoleCommand(__Params);
}

CAim::CAim()
{
	visible = false;
}
CAim::~CAim()
{
}

void CAim::Draw()
{
	if (!PH_UpdatePlayerEX(0)) return; //������� � ���������� ���������� PlayerEX ���������� � ������� ������ (��������)

	//DEBUG PRINT
	//fnP_DLL_AddMessageX(floor(PlayerEX.fangle*100),"f: ");

	int dir,ydir;
	if (PlayerEX.fangle >127) dir=-1; //����� ������� �����
	else dir=1;

	if (PlayerEX.fangle <64 || PlayerEX.fangle > 191 ) ydir=-1; //����� ������� �����
	else ydir=1;

	//��������, ��� ������� ���������� �����, ������ � Delphi �����-�� ����������
	if (PlayerEX.fangle >170 && PlayerEX.fangle <234) PlayerEX.fangle++;
	else
	if (PlayerEX.fangle > 31 && PlayerEX.fangle < 97) PlayerEX.fangle--;


	double a = M_PI_2+PlayerEX.fangle*M_PI/127; 
	double k = tanl(a); //����������� ������� ������
	

	double x,y;

	if (PlayerEX.crouch) PlayerEX.y -= 4;
	else PlayerEX.y -= 10;

	if (abs(k)<=1)
	{
		x = PlayerEX.x+dir*25;
		y = PlayerEX.y+k*dir*25;
	}
	else
	{
		x = PlayerEX.x+ydir*25/k;
		y = PlayerEX.y+ydir*25;
	}



	int xend,yend;
	if (abs(k)<=1)
	{
		xend=floor(x+dir*640);
		yend=floor(y+k*dir*640);
	}
	else
	{
		yend=floor(y+ydir*480);
		xend=floor(x+ydir*480/k);
	}
	
	fnP_DLL_FX_Line(floor(x),floor(y),xend,yend,0x99AAAAFF,0x102,true);
	return;


	//DEBUG PRINT
	//fnP_DLL_AddMessageX(floor(x),"x: ");
	//fnP_DLL_AddMessageX(floor(y),"y: ");
}

void CAim::AutoAim()
{
	//�������� �� �������
	bool IsTeamplay=false;
	char GameType[100];
	strcpy(GameType,fnP_DLL_GetSystemVariable("gametype"));
	
	//DEBUG PRINT
	fnP_DLL_AddMessage(GameType);

	if (!strcmp(GameType,"Teamplay")) IsTeamplay=true;

	if (!PH_UpdatePlayerEX(0)) return; //�������� ��������� ������
	BYTE PlayerTeam=PlayerEX.team; //�������� ������� ������
	double PlayerX = PlayerEX.x;
	double PlayerY = PlayerEX.y;
	WORD PlayerDXID = PlayerEX.DXID;

	//DEBUG PRINT
	fnP_DLL_AddMessage("name");
	fnP_DLL_AddMessage(PlayerEX_netname);

	//����������� �� ���� ����� � �������, ��� ������ ����������
	double min_range=1000000;
	double min_x,min_y;
	long range;
	if (!m_Bots.IsEmpty()) {
		m_Bots.MoveTop();
		do 
		{
			if (!PH_UpdatePlayerEX(((CBot*)m_Bots.GetCur())->GetDXID())) continue;
			if (!IsTeamplay || PlayerEX.team!=PlayerTeam)
			{
				range = sqrtl((PlayerEX.x-PlayerX)*(PlayerEX.x-PlayerX)+(PlayerEX.y-PlayerY)*(PlayerEX.y-PlayerY));
				if (range<min_range)
				{
					min_range=range;
					min_x = PlayerEX.x;
					min_y = PlayerEX.y;
				}
			}
		}
		while(m_Bots.MoveNext());
	}

	if (!m_Players.IsEmpty()) {
		m_Players.MoveTop();
		m_Players.MoveNext();
		do 
		{
			if (!PH_UpdatePlayerEX(((CPlayer*)m_Players.GetCur())->GetDXID())) continue;
			if (!IsTeamplay || PlayerEX.team!=PlayerTeam)
			{
				range = sqrtl((PlayerEX.x-PlayerX)*(PlayerEX.x-PlayerX)+(PlayerEX.y-PlayerY)*(PlayerEX.y-PlayerY));
				
				//DEBUG PRINT
				//fnP_DLL_AddMessageX(floor(range),"range: ");

				if (range<min_range)
				{
					min_range=range;
					min_x = PlayerEX.x;
					min_y = PlayerEX.y;
				}
			}
		}
		while(m_Players.MoveNext());
	}

	//�������� min_x � min_y - ���������� ������ �� �������� ��������
	double angle=atan2(min_x-PlayerX,min_y-PlayerY);
	angle += M_PI_2;
	angle = angle/M_PI*127;

	if (min_x<PlayerX && angle<127) angle+=127;

	fnP_DLL_AddMessageX(floor(angle),"angle");

	if (PH_SetPlayerAngle(0,angle)) fnP_DLL_AddMessage("true");	//PlayerDXID
	else fnP_DLL_AddMessage("false");
}