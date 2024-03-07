#pragma once
#include "Core/CoreBase.h"

#include "External/SDL/SDL_video.h"

struct BaseUserSettings
{
	Json Serialize();
	void Deserialize(const Json& inJson);

	fm::ivec2 mWindowSize = {1280, 720};
	uint32 mWindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	int32 mMaxFPS = 144;
};
