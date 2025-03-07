#include <Editor/Editor.h>

#include <DuckCore/Managers/Managers.h>

#include <Engine/Renderer/Renderer.h>

using namespace DC;

Editor::Editor(const GUID& aGUID) : Base(aGUID)
{
	mRenderTarget = Get<Renderer>().CreateRenderTarget({120, 120});
}

Editor::Editor(const Json& aJson) :
	Base(aJson)
{
	mRenderTarget = aJson["mRenderTarget"];
}

Editor::~Editor() = default;

Json Editor::ToJson() const
{
	Json json = Base::ToJson();
	json["mRenderTarget"] = mRenderTarget;

	return json;
}

void Editor::Update()
{
	// Calculate the current time and delta time
	std::chrono::time_point current_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> delta_time = current_time - mLastUpdateTimePoint;
	mLastUpdateTimePoint = current_time;

	mDeltaTime = delta_time.count();
}

void Editor::SetSize(IVec2 aSize)
{
	mRenderTarget = Get<Renderer>().CreateRenderTarget(aSize);
}

IVec2 Editor::GetSize() { return mRenderTarget->GetSize(); }
