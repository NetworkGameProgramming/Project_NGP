#include "stdafx.h"
#include "Maingame.h"
#include "GameObject.h"
#include "Player.h"


Maingame::Maingame()
{
}


Maingame::~Maingame()
{
	Release();
}

bool Maingame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	m_Player = new Player;
	if (false == m_Player->Initialize())
	{
		delete m_Player;
		m_Player = nullptr;
		return false;
	}

	return true;
}

int Maingame::Update(const float & TimeDelta)
{
	m_Player->Update(TimeDelta);

	return 0;
}

void Maingame::Render()
{
	Rectangle(m_hDC, 0, 0, WINSIZE_X, WINSIZE_Y);

	m_Player->Render(m_hDC);
}

void Maingame::Release()
{
	if (m_Player)
	{
		delete m_Player;
		m_Player = nullptr;
	}

	ReleaseDC(g_hWnd, m_hDC);
}
