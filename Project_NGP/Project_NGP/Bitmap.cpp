#include "stdafx.h"
#include "Bitmap.h"


Bitmap::Bitmap()
{
}


Bitmap::~Bitmap()
{
	// ���� ���� ����
	// DC�� ���õ� GDI ������Ʈ�� �ٷ� ������ �Ұ����ϴ�.
	// ����, m_OldBmp�� �̿��� �ع� ��Ų �� �����ؾ��Ѵ�.
	SelectObject(m_hMemDC, m_OldBmp);
	DeleteObject(m_Bitmap);
	DeleteDC(m_hMemDC);
}

Bitmap * Bitmap::LoadBmp(const TCHAR * pFilePath)
{
	m_hDC = GetDC(g_hWnd);

	// ���� ��� DC�� ȣȯ�� �Ǵ� m_hMemDC�� �о�� ��Ʈ���� �̸� �׷��־� ����
	m_hMemDC = CreateCompatibleDC(m_hDC);

	m_Bitmap = (HBITMAP)LoadImage(NULL, pFilePath, IMAGE_BITMAP
		, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (NULL == m_Bitmap)
	{
		MessageBox(g_hWnd, pFilePath, L"Image load failed!", MB_OK);
		return NULL;
	}

	m_OldBmp = (HBITMAP)SelectObject(m_hMemDC, m_Bitmap);

	ReleaseDC(g_hWnd, m_hDC);
	return this;
}
