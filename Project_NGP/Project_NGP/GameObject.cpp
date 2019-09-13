#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject()
{
	ZeroMemory(&m_Info, sizeof(GAMEOBJINFO));
	ZeroMemory(&m_Rect, sizeof(RECT));
}


GameObject::~GameObject()
{
}

int GameObject::Update(const float & TimeDelta)
{
	m_Rect.left = m_Info.Pos_X - m_Info.Size_Width / 2;
	m_Rect.top = m_Info.Pos_Y - m_Info.Size_Height / 2;
	m_Rect.right = m_Info.Pos_X + m_Info.Size_Width / 2;
	m_Rect.bottom = m_Info.Pos_Y + m_Info.Size_Height / 2;

	return 0;
}
