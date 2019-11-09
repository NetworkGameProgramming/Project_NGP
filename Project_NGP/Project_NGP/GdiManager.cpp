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
	switch (SceneState)
	{
	case SCENESTATE::SCENE_TEST:
		// Load Image
		GET_MANAGER<GdiManager>()->LoadGdiImage(L"balrock", L"../Resources/0.bmp");
		GET_MANAGER<GdiManager>()->LoadGdiImage(L"balrock_other", L"../Resources/510.bmp");
		GET_MANAGER<GdiManager>()->LoadGdiImage(L"back", L"../Resources/back.bmp");
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
