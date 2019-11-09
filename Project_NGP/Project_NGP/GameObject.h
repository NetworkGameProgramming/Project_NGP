#pragma once
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

public:
	const GAMEOBJINFO& GetInfo()		{ return m_Info; }
	const RECT& GetRect()				{ return m_Rect; }
	const RENDERTYPE& GetRenderType()	{ return m_RenderType; }
	const bool& GetState()				{ return m_isDead; }
	const float& GetAngle()				{ return m_Angle; }
	const float& GetSpeed()				{ return m_Speed; }

public:
	void SetPosition(int posX, int posY);
	void SetSize(int sizeWidth, int sizeHeight);
	void SetAngle(float angle);
	void SetSpeed(float speed);

public:
	virtual bool Initialize() PURE;
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc) PURE;
	virtual void Release() PURE;
	virtual void CollisionActivate(GameObject* collideTarget);

protected:
	GAMEOBJINFO		m_Info;
	RECT			m_Rect;
	SPRITEINFO		m_SpriteInfo;

	RENDERTYPE		m_RenderType = RENDER_END;

	bool			m_isNoScroll = false;
	bool			m_isDead = false;
	float			m_Angle = 0.f;

	float			m_Speed = 0.f;
};

