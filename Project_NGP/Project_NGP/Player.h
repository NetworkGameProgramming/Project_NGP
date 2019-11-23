#pragma once
#include "GameObject.h"

class Player
	: public GameObject
{
public:
	enum State { Idle, Idle_Att, Walk, Jump, 
		Att_1, Att_2, Att_3,
		Hanging, End };

public:
	Player();
	virtual ~Player();

public:
	int Update_Input(const float& TimeDelta);
	int Update_Position(const float& TimeDelta, const DIRECTION& Direction);
	int Update_Sprite(const float& TimeDelta);

public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();
	virtual void CollisionPixelPart(DIRECTION dir);

public:
	void StateChange();

private:
	float m_TimeDelta = 0.f;
	
	DIRECTION m_Direction = DIR_END;
	DWORD m_Dir = 0;

	float m_GravitySpeed = 0.f;
	float m_GravityAcc = 9.8f;
	bool m_fallCheck = true;
};

