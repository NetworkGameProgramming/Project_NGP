#include "stdafx.h"
#include "BmpManager.h"


BmpManager::BmpManager()
{
}


BmpManager::~BmpManager()
{
	ResetContainer();
}

Bitmap * BmpManager::FindBmp(const TCHAR * pKey)
{
	MAPBITMAP::const_iterator iter = m_mapBmp.find(pKey);

	if (m_mapBmp.end() == iter)
		return nullptr;

	return iter->second;
}

void BmpManager::LoadBmpBySceneState(SCENESTATE SceneState)
{
	switch (SceneState)
	{
	case SCENESTATE::SCENE_TEST:
		// Load Image
		LoadBmp(L"horse", L"../Resources/chess-knight.bmp");
		break;
	}
}

void BmpManager::LoadBmp(const TCHAR * pKey, const TCHAR * pFilePath)
{
	m_mapBmp.emplace(MAPBITMAP::value_type(pKey, (new Bitmap)->LoadBmp(pFilePath)));
}

void BmpManager::ResetContainer()
{
	for (auto& bmp : m_mapBmp)
	{
		delete bmp.second;
		bmp.second = nullptr;
	}

	m_mapBmp.clear();
}
