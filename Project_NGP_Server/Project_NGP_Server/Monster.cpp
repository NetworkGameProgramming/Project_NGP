#include "pch.h"
#include "Monster.h"

Monster::Monster()
	: GameObject()
{
}

Monster::~Monster()
{
	Release();
}

bool Monster::Initialize(int pos_x, int pos_y, int width, int height)
{
	GameObject::Initialize(pos_x, pos_y, width, height);

	return true;
}

int Monster::Update(const float& TimeDelta)
{
	GameObject::Update(TimeDelta);

	return 0;
}

void Monster::Release()
{
}

void Monster::CollisionPixelPart(DIRECTION dir)
{
	switch (dir)
	{
	case DIR_BOTTOM:
		m_GravitySpeed = 0.f;
		m_GravityAcc = 0.f;
		m_FallCheck = false;
		break;
	}
}
