#pragma once
#include "Core/CoreBase.h"
#include "Core/RTTI/RTTI.h"

// External includes
#include "External/SDL/SDL_video.h"

struct BaseUserSettings : public RTTIClass
{
	RTTI_CLASS(BaseUserSettings, RTTIClass)
public:
	IVec2 mWindowSize = {1280, 720};
	uint32 mWindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	int32 mMaxFPS = 144;
};
