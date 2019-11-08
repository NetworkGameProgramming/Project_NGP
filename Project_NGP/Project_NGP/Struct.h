#pragma once

typedef struct BaseInfo
{
	int X;
	int Y;
}POSITION, RESOLUTION;

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
