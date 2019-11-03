#include "stdafx.h"
#include "GdiPlusImage.h"

GdiPlusImage::GdiPlusImage()
{
}

GdiPlusImage::~GdiPlusImage()
{
}

Gdiplus::Image* GdiPlusImage::GetGdiPlusImage()
{
	return Image::FromFile(m_filePath);
}

GdiPlusImage* GdiPlusImage::LoadGdiPlusImage(const TCHAR* filePath)
{
	m_filePath = filePath;
	return this;
}
