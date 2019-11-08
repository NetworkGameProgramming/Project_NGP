#include "stdafx.h"
#include "CameraManager.h"

#include "GameObject.h"

CameraManager::CameraManager()
{
	ZeroMemory(&m_Pos, sizeof(POSITION));
	ZeroMemory(&m_Pos, sizeof(RESOLUTION));
}


CameraManager::~CameraManager()
{
	Release();
}

void CameraManager::SetTarget(GameObject * target)
{
	m_Target = target;
}

void CameraManager::SetResolution(int x, int y)
{
	m_Resolution.X = x;
	m_Resolution.Y = y;
}

int CameraManager::Update(const float & TimeDelta)
{
	if (nullptr != m_Target)
	{
		GAMEOBJINFO targetInfo = m_Target->GetInfo();

		int offsetX = 300;
		int offsetY = 300;

		int speed = int(m_Target->GetSpeed() / 1.25f * TimeDelta);

		if (targetInfo.Pos_X + m_Pos.X >= WINSIZE_X - offsetX)
			m_Pos.X -= speed;

		if (targetInfo.Pos_X + m_Pos.X <= offsetX)
			m_Pos.X += speed;

		if (targetInfo.Pos_Y + m_Pos.Y >= WINSIZE_Y - offsetY)
			m_Pos.Y -= speed;

		if (targetInfo.Pos_Y + m_Pos.Y <= offsetY)
			m_Pos.Y += speed;
	}

	if (0 < m_Pos.X) m_Pos.X = 0;
	if (0 < m_Pos.Y) m_Pos.Y = 0;

	return 0;
}

void CameraManager::Release()
{
	m_Target = nullptr;
}
