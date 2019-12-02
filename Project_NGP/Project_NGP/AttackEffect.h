#pragma once
#include "GameObject.h"
class AttackEffect : public GameObject
{
public:
	AttackEffect();
	~AttackEffect();

	virtual bool Initialize() { return 0; };
	virtual void Render(HDC hdc) {};
	virtual void Release() {};

public:
	void SetEffectSpawn(int x, int y, DIRECTION Dir, int AttCheck);
	void PlayEffect(AttackEffect* effect);

public:
	int m_AttCheck = false;
	DIRECTION		m_Direction;
};

