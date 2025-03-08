#pragma once
#include <DuckCore/Math/Vector.h>

#include <Engine/Objects/Object.h>

class DuckEditor;
class RenderTarget;

class Editor : public Object
{
	RTTI_CLASS(Editor, Object)
public:
	Editor(const DC::GUID& aGUID = DC::GUID::sNone());
	explicit Editor(const DC::Json& aJson);
	virtual ~Editor() override;
	virtual DC::Json ToJson() const override;

	virtual void OnFirstUpdate() {} // Initial update. After BeginFrame().
	virtual void Update();

	void SetSize(DC::IVec2 aSize);
	DC::IVec2 GetSize();

	virtual void OnAddedToDuckEditor(DuckEditor& aParentDuckEditor);
	virtual void OnRemovedFromDuckEditor(const DuckEditor& aParentDuckEditor);

	DuckEditor* GetDuckEditor() { return mDuckEditor; }
	RenderTarget& GetRenderTarget() { return *mRenderTarget; }
	float GetDeltaTime() const { return mDeltaTime; }

protected:
	DC::Ref<RenderTarget> mRenderTarget;

private:
	DuckEditor* mDuckEditor = nullptr;

	std::chrono::time_point<std::chrono::high_resolution_clock> mLastUpdateTimePoint = std::chrono::high_resolution_clock::now();
	float mDeltaTime = 0.0f;
};
