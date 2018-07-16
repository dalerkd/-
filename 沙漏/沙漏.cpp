// 沙漏.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "沙漏.h"
#include "shellapi.h"
#include <sstream>


#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


DWORD g_minutes = 0;//时间-分钟
DWORD g_minutesRealTime = 0;//会变动
std::wstring g_tips;
bool COMMAND_START = true;//命令行方式启动?

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




	// TODO: 在此放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

	MSG msg;

	// 主消息循环: 
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
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
		title = L"本次计时共计" + title+L"分钟";
		MessageBox(0, g_tips.c_str(), title.c_str(), MB_OK);
		ExitProcess(-1);
	}

}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

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
				MessageBoxA(0,"时间不能为空",0,0);
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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择: 
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
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
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

// “关于”框的消息处理程序。
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
