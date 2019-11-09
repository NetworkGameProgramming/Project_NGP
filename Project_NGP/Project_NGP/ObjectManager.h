#pragma once

class GameObject;
class ObjectManager : public Singleton<ObjectManager>
{
public:
	ObjectManager();
	virtual ~ObjectManager();

public:
	typedef unordered_map<const TCHAR*, GameObject*>	MAPOBJ;

public:
	const TCHAR* GetTagFromObj(GameObject* Obj, OBJTYPE ObjType);
	
	MAPOBJ& GetObjFromType(OBJTYPE ObjType) { return m_mapObj[ObjType]; }
	GameObject* GetObjFromTag(const TCHAR* tag, OBJTYPE ObjType) 
	{ return m_mapObj[ObjType].find(tag)->second; }
	
public:
	void AddObject(const TCHAR* tag, GameObject* Obj, OBJTYPE ObjType);
	void Update(const float& TimeDelta);
	void Render(HDC hDC);
	void ReleaseAll();
	void ReleaseType(OBJTYPE ObjType);
	void ReleaseObj(const TCHAR* tag, OBJTYPE ObjType);
	void ReleaseObj(GameObject* Obj, OBJTYPE ObjType);

private:
	unordered_map<const TCHAR*, GameObject*>			m_mapObj[OBJ_END];
	vector<GameObject*> m_vecRender[RENDER_END];
};

