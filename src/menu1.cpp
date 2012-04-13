#include "menu.h"
#include "animation.h"
#include "input.h"
#include "main_menu.h"

//#include "pqrmod.h"

#define _NFK_FONT_ 1
#define HEIGHT 18
#define OPTION_COLOR_LINE 0x99FF5555
#define OPTION_COLOR_FILL 0x11FF5555

#define MENU_HIDE_SPEED 8
#define MENU_SHOW_SPEED 8
#define SELECT_OPT_SPEED 10

typedef Item<CModMenu*> ItemMenu;
typedef List<ItemMenu> ListMenu;

ListMenu EXTERN_DLL_Menus;
ListMenu LMenus;  //Список всех меню

ListAnim LMenuAnimations; //Список анимаций. Всегда рисуется текущая анимация списка.
CAnimRect Anim1; //Объект анимации при переходе с пункта на пункт
CAnimRect AnimShow; //Анимация при появлении меню
CAnimRect AnimShow2;
CAnimRect AnimHide; //Анимация при скрытии меню
CAnimRect AnimHide2;
CModMenu* menuInUpDown;
CModMenu* curMenu;
CModMenu* ExternMenus[100];
BYTE ExternMenusCount = 0;

void SetCurMenu(CModMenu* c);

void ClearMenuInUpDownFlag()
{
	menuInUpDown=NULL;
}

void MENU_Init()
{
	Anim1.fnAfterStop = ClearMenuInUpDownFlag;
	Anim1.inList = &LMenuAnimations;
	AnimShow.inList = &LMenuAnimations;
	AnimShow2.inList = &LMenuAnimations;
	AnimHide.inList = &LMenuAnimations;
	AnimHide2.inList = &LMenuAnimations;
	
	//Добавление анимационного листа модуля меню в глобальный список анимационных листов (глобальный список объявлен модуле ANIMATION)
	AnimationListAdd(&LMenuAnimations);
	ClearMenuInUpDownFlag();
	curMenu = &m_MainMenu;
}
/*
bool MenuSetCurrentMenu(CModMenu* m)
{
	return LMenus.MoveTo(m);
}
*/
void MenuEnter(char* s)
{
	if (AnimShow.InProgress() || AnimShow2.InProgress()) return;

	if (LMenus.size() == 0) return; //Если список пуст, выходим
	if (!curMenu->IsVisible()) return; //Если оно скрыто, выходим
	
	//Если текущее меню создано вненшней библиотекой, то сообщение о клике нужно передать во внешнюю библиотеку
	if (curMenu->GetExternMenuId()) fnP_DLL_EVENT_ExternMenuEnter(curMenu->GetExternMenuId(),curMenu->GetOptionNumber());
	//Иначе сообщение уходит во внутреннюю функцию fnMenuEnter
	else curMenu->fnMenuEnter(curMenu->GetOptionNumber());
}

void MenuMoveDown(char* s)
{
	if (AnimShow.InProgress() || AnimShow2.InProgress()) return;
	
	if (LMenus.size()==0) return; //Если список пуст, выходим
	if (!curMenu->IsVisible()) return; //Если оно скрыто, выходим
	curMenu->MoveDown();
}
void MenuMoveUp(char* s)
{
	if (AnimShow.InProgress() || AnimShow2.InProgress()) return;
	
	if (LMenus.size()==0) return; //Если список пуст, выходим
	if (!curMenu->IsVisible()) return; //Если оно скрыто, выходим
	curMenu->MoveUp();
}
void MENU_EVENT_MainLoop() //Отрисовывает все видимые меню из списка CList LMenus
{
	if (LMenus.size()==0) {Mouse.Hide();return;} //Если список пуст, выходим
	
	bool HaveVisibleMenu = false;
	LMenus.top(); //Переходим к началу списка
	do
	{
		if (LMenus.cur()->d->IsVisible())
		{
			HaveVisibleMenu = true;
			LMenus.cur()->d->Draw(); //Отрисовываем
		}
	}
	while (LMenus.next());

	if (HaveVisibleMenu) Mouse.Show();
	else Mouse.Hide();
}

void MenuClick(int _x, int _y)
{
	if (MenuSelectByXY(_x,_y)) MenuEnter(NULL);
}

bool MenuSelectByXY(int _x,int _y)
{
	if (LMenus.size() == 0) return false; //Если список пуст, выходим

	if (!curMenu->IsVisible()) return false; //Если оно скрыто, выходим

	LMenus.top(); //перебираем в цикле все меню, находим на каком остановилсась мышь
	//все меню в списке после найденного нужно скрыть (каскадное меню)
	do
	{
		if (LMenus.cur()->d->SelectByXY(_x,_y)) //Нашли меню на котором мышь
		{	//Теперь нужно скрыть все последующие в списке менюшки
		/*	ItemMenu* C = LMenus.cur(); //Запоминаем найденное
			LMenus.las();
			while(LMenus.cur() == C)
			{
				LMenus.cur()->d->Hide(); //Скрываем меню
				LMenus.las();
			}*/
			return true; //Нашли меню под мышью - возвращаем true
		}
	}
	while(LMenus.next());

	return false; //НЕ Нашли меню под мышью - возвращаем false
}

void MenuShowAfterAnimation()
{
	curMenu->SetVisible();
}
void MenuShowWithAnim() //Запускает анимацию появления меню для текущего ьеню.
{
	if (curMenu==NULL) return;
	//ИНИЦИАЛИЗАЦИЯ АНИМАЦИИ
	int h=curMenu->GetMenuHeight();
	int x = curMenu->GetPositionX();
	int y = curMenu->GetPositionY();
	int Width = curMenu->GetWidth();;
	AnimShow.SetParams(x,y,x,y+h,x,y,x+Width,y+h,OPTION_COLOR_LINE,OPTION_COLOR_FILL,MENU_SHOW_SPEED);
	TRect R1 = curMenu->GetOptionRect(curMenu->GetOptionNumber());
	AnimShow2.SetParams(x,y,x+Width,y+h,R1.x1,R1.y1,R1.x2,R1.y2,OPTION_COLOR_LINE,OPTION_COLOR_FILL,MENU_SHOW_SPEED);
	//Заполняем указатель на функцию, которая вызовится по окончании анимации
	AnimShow2.fnAfterStop = MenuShowAfterAnimation;
	//Добавляем анимации в очередь на воспроизведение
	ItemAnim* AS = new ItemAnim(&AnimShow);
	LMenuAnimations.addAftLas(AS);
	AS = new ItemAnim(&AnimShow2);
	LMenuAnimations.addAftLas(AS);
}

void MENU_CMD_MenuShow(char* s) //Функция вызывается при вводе консольной команды pm_showmenu
{
	if (AnimShow.InProgress() || AnimShow2.InProgress()) return;

	if (curMenu==NULL) curMenu=&m_MainMenu;

	if (!curMenu->IsVisible())
	{
		curMenu->Show(); //Делает видимым текущее меню
	}
	else //Текущее  меню не скрыто, но возможно элемент указывает на подменю, в этом случае нужно открыть подменю
	{
		if (curMenu->IsSubmenuOption()) MenuEnter(NULL);
	}
}

void MenuSubMenuShow() //Исключительно для внешнего модуля input - вызывается при нажатии VK_RIGHT
{
	if (curMenu==NULL) return;
	if (!curMenu->IsVisible()) return;
	if (curMenu->IsSubmenuOption()) MenuEnter(NULL);
}

void MENU_CMD_MenuHide(char* s)
{
	//Делает неидимым текущее меню
	if (LMenus.size()==0) return; //Если список пуст, выходим
	curMenu->Hide(); 
}

void MENU_CMD_Menu(char* s) //Функция вызывается при вводе консольной команды pm_menu
{
	if (curMenu->IsVisible()) MENU_CMD_MenuHide(NULL);
	else MENU_CMD_MenuShow(NULL);
}

void MENU_CMD_MainMenu(char* s)
{
	while(!LMenus.size()==0)
	{
		curMenu->Hide();
	}
	m_MainMenu.Show();
}

void MENU_RegisterConsoleCommands()
{
	//Регистрируем консольные команды внутри PQR Mod
	c_cmd* pm_main_menu = new c_cmd("pm_main_menu","pm_main_menu",MENU_CMD_MainMenu);
	c_cmd* pm_menu = new c_cmd("pm_menu","pm_menu",MENU_CMD_Menu);
	c_cmd* pm_menushow = new c_cmd("pm_menushow","pm_menushow",MENU_CMD_MenuShow);
	c_cmd* pm_menuhide = new c_cmd("pm_menuhide","pm_menuhide",MENU_CMD_MenuHide);
	c_cmd* pm_menuup = new c_cmd("pm_menuup","pm_menuup",MenuMoveUp);
	c_cmd* pm_menudown = new c_cmd("pm_menudown","pm_menudown",MenuMoveDown);
	c_cmd* pm_menuenter = new c_cmd("pm_menuenter","pm_menuenter",MenuEnter);
	
	//Регистрируем консольные команды в NFK.exe
	fnP_DLL_RegisterConsoleCommand("pm_main_menu");
	fnP_DLL_RegisterConsoleCommand("pm_menu");
	fnP_DLL_RegisterConsoleCommand("pm_menushow");
	fnP_DLL_RegisterConsoleCommand("pm_menuhide");
	fnP_DLL_RegisterConsoleCommand("pm_menuup");
	fnP_DLL_RegisterConsoleCommand("pm_menudown");
	fnP_DLL_RegisterConsoleCommand("pm_menuenter");
}

CModMenu::CModMenu()
{
	visible = false;
	x=0;y=0;
	Width=200;
	SubMenuMask=0;
}
CModMenu::~CModMenu()
{
	RemoveAllOptions();
}

void CModMenu::RemoveAllOptions()
{
	if (m_menu_option.size() == 0) return;
	m_menu_option.top();
	do
	{
		if (m_menu_option.cur()->d != NULL) delete[] m_menu_option.cur()->d;
	}
	while (m_menu_option.next());
	m_menu_option.delAll();
}

void CModMenu::AddOption(char* _s)
{
	if (_s==NULL) return;
	char *str = new char[strlen(_s)+3];
	strcpy(str,"^7");
	strcat(str,_s);
	ItemString* NewIS = new ItemString(str);
	m_menu_option.addAftLas(NewIS);
}

void CModMenu::Draw()
{
	ItemString* CurEl = m_menu_option.cur(); //Запоминаем текущий (выбранный элемент)
	char tmp;
	char i=0;
	char* active_str=new char[3+strlen(m_menu_option.cur()->d)]; //Переменная для текста активной строки
	strcpy(active_str+2,m_menu_option.cur()->d);
	//Первые 4 символа текста активной строки ^3^b - подсветка
	active_str[0]='^';active_str[1]='n';
	//В цикле отрисовываем все элементы
	m_menu_option.top();
	do {
		if (m_menu_option.cur()==CurEl)
		{
			tmp = active_str[3];
			active_str[3] = '4';
			fnP_DLL_ExtendedTextout(x+1, y+i*VERTICAL_INTERVAL+1, _NFK_FONT_, false, active_str);
			active_str[3] = tmp;
			active_str[1]='b';
			fnP_DLL_ExtendedTextout(x, y+i*VERTICAL_INTERVAL, _NFK_FONT_, false, active_str);

			TRect R=GetOptionRect(i+1);
			if (!(Anim1.InProgress() && menuInUpDown==this)) fnP_DLL_FX_Rectangle(R.x1,R.y1,R.x2-R.x1,R.y2-R.y1,OPTION_COLOR_LINE,OPTION_COLOR_FILL,0x102,false);
		}
		else
			//fnP_DLL_debug_textout(x, y+i*16, (char*)m_menu_option.GetCur());
			fnP_DLL_ExtendedTextout(x, y+i*VERTICAL_INTERVAL, _NFK_FONT_, false, m_menu_option.cur()->d);
		i++;
	}
	while (m_menu_option.next());
	//После отрисовки нужно вернуть указатель на текущий элемент
	m_menu_option.jump(CurEl);
	delete active_str;
}

void CModMenu::Show()
{
	if (visible) return;
	//visible = true; инициализируется в true по окончании анимации
	ItemMenu* NewIM = new ItemMenu(this);
	LMenus.addAftLas(NewIM);
	curMenu = this;
	MenuShowWithAnim();	
}

void CModMenu::Hide()
{
	if (!visible) return; //Если оно скрыто, выходим
	
	//ИНИЦИАЛИЗАЦИЯ АНИМАЦИИ
	int h=GetMenuHeight();
	TRect R1 = GetOptionRect(GetOptionNumber());
	AnimHide.SetParams(R1.x1,R1.y1,R1.x2,R1.y2,x,y,x+Width,y+h,OPTION_COLOR_LINE,OPTION_COLOR_FILL,MENU_HIDE_SPEED);
	AnimHide2.SetParams(x,y,x+Width,y+h,x,y,x,y+h,OPTION_COLOR_LINE,OPTION_COLOR_FILL,MENU_HIDE_SPEED);
	//Добавляем анимации в очередь на воспроизведение
	ItemAnim* AH = new ItemAnim(&AnimHide);
	LMenuAnimations.addAftLas(AH);
	AH = new ItemAnim(&AnimHide2);
	LMenuAnimations.addAftLas(AH);

	visible = false;
	//Удаляем из списка отображаемых
	if (LMenus.size() != 0)
	{
		LMenus.top(); //Пробегаме в цикле все меню, ищем то, для которого вызвана Hide() и удаляем его из списка
		do
		{
			if (LMenus.cur()->d == this)
			{
				LMenus.delCur();
				break;
			}
		}
		while(LMenus.next());

		if (LMenus.size() != 0)
		{
			LMenus.las();
			curMenu = LMenus.cur()->d; //Текущим делается последнее в списке
		}
		
	}
}

void SetCurMenu(CModMenu* c)
{
	if (c == NULL || curMenu == NULL) return;
	//if (curMenu != c)
	//{
		//Находим новое активное меню (CModMenu* c) в списке, следующие по списку позднее надо скрыть
		if (LMenus.size() != 0)
		{
			LMenus.las();
			do
			{
				Console.print("cicle");
				if (LMenus.cur()->d != c) LMenus.cur()->d->Hide();
				else
				{
					Console.print("cicle return");
					curMenu = c; //Вообще это делать не обязательно, т.к. Hide() устанавливает в curMenu последний в списке элемент
					return;
				}
			}
			while(LMenus.prev());
			//Новое текущее не найдено! Этого не может быть. Результат все меню скрыты.
			//Установим главное в роли текущего
			curMenu = &m_MainMenu;
		}
	//}
}

bool CModMenu::MoveDown()
{
	SetCurMenu(this);
	int curNum = GetOptionNumber();
	if (m_menu_option.cnext())
	{
		menuInUpDown=this;
		//Инициализирцем анимацию движения рамки выбранного пункта меню
		TRect R1 = GetOptionRect(curNum);
		curNum = GetOptionNumber();
		TRect R2 = GetOptionRect(curNum);
		if (Anim1.InProgress()) Anim1.SetNewEnd(R2,SELECT_OPT_SPEED); //Если прямоугольник уже перемещается (анимация в процессе), меняем конечный пункт назначения для прямоугольника.
		else
		{
			Anim1.SetParams(R1,R2,OPTION_COLOR_LINE,OPTION_COLOR_FILL,SELECT_OPT_SPEED);
			ItemAnim* A = new ItemAnim(&Anim1);
			LMenuAnimations.addAftLas(A);
		}
		return true;
	}
	return false;
}

bool CModMenu::MoveUp()
{
	SetCurMenu(this);
	int curNum = GetOptionNumber();
	if (m_menu_option.cprev())
	{
		menuInUpDown=this;
		TRect R1 = GetOptionRect(curNum);
		curNum = GetOptionNumber();
		TRect R2 = GetOptionRect(curNum);
		if (Anim1.InProgress()) Anim1.SetNewEnd(R2,SELECT_OPT_SPEED); //Если прямоугольник уже перемещается (анимация в процессе), меняем конечный пункт назначения для прямоугольника.
		else
		{
			Anim1.SetParams(R1,R2,OPTION_COLOR_LINE,OPTION_COLOR_FILL,SELECT_OPT_SPEED);
			ItemAnim* A = new ItemAnim(&Anim1);
			LMenuAnimations.addAftLas(A);
		}
		return true;
	}
	return false;
}

int CModMenu::GetOptionNumber()
{
	if (m_menu_option.size() == 0) return 0;
	ItemString* CurEl = m_menu_option.cur();
	m_menu_option.top();
	int i=1;
	while (m_menu_option.cur()!=CurEl) {
		i++;
		if (!m_menu_option.next()) return 0;
	}
	return i;
}

void CModMenu::SetOption(BYTE n,char* _s)
{
	if (m_menu_option.size() == 0) return;
	ItemString* CurEl = m_menu_option.cur();
	m_menu_option.top();
	for(int i=1;i<n;i++) if (!m_menu_option.next()) return;
	char* p = m_menu_option.cur()->d;
	delete p;
	p = new char[strlen(_s)+3];
	strcpy(p,"^7");
	strcat(p,_s);
	m_menu_option.cur()->d = p;
	m_menu_option.jump(CurEl);
}

void CModMenu::SetOptionColor(BYTE n,char c)
{
	if (m_menu_option.size() == 0) return;
	ItemString* CurEl = m_menu_option.cur();
	m_menu_option.top();
	for(int i=1;i<n;i++) if (!m_menu_option.next()) return;
	char* p = m_menu_option.cur()->d;
	p[1]=c;
	m_menu_option.jump(CurEl);
}

bool CModMenu::SelectByXY(int _x,int _y) //По переданым координатам определяем пункт меню на который навелась мышь, делает его активным и запускает анимацию перемещения прямоугольника
{
	if (_x<(x-2) || _x > (x+Width) || _y<(y-1)) return false;

	if (m_menu_option.size() == 0) return false;
	ItemString* CurEl = m_menu_option.cur();
	int curNum=GetOptionNumber();
	m_menu_option.top();
	TRect R;
	int i=1;
	do
	{
		R = GetOptionRect(i);
		if (_y >= R.y1 && _y <= R.y2)
		{
			if (i!=curNum)
			{
				SetCurMenu(this);
				//Инициализируем анимацию перемещения прямоугольника по пунктам меню.
				TRect R2 = GetOptionRect(i);
				if (Anim1.InProgress()) Anim1.SetNewEnd(R2,SELECT_OPT_SPEED); //Если прямоугольник уже перемещается (анимация в процессе), меняем конечный пункт назначения для прямоугольника.
				else
				{
					menuInUpDown = this;
					TRect R1 = GetOptionRect(curNum);
					Anim1.SetParams(R1,R2,OPTION_COLOR_LINE,OPTION_COLOR_FILL,SELECT_OPT_SPEED);
					ItemAnim* A = new ItemAnim(&Anim1);
					LMenuAnimations.addAftLas(A);
				}
			}
			return true; //Текущий пункт меню изменён, анимация запущена, фунекция завершила свою работу
		}
		i++;
	}
	while (m_menu_option.next());
	m_menu_option.jump(CurEl); //В цикле так и найден пункт меню соответствующий заданным координатам, восстанавливаем указатель на текущий пункт меню
	return false;
}

TRect CModMenu::GetOptionRect(int num)
{
	TRect R;
	R.x1=x-2;
	R.y1=y+(num-1)*VERTICAL_INTERVAL-(VERTICAL_INTERVAL-HEIGHT)/2;
	R.x2=R.x1+Width;
	R.y2=R.y1+HEIGHT;
	return R;
}

int CModMenu::GetMenuHeight() //Вычисляет высоту меню
{
	if (m_menu_option.size() == 0) return -1;
	ItemString* CurEl = m_menu_option.cur();
	m_menu_option.top();
	int i=0;
	while(m_menu_option.next()) i++;
	m_menu_option.jump(CurEl);

	TRect R0 = GetOptionRect(0);
	TRect RE = GetOptionRect(i);
	return (RE.y2-R0.y1);
}

bool CModMenu::IsSubmenuOption()
{
	return ((SubMenuMask >> (GetOptionNumber()-1)) & 1);
}