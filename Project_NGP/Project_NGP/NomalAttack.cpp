#include "stdafx.h"
#include "NomalAttack.h"
NomalAttack::NomalAttack()
{
}

NomalAttack::~NomalAttack()
{
	Release();
}

void NomalAttack::SetPosition(int posX, int posY)
{
	m_Info.Pos_X = posX;
	m_Info.Pos_Y = posY;

	if (DIR_LEFT == m_Direction)
	{
		m_Info.Pos_X += 100;
	}
	else if (DIR_RIGHT == m_Direction)
	{
		m_Info.Pos_X -= 100;
	}
}

bool NomalAttack::Initialize()
{
	m_Info = GAMEOBJINFO{ 0, 0, 139, 118 };
	m_CollideInfo = GAMEOBJINFO{ 0, 0, 139, 118 };
	m_Speed = 0.f;
	m_RenderType = RENDER_EFFECT;
	m_SpriteInfo.key = L"nomal_attack";
	m_SpriteInfo.CurState = Ready;
	m_SpriteInfo.PreState = Play;
	m_SpriteInfo.SpriteIndex = 0.f;
	m_SpriteInfo.StateIndex = 0;

	return true;
}

int NomalAttack::Update_Input(const float& TimeDelta)
{
	if (Play == m_SpriteInfo.CurState)
	{
		/*if (m_SpriteInfo.SpriteIndex == m_SpriteInfo.MaxFrame)
		{
			m_SpriteInfo.CurState = Ready;
		}
		else*/
		return 0;
	}
	else if (Ready == m_SpriteInfo.CurState)
	{
		m_SpriteInfo.SpriteIndex = 0.f;
		m_SpriteInfo.CurState = Play;
	}

	return 0;
}

int NomalAttack::Update(const float& TimeDelta)
{
	m_TimeDelta = TimeDelta;
	if (-1 == GameObject::Update(TimeDelta))
	{
		return -1;
	}

	if (-1 == Update_Input(TimeDelta))
	{
		return -1;
	}

	if (-1 == Update_Position(TimeDelta, m_Direction))
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

int NomalAttack::Update_Position(const float& TimeDelta, const DIRECTION& Direction)
{
	//int speed = int(m_Speed * TimeDelta);
	/*if (DIR_LEFT == m_Direction)
	{
		m_Info.Pos_X += 100;
	}
	else if (DIR_RIGHT == m_Direction)
	{
		m_Info.Pos_X -= 100;
	}*/

	return 0;
}

int NomalAttack::Update_Sprite(const float& TimeDelta)
{
	m_SpriteInfo.SpriteIndex += m_SpriteInfo.Speed * TimeDelta;

	switch (m_SpriteInfo.Type)
	{
	case SPRITE_ONCE:
		if ((float)m_SpriteInfo.MaxFrame <= m_SpriteInfo.SpriteIndex)
		{
			m_SpriteInfo.CurState = Ready;
		}
		break;
	}

	switch (m_Direction)
	{
	case DIR_LEFT: m_SpriteInfo.key = L"nomal_attack"; break;
	case DIR_RIGHT: m_SpriteInfo.key = L"nomal_attack"; break;
	}

	StateChange();
	return 0;
}

void NomalAttack::Render(HDC hdc)
{
	if (true == GET_MANAGER<CollisionManager>()->GetRenderCheck())
		Rectangle(hdc, m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);

	Graphics G(hdc);

	Image* img = GET_MANAGER<GdiPlusManager>()->FindImage(m_SpriteInfo.key)->GetGdiPlusImageFromIndex((int)m_SpriteInfo.SpriteIndex);

	G.DrawImage(img, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height);
}

void NomalAttack::Release()
{
}


void NomalAttack::StateChange()
{
	if (m_SpriteInfo.CurState != m_SpriteInfo.PreState)
	{
		m_SpriteInfo.SpriteIndex = 0.f;
		switch (m_SpriteInfo.CurState)
		{
		case Ready:
			m_SpriteInfo.Type = SPRITE_ONCE;
			m_SpriteInfo.MaxFrame = 1;
			m_SpriteInfo.Speed = 0.f;
			break;
		case Play:
			m_SpriteInfo.Type = SPRITE_ONCE;
			m_SpriteInfo.MaxFrame = 5;
			m_SpriteInfo.Speed = 15.f;
			break;
		}
	}

	m_SpriteInfo.PreState = m_SpriteInfo.CurState;
}