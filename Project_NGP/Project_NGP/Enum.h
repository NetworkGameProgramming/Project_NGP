#pragma once

enum DIRECTION
{
	DIR_LEFT,
	DIR_RIGHT,
	DIR_TOP,
	DIR_BOTTOM,
	DIR_END
};

enum KEYSTATE
{
	STATE_DOWN,
	STATE_PUSH,
	STATE_UP,
	STATE_END
};

enum SCENESTATE
{
	SCENE_MENU,
	SCENE_TEST,
	SCENE_END
};

enum FILETYPE
{
	FILE_FILE,
	FILE_FOLDER,
	FILE_END_
};

enum SPRITEPLAY
{
	PLAY_EACH,
	PLAY_ALL,
	PLAY_END
};

enum SPRITETYPE
{
	SPRITE_ONCE,
	SPRITE_ONCE_END,
	SPRITE_REPEAT,
	SPRITE_REPEAT_END,
	SPRITE_END
};

enum OBJTYPE
{
	OBJ_BACK,
	OBJ_OTHERPLAYER,
	OBJ_PLAYER,
	OBJ_MONSTER,
	OBJ_SKILL,
	OBJ_MOUSE,
	OBJ_BUTTON,
	OBJ_END
};

enum RENDERTYPE
{
	RENDER_BACKGROUND,
	RENDER_OBJ,
	RENDER_EFFECT,
	RENDER_UI,
	RENDER_END
};

enum MONSTERTYPE
{
	MONTYPE_SNAIL,
	MONTYPE_SLIME,
	MONTYPE_MUSHROOM,
	MONTYPE_BOSS,
	MONTYPE_END
};