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
	HDC hMemDC = GET_MANAGER<BmpManager>()->FindBmp(L"back")->GetMemDC();

	TransparentBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height
		, hMemDC, 0, 0, m_Info.Size_Width, m_Info.Size_Height, RGB(255, 255, 255));

	//Image* image = GET_MANAGER<GdiPlusManager>()->FindImage(L"background")->GetGdiPlusImageDefault();

	//Bitmap *bmp = new Bitmap(L"../Resources/back.bmp", PixelFormat32bppPARGB);

	//if (nullptr == image)
	//{
	//	return;
	//}

	//Graphics g(hdc);

	//g.DrawImage(bmp, m_Rect.left + (m_Info.Size_Width / 2),
	//	m_Rect.top + (m_Info.Size_Height / 2)/*,
	//	m_Info.Size_Width, m_Info.Size_Height*/);

}

void Background::Release()
{
}
