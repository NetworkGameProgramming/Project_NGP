#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject()
{
	ZeroMemory(&m_Info, sizeof(GAMEOBJINFO));
	ZeroMemory(&m_Rect, sizeof(RECT));
	ZeroMemory(&m_SpriteInfo, sizeof(SPRITEINFO));
}


GameObject::~GameObject()
{
}

void GameObject::SetPosition(int posX, int posY)
{
	m_Info.Pos_X = posX;
	m_Info.Pos_Y = posY;
}

void GameObject::SetSize(int sizeWidth, int sizeHeight)
{
	m_Info.Size_Width = sizeWidth;
	m_Info.Size_Height = sizeHeight;
}

void GameObject::SetAngle(float angle)
{
	m_Angle = angle;
}

void GameObject::SetSpeed(float speed)
{
	m_Speed = speed;
}

int GameObject::Update(const float & TimeDelta)
{
	POSITION CamPos = GET_MANAGER<CameraManager>()->GetPos();

	if (true == m_isNoScroll)
	{
		ZeroMemory(&CamPos, sizeof(POSITION));
	}

	POSITION TotalPos = POSITION{ m_Info.Pos_X + CamPos.X , m_Info.Pos_Y + CamPos.Y };

	// 오브젝트 정보를 바탕으로 Rect 정보를 갱신한다.
	m_Rect.left = TotalPos.X - m_Info.Size_Width / 2;
	m_Rect.top = TotalPos.Y - m_Info.Size_Height / 2;
	m_Rect.right = TotalPos.X + m_Info.Size_Width / 2;
	m_Rect.bottom = TotalPos.Y + m_Info.Size_Height / 2;

	return 0;
}
