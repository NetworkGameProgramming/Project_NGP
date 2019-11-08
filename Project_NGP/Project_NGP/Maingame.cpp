#include "stdafx.h"
#include "Maingame.h"
#include "GameObject.h"


Maingame::Maingame()
{
}


Maingame::~Maingame()
{
	Release();
}

bool Maingame::Initialize()
{
	// BackBuffer
	m_GdiPlusMgr = GET_MANAGER<GdiPlusManager>();
	m_SceneMgr = GET_MANAGER<SceneManager>();

	if (false == m_SceneMgr->ChangeSceneState(SCENE_TEST))
	{
		return false;
	}

	m_hDC = GetDC(g_hWnd);

	return true;
}

int Maingame::Update(const float & TimeDelta)
{
	if (m_SceneMgr)
	{
		m_SceneMgr->Update(TimeDelta);
	}

	return 0;
}

void Maingame::Render()
{
	HDC Memhdc;
	HBITMAP Membitmap;

	// m_hDC와 호환되는 새로운 DC를 만든다.
	// 호환 : 출력장치 종류 or 그래픽 드라이버 정보
	Memhdc = CreateCompatibleDC(m_hDC);
	// 인자로 주어진 DC에 호환하는 비트맵을 생성하여 반환
	Membitmap = CreateCompatibleBitmap(m_hDC, WINSIZE_X, WINSIZE_Y);
	// Memhdc에게 GDI Object를 설정한다.
	SelectObject(Memhdc, Membitmap);

	// drawing code goes in here
	if (m_SceneMgr)
	{
		m_SceneMgr->Render(Memhdc);
	}

	// 장면을 그린 DC를 m_hDC에 복사한다.
	BitBlt(m_hDC, 0, 0, WINSIZE_X, WINSIZE_Y,
			Memhdc, 0, 0, SRCCOPY);

	DeleteObject(Membitmap);
	DeleteDC(Memhdc);
}

void Maingame::Release()
{
	m_SceneMgr->DestroyInstance();
	m_GdiPlusMgr->DestroyInstance();

	ReleaseDC(g_hWnd, m_hDC);
}
