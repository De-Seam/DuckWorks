#pragma once
#include <DuckCore/Containers/Array.h>

#include <Engine/Objects/Object.h>

class DebugWidget : public Object
{
	RTTI_CLASS(DebugWidget, Object)
public:
	explicit DebugWidget(DC::GUID aGUID = DC::GUID::sCreate()) : Base(aGUID) {}

	template<typename taType, typename... taArgs> requires std::constructible_from<taType, taArgs...>
	DebugWidget* CreateChild(taArgs&&... aArguments);

	void Remove(); // Remove this widget from its parent.

private:
	DC::WeakRef<DebugWidget> mParentWidget;
	DC::Array<DC::Ref<DebugWidget>> mChildWidgets;
};

template<typename taType, typename ... taArgs> requires std::constructible_from<taType, taArgs...>
DebugWidget* DebugWidget::CreateChild(taArgs&&... aArguments)
{
	DebugWidget* widget = new taType(std::forward<taArgs>(aArguments)...);
	
	widget->mParentWidget = this;
	mChildWidgets.Add(widget);

	return widget;
}
