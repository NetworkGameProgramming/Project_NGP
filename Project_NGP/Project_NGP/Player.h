#pragma once
#include "GameObject.h"

class Player
	: public GameObject
{
public:
	enum State { Idle, Attack, End };

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

public:
	void StateChange();

private:
	float m_SpeedAcc = 0.f;
	DWORD m_Dir = 0;
	DIRECTION m_Direction = DIR_END;

	float  m_SpriteIndex = 0.f;
	State  m_CurState = Idle;
	State  m_PreState = End;
	const TCHAR* m_SpriteKey = L"balrok";
};

