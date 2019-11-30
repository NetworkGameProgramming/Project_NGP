#include "pch.h"
#include "BlueSnail.h"

BlueSnail::BlueSnail()
	: Monster()
{
	m_Type = MONTYPE_SNAIL;
	m_State = Monster_Idle;
}

BlueSnail::~BlueSnail()
{
	Release();
}

bool BlueSnail::Initialize(int pos_x, int pos_y, int width, int height)
{
	Monster::Initialize(pos_x, pos_y, width, height);

	return true;
}

int BlueSnail::Update(const float& TimeDelta)
{
	Monster::Update(TimeDelta);
	// AI

	return 0;
}

void BlueSnail::Release()
{
}
