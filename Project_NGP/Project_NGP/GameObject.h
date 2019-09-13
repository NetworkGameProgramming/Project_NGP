#pragma once
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

public:
	virtual bool Initialize() PURE;
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc) PURE;
	virtual void Release() PURE;

protected:
	GAMEOBJINFO		m_Info;
	RECT			m_Rect;
};

