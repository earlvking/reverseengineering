// HelloOlly.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "windows.h"
#include "plugin.h"
#include "HelloOlly.h"

#include "resource.h"


#pragma comment(lib, "OLLYDBG.LIB")


HWND             hwmain;               // Handle of main OllyDbg window
HINSTANCE        hinst;                // DLL instance



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call==DLL_PROCESS_ATTACH)
		hinst=(HINSTANCE)hModule;       // Mark plugin instance

	return TRUE;
}

extc int _export cdecl ODBG_Plugindata(char shortname[32]) 
{
	strcpy(shortname, "HelloOlly");       // Name of plugin
	return PLUGIN_VERSION;
};

extc int _export cdecl ODBG_Plugininit(
  int ollydbgversion,HWND hw,ulong *features) 
{
	// OllyDBG�� Plugin SDK ������ ���Ͽ�
	// Plugin�� �� ���ٸ� -1 �����Ͽ� �����Ű�� ����
	if (ollydbgversion<PLUGIN_VERSION)
		return -1;

	// OllyDBG�� ���� ������ �ڵ��� �޾Ƴ��´�. 
	// �� �ڵ��� �޽����ڽ����� ����ϱ� ���� �ݵ�� �ʿ��ϴ�	
	hwmain=hw;

	// Log Window�� ���ڿ��� ����Ѵ�.
	Addtolist(0, 0, "Hello Olly  plugin v0.01 (test plugin)");
	Addtolist(0, -1, "made by window31");	

	return 0;
}

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) 
{
	switch (origin) 
	{
	case PM_MAIN: // Plugin menu in main window
		strcpy(data,
			"0 &Hello Olly Dialog,"
			"1 Test|"
			"2 About"
			);
		return 1;
	
	case PM_DISASM:
		if (Getstatus() == STAT_NONE) 
		{
			return 0;
		}
		strcpy(data,"0 &Hello Olly Dialog");
		return 1;
	
	case PM_THREADS:
		if (Getstatus() == STAT_NONE) 
		{
			return 0;
		}
		strcpy(data,"0 &Hello Olly Dialog");
		return 1;
	
	default:
		break; // Any other window
	};
	return 0; // Window not supported by plugin
};

#define MENU_HELLOOLLY_DIALOG		0 // "0 &Hello Olly Dialog,"
#define MENU_TEST						1 // "1 Test|"
#define MENU_ABOUT					2 // "2 About"

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
	int id;	
	
	switch(origin) 
	{
	case PM_MAIN:
	case PM_DISASM:
		switch (action) 
		{
		case MENU_HELLOOLLY_DIALOG:
			// ���μ����� Attach ���� �ʾ����� ���� ���
			if (Getstatus() == STAT_NONE) 
			{
				MessageBox(hwmain, "No process!!", "ERR", MB_OK);
				return;
			}
			
			id = DialogBox(hinst, MAKEINTRESOURCE(IDD_HELLOOLLY), hwmain, (DLGPROC)MainDlgProc);
			if (id == IDOK) 
			{
				//Doit(hwmain);
			}			
			
			break;
		case MENU_TEST:
			MessageBox(hwmain, "TEST Selection", "TEST", MB_OK);
			
			break;
		case MENU_ABOUT:			
			MessageBox(hwmain, "Hello Olly ver 0.01", "TEST", MB_OK);

			break;
		default:
			break;
		}
	}
}

LRESULT CALLBACK MainDlgProc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp)
{
   HDC hDC;
   PAINTSTRUCT ps;      	

	RECT  rect;
	UINT  x,y,w,h,xMax,yMax;

	switch (msg) 
	{
		case WM_INITDIALOG:
			// ���̾�α׸� �Ѱ���� ���
			GetWindowRect(hDlgWnd, &rect);
			h = rect.bottom - rect.top;
			w = rect.right  - rect.left;

			xMax = GetSystemMetrics(SM_CXMAXIMIZED);
			yMax = GetSystemMetrics(SM_CYMAXIMIZED);
			
			x = xMax/2 - w/2;
			y = yMax/2 - h;
			
			MoveWindow(hDlgWnd, x, y, w, h, TRUE);
			break;

		case WM_COMMAND:
			switch (LOWORD(wp)) 
			{
			case IDOK:

				EndDialog(hDlgWnd, IDOK);
				break;

			case IDCANCEL:
				EndDialog(hDlgWnd, IDCANCEL);
				break;

			default:
				break;
			}
		case WM_PAINT:			
			hDC = BeginPaint(hDlgWnd, &ps);
			EndPaint(hDlgWnd, &ps);
			break;
			
		default:
			break;
	}
	
	return 0;
}

extc int _export cdecl ODBG_Pluginclose(void) 
{
	//if(lpMyAlloc) 
	//{
	//	FreeMyAlloc();
	//	lpMyAlloc = NULL;
		Addtolist(0, -1, "== Close Hello Plugin ==");
	//}
	return 0;
}
