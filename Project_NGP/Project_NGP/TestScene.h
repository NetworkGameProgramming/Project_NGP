#pragma once
#include "Scene.h"

class TestScene
	: public Scene
{
public:
	TestScene();
	virtual ~TestScene();

public:
	virtual bool Initialize();
	virtual int Update(const float& TimeDelta);
	virtual void Render(HDC hDC);
	virtual void Release();

private:
	GameObject *m_Player = nullptr;
	GameObject *m_Mouse = nullptr;

	bool m_IsProcess = false;
	float m_Process = 0.f;
	float m_Acc = 1.f;
	int  m_SrcX = 0;
	int  m_SrcY = 0;
	int	 m_DestX = 0;
	int  m_DestY = 0;
};

