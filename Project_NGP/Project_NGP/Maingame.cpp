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
	// 더블 버퍼링
	// 후면 버퍼
	HDC BackBuffer = m_BmpMgr->FindBmp(L"backbuffer")->GetMemDC();

	// 잔상을 없애줄 비트맵
	HDC hMemDC = m_BmpMgr->FindBmp(L"back")->GetMemDC();

	BitBlt(BackBuffer, 0, 0, WINSIZE_X, WINSIZE_Y,
		hMemDC, 0, 0, SRCCOPY);

	// 후면 버퍼에 Render
	if (m_SceneMgr)
	{
		m_SceneMgr->Render(BackBuffer);
	}

	// 후면 버퍼에 모든 오브젝트들의 이미지를 미리 그려넣은 다음
	// 출력 버퍼(전면버퍼)로 고속 복사한다.
	BitBlt(m_hDC, 0, 0, WINSIZE_X, WINSIZE_Y,
		BackBuffer, 0, 0, SRCCOPY);
}

void Maingame::Release()
{
	m_SceneMgr->DestroyInstance();
	m_BmpMgr->DestroyInstance();

	ReleaseDC(g_hWnd, m_hDC);
}
