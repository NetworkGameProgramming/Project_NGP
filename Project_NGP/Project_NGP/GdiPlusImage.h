#pragma once

#include <Gdiplus.h>

class GdiPlusImage
{
public:
	GdiPlusImage();
	virtual ~GdiPlusImage();

public:
	Gdiplus::Image* GetGdiPlusImage();

public:
	GdiPlusImage* LoadGdiPlusImage(const TCHAR* filePath);

private:
	const TCHAR* m_filePath;
};

