#pragma once
#include "GameObject.h"

class Chat
	:public GameObject
{
public:
	Chat();
	virtual ~Chat();
public:
	virtual bool Initialize();
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc);
	virtual void Release();
};

