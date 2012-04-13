#include "alias.h"
#include <string.h>
#include "dll_register.h"
#include "console.h"
#include "pqrmod.h" //Äëÿ NFKVersionI

#ifndef NO_CHEAT
#include "time_help.h"
#include "aim.h"
#endif




/*
bool ALIAS_EVENT_ConsoleCommand(CList* _Params)
{
	if (!strcmp((char*)_Params->GetCur(),"bind"))
		if (_Params->MoveNext())
		{
			char mkey[100];
			strcpy(mkey,(char*)_Params->GetCur());
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
				if (_Params->MoveNext())
				{
					char cmd[200]="";
					strcpy(cmd,(char*)_Params->GetCur());
					while(_Params->MoveNext()) {
						strcat(cmd," ");
						strcat(cmd,(char*)_Params->GetCur());
					}
					K->SetCmd(cmd);
				}
				else
				{
					if (K->GetCmdDown()!=NULL)
					{
						strcat(mkey," binded to ");
						strcat(mkey,K->GetCmdDown());
						fnP_DLL_AddMessage(mkey);
						}
					else
					{
						strcat(mkey," is unbinded");
						fnP_DLL_AddMessage(mkey);
					}
				}
		}
		else fnP_DLL_AddMessage("bind <key> [command]");
	else return false;
	return true;
}
void ALIAS_ConfigSave(FILE* _f)
{
	if (KEY_Mouse5.GetCmdDown()!=NULL) fprintf(_f,"bind mouse5 %s\n",KEY_Mouse5.GetCmdDown());
	if (KEY_Mouse5.GetCmdDown()!=NULL) fprintf(_f,"bind mouse6 %s\n",KEY_Mouse6.GetCmdDown());
	if (KEY_Mouse5.GetCmdDown()!=NULL) fprintf(_f,"bind mouse4 %s\n",KEY_Mouse4.GetCmdDown());
	if (KEY_Mouse5.GetCmdDown()!=NULL) fprintf(_f,"bind mouse7 %s\n",KEY_Mouse7.GetCmdDown());
	if (KEY_Mouse5.GetCmdDown()!=NULL) fprintf(_f,"bind mouse8 %s\n\n",KEY_Mouse8.GetCmdDown());
}
bool ALIAS_ConfigLoad(CList* __Params)
{
	return ALIAS_EVENT_ConsoleCommand(__Params);
}
