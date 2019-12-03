#pragma once
#include "GameObject.h"

class Text;
class Chat_Box
	: public GameObject
{
public:
	Chat_Box();
	virtual ~Chat_Box();
public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();
private:
	Text* m_Text = nullptr;
};
