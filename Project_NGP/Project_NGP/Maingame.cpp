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
	// ����� �Ŵ��� �ʱ�ȭ
	m_GdiPlusMgr = GET_MANAGER<GdiPlusManager>();
	m_SceneMgr = GET_MANAGER<SceneManager>();
	
	// ���� �ʿ��� �͵��� �̸� �غ��� ���´�.
#ifdef SERVER_MODE
	if (false == m_SceneMgr->ChangeSceneState(SCENE_MENU))
	{
		return false;
	}
#endif

#ifdef CLIENT_MODE
	if (false == m_SceneMgr->ChangeSceneState(SCENE_TEST))
	{
		return false;
	}
#endif

	// �׸� DC�ڵ��� �������� �ڵ�� ���� �����´�.
	m_hDC = GetDC(g_hWnd);

	return true;
}

int Maingame::Update(const float & TimeDelta)
{
	// ���� ������Ʈ
	if (m_SceneMgr)
	{
		if (-1 == m_SceneMgr->Update(TimeDelta))
			return -1;
	}

	return 0;
}

void Maingame::Render()
{
	// ���� ���۸��� �ϴ� �۾��̴�.

	HDC Memhdc;
	HBITMAP Membitmap;

	// m_hDC�� ȣȯ�Ǵ� ���ο� DC�� �����.
	// ȣȯ : �����ġ ���� or �׷��� ����̹� ����
	Memhdc = CreateCompatibleDC(m_hDC);
	// ���ڷ� �־��� DC�� ȣȯ�ϴ� ��Ʈ���� �����Ͽ� ��ȯ
	Membitmap = CreateCompatibleBitmap(m_hDC, WINSIZE_X, WINSIZE_Y);
	// Memhdc���� GDI Object�� �����Ѵ�.
	SelectObject(Memhdc, Membitmap);

	// �׸� �ڵ�� ���ʿ� �´�.
	///////////////////////////////////////////////////
	if (m_SceneMgr)
	{
		m_SceneMgr->Render(Memhdc);
	}
	///////////////////////////////////////////////////

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
	GET_MANAGER<ObjectManager>()->DestroyInstance();
	GET_MANAGER<CameraManager>()->DestroyInstance();

	ReleaseDC(g_hWnd, m_hDC);
}
