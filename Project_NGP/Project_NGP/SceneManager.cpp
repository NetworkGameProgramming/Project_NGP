#include "stdafx.h"
#include "SceneManager.h"
#include "MainMenu.h"
#include "TestScene.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	Release();
}

bool SceneManager::ChangeSceneState(SCENESTATE SceneState)
{
	if (nullptr != m_Scene)
	{
		Release();
	}

	switch (SceneState)
	{
	case SCENE_MENU :
		m_Scene = new MainMenu;
		break;
	case SCENE_TEST:
		m_Scene = new TestScene;
		break;
	}

	if (false == m_Scene->Initialize())
	{
		Release();
		return false;
	}

	m_CurrentScene = SceneState;

	return true;
}

void SceneManager::Update(const float & TimeDelta)
{
	m_Scene->Update(TimeDelta);
}

void SceneManager::Render(HDC hDC)
{
	m_Scene->Render(hDC);
}

void SceneManager::Release()
{
	if (m_Scene)
	{
		delete m_Scene;
		m_Scene = nullptr;
	}
}
