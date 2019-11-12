#include "stdafx.h"
#include "CameraManager.h"

#include "GameObject.h"

CameraManager::CameraManager()
{
	ZeroMemory(&m_Pos, sizeof(POSITION));
	ZeroMemory(&m_Pos, sizeof(RESOLUTION));
	ZeroMemory(&m_OldDestPos, sizeof(POSITION));
	ZeroMemory(&m_DestPos, sizeof(POSITION));

	// ī�޶� ������ ������ ���߾����� ���´�.
	m_Offset.X = ((float)WINSIZE_X / 2);
	m_Offset.Y = ((float)WINSIZE_Y / 2);
}

CameraManager::~CameraManager()
{
	Release();
}

void CameraManager::SetPos(int x, int y)
{
	if (nullptr == m_Target)
		return;

	m_Pos.X = -float(x - ((int)WINSIZE_X / 2));
	m_Pos.Y = -float(y - ((int)WINSIZE_Y / 2));
}

void CameraManager::SetTarget(GameObject * target)
{
	m_Target = target;
	
	// Ÿ���� ���԰� ���ÿ� Ÿ���� ��ġ�� �̸� Set
	GAMEOBJINFO targetInfo = m_Target->GetInfo();
	SetPos(targetInfo.Pos_X, targetInfo.Pos_Y);
}

void CameraManager::SetResolution(int x, int y)
{
	// ��׶��� �ؽ����� ũ���̴�.
	m_Resolution.X = (float)x;
	m_Resolution.Y = (float)y;
}

void CameraManager::SetOffset(int x, int y)
{
	m_Offset.X = ((float)WINSIZE_X / 2) - (float)x;
	m_Offset.Y = ((float)WINSIZE_Y / 2) - (float)y;
}

int CameraManager::Update(const float & TimeDelta)
{
	if (nullptr != m_Target)
	{
		if (true == m_Refresh) m_Time += TimeDelta * 1.f;
		else m_Time -= TimeDelta * 1.f;

		if (m_Time > 1.f) m_Time = 1.f;
		else if (m_Time <= 0.f) m_Time = 0.f;

		GAMEOBJINFO targetInfo = m_Target->GetInfo();

		m_Refresh = false;

		// X�� ī�޶� �������� �����ϸ�
		if ((targetInfo.Pos_X + (int)m_Pos.X + (int)m_Offset.X) >= (int)WINSIZE_X ||
			(targetInfo.Pos_X + (int)m_Pos.X - (int)m_Offset.X) <= 0 )
		{
			m_OldDestPos.X = -float(targetInfo.Pos_X - ((int)WINSIZE_X / 2));
			// ���� �������� ��ġ�� ��ȭ�� ���� ��� ������Ʈ ���� �ʴ´�.
			// �ֳĸ� �����̴� ��Ȳ�� �ƴϱ� ������
			if ((int)m_OldDestPos.X != (int)m_DestPos.X)
			{
				m_DestPos.X = -float(targetInfo.Pos_X - ((int)WINSIZE_X / 2));
				m_Refresh = true;
			}
		}

		// Y�� ī�޶� �������� �����ϸ�
		if ((targetInfo.Pos_Y + (int)m_Pos.Y + (int)m_Offset.Y) >= (int)WINSIZE_Y ||
			(targetInfo.Pos_Y + (int)m_Pos.Y - (int)m_Offset.Y) <= 0 )
		{
			m_OldDestPos.Y = -float(targetInfo.Pos_Y - ((int)WINSIZE_Y / 2));
			if ((int)m_OldDestPos.Y != (int)m_DestPos.Y)
			{
				m_DestPos.Y = -float(targetInfo.Pos_Y - ((int)WINSIZE_Y / 2));
				m_Refresh = true;
			}
		}

		// ī�޶��� ������ �ӵ��� ������ ���ش�.
		float m_speed = Lerp<float, float>(10.f, m_Target->GetSpeed(), m_Time);

		// �����ϴ� Direction ���͸� ���Ͽ� �� �������� ������ ����Ѵ�.
		POSITION Dir = m_DestPos - m_Pos;
		float Length = sqrt(Dir.X * Dir.X + Dir.Y * Dir.Y);
		POSITION totalDir = POSITION{ 0.f, 0.f };

		if (0.f != Length)
		{
			totalDir.X = Dir.X / Length;
			totalDir.Y = Dir.Y / Length;
		}

		//printf("Length : %f\n", Length);
		//printf("DirX : %f,  DirY : %f\n", Dir.X, Dir.Y);
		
		// ���߾����� �������� ī�޶� ��ǥ�� �������ش�.
		if (targetInfo.Pos_X + (int)m_Pos.X > ((int)WINSIZE_X / 2) ||
			targetInfo.Pos_X + (int)m_Pos.X < ((int)WINSIZE_X / 2))
		{
			//float temp = totalDir.X * m_speed * TimeDelta;

			m_Pos.X += totalDir.X * m_speed * TimeDelta;
		}

		if (targetInfo.Pos_Y + (int)m_Pos.Y > ((int)WINSIZE_Y / 2) ||
			targetInfo.Pos_Y + (int)m_Pos.Y < ((int)WINSIZE_Y / 2))
		{
			m_Pos.Y += totalDir.Y * m_speed * TimeDelta;
		}

		// ��׶����� ũ�⸦ ī�޶� ������� �ϸ� ī�޶� �������� ���� ��Ų��.
		if (0.f <= m_Pos.X) m_Pos.X = 0.f;
		if (0.f <= m_Pos.Y) m_Pos.Y = 0.f;
		if (-m_Resolution.X > m_Pos.X - (int)WINSIZE_X) m_Pos.X = -float(m_Resolution.X - (int)WINSIZE_X);
		if (-m_Resolution.Y > m_Pos.Y - (int)WINSIZE_Y) m_Pos.Y = -float(m_Resolution.Y - (int)WINSIZE_Y);

		// ī�޶� ��ġ�� ��ǥ ��ġ�� ������ ���̰� �ʹ� ����� ��� 
		// ī�޶��� �ӵ��� Ȯ ���δ�.
		if (Length < 30.f)
		{
			//m_DestPos = m_Pos;
			m_Time = 0.f;
		}

	}

	return 0;
}

void CameraManager::Release()
{
	m_Target = nullptr;
}
