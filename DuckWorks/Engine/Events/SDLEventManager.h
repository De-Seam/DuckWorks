#pragma once
// Core includes
#include <DuckCore/RTTI/Ref.h>

// Engine includes
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/StaticArray.h>
#include <DuckCore/Math/Vector.h>

#include <Engine/Manager.h>

#include <SDL/SDL_keycode.h>

class EngineUpdateHandle;

enum class EMouseButton : uint8
{
	Left = 1,
	Middle = 2,
	Right = 3,
	X1 = 4,
	X2 = 5
};

class SDLEventManager : public Manager
{
	RTTI_CLASS(SDLEventManager, Manager)
public:
	SDLEventManager();
	virtual ~SDLEventManager() override;

	void Update();

	// Change the input of IsMouseButtonDown to use the SDL event button
	bool IsMouseButtonDown(EMouseButton inButton) const { return mMouseButtons[gStaticCast<uint8>(inButton)]; }
	bool IsKeyDown(SDL_KeyCode inKey) const { return mKeys.At(inKey); }

	DC::IVec2 GetMousePosition() const { return mMousePosition; }

private:
	DC::Ref<EngineUpdateHandle> mUpdateHandle;

	DC::StaticArray<bool, 6> mMouseButtons;
	DC::HashMap<int32, bool> mKeys;
	DC::IVec2 mMousePosition = { 0, 0 };
};
