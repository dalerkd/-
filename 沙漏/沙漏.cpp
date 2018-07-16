// ɳ©.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ɳ©.h"
#include "shellapi.h"
#include <sstream>


#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


DWORD g_minutes = 0;//ʱ��-����
DWORD g_minutesRealTime = 0;//��䶯
std::wstring g_tips;
bool COMMAND_START = true;//�����з�ʽ����?

void   CALLBACK   TimerProc(HWND   hWnd, UINT   nMsg, UINT   nTimerid, DWORD   dwTime);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	LPWSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	if (szArgList == NULL)
	{
		MessageBox(NULL, L"Unable to parse command line", L"Error", MB_OK);
		return 10;
	}

	std::wstringstream ss;

	switch (argCount)
	{
	case 3:

		g_tips = szArgList[2];

	case 2:

		
		ss << szArgList[1];
		ss >> g_minutes;
		g_minutesRealTime = g_minutes;


		break;
	default:
	{
		COMMAND_START = false;
	}
		break;
		
	}

	LocalFree(szArgList);

	if(g_minutes==0)
	{
		TimerProc(0, 0, 0, 0);
	}




	// TODO: �ڴ˷��ô��롣

	// ��ʼ��ȫ���ַ���
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

	MSG msg;

	// ����Ϣѭ��: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MY));

	return RegisterClassExW(&wcex);
}

#include "Work.h"

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//

CWork* pCWORK = nullptr;
HWND myHWND = nullptr;

void   CALLBACK   TimerProc(HWND   hWnd, UINT   nMsg, UINT   nTimerid, DWORD   dwTime)
{
	if (0 == g_minutesRealTime || 1 == g_minutesRealTime)
	{
		KillTimer(myHWND, 1);
		g_minutesRealTime = 0;

		std::wstringstream ss;
		ss << g_minutes;
		std::wstring title;
		ss >> title;
		title = L"���μ�ʱ����" + title+L"����";
		MessageBox(0, g_tips.c_str(), title.c_str(), MB_OK);
		ExitProcess(-1);
	}

}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	//ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);
	myHWND = hWnd;
	pCWORK = new CWork(hWnd);

	pCWORK->AddTrayIcon();

	if(COMMAND_START)
	{
	SetTimer(myHWND, 1, 60 * 1000, TimerProc);
	}

	return TRUE;
}

LRESULT CheckLogInDlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	std::wstringstream ss;
	TCHAR szTime[64]{ 0 }, szTips[64]{ 0 };
	switch (Msg)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;


	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDOK:
			int time_len, tips_len;

			time_len = GetDlgItemText(hDlg, IDC_TIME_MIN, szTime, 64);
			
			tips_len = GetDlgItemText(hDlg, IDC_TIPS, szTips, 64);


			if (time_len == 0)
			{
				//EndDialog(hDlg, ~ERROR_SUCCESS);
				MessageBoxA(0,"ʱ�䲻��Ϊ��",0,0);
				return FALSE;
			}

			if(tips_len!=0)
			{
				g_tips = szTips;
			}
			
			ss << szTime;
			ss >> g_minutes;
			g_minutesRealTime = g_minutes;
			EndDialog(hDlg, TRUE);
			SetTimer(myHWND, 1, 60 * 1000, TimerProc);
			return (INT_PTR)TRUE;
		default:
			break;
		}
	}
		return (INT_PTR)FALSE;
}

ULONG StartIn(HWND hWnd)
{
	return    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX),
		hWnd, (DLGPROC)CheckLogInDlgProc);
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SHOWTASK:
		pCWORK->OnTrayIcon(hWnd, message, wParam, lParam);


		break;
	case WM_CREATE:
	{
		if (!COMMAND_START)
		{
			StartIn(hWnd);
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
