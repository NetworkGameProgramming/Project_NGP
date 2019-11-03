#pragma once
class CBitmap
{
public:
	CBitmap();
	virtual ~CBitmap();

public:
	HDC GetMemDC() { return m_hMemDC; }

public:
	CBitmap* LoadBmp(const TCHAR* pFilePath);

private:
	HDC		m_hDC;
	HDC		m_hMemDC;

	HBITMAP m_Bitmap;
	HBITMAP m_OldBmp;
};

