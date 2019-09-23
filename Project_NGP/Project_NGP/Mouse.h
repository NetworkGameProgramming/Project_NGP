#pragma once
#include "GameObject.h"

class Mouse 
	: public GameObject
{
public:
	Mouse();
	virtual ~Mouse();

public:
	// ���콺 ��ü�� ��� ����� �����ϰ�
	static const POINT& GetMousePos()
	{
		POINT pos;

		GetCursorPos(&pos);
		ScreenToClient(g_hWnd, &pos);

		return pos;
	}

public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();

private:
	POINT m_MousePos;
};

