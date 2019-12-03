#include "pch.h"
#include "Slime.h"

Slime::Slime()
	: Monster()
{
	m_Type = MONTYPE_SLIME;
	m_State = Monster_Idle;
}

Slime::~Slime()
{
	Release();
}

bool Slime::Initialize(int pos_x, int pos_y, int width, int height)
{
	Monster::Initialize(pos_x, pos_y, width, height);
	m_hp = 10;
	return true;
}

int Slime::Update(const float& TimeDelta)
{
	Monster::Update(TimeDelta);

	if (Monster_Move == m_State)
	{
		m_Speed = 120;
	}

	return 0;
}

void Slime::Release()
{
}
