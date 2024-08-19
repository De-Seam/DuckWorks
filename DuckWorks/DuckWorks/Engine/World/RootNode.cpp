#include <Precomp.h>
#include <Engine/World/RootNode.h>

Json RootNode::Serialize() const
{
	Json json = RTTIRefObject::Serialize();

	JSON_SAVE(json, mChildren);

	return json;
}

void RootNode::Deserialize(const Json& inJson)
{
	RTTIRefObject::Deserialize(inJson);

	gAssert(GetParent() != nullptr || mWorld != nullptr); // We should have a parent before deserializing (or we're the root node and already have a world)

	ClearChildren();

	for (const Json& child_json : inJson["mChildren"])
	{
		Ref<Node> child = gCoreModule->mRTTIFactory.NewInstance<Node>(String(child_json["ClassName"].get<std::string>()));
		AddChild(child);
		child->Deserialize(child_json);
	}
}
