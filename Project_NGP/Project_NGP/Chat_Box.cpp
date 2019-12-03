#include "stdafx.h"
#include "Chat_Box.h"
#include "Text.h"

Chat_Box::Chat_Box()
	:GameObject()
{
}

Chat_Box::~Chat_Box()
{
	Release();
}

bool Chat_Box::Initialize()
{
	m_Info = GAMEOBJINFO{ 0, 0, 97, 77 };
	m_RenderType = RENDER_UI_2;

	/*m_Text = new Text;
	m_Text->Initialize(11);
	m_Text->SetNewlineCount(8);
	m_Text->SetText(L"123");
	m_Text->SetBackColor(255, 255, 255);
	m_Text->SetBackMode(OPAQUE);*/

	return true;
}

int Chat_Box::Update(const float& TimeDelta)
{
	if (-1 == GameObject::Update(TimeDelta))
		return -1;

	if (nullptr != m_FollowedObj)
	{
		GAMEOBJINFO objInfo = m_FollowedObj->GetInfo();
		m_Info.Pos_X = objInfo.Pos_X;
		m_Info.Pos_Y = objInfo.Pos_Y - (m_Info.Size_Height + 0);
	}

	return 0;
}

void Chat_Box::Render(HDC hdc)
{
	HDC hMemDC = GET_MANAGER<GdiManager>()->FindImage(L"chat_box")->GetGdiImageDefault();

	TransparentBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height,
		hMemDC, 0, 0, m_Info.Size_Width, m_Info.Size_Height, RGB(255, 0, 255));
	
	if (nullptr != m_Text)
	{
		m_Text->SetPosition(m_Rect.left + 5, m_Rect.top + 5);
		m_Text->Render(hdc);
	}
}

void Chat_Box::Release()
{
	if (nullptr != m_Text)
	{
		delete m_Text;
		m_Text = nullptr;
	}
}
