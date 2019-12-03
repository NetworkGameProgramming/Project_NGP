#pragma once
#include "GameObject.h"
#include "NomalAttack.h"
#include "SmashAttack.h"
class Player
	: public GameObject
{
public:
	enum State { Idle, Idle_Att, Walk, Jump, Crouch,
		Att_1, Att_2, Att_3,
		Hanging, End };

public:
	Player();
	virtual ~Player();

public:
	void SetOtherCheck(bool check) { m_isOther = check; }

private:
	int Update_Input(const float& TimeDelta);
	int Update_Position(const float& TimeDelta, const DIRECTION& Direction);
	int Update_Skill(const float& TimeDelta);
	int Update_Sprite(const float& TimeDelta);

public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();
	virtual void CollisionPixelPart(DIRECTION dir);

public:
	virtual void CollisionActivate(GameObject* collideTarget);
	virtual void CollisionDeactivate(GameObject* collideTarget);

public:
	void StateChange();

private:
	float m_TimeDelta = 0.f;
	//float m_GravitySpeed = 0.f;
	//float m_GravityAcc = 9.8f;
	bool m_isOther = false;
	
	SCENESTATE m_NextSceneInfo = SCENE_END;
	bool	  m_isReadyGoNext = false;

	bool m_OnceCheck = false;
};

