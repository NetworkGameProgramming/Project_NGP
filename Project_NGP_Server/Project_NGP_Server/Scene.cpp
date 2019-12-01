#include "pch.h"
#include "Scene.h"
#include "Spawn.h"
#include "Monster.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	Release();
}

int Scene::Update(const float& TimeDelta)
{
	// �ȼ� �浹
	if (nullptr == m_PixelInfo)
		return -1;

	for (auto& Src : m_mapSpawn)
	{
		Monster* mon = Src.second->GetMonster();

		if (nullptr == mon)
			continue;

		GAMEOBJINFO info = mon->GetInfo();
		//GAMEOBJINFO collideInfo = Src.second->GetCollideInfo();
		RECT rc;
		rc.bottom = info.Pos_Y + (info.Size_Height / 2);

		int addr;

		// �Ʒ� 
		addr = rc.bottom * m_PixelInfo->Width + info.Pos_X;
		if (addr < 0 || addr >= (int)m_PixelInfo->vecPixel.size()) 
			return -1;

		// ���� �浹 �ȼ��϶� 
		if (m_PixelInfo->vecPixel[addr].r == m_PixelInfo->CollPixel.r &&
			m_PixelInfo->vecPixel[addr].g == m_PixelInfo->CollPixel.g &&
			m_PixelInfo->vecPixel[addr].b == m_PixelInfo->CollPixel.b)
		{
			// ���� ���̰� ��ŭ ���� ������Ʈ�� �÷��ش�.
			int Y = rc.bottom;
			while (Y > 0)
			{
				--Y;
				addr = Y * m_PixelInfo->Width + info.Pos_X;

				if (m_PixelInfo->vecPixel[addr].r == m_PixelInfo->CollPixel.r &&
					m_PixelInfo->vecPixel[addr].g == m_PixelInfo->CollPixel.g &&
					m_PixelInfo->vecPixel[addr].b == m_PixelInfo->CollPixel.b)
				{
					continue;
				}
				else
				{
					if (0 < mon->GetGravity())
					{
						mon->SetPosition(info.Pos_X, Y - (info.Size_Height / 2));
						mon->CollisionPixelPart(DIR_BOTTOM);
					}
					break;
				}
			}
		}
		else
		{
			// ���� ����
			// �ؿ� �� �ȼ� �� �˻�
			addr = (rc.bottom + 2) * m_PixelInfo->Width + info.Pos_X;
			if (addr < 0 || addr >= (int)m_PixelInfo->vecPixel.size()) return -1;

			// ���Ͱ� ���ƺ��� ��ġ�� �ִ°��
			if (m_PixelInfo->vecPixel[addr].r == m_PixelInfo->TurnPixel.r &&
				m_PixelInfo->vecPixel[addr].g == m_PixelInfo->TurnPixel.g &&
				m_PixelInfo->vecPixel[addr].b == m_PixelInfo->TurnPixel.b)
			{
				mon->SetTurnCheck(true);
			}
			else if (m_PixelInfo->vecPixel[addr].r != m_PixelInfo->CollPixel.r &&
				m_PixelInfo->vecPixel[addr].g != m_PixelInfo->CollPixel.g &&
				m_PixelInfo->vecPixel[addr].b != m_PixelInfo->CollPixel.b)
			{
				mon->SetFall(true);
			}
		}
	}

	return 0;
}

void Scene::Release()
{
	if (nullptr != m_PixelInfo)
	{
		m_PixelInfo->vecPixel.clear();

		delete m_PixelInfo;
		m_PixelInfo = nullptr;
	}
}

bool Scene::LoadPixelCollider(const char* pFilePath, unsigned char r, unsigned char g, unsigned char b)
{
	if (nullptr != m_PixelInfo)
	{
		m_PixelInfo->vecPixel.clear();

		delete m_PixelInfo;
		m_PixelInfo = nullptr;
	}

	m_PixelInfo = new PIXELCOLLIDERINFO;
	// �����浹 üũ
	m_PixelInfo->CollPixel.r = r;
	m_PixelInfo->CollPixel.g = g;
	m_PixelInfo->CollPixel.b = b;

	// ���ƺ��� üũ
	m_PixelInfo->TurnPixel.r = 255;
	m_PixelInfo->TurnPixel.g = 255;
	m_PixelInfo->TurnPixel.b = 255;

	FILE* pFile = NULL;

	fopen_s(&pFile, pFilePath, "rb");

	if (pFile)
	{
		// File Header�� Info Header ����
		// ��Ʈ�� ���� ��ü�� ���� ������ ������.
		BITMAPFILEHEADER	tBf = {};
		// ũ��(���� ��, ���� ����)�� ���� ���� � ���� ������ ������.
		BITMAPINFOHEADER	tIf = {};

		// ���� ��ü�� ������ �д´�.
		fread(&tBf, sizeof(tBf), 1, pFile);
		fread(&tIf, sizeof(tIf), 1, pFile);

		fseek(pFile, tBf.bfOffBits, SEEK_SET);

		m_PixelInfo->Width = tIf.biWidth;
		m_PixelInfo->Height = tIf.biHeight;

		m_PixelInfo->vecPixel.clear();
		m_PixelInfo->vecPixel.resize(tIf.biWidth * tIf.biHeight);

		fread(&m_PixelInfo->vecPixel[0], sizeof(PIXEL24), m_PixelInfo->vecPixel.size(), pFile);

		fclose(pFile);
	}
	else
	{
		return false;
	}
	return true;
}
