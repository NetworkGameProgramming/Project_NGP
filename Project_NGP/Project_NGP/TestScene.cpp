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
	// �÷��̾� ������ ������.
	GameObject *player = m_ObjManager->GetObjFromTag(L"player", OBJ_PLAYER);
	
	GAMEOBJINFO objInfo = player->GetInfo();
	char objDir = (char)player->GetDirection();

	PLAYERINFO PInfo = PLAYERINFO{ (short)objInfo.Pos_X, (short)objInfo.Pos_Y,
						player->GetSpriteInfo().CurState, objDir };
	
	if (false == m_NetworkManager->SendPlayerInfo(curScene, PInfo))
	{
		MessageBox(g_hWnd, L"������ ������ �� �����ϴ�.", L"Error", MB_OK);
		return -1;
	}
	
	// �ٸ� �÷��̾� ������ �޴´�.
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
				// ��ġ
				other_player->SetPosition(other_PInfo.info.pos_x, other_PInfo.info.pos_y);

				// ��������Ʈ ����
				SPRITEINFO sprite_info = other_player->GetSpriteInfo();
				sprite_info.CurState = other_PInfo.info.player_state;
				other_player->SetSpriteInfo(sprite_info);

				// ����
				other_player->SetDirection((DIRECTION)other_PInfo.info.player_dir);
			}
		}
	}
	else
	{
		MessageBox(g_hWnd, L"������ ������ �� �����ϴ�.", L"Error", MB_OK);
		return -1;
	}

	// ���� ������ �޴´�.
	// �� ������ �ִ´�.
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
			// ��ġ
			monster->SetPosition(MInfo.info.pos_x, MInfo.info.pos_y);

			// ��������Ʈ ����
			SPRITEINFO sprite_info = monster->GetSpriteInfo();
			sprite_info.CurState = MInfo.info.monster_state;
			monster->SetSpriteInfo(sprite_info);

			// ����
			monster->SetDirection((DIRECTION)MInfo.info.monster_dir);
		}
	}
	else
	{
		MessageBox(g_hWnd, L"������ ������ �� �����ϴ�.", L"Error", MB_OK);
		return -1;
	}

	// �̺�Ʈ ó���� �Ѵ�.
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

			// ť�κ��� ������ ������ �ٸ� ������ Scene�� ���� Scene�� �ٸ� ���� 
			// ���� �ٸ� Scene�� �ִٴ� ���̹Ƿ� �߰����� �ʴ´�.
			if (evInfo.scene_state != curScene)
				break;

			// ���� ��ϵ� id�� �÷��̾ ���ٸ� �����.
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
		MessageBox(g_hWnd, L"������ ������ �� �����ϴ�.", L"Error", MB_OK);
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