#include "stdafx.h"
#include "CollisionManager.h"
#include "GameObject.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::CollisionRect(ObjectManager::MAPOBJ* DstList, ObjectManager::MAPOBJ* SrcList)
{
	// ��Ʈ �浹 �˻�
	RECT rc = {};

	for (auto& Dst : *DstList)
	{
		if (true == Dst.second->GetState())
			continue;

		for (auto& Src : *SrcList)
		{
			if (true == Src.second->GetState())
				continue;

			if (IntersectRect(&rc, &Dst.second->GetCollideRect(), &Src.second->GetCollideRect()))
			{
				// �浹 �Ͽ��ٸ� ���� �浹 ó���� �Ѵ�. 
				Src.second->CollisionActivate(Dst.second);
				Dst.second->CollisionActivate(Src.second);
			}
			else
			{
				Src.second->CollisionDeactivate(Dst.second);
				Dst.second->CollisionDeactivate(Src.second);
			}
		}
	}
}

void CollisionManager::CollisionRectEx(ObjectManager::MAPOBJ* DstList, ObjectManager::MAPOBJ* SrcList)
{
	// �浹�� ��� ���� �з����� �Ѵ�.
	int moveX = 0;
	int moveY = 0;

	for (auto& Dst : *DstList)
	{
		if (true == Dst.second->GetState())
			continue;

		for (auto& Src : *SrcList)
		{
			if (true == Src.second->GetState())
				continue;

			if (CheckCollisionRectDist(&moveX, &moveY, Dst.second, Src.second))
			{
				int PosX = 0;
				int PosY = 0;

				if (moveX > moveY)
				{
					PosX = Dst.second->GetInfo().Pos_X;
					PosY = Dst.second->GetInfo().Pos_Y;

					if (Src.second->GetInfo().Pos_Y > PosY)
						moveY *= -1;

					Dst.second->SetPosition(PosX, PosY + moveY);
				}
				else
				{
					PosX = Dst.second->GetInfo().Pos_X;
					PosY = Dst.second->GetInfo().Pos_Y;

					if (Src.second->GetInfo().Pos_X > PosX)
						moveX *= -1;

					Dst.second->SetPosition(PosX + moveX, PosY);
				}
			}
		}
	}
}

void CollisionManager::CollisionPixelToRect(ObjectManager::MAPOBJ* pixel, ObjectManager::MAPOBJ* rect)
{
	for (auto& Dst : *pixel)
	{
		if (true == Dst.second->GetState())
			continue;

		const PIXELCOLLIDERINFO* pixelCollide = Dst.second->GetPixelCollider();

		if (nullptr == pixelCollide)
			continue;


		for (auto& Src : *rect)
		{
			GAMEOBJINFO info = Src.second->GetInfo();
			GAMEOBJINFO collideInfo = Src.second->GetCollideInfo();
			RECT rc = Src.second->GetCollideRect();
			POSITION CamPos = GET_MANAGER<CameraManager>()->GetPos();
			rc.bottom -= (int)CamPos.Y;
			
			int addr;

			// �Ʒ� 
			addr = rc.bottom * pixelCollide->Width + info.Pos_X;
			if (addr < 0 || addr >= (int)pixelCollide->vecPixel.size()) return;
			if (pixelCollide->vecPixel[addr].r == pixelCollide->CollPixel.r &&
				pixelCollide->vecPixel[addr].g == pixelCollide->CollPixel.g &&
				pixelCollide->vecPixel[addr].b == pixelCollide->CollPixel.b)
			{
				int Y = rc.bottom;
				while (Y > 0)
				{
					--Y;
					addr = Y * pixelCollide->Width + info.Pos_X;

					if (pixelCollide->vecPixel[addr].r == pixelCollide->CollPixel.r &&
						pixelCollide->vecPixel[addr].g == pixelCollide->CollPixel.g &&
						pixelCollide->vecPixel[addr].b == pixelCollide->CollPixel.b)
					{
						continue;
					}
					else
					{
						if (0 < Src.second->GetGravity())
						{
							Src.second->SetPosition(info.Pos_X, Y - collideInfo.Size_Height / 2);
							Src.second->CollisionPixelPart(DIR_BOTTOM);
						}
						break;
					}
				}
			}
			else
			{
				// �ؿ� �� �ȼ� �� �˻�
				addr = (rc.bottom + 2) * pixelCollide->Width + info.Pos_X;
				if (addr < 0 || addr >= (int)pixelCollide->vecPixel.size()) return;
				if (pixelCollide->vecPixel[addr].r != pixelCollide->CollPixel.r &&
					pixelCollide->vecPixel[addr].g != pixelCollide->CollPixel.g &&
					pixelCollide->vecPixel[addr].b != pixelCollide->CollPixel.b)
				{
					Src.second->SetFall(true);
				}
			}
		}
	}
}

void CollisionManager::CollisionPixelToPoint(ObjectManager::MAPOBJ* pixel, ObjectManager::MAPOBJ* rect)
{
	for (auto& Dst : *pixel)
	{
		if (true == Dst.second->GetState())
			continue;

		const PIXELCOLLIDERINFO* pixelCollide = Dst.second->GetPixelCollider();

		if (nullptr == pixelCollide)
			continue;


		for (auto& Src : *rect)
		{
			GAMEOBJINFO rc = Src.second->GetInfo();
			int addr = rc.Pos_Y * pixelCollide->Width + rc.Pos_X;

			if (pixelCollide->vecPixel[addr].r == pixelCollide->CollPixel.r &&
				pixelCollide->vecPixel[addr].g == pixelCollide->CollPixel.g &&
				pixelCollide->vecPixel[addr].b == pixelCollide->CollPixel.b)
			{
				printf("Collide!\n");
			}

		}
	}
}

bool CollisionManager::CheckCollisionRectDist(int* moveX, int* moveY, GameObject* Dst, GameObject* Src)
{
	// �� ���� ���� �� ��ü���� ���� �Ÿ����� ū ��� => �浹
	// �浹�� �Ÿ��� ���Ѵ�.

	// ���� ���� ���Ѵ�.
	int width = (Dst->GetCollideInfo().Size_Width / 2) + (Src->GetCollideInfo().Size_Width / 2);
	int height = (Dst->GetCollideInfo().Size_Height / 2) + (Src->GetCollideInfo().Size_Height / 2);

	int distX = abs(Dst->GetInfo().Pos_X - Src->GetInfo().Pos_X);
	int distY = abs(Dst->GetInfo().Pos_Y - Src->GetInfo().Pos_Y);

	//  �浹�� ���
	if ((width > distX) && (height > distY))
	{
		// �浹�� ���̸� ���Ѵ�.
		*moveX = width - distX;
		*moveY = height - distY;

		return true;
	}

	return false;
}
