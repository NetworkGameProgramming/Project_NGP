#include "stdafx.h"
#include "Mouse.h"

Mouse::Mouse()
{
}

Mouse::~Mouse()
{
	Release();
}

bool Mouse::Initialize()
{
	m_Info.Size_Width = 30;
	m_Info.Size_Height = 30;

	ShowCursor(false);

	return true;
}

int Mouse::Update(const float & TimeDelta)
{
	// GetCursorPos: ���� ���콺 Ŀ�� ��ġ�� ��ǥ�� ������ �Լ�.
	GetCursorPos(&m_MousePos);

	// ScreenToClient: Ŭ���̾�Ʈ ��ǥ �������� ��ȯ���ִ� �Լ�.
	ScreenToClient(g_hWnd, &m_MousePos);

	m_Info.Pos_X = m_MousePos.x;
	m_Info.Pos_Y = m_MousePos.y;

	GameObject::Update(TimeDelta);
	return 0;
}

void Mouse::Render(HDC hdc)
{
	Image* image = GET_MANAGER<GdiPlusManager>()->FindImage(L"cursor")->GetGdiPlusImageDefault();

	Graphics g(hdc);
	g.DrawImage(image, m_Info.Pos_X, m_Info.Pos_Y, m_Info.Size_Width, m_Info.Size_Height);

	//Ellipse(hdc, m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);
}

void Mouse::Release()
{
	ShowCursor(true);
}
