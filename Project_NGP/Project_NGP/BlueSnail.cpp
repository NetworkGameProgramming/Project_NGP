#include "stdafx.h"
#include "BlueSnail.h"

BlueSnail::BlueSnail()
{
}

BlueSnail::~BlueSnail()
{
	Release();
}

bool BlueSnail::Initialize()
{
	m_Info = GAMEOBJINFO{ 0, 0, 41, 39 };
	m_CollideInfo = GAMEOBJINFO{ 0, 0, 40, 70 };
	m_RenderType = RENDER_OBJ;
	m_SpriteInfo.key = L"bluesnail_right";
	m_SpriteInfo.CurState = Idle;
	m_SpriteInfo.PreState = End;
	m_SpriteInfo.SpriteIndex = 0.f;
	m_SpriteInfo.StateIndex = 0;
	//몬스터가 Idle이 되기까지의 시간
	return true;
}

int BlueSnail::Update(const float& TimeDelta)
{
	if (-1 == GameObject::Update(TimeDelta))
	{
		return -1;
	}

	if (-1 == Update_Sprite(TimeDelta))
	{
		return -1;
	}

	StateChange();
	return 0;
}

int BlueSnail::Update_Sprite(const float& TimeDelta)
{
	m_SpriteInfo.SpriteIndex += m_SpriteInfo.Speed * TimeDelta;

	switch (m_SpriteInfo.Type)
	{
	case SPRITE_ONCE:
		if ((float)m_SpriteInfo.MaxFrame <= m_SpriteInfo.SpriteIndex)
		{

		}
		break;
	case SPRITE_ONCE_END:
		if ((float)m_SpriteInfo.MaxFrame > m_SpriteInfo.SpriteIndex)
		{
			m_SpriteInfo.CurState = m_SpriteInfo.PreState;
		}
		else
		{
			//m_SpriteInfo.CurState = 0;
		}
		break;
	case SPRITE_REPEAT:
		if ((float)m_SpriteInfo.MaxFrame <= m_SpriteInfo.SpriteIndex)
		{
			m_SpriteInfo.SpriteIndex = 0.f;
		}
		break;
	case SPRITE_REPEAT_END:
		if ((float)m_SpriteInfo.MaxFrame <= m_SpriteInfo.SpriteIndex)
		{
			//m_SpriteInfo.CurState = m_SpriteInfo.PreState;
			m_SpriteInfo.SpriteIndex = 4.f;
		}
		break;
	}

	switch (m_Direction)
	{
	case DIR_LEFT: m_SpriteInfo.key = L"bluesnail_left"; break;
	case DIR_RIGHT: m_SpriteInfo.key = L"bluesnail_right"; break;
	}

	StateChange();
	return 0;
}

void BlueSnail::Render(HDC hdc)
{
	if (true == GET_MANAGER<CollisionManager>()->GetRenderCheck())
		Rectangle(hdc, m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);

	HDC hMemDC = GET_MANAGER<GdiManager>()->FindImage(m_SpriteInfo.key)->GetGdiImageFromIndex((int)m_SpriteInfo.SpriteIndex);

	TransparentBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height
		, hMemDC, 0, 0, m_Info.Size_Width, m_Info.Size_Height, RGB(255, 0, 255));
}

void BlueSnail::Release()
{
}

void BlueSnail::StateChange()
{
	if (m_SpriteInfo.CurState != m_SpriteInfo.PreState)
	{
		m_SpriteInfo.SpriteIndex = 0.f;
		switch (m_SpriteInfo.CurState)
		{
		case Idle:
			m_SpriteInfo.Type = SPRITE_ONCE;
			m_SpriteInfo.MaxFrame = 1;
			m_SpriteInfo.Speed = 0.f;
			break;
		case Move:
			m_SpriteInfo.Type = SPRITE_REPEAT;
			m_SpriteInfo.MaxFrame = 4;
			m_SpriteInfo.Speed = 6.f;
			break;
		case Hit:
			m_SpriteInfo.Type = SPRITE_REPEAT_END;
			m_SpriteInfo.SpriteIndex = 4.f;
			m_SpriteInfo.MaxFrame = 1;
			m_SpriteInfo.Speed = 1.f;
			break;
		case Die:
			m_SpriteInfo.Type = SPRITE_ONCE_END;
			m_SpriteInfo.MaxFrame = 3;
			m_SpriteInfo.Speed = 4.f;
			break;
		}
	}

	m_SpriteInfo.PreState = m_SpriteInfo.CurState;
}