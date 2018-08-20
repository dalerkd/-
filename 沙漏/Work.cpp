#include "stdafx.h"
#include "Work.h"
#include <string>


CWork::CWork(HWND tmp):m_hd_hwnd(tmp)
{
}


CWork::~CWork()
{
}
#include <sstream>
extern std::wstring g_tips;
extern UINT_PTR g_TIME_ID;
extern DWORD g_minutesRealTime;

LRESULT CWork::OnTrayIcon(HWND hd_hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//如果在图标中单击左键则还原
	if (lParam == WM_LBUTTONDOWN)
	{
		//删除托盘图标
		Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
		//显示主窗口
		ShowWindow(hd_hwnd, SW_SHOWNORMAL);
	}
	//如果在图标中单击右键则弹出声明式菜单
	if (lParam == WM_RBUTTONDOWN)
	{
		//获取鼠标坐标
		POINT pt; GetCursorPos(&pt);
		//右击后点别地可以清除“右击出来的菜单”
		SetForegroundWindow(hd_hwnd);
		//托盘菜单    win32程序使用的是HMENU，如果是MFC程序可以使用CMenu
		HMENU hMenu;
		//生成托盘菜单
		hMenu = CreatePopupMenu();
		//添加菜单,关键在于设置的一个标识符  WM_ONCLOSE 点击后会用到


		std::wstringstream ss;
		ss << g_minutesRealTime;
		std::wstring title;
		ss >> title;
		title = L"距离 "+ g_tips +L" 还有" + title + L"分钟";



		AppendMenu(hMenu, MF_STRING, NULL, title.c_str());
		AppendMenu(hMenu, MF_STRING, WM_ONCLOSE, _T("退出沙漏"));
		//弹出菜单,并把用户所选菜单项的标识符返回
		int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hd_hwnd, NULL);
		//如果标识符是WM_ONCLOSE则关闭
		if (cmd == WM_ONCLOSE)
		{

			m_trayIcon.hIcon = NULL;
			Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
			KillTimer(NULL, g_TIME_ID);
			//退出程序
			::PostQuitMessage(0);
		}
	}

	return 0;
}
/*定义托盘图标对象*/
//定义托盘图标对象
//NOTIFYICONDATA m_trayIcon;
//添加托盘图标(初始化)
//void AddTrayIcon();	
//处理托盘图标上的事件
//LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


//最小化至托盘
void CWork::AddTrayIcon() {
	memset(&m_trayIcon, 0, sizeof(NOTIFYICONDATA));
	m_trayIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_trayIcon.hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MY));
	m_trayIcon.hWnd = m_hd_hwnd;
	lstrcpy(m_trayIcon.szTip, _T("沙漏"));
	m_trayIcon.uCallbackMessage = WM_SHOWTASK;
	m_trayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &m_trayIcon);
	//ShowWindow(m_hd_hwnd, SW_HIDE);
}

