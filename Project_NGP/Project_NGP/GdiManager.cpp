#include "stdafx.h"
#include "GdiManager.h"


GdiManager::GdiManager()
{
}


GdiManager::~GdiManager()
{
	ResetContainer();
}

GdiImage * GdiManager::FindImage(const TCHAR * pKey)
{
	MAPBITMAP::const_iterator iter = m_mapBmp.find(pKey);

	if (m_mapBmp.end() == iter)
		return nullptr;

	return iter->second;
}

void GdiManager::LoadImageBySceneState(SCENESTATE SceneState)
{
	// 씬에 필요한 이미지를 여기서 미리 로드해 놓는다.
	GdiManager* GdiMgr = GET_MANAGER<GdiManager>();
	switch (SceneState)
	{
	case SCENESTATE::SCENE_TEST:
		// Load Image

		// Test
		GdiMgr->LoadGdiImage(L"balrock", L"../Resources/0.bmp");
		GdiMgr->LoadGdiImage(L"balrock_other", L"../Resources/510.bmp");
		GdiMgr->LoadGdiImage(L"snail", L"../Resources/Monster/0.bmp");
		// Background
		GdiMgr->LoadGdiImage(L"background", L"../Resources/Map/Back.bmp");
		GdiMgr->LoadGdiImage(L"background_collider", L"../Resources/Map/Back_Collider.bmp");
		// Player
		GdiMgr->LoadGdiImage(L"player_left", L"../Resources/Player/player_L.bmp");
		GdiMgr->LoadGdiImage(L"player_right", L"../Resources/Player/player_R.bmp");
		break;
	}
}

void GdiManager::LoadGdiImage(const TCHAR * pKey, const TCHAR * pFilePath)
{
	m_mapBmp.emplace(MAPBITMAP::value_type(pKey, (new GdiImage)->LoadGdiImage(pFilePath)));
}

void GdiManager::LoadGdiImageFromFolder(const TCHAR* pKey, bstr_t folderPath)
{
	m_mapBmp.emplace(MAPBITMAP::value_type(pKey, (new GdiImage)->LoadGdiImageFromFolder(folderPath)));
}

void GdiManager::ResetContainer()
{
	for (auto& bmp : m_mapBmp)
	{
		delete bmp.second;
		bmp.second = nullptr;
	}

	m_mapBmp.clear();
}
