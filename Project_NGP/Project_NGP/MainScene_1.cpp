#include "stdafx.h"
#include "MainScene_1.h"
#include "Player.h"
#include "Mouse.h"
#include "Background.h"
#include "Monster.h"
#include "BlueSnail.h"
#include "NomalAttack.h"

MainScene_1::MainScene_1()
	:Scene()
{
}

MainScene_1::~MainScene_1()
{
	Release();
}

bool MainScene_1::Initialize()
{
	GET_MANAGER<GdiPlusManager>()->LoadImageBySceneState(SCENE_TEST);
	GET_MANAGER<GdiManager>()->LoadImageBySceneState(SCENE_TEST);

	m_ObjManager->AddObject(L"background", AbstractFactory<Background>::CreateObj(), OBJ_BACK);
	m_ObjManager->AddObject(L"nomalattack", AbstractFactory<NomalAttack>::CreateObj(), OBJ_EFFECT);
	m_ObjManager->AddObject(L"player", AbstractFactory<Player>::CreateObj(), OBJ_PLAYER);
	m_ObjManager->AddObject(L"mouse", AbstractFactory<Mouse>::CreateObj(), OBJ_MOUSE);

	GameObject* pPlayer = m_ObjManager->GetObjFromTag(L"player", OBJ_PLAYER);
	m_CamManager->SetTarget(pPlayer);

	GameObject* pBackGround = m_ObjManager->GetObjFromTag(L"background", OBJ_BACK);
	m_CamManager->SetResolution(pBackGround->GetInfo().Size_Width, pBackGround->GetInfo().Size_Height);


	if (false == InitializeNetwork(SCENE_MAIN_1))
		return false;

	return true;
}

int MainScene_1::Update(const float& TimeDelta)
{
	KeyManager* keyManager = GET_MANAGER<KeyManager>();

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_F1))
	{
		bool recentCheck = GET_MANAGER<CollisionManager>()->GetRenderCheck();

		if (true == recentCheck)
			GET_MANAGER<CollisionManager>()->SetRenderCheck(false);
		else
			GET_MANAGER<CollisionManager>()->SetRenderCheck(true);
	}

	m_ObjManager->Update(TimeDelta);
	m_CamManager->Update(TimeDelta);

#ifdef SERVER_MODE
	if (false == UpdateNetwork())
		return -1;
#endif

	return 0;
}

void MainScene_1::Render(HDC hDC)
{
	m_ObjManager->Render(hDC);
}

void MainScene_1::Release()
{
	m_ObjManager->ReleaseFromType(OBJ_BACK);
	m_ObjManager->ReleaseFromType(OBJ_OTHERPLAYER);
	m_ObjManager->ReleaseFromType(OBJ_MONSTER);
	m_ObjManager->ReleaseFromType(OBJ_EFFECT);
}
