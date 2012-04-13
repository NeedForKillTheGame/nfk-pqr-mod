/*
 * MouseWare Hook by PQR
 */

#include <windows.h>
#include "mw_hook.h"

#define EXPORT __declspec(dllexport)

#define K_MOUSE2 16
#define K_MOUSE3 8
#define K_MOUSE4 64
#define K_MOUSE7 128
#define K_MOUSE8 32

#pragma data_seg(".shared")
HANDLE	hInstance	= NULL;
HHOOK	Hook		= NULL;
HWND	MW_Window	= NULL;
long buttons = 0;
#pragma data_seg()
#pragma comment(linker, "/section:.shared,rws")



char str[100];

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	hInstance = hinstDLL;
	return TRUE;
}
/*
void Key_Event(long buttons)
{
	buttons &= 0xFFFF;
	switch (buttons ^ old_buttons) {
		case K_MOUSE2:	if (buttons > old_buttons) bt_bitmask |= K_MOUSE2; else bt_bitmask &= ~K_MOUSE2; break;
		case K_MOUSE3:	if (buttons > old_buttons) bt_bitmask |= K_MOUSE3; else bt_bitmask &= ~K_MOUSE3; break;
		case K_MOUSE4:	if (buttons > old_buttons) bt_bitmask |= K_MOUSE4; else bt_bitmask &= ~K_MOUSE4; break;
		case K_MOUSE7:	if (buttons > old_buttons) bt_bitmask |= K_MOUSE7; else bt_bitmask &= ~K_MOUSE7; break;
		case K_MOUSE8:	if (buttons > old_buttons) bt_bitmask |= K_MOUSE8; else bt_bitmask &= ~K_MOUSE8; break;
	default: break;
	}

	old_buttons = buttons;
	//else bt_bitmask = bt_bitmask & (~k);
}
*/
LRESULT CALLBACK MW_Hook (int nCode, WPARAM wParam, LPARAM lParam) {
	LPMSG msg = (LPMSG)lParam;

	if (nCode < 0) {
		CallNextHookEx(Hook, nCode, wParam, lParam);
		return 0;
	}
	if ((msg->hwnd == MW_Window) && (msg->message == WM_USER + 1221)) {
		buttons = msg->lParam;
		buttons &= 0xFFFF;
		msg->message = WM_NULL;
	}
	return CallNextHookEx(Hook, nCode, wParam, lParam);
}

EXPORT int MW_SetHook() {
	if (!(MW_Window = FindWindow("Logitech GetMessage Hook", "Logitech GetMessage Hook")))
		return 11;
	if (!(Hook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC) &MW_Hook, hInstance, 0)))
		return 12;
	return 0;
}

EXPORT void MW_RemoveHook() {
	UnhookWindowsHookEx(Hook);
}

EXPORT unsigned char MW_GetButtons() {
	return buttons;
}