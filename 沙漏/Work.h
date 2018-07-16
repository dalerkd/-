#pragma once

#include "Resource.h"

#include <windows.h>
#include <shellapi.h>

#define WM_SHOWTASK (WM_USER +1)
#define WM_ONCLOSE (WM_USER+2)

class CWork
{
public:
	CWork(HWND);
	~CWork();
	LRESULT OnTrayIcon(HWND hd_hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void AddTrayIcon();
	NOTIFYICONDATA m_trayIcon;
	HWND m_hd_hwnd;
};



