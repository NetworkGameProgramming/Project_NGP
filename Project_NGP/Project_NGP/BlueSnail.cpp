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
	m_Info = GAMEOBJINFO{ 800, 280, 41, 39 };
	m_CollideInfo = GAMEOBJINFO{ 0, 0, 40, 70 };
	m_Speed = 0.f;
	m_RenderType = RENDER_OBJ;
	m_Direction = DIR_RIGHT;
	m_SpriteInfo.key = L"bluesnail_right";
	m_SpriteInfo.CurState = Idle;
	m_SpriteInfo.PreState = End;
	m_SpriteInfo.SpriteIndex = 0.f;
	m_SpriteInfo.StateIndex = 0;
	//몬스터가 Idle이 되기까지의 시간
	m_ChangeIdleTime = float((rand() % 10) + 5);
	m_CloseIdleTime = 3.f;
	//m_Dir |= 0x00000002;
	return true;
}

int BlueSnail::Update_Input(const float& TimeDelta)
{
	if (true == m_isOther)
		return 0;
	KeyManager* keyManager = GET_MANAGER<KeyManager>();
	//몬스터가 지형의 끝에 도달했는지
	bool OnBulePixel = false;
	//몬스터가 플레이어로부터 공격받았는지
	bool Damaged = false;
	//몬스터가 플레이어의 방향과 일치하는 방향에서 맞았는지
	bool HitBack = false;

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_LCONTROL))
	{
		Damaged = true;
	}

	if (m_SpriteInfo.CurState == Idle)
	{
		m_CloseIdleDelta += TimeDelta;
		if (m_CloseIdleTime <= m_CloseIdleDelta)
		{
			m_Direction = DIRECTION(rand() % 2);
			m_CloseIdleDelta = 0;
			m_SpriteInfo.CurState = Move;
			m_Speed = 100;
		}
	}
	else
	{
		//Idle 상태가 되기 까지의 시간의 누적치
		if (m_SpriteInfo.CurState == Hit) {}
		else
			m_IdleTimeDelta += TimeDelta;
	}
	//Idle이 되는 조건
	if (m_ChangeIdleTime <= m_IdleTimeDelta)
	{
		m_SpriteInfo.CurState = Idle;
		m_Speed = 0;
		//Idle이 되는 시간을 다시 랜덤하게 조절
		m_ChangeIdleTime = float((rand() % 10) + 5);
		m_IdleTimeDelta = 0;
	}
	//몬스터가 지형의 끝에 도달했으면 방향을 바꿔준다.
	if (OnBulePixel == true)
	{
		if (m_Direction == DIR_RIGHT)
		{
			m_Direction = DIR_LEFT;
			m_SpriteInfo.CurState = Move;
			OnBulePixel = false;
		}
		else if (m_Direction == DIR_LEFT)
		{
			m_Direction = DIR_RIGHT;
			m_SpriteInfo.CurState = Move;
			OnBulePixel = false;
		}
	}
	//몬스터가 공격 받았을때
	if (Damaged == true)
	{
		//만약 플레어의 방향과 몬스터의 방향이 반대라면 플레이어의 반대 방향으로 뒤돌아본다.
		if (HitBack)
		{
			if (m_Direction == DIR_LEFT)
			{
				m_Direction = DIR_RIGHT;
			}
			else
			{
				m_Direction = DIR_LEFT;
			}
		}
		m_SpriteInfo.CurState = Hit;
	}
	return 0;
}

int BlueSnail::Update(const float& TimeDelta)
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

int BlueSnail::Update_Position(const float& TimeDelta, const DIRECTION& Direction)
{
	int speed = int(m_Speed * TimeDelta);
	// L
	//if (0x00000001 == (m_Dir & 0x00000001))
	if (m_Direction == DIR_LEFT)
	{
		m_Info.Pos_X -= speed;
	}

	// R
	//if (0x00000002 == (m_Dir & 0x00000002))
	if (m_Direction == DIR_RIGHT)
	{
		m_Info.Pos_X += speed;
	}

	if (m_SpriteInfo.CurState == Hit)
	{
		m_KnockBackTimeDelta += TimeDelta;
		if (m_KnockBackTime <= m_KnockBackTimeDelta)

		{

			m_SpriteInfo.CurState = Move;

			m_Speed = 100;

			m_KnockBackTimeDelta = 0;

			return 0;

		}

		else

		{

			//m_Speed = 300;

			if (m_Direction == DIR_LEFT)

			{

				m_Info.Pos_X = Lerp<float, float>(m_Info.Pos_X, m_Info.Pos_X += 5, 5 * TimeDelta);

			}

			else

			{

				m_Info.Pos_X = Lerp<float, float>(m_Info.Pos_X, m_Info.Pos_X -= 5, 5 * TimeDelta);

			}



		}

	}

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