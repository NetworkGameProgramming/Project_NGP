#include "stdafx.h"
#include "TestScene.h"
#include "Player.h"
#include "Mouse.h"
#include "Background.h"
#include "Monster.h"
#include "BlueSnail.h"
#include "AttackEffect.h"
#include "NomalAttack.h"
TestScene::TestScene()
	:Scene()
{
}

TestScene::~TestScene()
{
	Release();
}

bool TestScene::Initialize()
{
	GET_MANAGER<GdiPlusManager>()->LoadImageBySceneState(SCENE_TEST);
	GET_MANAGER<GdiManager>()->LoadImageBySceneState(SCENE_TEST);
	
	m_ObjManager->AddObject(L"background", AbstractFactory<Background>::CreateObj(), OBJ_BACK);
	//m_ObjManager->AddObject(L"attackeffect", AbstractFactory<AttackEffect>::CreateObj(), OBJ_EFFECT);
	m_ObjManager->AddObject(L"nomalattack", AbstractFactory<NomalAttack>::CreateObj(), OBJ_EFFECT);
	m_ObjManager->AddObject(L"player", AbstractFactory<Player>::CreateObj(), OBJ_PLAYER);
	//m_ObjManager->AddObject(L"monster", AbstractFactory<Monster>::CreateObj(), OBJ_MONSTER);
	//m_ObjManager->AddObject(L"BlueSnail", AbstractFactory<BlueSnail>::CreateObj(), OBJ_MONSTER);
	m_ObjManager->AddObject(L"bluesnail", AbstractFactory<BlueSnail>::CreateObj(), OBJ_MONSTER);
	m_ObjManager->AddObject(L"mouse", AbstractFactory<Mouse>::CreateObj(), OBJ_MOUSE);

	GameObject* pPlayer = m_ObjManager->GetObjFromTag(L"player", OBJ_PLAYER);
	m_CamManager->SetTarget(pPlayer);

	GameObject* pBackGround = m_ObjManager->GetObjFromTag(L"background", OBJ_BACK);
	m_CamManager->SetResolution(pBackGround->GetInfo().Size_Width, pBackGround->GetInfo().Size_Height);

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

	m_ObjManager->Update(TimeDelta);
	m_CamManager->Update(TimeDelta);

	return 0;
}

void TestScene::Render(HDC hDC)
{
	//Rectangle(hDC, 0, 0, WINSIZE_X, WINSIZE_Y);
	m_ObjManager->Render(hDC);
}

void TestScene::Release()
{
	m_ObjManager->ReleaseFromType(OBJ_BACK);
	m_ObjManager->ReleaseFromType(OBJ_OTHERPLAYER);
	m_ObjManager->ReleaseFromType(OBJ_MONSTER);
	m_ObjManager->ReleaseFromType(OBJ_EFFECT);
}