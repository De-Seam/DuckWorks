#pragma once
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

// External includes
#include "External/SDL/SDL_video.h"

struct BaseUserSettings : public RTTIBaseClass
{
	RTTI_CLASS(BaseUserSettings, RTTIBaseClass)

	fm::ivec2 mWindowSize = {1280, 720};
	uint32 mWindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	int32 mMaxFPS = 144;
};
