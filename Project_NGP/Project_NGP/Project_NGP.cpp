﻿// Project_Beom.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Project_NGP.h"

#include "Timer.h"
#include "Maingame.h"

#define MAX_LOADSTRING 100

// 전역 변수:
// 윈도우 핸들 전역변수화
HWND g_hWnd;

HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	wsprintf(szTitle, L"Game");
	//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PROJECTNGP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECTNGP));

	MSG msg;
	msg.message = WM_NULL;

	// Timer 선언
	Timer *pTimer = new Timer;
	if (false == pTimer->Initialize())
	{
		delete pTimer;
		pTimer = nullptr;
	}
	TimerManager *pTimerManager = GET_MANAGER<TimerManager>();
	pTimerManager->Add_Timer(L"Timer_Default", pTimer);

	// Frame 선언
	FrameManager *pFrameManager = GET_MANAGER<FrameManager>();
	if (false == pFrameManager->Add_Frame(L"Frame_30", 30.f))
		return FALSE;

	if (false == pFrameManager->Add_Frame(L"Frame_60", 60.f))
		return FALSE;

	if (false == pFrameManager->Add_Frame(L"Frame_200", 200.f))
		return FALSE;

	int frameCount = 0;
	float timeCount = 0.f;

	const TCHAR* strFrame = L"Frame_60";

	// KeyManager 선언
	KeyManager *pKeyManager = GET_MANAGER<KeyManager>();

	// MainGame
	Maingame mainGame;
	if (false == mainGame.Initialize())
	{
		exit(-1);
		return -1;
	}

	// 기본 메시지 루프입니다:
	while (msg.message != WM_QUIT)
	{
		// while (GetMessage(&msg, nullptr, 0, 0))
		// GetMessage : 시스템이 유지하는 메시지 큐에서 메시지를 읽어오는 함수.
		// 읽어올 메시지가 없다면 블로킹 상태에 빠지게 만든다. -> 메시지 입력이 있을 때까지..

		// PeekMessage : 읽어올 메시지가 있다면 true, 없으면 false 반환
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 읽어온 메시지를 알맞게 API에서 해석하는 부분이다.
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			pTimerManager->Compute_Timer(L"Timer_Default");
			float TimeDelta = pTimerManager->Get_TimeDelta(L"Timer_Default");

			timeCount += TimeDelta;

			if (pFrameManager->Permit_Call(strFrame, TimeDelta))
			{
				++frameCount;

				float FrameTimeDelta = pFrameManager->Get_FrameTimeDelta(strFrame);

				if (timeCount >= 1.f)
				{
					char	strFPS[128] = {};
					sprintf_s(strFPS, "Delta Time : %.5f  FPS : %d", FrameTimeDelta, frameCount);
					SetWindowTextA(g_hWnd, strFPS);

					timeCount = 0.f;
					frameCount = 0;
				}

				// Key Update
				if (-1 == pKeyManager->UpdateKey())
				{
					exit(-1);
					return -1;
				}

				// Update
				if (-1 == mainGame.Update(FrameTimeDelta))
				{
					exit(-1);
					return -1;
				}
				// Render
				mainGame.Render();
			}
		}
	}

	pFrameManager->DestroyInstance();
	pTimerManager->DestroyInstance();
	pKeyManager->DestroyInstance();

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECTNGP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, WINSIZE_X, WINSIZE_Y, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	g_hWnd = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
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
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
