#pragma once

// 그냥 기본으로 사용할 정보
typedef struct BaseInfo
{
	float X;
	float Y;

	void operator=(const BaseInfo& other)
	{
		this->X = other.X;
		this->Y = other.Y;
	}

	bool operator==(const BaseInfo& other)
	{
		if (this->X == other.X &&
			this->Y == other.Y )
			return true;
		else
			return false;
	}

	BaseInfo operator+(const BaseInfo& other)
	{
		return BaseInfo{ this->X + other.X, this->Y + other.Y };
	}

	void operator+=(const BaseInfo& other)
	{
		this->X += other.X; this->Y += other.Y;
	}

	BaseInfo operator-(const BaseInfo& other)
	{
		return BaseInfo{ this->X - other.X, this->Y - other.Y };
	}

	BaseInfo operator*(const float& other)
	{
		return BaseInfo{ this->X * other, this->Y * other };
	}

	BaseInfo operator/(const float &other)
	{
		return BaseInfo{ this->X / other, this->Y / other };
	}

}POSITION, RESOLUTION, OFFSET;

// 게임 오브젝트 정보
typedef struct GameObjectInfo
{
	int Pos_X;
	int Pos_Y;
	int Size_Width;
	int Size_Height;
}GAMEOBJINFO;

// 스프라이트 정보
typedef struct SpriteInfo
{
	SPRITETYPE Type;
	int   MaxFrame;
	float Speed;
}SPRITEINFO;

// GDI 이미지 정보 (GDI+는 해당하지 않음)
typedef struct GdiImageInfo
{
	HDC		hDC;
	HDC		hMemDC;

	HBITMAP Bitmap;
	HBITMAP OldBmp;
}GDIINFO;