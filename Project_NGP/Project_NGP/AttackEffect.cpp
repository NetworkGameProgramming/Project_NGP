#include "stdafx.h"
#include "AttackEffect.h"

AttackEffect::AttackEffect()
{
}

AttackEffect::~AttackEffect()
{
}

void AttackEffect::SetEffectSpawn(int x, int y, DIRECTION Dir ,int AttCheck)
{
	SetPosition(x, y);
	m_AttCheck = AttCheck;
	m_Direction = Dir;
}

void AttackEffect::PlayEffect(AttackEffect* effect)
{
	effect->Initialize();
}
