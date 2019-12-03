#include "stdafx.h"
#include "Chat.h"

Chat::Chat()
	: GameObject()
{
}

Chat::~Chat()
{
}

bool Chat::Initialize()
{
	m_Info = GAMEOBJINFO{ 450 / 2, WINSIZE_Y - 56 - (24 / 2), 450, 24 };
	m_isNoScroll = true;
	m_RenderType = RENDER_UI;
	return true;
}

int Chat::Update(const float& TimeDelta)
{
	if (-1 == GameObject::Update(TimeDelta))
		return -1;

	return 0;
}

void Chat::Render(HDC hdc)
{
	HDC hMemDC = GET_MANAGER<GdiManager>()->FindImage(L"chat")->GetGdiImageDefault();

	BitBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height,
		hMemDC, 0, 0, SRCCOPY);
}

void Chat::Release()
{
}
