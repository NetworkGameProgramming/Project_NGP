#pragma once

class GameObject;

class CameraManager : public Singleton<CameraManager>
{
public:
	CameraManager();
	virtual ~CameraManager();

public:
	const POSITION& GetPos() { return m_Pos; }
	const RESOLUTION& GetResolution() { return m_Resolution; }

public:
	void SetTarget(GameObject * target);
	void SetResolution(int x, int y);

public:
	int  Update(const float& TimeDelta);
	void Release();

private:
	GameObject* m_Target = nullptr;
	POSITION	m_Pos;
	RESOLUTION	m_Resolution;
};

