#include "stdafx.h"
#include "TestScene.h"
#include "Player.h"
#include "Mouse.h"
#include "Background.h"

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
	Release();
}

bool TestScene::Initialize()
{
	m_CamMgr = GET_MANAGER<CameraManager>();

	GET_MANAGER<GdiPlusManager>()->LoadImageBySceneState(SCENE_TEST);
	GET_MANAGER<BmpManager>()->LoadBmpBySceneState(SCENE_TEST);
	m_Background = AbstractFactory<Background>::CreateObj();
	//m_Player = AbstractFactory<Player>::CreateObj();
	
	for (int i = 0; i < 100; ++i)
	{
		GameObject *player = AbstractFactory<Player>::CreateObj(rand() % WINSIZE_X, rand() % WINSIZE_Y);
		m_vecPlayers.push_back(player);
	}
	
	m_Mouse = AbstractFactory<Mouse>::CreateObj();

	//m_CamMgr->SetTarget(m_Player);

	return true;
}

int TestScene::Update(const float & TimeDelta)
{
	KeyManager *keyManager = GET_MANAGER<KeyManager>();
	m_Background->Update(TimeDelta);
	//m_Player->Update(TimeDelta);
	for (const auto& player : m_vecPlayers)
	{
		player->Update(TimeDelta);
	}
	m_Mouse->Update(TimeDelta);
	m_CamMgr->Update(TimeDelta);
	return 0;
}

void TestScene::Render(HDC hDC)
{
	//Rectangle(hDC, 0, 0, WINSIZE_X, WINSIZE_Y);
	m_Background->Render(hDC);
	//m_Player->Render(hDC);
	for (const auto& player : m_vecPlayers)
	{
		player->Render(hDC);
	}
	m_Mouse->Render(hDC);
}

void TestScene::Release()
{
	if (m_Background)
	{
		delete m_Background;
		m_Background = nullptr;
	}

	/*if (m_Player)
	{
		delete m_Player;
		m_Player = nullptr;
	}
*/
	if (m_Mouse)
	{
		delete m_Mouse;
		m_Mouse = nullptr;
	}

	m_CamMgr->DestroyInstance();
}
