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

	// ������Ʈ ������ �������� Rect ������ �����Ѵ�.
	m_Rect.left = (int)TotalPos.X - m_Info.Size_Width / 2;
	m_Rect.top = (int)TotalPos.Y - m_Info.Size_Height / 2;
	m_Rect.right = (int)TotalPos.X + m_Info.Size_Width / 2;
	m_Rect.bottom = (int)TotalPos.Y + m_Info.Size_Height / 2;

	return 0;
}

void GameObject::CollisionActivate(GameObject* collideTarget)
{
}
