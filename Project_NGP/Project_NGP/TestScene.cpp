#include "stdafx.h"
#include "TestScene.h"
#include "Player.h"
#include "Mouse.h"

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
	Release();
}

bool TestScene::Initialize()
{
	GET_MANAGER<GdiPlusManager>()->LoadImageBySceneState(SCENE_TEST);
	m_Player = AbstractFactory<Player>::CreateObj();
	m_Mouse = AbstractFactory<Mouse>::CreateObj();

	return true;
}

int TestScene::Update(const float & TimeDelta)
{
	
	KeyManager *keyManager = GET_MANAGER<KeyManager>();
	
	m_Player->Update(TimeDelta);
	m_Mouse->Update(TimeDelta);

	return 0;
}

void TestScene::Render(HDC hDC)
{
	Rectangle(hDC, 0, 0, WINSIZE_X, WINSIZE_Y);
	m_Player->Render(hDC);
	m_Mouse->Render(hDC);
}

void TestScene::Release()
{
	if (m_Player)
	{
		delete m_Player;
		m_Player = nullptr;
	}

	if (m_Mouse)
	{
		delete m_Mouse;
		m_Mouse = nullptr;
	}
}
