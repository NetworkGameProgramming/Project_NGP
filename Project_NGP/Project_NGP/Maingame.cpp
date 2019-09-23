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
	m_BmpMgr = GET_MANAGER<BmpManager>();

	m_BmpMgr->LoadBmp(L"backbuffer", L"../Resources/backbuffer.bmp");
	m_BmpMgr->LoadBmp(L"back", L"../Resources/back.bmp");

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
	// ���� ���۸�
	// �ĸ� ����
	HDC BackBuffer = m_BmpMgr->FindBmp(L"backbuffer")->GetMemDC();

	// �ܻ��� ������ ��Ʈ��
	HDC hMemDC = m_BmpMgr->FindBmp(L"back")->GetMemDC();

	BitBlt(BackBuffer, 0, 0, WINSIZE_X, WINSIZE_Y,
		hMemDC, 0, 0, SRCCOPY);

	// �ĸ� ���ۿ� Render
	if (m_SceneMgr)
	{
		m_SceneMgr->Render(BackBuffer);
	}

	// �ĸ� ���ۿ� ��� ������Ʈ���� �̹����� �̸� �׷����� ����
	// ��� ����(�������)�� ��� �����Ѵ�.
	BitBlt(m_hDC, 0, 0, WINSIZE_X, WINSIZE_Y,
		BackBuffer, 0, 0, SRCCOPY);
}

void Maingame::Release()
{
	m_SceneMgr->DestroyInstance();
	m_BmpMgr->DestroyInstance();

	ReleaseDC(g_hWnd, m_hDC);
}
