#pragma once
#include <DuckCore/Core/Core.h>
#include <DuckCore/Math/Vector.h>

#include <Engine/Objects/Object.h>

class DuckEditor;
class RenderTarget;

class Editor : public Object
{
	RTTI_CLASS(Editor, Object)
public:
	static void sStartup();

	explicit Editor(DuckEditor& aDuckEditor, const DC::GUID& aGUID = DC::GUID::sCreate());
	explicit Editor(const DC::Json& aJson);
	virtual ~Editor() override;
	virtual DC::Json ToJson() const override;

	virtual void OnFirstUpdate() {} // Initial update. After BeginFrame().
	virtual void Update();

	void SetSize(DC::IVec2 aSize);
	DC::IVec2 GetSize();

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
