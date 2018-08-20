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
	//�����ͼ���е��������ԭ
	if (lParam == WM_LBUTTONDOWN)
	{
		//ɾ������ͼ��
		Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
		//��ʾ������
		ShowWindow(hd_hwnd, SW_SHOWNORMAL);
	}
	//�����ͼ���е����Ҽ��򵯳�����ʽ�˵�
	if (lParam == WM_RBUTTONDOWN)
	{
		//��ȡ�������
		POINT pt; GetCursorPos(&pt);
		//�һ�����ؿ���������һ������Ĳ˵���
		SetForegroundWindow(hd_hwnd);
		//���̲˵�    win32����ʹ�õ���HMENU�������MFC�������ʹ��CMenu
		HMENU hMenu;
		//�������̲˵�
		hMenu = CreatePopupMenu();
		//��Ӳ˵�,�ؼ��������õ�һ����ʶ��  WM_ONCLOSE �������õ�


		std::wstringstream ss;
		ss << g_minutesRealTime;
		std::wstring title;
		ss >> title;
		title = L"���� "+ g_tips +L" ����" + title + L"����";



		AppendMenu(hMenu, MF_STRING, NULL, title.c_str());
		AppendMenu(hMenu, MF_STRING, WM_ONCLOSE, _T("�˳�ɳ©"));
		//�����˵�,�����û���ѡ�˵���ı�ʶ������
		int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hd_hwnd, NULL);
		//�����ʶ����WM_ONCLOSE��ر�
		if (cmd == WM_ONCLOSE)
		{

			m_trayIcon.hIcon = NULL;
			Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
			KillTimer(NULL, g_TIME_ID);
			//�˳�����
			::PostQuitMessage(0);
		}
	}

	return 0;
}
/*��������ͼ�����*/
//��������ͼ�����
//NOTIFYICONDATA m_trayIcon;
//�������ͼ��(��ʼ��)
//void AddTrayIcon();	
//��������ͼ���ϵ��¼�
//LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


//��С��������
void CWork::AddTrayIcon() {
	memset(&m_trayIcon, 0, sizeof(NOTIFYICONDATA));
	m_trayIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_trayIcon.hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MY));
	m_trayIcon.hWnd = m_hd_hwnd;
	lstrcpy(m_trayIcon.szTip, _T("ɳ©"));
	m_trayIcon.uCallbackMessage = WM_SHOWTASK;
	m_trayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &m_trayIcon);
	//ShowWindow(m_hd_hwnd, SW_HIDE);
}

