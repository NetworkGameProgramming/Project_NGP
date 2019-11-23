#include "stdafx.h"
#include "Background.h"


Background::Background()
	: GameObject()
{
}


Background::~Background()
{
	Release();
}

bool Background::Initialize()
{
	// 1280 720
	m_Info = GAMEOBJINFO{ (int)(1600 * 1) / 2,(int)(1200 * 1) /2, (int)(1600 * 1), (int)(1200 * 1)};
	m_RenderType = RENDER_BACKGROUND;

	LoadPixelCollider("../Resources/back_col.bmp", 255, 0, 255);

	return true;
}

int Background::Update(const float & TimeDelta)
{
	if (-1 == GameObject::Update(TimeDelta))
	{
		return -1;
	}

	return 0;
}

void Background::Render(HDC hdc)
{
	HDC hMemDC = GET_MANAGER<GdiManager>()->FindImage(L"back")->GetGdiImageDefault();

	BitBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height
		, hMemDC, 0, 0, SRCCOPY);
}

void Background::Release()
{
}
