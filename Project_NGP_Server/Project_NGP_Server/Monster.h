#pragma once
#include "GameObject.h"

class Monster 
	:public GameObject
{
public:
	Monster();
	virtual ~Monster();

public:
	const MONSTERTYPE& GetMonType() { return m_Type; }
	const MonsterState& GetMonState() { return m_State; }

public:
	virtual bool Initialize(int pos_x, int pos_y, int width, int height);
	virtual int  Update(const float& TimeDelta);
	virtual void Release();
	virtual void CollisionPixelPart(DIRECTION dir);

protected:
	MONSTERTYPE m_Type = MONTYPE_END;
	MonsterState m_State = Monster_End;
	int target_id = 0;
};