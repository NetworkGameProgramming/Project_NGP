#pragma once

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

typedef struct GameObjectInfo
{
	int Pos_X;
	int Pos_Y;
	int Size_Width;
	int Size_Height;
}GAMEOBJINFO;

typedef struct SpriteInfo
{
	SPRITETYPE Type;
	int   MaxFrame;
	float Speed;
}SPRITEINFO;

typedef struct GdiImageInfo
{
	HDC		hDC;
	HDC		hMemDC;

	HBITMAP Bitmap;
	HBITMAP OldBmp;
}GDIINFO;