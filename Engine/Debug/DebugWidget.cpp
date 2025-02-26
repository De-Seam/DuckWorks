#include <Engine/Debug/DebugWidget.h>

using namespace DC;

void DebugWidget::Remove()
{
	Ref<DebugWidget> parent = mParentWidget.Ref();
	mParentWidget = nullptr;

	parent->mChildWidgets.RemoveFirstIf([this](const DebugWidget* aWidget) { return aWidget == this; });
}