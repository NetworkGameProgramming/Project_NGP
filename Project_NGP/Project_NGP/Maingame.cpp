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

	m_GdiPlusMgr->LoadGdiPlusImage(L"backbuffer", L"../Resources/backbuffer.bmp");
	m_GdiPlusMgr->LoadGdiPlusImage(L"back", L"../Resources/back.bmp");

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

	// m_hDC�� ȣȯ�Ǵ� ���ο� DC�� �����.
	// ȣȯ : �����ġ ���� or �׷��� ����̹� ����
	Memhdc = CreateCompatibleDC(m_hDC);
	// ���ڷ� �־��� DC�� ȣȯ�ϴ� ��Ʈ���� �����Ͽ� ��ȯ
	Membitmap = CreateCompatibleBitmap(m_hDC, WINSIZE_X, WINSIZE_Y);
	// Memhdc���� GDI Object�� �����Ѵ�.
	SelectObject(Memhdc, Membitmap);

	// drawing code goes in here
	if (m_SceneMgr)
	{
		m_SceneMgr->Render(Memhdc);
	}

	// ����� �׸� DC�� m_hDC�� �����Ѵ�.
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
