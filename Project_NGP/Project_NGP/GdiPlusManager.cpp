#include "stdafx.h"
#include "GdiPlusManager.h"

GdiPlusManager::GdiPlusManager()
{
	// Gdi+ 라이브러리 초기화
	GdiplusStartupInput m_gdiPlusStartupInput;
	GdiplusStartup(&m_gdiPlusToken, &m_gdiPlusStartupInput, NULL);
}

GdiPlusManager::~GdiPlusManager()
{
	ResetContainer();
	GdiplusShutdown(m_gdiPlusToken);
}

void GdiPlusManager::LoadImageBySceneState(SCENESTATE SceneState)
{
	switch (SceneState)
	{
	case SCENESTATE::SCENE_TEST:
		// Load Image
		LoadGdiPlusImage(L"balrok", L"../Resources/0.png");
		LoadGdiPlusImage(L"horse", L"../Resources/chess-knight.bmp");
		break;
	}
}

GdiPlusImage* GdiPlusManager::FindImage(const TCHAR* tag)
{
	MAPIMAGES::const_iterator iter = m_mapImages.find(tag);

	if (m_mapImages.end() == iter)
		return nullptr;

	return iter->second;
}

void GdiPlusManager::LoadGdiPlusImage(const TCHAR* tag, const TCHAR* filePath)
{
	m_mapImages.emplace(MAPIMAGES::value_type(tag, (new GdiPlusImage)->LoadGdiPlusImage(filePath)));
}

void GdiPlusManager::ResetContainer()
{
	for (auto& image : m_mapImages)
	{
		delete image.second;
		image.second = nullptr;
	}

	m_mapImages.clear();
}
