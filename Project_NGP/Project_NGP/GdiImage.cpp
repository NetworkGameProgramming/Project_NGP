#include "stdafx.h"
#include "GdiImage.h"


GdiImage::GdiImage()
{
}


GdiImage::~GdiImage()
{
	// 해제 순서 유의
	// DC에 선택된 GDI 오브젝트는 바로 해제가 불가능하다.
	// 따라서, OldBmp를 이용해 해방 시킨 후 해제해야한다.
	for (auto& image : m_vecImages)
	{
		SelectObject(image.hMemDC, image.OldBmp);
		DeleteObject(image.Bitmap);
		DeleteDC(image.hMemDC);
	}
	m_vecImages.clear();
}

HDC GdiImage::GetGdiImageDefault()
{
	if (m_vecImages.empty())
		return NULL;

	return m_vecImages[0].hMemDC;
}

HDC GdiImage::GetGdiImageFromIndex(int index)
{
	if (index >= (int)m_vecImages.size() || m_vecImages.empty())
		return NULL;

	return m_vecImages[0].hMemDC;
}

GdiImage * GdiImage::LoadGdiImage(const TCHAR * pFilePath)
{
	GDIINFO info;
	ZeroMemory(&info, sizeof(GDIINFO));

	info.hDC = GetDC(g_hWnd);
	// 실제 출력 DC와 호환이 되는 MemDC에 읽어온 비트맵을 미리 그려넣어 저장
	info.hMemDC = CreateCompatibleDC(info.hDC);
	info.Bitmap = (HBITMAP)LoadImage(NULL, pFilePath, IMAGE_BITMAP
		, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (NULL == info.Bitmap)
	{
		MessageBox(g_hWnd, pFilePath, L"Image load failed!", MB_OK);
		return NULL;
	}
	info.OldBmp = (HBITMAP)SelectObject(info.hMemDC, info.Bitmap);
	ReleaseDC(g_hWnd, info.hDC);

	m_vecImages.emplace_back(info);
	return this;
}

GdiImage* GdiImage::LoadGdiImageFromFolder(bstr_t folderPath)
{
	vector<bstr_t> fileNames;
	GET_MANAGER<FileManager>()->GetFileListFromFolder(folderPath + "/*.*", fileNames);

	TCHAR originPath[MAX_BUFFER];
	memset(originPath, 0, sizeof(TCHAR) * MAX_BUFFER);

	lstrcpy(originPath, (TCHAR*)(folderPath + "/"));

	for (auto& file : fileNames)
	{
		TCHAR* tmpPath = new TCHAR[MAX_BUFFER];
		memset(tmpPath, 0, sizeof(TCHAR) * MAX_BUFFER);

		lstrcpy(tmpPath, originPath);
		lstrcat(tmpPath, (TCHAR*)file);

		GDIINFO info;
		ZeroMemory(&info, sizeof(GDIINFO));

		info.hDC = GetDC(g_hWnd);
		// 실제 출력 DC와 호환이 되는 MemDC에 읽어온 비트맵을 미리 그려넣어 저장
		info.hMemDC = CreateCompatibleDC(info.hDC);
		info.Bitmap = (HBITMAP)LoadImage(NULL, tmpPath, IMAGE_BITMAP
			, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (NULL == info.Bitmap)
		{
			MessageBox(g_hWnd, tmpPath, L"Image load failed!", MB_OK);
			return NULL;
		}
		info.OldBmp = (HBITMAP)SelectObject(info.hMemDC, info.Bitmap);
		ReleaseDC(g_hWnd, info.hDC);

		m_vecImages.emplace_back(info);
	}

	return this;
}
