#pragma once
#include "GameObject.h"

class CEffect
	: public GameObject
{
public:
	CEffect();
	virtual ~CEffect();

public:
	virtual bool Initialize();
	virtual int Update(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();

public:
	void SetEffectSpawn(int x, int y, DIRECTION Dir, int AttCheck);

protected:
	int				m_AttCheck = false;
	int				m_EffectRectX = 0;
	int				m_EffectRectY = 0;
};

