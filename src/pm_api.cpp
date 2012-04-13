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
#include "pm_api.h"

#include "menu.h"

typedef struct {
	int x,y,Width;
	int SubMenuMask;
} C_DLL_PMA_TMenu;

PQRMOD_API BYTE C_DLL_PMA_MenuCreate(C_DLL_PMA_TMenu m)
{
	CModMenu* NewMenu = new CModMenu(ExternMenusCount);
	NewMenu->SetPosition(m.x,m.y);
	NewMenu->SetWidth(m.Width);
	NewMenu->SetSubMenuMask(m.SubMenuMask);
	ExternMenus[ExternMenusCount]=NewMenu;
	if (ExternMenusCount<99) ExternMenusCount++;
	return ExternMenusCount-1;
}

PQRMOD_API void C_DLL_PMA_MenuSets(int mid, C_DLL_PMA_TMenu m)
{
	if (mid>=ExternMenusCount) return;
	ExternMenus[mid]->SetPosition(m.x,m.y);
	ExternMenus[mid]->SetWidth(m.Width);
	ExternMenus[mid]->SetSubMenuMask(m.SubMenuMask);
}
PQRMOD_API void C_DLL_PMA_MenuAddOption(int mid,char *s)
{
	if (mid>=ExternMenusCount) return;
	ExternMenus[mid]->AddOption(s);
}
PQRMOD_API void C_DLL_PMA_MenuSetOption(int mid, int oid, char* s)
{
	if (mid>=ExternMenusCount) return;
	ExternMenus[mid]->SetOption(oid,s);
}
PQRMOD_API void C_DLL_PMA_DeleteAllOptions(int mid)
{
	if (mid>=ExternMenusCount) return;
	ExternMenus[mid]->RemoveAllOptions();
}
PQRMOD_API void C_DLL_PMA_MenuShowHide(int mid,bool vis)
{
	if (mid>=ExternMenusCount) return;
	if (vis) ExternMenus[mid]->Show();
	else ExternMenus[mid]->Hide();
}