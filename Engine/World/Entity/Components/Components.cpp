#include <Engine/World/Entity/Components/Components.h>

using namespace DC;

void Internal::RegisterAddComponentFunction(std::function<ECSEntity&> aAddComponentFunction) {}

void Internal::RegisterRemoveComponentFunction(std::function<ECSEntity&> aRemoveComponentFunction) {}

void Internal::RegisterGetComponentFunction(std::function<ECSEntity&> aGetComponentFunction) {}

void Internal::RegisterHasComponentFunction(std::function<ECSEntity&> aHasComponentFunction) {}
