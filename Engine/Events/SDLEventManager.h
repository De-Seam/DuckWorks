#pragma once
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/StaticArray.h>
#include <DuckCore/Managers/Manager.h>
#include <DuckCore/Math/Vector.h>
#include <DuckCore/RTTI/Ref.h>

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

class SDLEventManager : public DC::Manager
{
	MANAGER_BASE_CLASS(SDLEventManager)
public:
	SDLEventManager();
	virtual ~SDLEventManager() override;

	void Update();

	// Change the input of IsMouseButtonDown to use the SDL event button
	bool IsMouseButtonDown(EMouseButton inButton) const { return mMouseButtons[gStaticCast<uint8>(inButton)]; }
	bool IsKeyDown(SDL_KeyCode inKey) const { return mKeys.At(inKey); }

	const DC::IVec2& GetMousePosition() const { return mMousePosition; }

private:
	DC::Ref<EngineUpdateHandle> mUpdateHandle;

	DC::StaticArray<bool, 6> mMouseButtons;
	DC::HashMap<int32, bool> mKeys;
	DC::IVec2 mMousePosition = { 0, 0 };
};
