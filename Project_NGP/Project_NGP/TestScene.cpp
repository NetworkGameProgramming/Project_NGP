#include "stdafx.h"
#include "TestScene.h"
#include "Player.h"
#include "Mouse.h"
#include "Background.h"
#include "Monster.h"
#include "BlueSnail.h"
#include "NomalAttack.h"
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
	m_ObjManager->AddObject(L"nomalattack", AbstractFactory<NomalAttack>::CreateObj(), OBJ_EFFECT);
	m_ObjManager->AddObject(L"player", AbstractFactory<Player>::CreateObj(), OBJ_PLAYER);
	//m_ObjManager->AddObject(L"monster", AbstractFactory<Monster>::CreateObj(), OBJ_MONSTER);
	//m_ObjManager->AddObject(L"BlueSnail", AbstractFactory<BlueSnail>::CreateObj(), OBJ_MONSTER);
	m_ObjManager->AddObject(L"bluesnail", AbstractFactory<BlueSnail>::CreateObj(), OBJ_MONSTER);
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

	SCENESTATE curScene = GET_MANAGER<SceneManager>()->GetCurrentSceneState();

#ifdef SERVER_MODE
	// 플레이어 정보를 보낸다.
	GameObject *player = m_ObjManager->GetObjFromTag(L"player", OBJ_PLAYER);
	
	GAMEOBJINFO objInfo = player->GetInfo();
	char objDir = (char)player->GetDirection();

	PLAYERINFO PInfo = PLAYERINFO{ (short)objInfo.Pos_X, (short)objInfo.Pos_Y,
						player->GetSpriteInfo().CurState, objDir };
	
	if (false == m_NetworkManager->SendPlayerInfo(curScene, PInfo))
	{
		MessageBox(g_hWnd, L"서버에 연결할 수 없습니다.", L"Error", MB_OK);
		return -1;
	}
	
	// 다른 플레이어 정보를 받는다.
	char otherInfo[MAX_BUFFER] = { 0, };
	if(true == m_NetworkManager->SendAndRecvOtherInfo(otherInfo))
	{
		short size = 0;
		memcpy(&size, otherInfo, sizeof(short));
		if (0 == size)
			return 0;

		int startAddrPos = 2 + 1;
		int count = (size - startAddrPos) / sizeof(SPOTHERPLAYERS);
		for (int i = 0; i < count; ++i)
		{
			SPOTHERPLAYERS other_PInfo = SPOTHERPLAYERS{};
			memcpy(&other_PInfo, (otherInfo + startAddrPos + sizeof(SPOTHERPLAYERS) * i),
				sizeof(SPOTHERPLAYERS));

			GameObject* other_player = nullptr;

			other_player = m_ObjManager->GetObjFromTag(to_wstring(other_PInfo.id).c_str(),
				OBJ_OTHERPLAYER);

			if (nullptr != other_player)
			{
				// 위치
				other_player->SetPosition(other_PInfo.info.pos_x, other_PInfo.info.pos_y);

				// 스프라이트 상태
				SPRITEINFO sprite_info = other_player->GetSpriteInfo();
				sprite_info.CurState = other_PInfo.info.player_state;
				other_player->SetSpriteInfo(sprite_info);

				// 방향
				other_player->SetDirection((DIRECTION)other_PInfo.info.player_dir);
			}
		}
	}
	else
	{
		MessageBox(g_hWnd, L"서버에 연결할 수 없습니다.", L"Error", MB_OK);
		return -1;
	}

	// 몬스터 정보를 받는다.
	// 씬 정보를 넣는다.
	otherInfo[3] = curScene;
	if (true == m_NetworkManager->SendAndRecvMonster(otherInfo))
	{
		short size = 0;
		memcpy(&size, otherInfo, sizeof(short));
		if (0 == size)
			return 0;

		int startAddrPos = 2 + 1;
		int count = (size - startAddrPos) / sizeof(SPMONSTER);
		for (int i = 0; i < count; ++i)
		{
			SPMONSTER MInfo = SPMONSTER{};
			memcpy(&MInfo, (otherInfo + startAddrPos + sizeof(SPMONSTER) * i),
				sizeof(SPMONSTER));

			GameObject* monster = nullptr;

			monster = m_ObjManager->GetObjFromTag(to_wstring(MInfo.monster_id).c_str(),
				OBJ_MONSTER);

			if (nullptr == monster)
			{
				TCHAR* tchar = new TCHAR[64];
				wsprintf(tchar, L"%d", MInfo.monster_id);

				monster = AbstractFactory<BlueSnail>::CreateObj();
				m_ObjManager->AddObject(tchar, monster,
					OBJ_MONSTER);
			}
			// 위치
			monster->SetPosition(MInfo.info.pos_x, MInfo.info.pos_y);

			// 스프라이트 상태
			SPRITEINFO sprite_info = monster->GetSpriteInfo();
			sprite_info.CurState = MInfo.info.monster_state;
			monster->SetSpriteInfo(sprite_info);

			// 방향
			monster->SetDirection((DIRECTION)MInfo.info.monster_dir);
		}
	}
	else
	{
		MessageBox(g_hWnd, L"서버에 연결할 수 없습니다.", L"Error", MB_OK);
		return -1;
	}

	// 이벤트 처리를 한다.
	EVENTINFO evInfo;
	if (true == m_NetworkManager->SendAndRecvEvent(&evInfo))
	{
		switch (evInfo.event_state)
		{
		case EV_PUTOTHERPLAYER:
		{
			GameObject* other_player = nullptr;

			other_player = m_ObjManager->GetObjFromTag(to_wstring(evInfo.id).c_str(),
				OBJ_OTHERPLAYER);

			// 큐로부터 보내온 정보의 다른 유저의 Scene이 현재 Scene과 다른 경우는 
			// 서로 다른 Scene에 있다는 뜻이므로 추가하지 않는다.
			if (evInfo.scene_state != curScene)
				break;

			// 만약 등록된 id의 플레이어가 없다면 만든다.
			if (nullptr == other_player)
			{
				TCHAR* tchar = new TCHAR[64];
				wsprintf(tchar, L"%d", evInfo.id);

				other_player = AbstractFactory<Player>::CreateObj();
				dynamic_cast<Player*>(other_player)->SetOtherCheck(true);
				m_ObjManager->AddObject(tchar, other_player,
					OBJ_OTHERPLAYER);
			}
		}
		break;
		case EV_END:
		{
			m_ObjManager->ReleaseObjFromTag(to_wstring(evInfo.id).c_str(),
				OBJ_OTHERPLAYER);
		}
		break;
		case EV_NONE:
			break;
		}
	}
	else
	{
		MessageBox(g_hWnd, L"서버에 연결할 수 없습니다.", L"Error", MB_OK);
		return -1;
	}
#endif

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