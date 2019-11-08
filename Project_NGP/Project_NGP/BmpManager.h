#pragma once

#include "Bitmap.h"

class BmpManager : public Singleton<BmpManager>
{
public:
	BmpManager();
	virtual ~BmpManager();

public:
	CBitmap* FindBmp(const TCHAR* pKey);

public:
	void LoadBmpBySceneState(SCENESTATE SceneState);
	void LoadBmp(const TCHAR* pKey, const TCHAR* pFilePath);
	void ResetContainer();

public:
	unordered_map<const TCHAR*, CBitmap*>& GetBmpContainer() { return m_mapBmp; }

private:
	unordered_map<const TCHAR*, CBitmap*>				m_mapBmp;
	typedef unordered_map<const TCHAR*, CBitmap*>		MAPBITMAP;
};

