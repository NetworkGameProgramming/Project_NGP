#pragma once
#include "GameObject.h"
class BlueSnail 
	: public GameObject
{
	enum State {
		Idle, Move, Hit, Die, Att, End
	};

public:
	BlueSnail();
	virtual ~BlueSnail();

public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	int Update_Sprite(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();

public:
	void StateChange();

private:
	/*float m_TimeDelta = 0.f;
	float m_IdleTimeDelta = 0.f;
	float m_CloseIdleDelta = 0.f;
	float m_ChangeIdleTime = 0.f;
	float m_CloseIdleTime = 0.f;
	float m_KnockBackTime = 0.2f;
	float m_KnockBackTimeDelta = 0.f;*/

	bool m_isOther = false;
};

