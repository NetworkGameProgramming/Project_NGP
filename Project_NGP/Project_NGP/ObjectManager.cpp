#include "stdafx.h"
#include "ObjectManager.h"
#include "GameObject.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	ReleaseAll();
}

const TCHAR* ObjectManager::GetTagFromObj(GameObject* Obj, OBJTYPE ObjType)
{
	MAPOBJ::iterator iter = find_if(begin(m_mapObj[ObjType]), end(m_mapObj[ObjType]),
		[&](auto& p) {return p.second == Obj; });

	if (end(m_mapObj[ObjType]) == iter)
		return nullptr;

	return iter->first;
}

void ObjectManager::AddObject(const TCHAR* tag, GameObject* Obj, OBJTYPE ObjType)
{
	if (nullptr == Obj)
		return;

	m_mapObj[ObjType].insert(MAPOBJ::value_type(tag, Obj));
}

void ObjectManager::Update(const float& TimeDelta)
{
	// Update
	for (auto i = 0; i < OBJ_END; ++i)
	{
		for (auto& obj : m_mapObj[i])
		{
			obj.second->Update(TimeDelta);
		}
	}

	// Collision
	GET_MANAGER<CollisionManager>()->CollisionRect(&m_mapObj[OBJ_PLAYER], &m_mapObj[OBJ_MONSTER]);
	GET_MANAGER<CollisionManager>()->CollisionRectEx(&m_mapObj[OBJ_PLAYER], &m_mapObj[OBJ_MONSTER]);
}

void ObjectManager::Render(HDC hDC)
{
	for (auto i = 0; i < OBJ_END; ++i)
	{
		for (auto& obj : m_mapObj[i])
		{
			RENDERTYPE type = obj.second->GetRenderType();
			m_vecRender[type].emplace_back(obj.second);
		}
	}

	// Y축 기준으로 render 순서 정렬
	sort(m_vecRender[RENDER_OBJ].begin(), m_vecRender[RENDER_OBJ].end(),
		[](GameObject* a, GameObject* b) { return a->GetInfo().Pos_Y > b->GetInfo().Pos_Y; });

	for (auto i = 0; i < RENDER_END; ++i)
	{
		for (auto& obj : m_vecRender[i])
		{
			obj->Render(hDC);
		}
		m_vecRender[i].clear();
	}
}

void ObjectManager::ReleaseAll()
{
	for (auto i = 0; i < OBJ_END; ++i)
	{
		for (auto& obj : m_mapObj[i])
		{
			if (nullptr != obj.second)
			{
				delete obj.second;
				obj.second = nullptr;
			}
		}
		m_mapObj[i].clear();
	}
}

void ObjectManager::ReleaseType(OBJTYPE ObjType)
{
	for (auto& obj : m_mapObj[ObjType])
	{
		if (nullptr != obj.second)
		{
			delete obj.second;
			obj.second = nullptr;
		}
	}
	m_mapObj[ObjType].clear();
}

void ObjectManager::ReleaseObj(const TCHAR* tag, OBJTYPE ObjType)
{
	MAPOBJ::iterator iter = m_mapObj[ObjType].find(tag);

	if (end(m_mapObj[ObjType]) == iter)
		return;

	if (nullptr != iter->second)
	{
		delete iter->second;
		iter->second = nullptr;
	}

	m_mapObj[ObjType].erase(tag);
}

void ObjectManager::ReleaseObj(GameObject* Obj, OBJTYPE ObjType)
{
	ReleaseObj(GetTagFromObj(Obj, ObjType), ObjType);
}
