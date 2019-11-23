#pragma once
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

public:
	const GAMEOBJINFO& GetInfo()				{ return m_Info; }
	const GAMEOBJINFO& GetCollideInfo()			{ return m_CollideInfo; }
	const RECT& GetCollideRect()				{ return m_CollideRect; }
	const RENDERTYPE& GetRenderType()			{ return m_RenderType; }
	const bool& GetState()						{ return m_isDead; }
	const float& GetAngle()						{ return m_Angle; }
	const float& GetSpeed()						{ return m_Speed; }
	const PIXELCOLLIDERINFO* GetPixelCollider() { return m_PixelInfo; }
public:
	void SetPosition(int posX, int posY);
	void SetSize(int sizeWidth, int sizeHeight);
	void SetAngle(float angle);
	void SetSpeed(float speed);
	void SetCollideOn(bool on);

public:
	bool LoadPixelCollider(const char* pFilePath,
		unsigned char r, unsigned char g, unsigned char b);

public:
	virtual bool Initialize() PURE;
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc) PURE;
	virtual void Release() PURE;
	virtual void CollisionActivate(GameObject* collideTarget);
	virtual void CollisionPixelPart(DIRECTION dir);
protected:
	GAMEOBJINFO		m_Info;
	SPRITEINFO		m_SpriteInfo;

	RENDERTYPE		m_RenderType = RENDER_END;

	bool			m_isNoScroll = false;
	bool			m_isDead = false;
	bool			m_isCollideOn = true;
	float			m_Angle = 0.f;
	float			m_Speed = 0.f;

	// Collider
	PIXELCOLLIDERINFO	*m_PixelInfo = nullptr;
	RECT				m_Rect;

	GAMEOBJINFO			m_CollideInfo;
	RECT				m_CollideRect;
};

