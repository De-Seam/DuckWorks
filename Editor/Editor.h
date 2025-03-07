#pragma once
#include <DuckCore/Math/Vector.h>

#include <Engine/Objects/Object.h>

class RenderTarget;

class Editor : public Object
{
	RTTI_CLASS(Editor, Object)
public:
	Editor(const DC::GUID& aGUID = DC::GUID::sNone());
	explicit Editor(const DC::Json& aJson);
	virtual ~Editor() override;
	virtual DC::Json ToJson() const override;

	virtual void Update();

	void SetSize(DC::IVec2 aSize);
	DC::IVec2 GetSize();

	RenderTarget& GetRenderTarget() { return *mRenderTarget; }
	float GetDeltaTime() const { return mDeltaTime; }

protected:
	DC::Ref<RenderTarget> mRenderTarget;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> mLastUpdateTimePoint = std::chrono::high_resolution_clock::now();
	float mDeltaTime = 0.0f;
};
