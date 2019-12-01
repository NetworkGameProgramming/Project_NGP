#pragma once
#include "GameObject.h"
class NomalAttack
	: public GameObject
{
	enum State 
	{
		Ready, Play
	};

public:
	NomalAttack();
	virtual ~NomalAttack();

public:
	void SetPosition(int posX, int posY);

public:
	int Update_Input(const float& TimeDelta);
public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	int Update_Position(const float& TimeDelta, const DIRECTION& Direction);
	int Update_Sprite(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();
public:
	void StateChange();

private:
	float m_TimeDelta = 0.f;
};

