#include "stdafx.h"
#include "Player.h"


Player::Player()
	: GameObject()
{
}

Player::~Player()
{
	Release();
}

int Player::Update_Input(const float& TimeDelta)
{
	m_Dir = 0;
	KeyManager *keyManager = GET_MANAGER<KeyManager>();

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_LEFT))
	{
		m_Dir |= 0x00000001;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_RIGHT))
	{
		m_Dir |= 0x00000002;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_UP))
	{
		m_Dir |= 0x00000004;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_DOWN))
	{
		m_Dir |= 0x00000008;
	}

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_SPACE))
	{
		m_CurState = Attack;
	}

	return 0;
}

int Player::Update_Position(const float& TimeDelta, const DIRECTION & Direction)
{
	// L
	if (0x00000001 == (m_Dir & 0x00000001))
	{
		m_Info.Pos_X -= int(500.f * TimeDelta);
	}

	// R
	if (0x00000002 == (m_Dir & 0x00000002))
	{
		m_Info.Pos_X += int(500.f * TimeDelta);
	}

	// U
	if (0x00000004 == (m_Dir & 0x00000004))
	{
		m_Info.Pos_Y -= int(500.f * TimeDelta);
	}

	// D
	if (0x00000008 == (m_Dir & 0x00000008))
	{
		m_Info.Pos_Y += int(500.f * TimeDelta);
	}

	return 0;
}

int Player::Update_Sprite(const float& TimeDelta)
{
	m_SpriteIndex += m_SpriteInfo.Speed * TimeDelta;

	switch (m_SpriteInfo.Type)
	{
	case SPRITE_ONCE:
		if ((float)m_SpriteInfo.MaxFrame <= m_SpriteIndex)
		{
			m_CurState = Idle;
		}
		break;
	case SPRITE_REPEAT:
		if ((float)m_SpriteInfo.MaxFrame <= m_SpriteIndex)
		{
			m_SpriteIndex = 0.f;
		}
		break;
	}

	return 0;
}

bool Player::Initialize()
{
	m_Info = GAMEOBJINFO{ 400, 300, 312, 311};

	return true;
}

int Player::Update(const float & TimeDelta)
{
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

	if (-1 == GameObject::Update(TimeDelta))
	{
		return -1;
	}

	StateChange();

	return 0;
}

void Player::Render(HDC hdc)
{
	Image* image = GET_MANAGER<GdiPlusManager>()->FindImage(m_SpriteKey)->GetGdiPlusImageFromIndex((int)m_SpriteIndex);

	if (nullptr == image)
	{
		return;
	}

	Graphics g(hdc);
	g.DrawImage(image, m_Info.Pos_X, m_Info.Pos_Y, m_Info.Size_Width, m_Info.Size_Height);

	//Rectangle(hdc, m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);
}

void Player::Release()
{
}

void Player::StateChange()
{
	if (m_CurState != m_PreState)
	{
		m_SpriteIndex = 0.f;
		switch (m_CurState)
		{
		case Idle:
			m_SpriteKey = L"balrok";
			m_SpriteInfo.Type = SPRITE_REPEAT;
			m_SpriteInfo.MaxFrame = 1;
			m_SpriteInfo.Speed = 1.f;
			break;
		case Attack:
			m_SpriteKey = L"balrok_sprite";
			m_SpriteInfo.Type = SPRITE_ONCE;
			m_SpriteInfo.MaxFrame = 8;
			m_SpriteInfo.Speed = 10.f;
			break;
		}
	}

	m_PreState = m_CurState;
}
