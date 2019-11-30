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
	if (true == m_isOther)
		return 0;

	m_Dir = 0;
	KeyManager* keyManager = GET_MANAGER<KeyManager>();

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_LEFT))
	{
		m_Dir |= 0x00000001;
		m_Direction = DIR_LEFT;

		if (false == m_fallCheck)
			m_SpriteInfo.CurState = Walk;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_RIGHT))
	{
		m_Dir |= 0x00000002;
		m_Direction = DIR_RIGHT;

		if (false == m_fallCheck)
			m_SpriteInfo.CurState = Walk;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_UP))
	{
		m_Dir |= 0x00000004;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_DOWN))
	{
		m_Dir |= 0x00000008;
		m_SpriteInfo.CurState = Crouch;
	}

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_SPACE))
	{
		// ����
		m_SpriteInfo.CurState = Jump;
		m_GravitySpeed = -300.f;
		m_GravityAcc = 0.f;
		m_fallCheck = true;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_LCONTROL))
	{
		int r = rand() % 2;
		if (0 == r)
			m_SpriteInfo.CurState = Att_1;
		else
			m_SpriteInfo.CurState = Att_2;
	}

	if (false == m_fallCheck &&
		false == keyManager->GetKeyState(STATE_PUSH, VK_LEFT) &&
		false == keyManager->GetKeyState(STATE_PUSH, VK_RIGHT) &&
		false == keyManager->GetKeyState(STATE_PUSH, VK_LCONTROL))
	{
		m_SpriteInfo.CurState = Idle;
	}

	return 0;
}

int Player::Update_Position(const float& TimeDelta, const DIRECTION& Direction)
{
	if (true == m_isOther)
		return 0;

	int speed = int(m_Speed * TimeDelta);

	// L
	if (0x00000001 == (m_Dir & 0x00000001))
	{
		m_Info.Pos_X -= speed;
	}

	// R
	if (0x00000002 == (m_Dir & 0x00000002))
	{
		m_Info.Pos_X += speed;
	}

	// U
	if (0x00000004 == (m_Dir & 0x00000004))
	{
		// m_Info.Pos_Y -= speed * 2;
	}

	// D
	if (0x00000008 == (m_Dir & 0x00000008))
	{
		// m_Info.Pos_Y += speed;
	}

	// �߷�
	m_Info.Pos_Y += int(m_GravitySpeed * TimeDelta);
	m_GravitySpeed += m_GravityAcc * TimeDelta;

	if (true == m_fallCheck)
	{
		if (1000.f > m_GravityAcc)
			m_GravityAcc += 9.8f * 10.f;
	}

	return 0;
}

int Player::Update_Sprite(const float& TimeDelta)
{
	m_SpriteInfo.SpriteIndex += m_SpriteInfo.Speed * TimeDelta;

	switch (m_SpriteInfo.Type)
	{
	case SPRITE_ONCE:
		if ((float)m_SpriteInfo.MaxFrame <= m_SpriteInfo.SpriteIndex)
		{
			m_SpriteInfo.CurState = 0;
		}
		break;
	case SPRITE_ONCE_END:
		if ((float)m_SpriteInfo.MaxFrame > m_SpriteInfo.SpriteIndex)
		{
			m_SpriteInfo.CurState = m_SpriteInfo.PreState;
		}
		else
		{
			m_SpriteInfo.CurState = 0;
		}
		break;
	case SPRITE_REPEAT:
		if ((float)m_SpriteInfo.MaxFrame <= m_SpriteInfo.SpriteIndex)
		{
			m_SpriteInfo.SpriteIndex = 0.f;
		}
		break;
	}

	switch (m_Direction)
	{
	case DIR_LEFT: m_SpriteInfo.key = L"player_left"; break;
	case DIR_RIGHT: m_SpriteInfo.key = L"player_right"; break;
	}

	return 0;
}

bool Player::Initialize()
{
	m_Info = GAMEOBJINFO{ 800, 600, 200, 200 };
	m_CollideInfo = GAMEOBJINFO{ 0, 0, 40, 70 };
	m_Speed = 200.f;
	m_RenderType = RENDER_OBJ;

	m_Direction = DIR_RIGHT;
	m_SpriteInfo.key = L"player_right";
	m_SpriteInfo.CurState = Idle;
	m_SpriteInfo.PreState = End;
	m_SpriteInfo.SpriteIndex = 0.f;
	m_SpriteInfo.StateIndex = 0;

	return true;
}

int Player::Update(const float& TimeDelta)
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

void Player::Render(HDC hdc)
{
	if (true == GET_MANAGER<CollisionManager>()->GetRenderCheck())
		Rectangle(hdc, m_CollideRect.left, m_CollideRect.top, m_CollideRect.right, m_CollideRect.bottom);

	HDC hMemDC = GET_MANAGER<GdiManager>()->FindImage(m_SpriteInfo.key)->GetGdiImageDefault();

	TransparentBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height,
		hMemDC,
		(int)m_SpriteInfo.SpriteIndex * m_Info.Size_Width,
		m_SpriteInfo.StateIndex * m_Info.Size_Height,
		m_Info.Size_Width, m_Info.Size_Height, RGB(255, 0, 255));

	//std::cout << m_Info.Pos_X << ", " << m_Info.Pos_Y << std::endl;
}

void Player::Release()
{
}

void Player::CollisionPixelPart(DIRECTION dir)
{
	int speed = int(m_Speed * m_TimeDelta);
	switch (dir)
	{
	case DIR_BOTTOM:
		m_GravitySpeed = 0.f;
		m_GravityAcc = 0.f;
		m_fallCheck = false;
		m_SpriteInfo.CurState = Idle;
		break;
	}
}

void Player::StateChange()
{
	if (m_SpriteInfo.CurState != m_SpriteInfo.PreState)
	{
		m_SpriteInfo.SpriteIndex = 0.f;
		switch (m_SpriteInfo.CurState)
		{
		case Idle:
			m_SpriteInfo.Type = SPRITE_REPEAT;
			m_SpriteInfo.StateIndex = 0;
			m_SpriteInfo.MaxFrame = 5;
			m_SpriteInfo.Speed = 2.f;
			break;
		case Walk:
			m_SpriteInfo.Type = SPRITE_REPEAT;
			m_SpriteInfo.StateIndex = 2;
			m_SpriteInfo.MaxFrame = 4;
			m_SpriteInfo.Speed = 6.f;
			break;
		case Jump:
			m_SpriteInfo.Type = SPRITE_REPEAT;
			m_SpriteInfo.StateIndex = 5;
			m_SpriteInfo.MaxFrame = 1;
			m_SpriteInfo.Speed = 1.f;
			break;
		case Crouch:
			m_SpriteInfo.Type = SPRITE_REPEAT;
			m_SpriteInfo.StateIndex = 11;
			m_SpriteInfo.MaxFrame = 1;
			m_SpriteInfo.Speed = 1.f;
			break;
		case Att_1:
			m_SpriteInfo.Type = SPRITE_ONCE_END;
			m_SpriteInfo.StateIndex = 14;
			m_SpriteInfo.MaxFrame = 3;
			m_SpriteInfo.Speed = 4.f;
			break;
		case Att_2:
			m_SpriteInfo.Type = SPRITE_ONCE_END;
			m_SpriteInfo.StateIndex = 15;
			m_SpriteInfo.MaxFrame = 3;
			m_SpriteInfo.Speed = 4.f;
			break;
		}
	}

	m_SpriteInfo.PreState = m_SpriteInfo.CurState;
}
