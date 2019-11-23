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
		// 점프
		m_SpriteInfo.CurState = Att_1;
		m_GravitySpeed = -400.f;
		m_GravityAcc = 0.f;
		m_fallCheck = true;
	}

	return 0;
}

int Player::Update_Position(const float& TimeDelta, const DIRECTION & Direction)
{
	int speed = int(m_Speed * TimeDelta);

	// L
	if (0x00000001 == (m_Dir & 0x00000001))
	{
		m_Info.Pos_X -= speed;
		m_fallCheck = true;
	}

	// R
	if (0x00000002 == (m_Dir & 0x00000002))
	{
		m_Info.Pos_X += speed;
		m_fallCheck = true;
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

	// 중력
	m_Info.Pos_Y += int(m_GravitySpeed * TimeDelta);
	m_GravitySpeed += m_GravityAcc * TimeDelta;
	
	if (true == m_fallCheck)
	{
		if (2000.f > m_GravityAcc)
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
	case SPRITE_REPEAT:
		if ((float)m_SpriteInfo.MaxFrame <= m_SpriteInfo.SpriteIndex)
		{
			m_SpriteInfo.SpriteIndex = 0.f;
		}
		break;
	}

	return 0;
}

bool Player::Initialize()
{
	m_Info = GAMEOBJINFO{ 800, 600, 200, 200};
	m_CollideInfo = GAMEOBJINFO{ 0, 0, 40, 70 };
	m_Speed = 200.f;
	m_RenderType = RENDER_OBJ;

	m_SpriteInfo.key = L"player_left";
	m_SpriteInfo.CurState = Idle;
	m_SpriteInfo.PreState = End;
	m_SpriteInfo.SpriteIndex = 0.f;
	m_SpriteInfo.StateIndex = 0;

	return true;
}

int Player::Update(const float & TimeDelta)
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
	if(true == GET_MANAGER<CollisionManager>()->GetRenderCheck())
		Rectangle(hdc, m_CollideRect.left, m_CollideRect.top, m_CollideRect.right, m_CollideRect.bottom);

	HDC hMemDC = GET_MANAGER<GdiManager>()->FindImage(m_SpriteInfo.key)->GetGdiImageDefault();
	
	TransparentBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height, 
		hMemDC, 
		(int)m_SpriteInfo.SpriteIndex * m_Info.Size_Width, 
		m_SpriteInfo.StateIndex * m_Info.Size_Height, 
		m_Info.Size_Width, m_Info.Size_Height, RGB(255, 0, 255));
}

void Player::Release()
{
}

void Player::CollisionPixelPart(DIRECTION dir)
{
	int speed = int(m_Speed * m_TimeDelta);
	switch (dir)
	{
	case DIR_LEFT:
		m_Info.Pos_X += speed;
		break;
	case DIR_RIGHT:
		m_Info.Pos_X -= speed;
		break;
	case DIR_BOTTOM:
		m_GravitySpeed = 0.f;
		m_GravityAcc = 0.f;
		m_fallCheck = false;
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
		case Att_1:
			m_SpriteInfo.Type = SPRITE_ONCE;
			m_SpriteInfo.StateIndex = 5;
			m_SpriteInfo.MaxFrame = 1;
			m_SpriteInfo.Speed = 1.5f;
			break;
		}
	}

	m_SpriteInfo.PreState = m_SpriteInfo.CurState;
}
