#include "Precomp.h"
#include "Core/Utilities/RefObject.h"

RTTI_CLASS_DEFINITION(RefObject)

RTTI_EMPTY_SERIALIZE_DEFINITION(RefObject)

// This is an invalid weak ref counter to be used when nullptr is passed. It always keeps 1 reference to itself to prevent it's destruction
RefObject::WeakRefCounter gInvalidWeakRefCounter = {false, 1};
