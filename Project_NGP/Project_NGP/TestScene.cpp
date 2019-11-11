#include "stdafx.h"
#include "TestScene.h"
#include "Player.h"
#include "Mouse.h"
#include "Background.h"
#include "Monster.h"

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
	m_pObjManager = GET_MANAGER<ObjectManager>();

	GET_MANAGER<GdiPlusManager>()->LoadImageBySceneState(SCENE_TEST);
	GET_MANAGER<GdiManager>()->LoadImageBySceneState(SCENE_TEST);
	
	m_pObjManager->AddObject(L"background", AbstractFactory<Background>::CreateObj(), OBJ_BACK);
	m_pObjManager->AddObject(L"player", AbstractFactory<Player>::CreateObj(), OBJ_PLAYER);
	//m_pObjManager->AddObject(L"monster", AbstractFactory<Monster>::CreateObj(), OBJ_MONSTER);
	m_pObjManager->AddObject(L"mouse", AbstractFactory<Mouse>::CreateObj(), OBJ_MOUSE);

	GameObject* pPlayer = m_pObjManager->GetObjFromTag(L"player", OBJ_PLAYER);
	m_CamMgr->SetTarget(pPlayer);

	GameObject* pBackGround = m_pObjManager->GetObjFromTag(L"background", OBJ_BACK);
	m_CamMgr->SetResolution(pBackGround->GetInfo().Size_Width, pBackGround->GetInfo().Size_Height);

	return true;
}

int TestScene::Update(const float & TimeDelta)
{
	KeyManager *keyManager = GET_MANAGER<KeyManager>();

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_F1))
	{
		bool recentCheck = GET_MANAGER<CollisionManager>()->GetRenderCheck();

		if (true == recentCheck)
			GET_MANAGER<CollisionManager>()->SetRenderCheck(false);
		else
			GET_MANAGER<CollisionManager>()->SetRenderCheck(true);
	}

	m_pObjManager->Update(TimeDelta);
	m_CamMgr->Update(TimeDelta);
	
	return 0;
}

void TestScene::Render(HDC hDC)
{
	//Rectangle(hDC, 0, 0, WINSIZE_X, WINSIZE_Y);
	m_pObjManager->Render(hDC);
}

void TestScene::Release()
{
	m_CamMgr->DestroyInstance();
	m_pObjManager->DestroyInstance();
}
