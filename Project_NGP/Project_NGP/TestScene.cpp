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
	m_ObjManager = GET_MANAGER<ObjectManager>();
	m_NetworkManager = GET_MANAGER<NetworkManager>();

	GET_MANAGER<GdiPlusManager>()->LoadImageBySceneState(SCENE_TEST);
	GET_MANAGER<GdiManager>()->LoadImageBySceneState(SCENE_TEST);
	
	m_ObjManager->AddObject(L"background", AbstractFactory<Background>::CreateObj(), OBJ_BACK);
	m_ObjManager->AddObject(L"player", AbstractFactory<Player>::CreateObj(), OBJ_PLAYER);
	//m_ObjManager->AddObject(L"monster", AbstractFactory<Monster>::CreateObj(), OBJ_MONSTER);
	m_ObjManager->AddObject(L"mouse", AbstractFactory<Mouse>::CreateObj(), OBJ_MOUSE);

	GameObject* pPlayer = m_ObjManager->GetObjFromTag(L"player", OBJ_PLAYER);
	m_CamMgr->SetTarget(pPlayer);

	GameObject* pBackGround = m_ObjManager->GetObjFromTag(L"background", OBJ_BACK);
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

	m_ObjManager->Update(TimeDelta);
	m_CamMgr->Update(TimeDelta);

	// 플레이어 정보를 보낸다.
	GameObject *player = m_ObjManager->GetObjFromTag(L"player", OBJ_PLAYER);
	
	GAMEOBJINFO objInfo = player->GetInfo();
	m_NetworkManager->SendPlayerInfo(objInfo.Pos_X, objInfo.Pos_Y, player->GetSpriteInfo().CurState);
	
	// 다른 플레이어 정보를 받는다.
	char otherInfo[MAX_BUFFER] = { 0, };
	if(0 != m_NetworkManager->SendAndRecvOtherInfo(otherInfo))
	{
		char size = otherInfo[0];
		if (0 == size)
			return 0;

		int startAddrPos = 2;
		int count = (size - startAddrPos) / sizeof(SPOTHERPLAYERS);

		for (int i = 0; i < count; ++i)
		{
			SPOTHERPLAYERS info = SPOTHERPLAYERS{};
			memcpy(&info, (otherInfo + startAddrPos + sizeof(SPOTHERPLAYERS) * i),
				sizeof(SPOTHERPLAYERS));

			// 만약 등록된 id의 플레이어가 없다면 만든다.
			wstring s = to_wstring(info.id);
			GameObject* other_player = nullptr;
			other_player = m_ObjManager->GetObjFromTag(s.c_str(), OBJ_OTHERPLAYER);

			if (nullptr == other_player)
			{
				other_player = AbstractFactory<Player>::CreateObj();
				dynamic_cast<Player*>(other_player)->SetOtherCheck(true);
				m_ObjManager->AddObject(s.c_str(), other_player, OBJ_OTHERPLAYER);
			}

			// 위치
			other_player->SetPosition(info.pos_x, info.pos_y);

			// 스프라이트 상태
			SPRITEINFO sprite_info = other_player->GetSpriteInfo();
			sprite_info.CurState = info.player_state;
			other_player->SetSpriteInfo(sprite_info);
		}
	}
	
	return 0;
}

void TestScene::Render(HDC hDC)
{
	//Rectangle(hDC, 0, 0, WINSIZE_X, WINSIZE_Y);
	m_ObjManager->Render(hDC);
}

void TestScene::Release()
{
	m_CamMgr->DestroyInstance();
	m_ObjManager->DestroyInstance();
}