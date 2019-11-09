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
	RECT rc = {};

	for (auto& Dst : *DstList)
	{
		if (true == Dst.second->GetState())
			continue;

		for (auto& Src : *SrcList)
		{
			if (true == Src.second->GetState())
				continue;

			if (IntersectRect(&rc, &Dst.second->GetRect(), &Src.second->GetRect()))
			{
				Src.second->CollisionActivate(Dst.second);
				Dst.second->CollisionActivate(Src.second);
			}
		}
	}
}

void CollisionManager::CollisionRectEx(ObjectManager::MAPOBJ* DstList, ObjectManager::MAPOBJ* SrcList)
{
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

bool CollisionManager::CheckCollisionRectDist(int* moveX, int* moveY, GameObject* Dst, GameObject* Src)
{
	// 각 축의 합이 두 객체간의 축의 거리보다 큰 경우 => 충돌

	// 축의 합을 구한다.
	int width = (Dst->GetInfo().Size_Width / 2) + (Src->GetInfo().Size_Width / 2);
	int height = (Dst->GetInfo().Size_Height / 2) + (Src->GetInfo().Size_Height / 2);

	int distX = abs(Dst->GetInfo().Pos_X - Src->GetInfo().Pos_X);
	int distY = abs(Dst->GetInfo().Pos_Y - Src->GetInfo().Pos_Y);

	//  충돌된 경우
	if ((width > distX) && (height > distY))
	{
		// 충돌된 길이를 구한다.
		*moveX = width - distX;
		*moveY = height - distY;

		return true;
	}

	return false;
}
