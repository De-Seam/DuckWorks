#include <Precomp.h>
#include <Engine/Entity/EntityNode.h>

void EntityNode::OnAddedToWorld()
{
	Node::OnAddedToWorld();

	mEntity = Entity::sNewInstance();
}

void EntityNode::OnRemovedFromWorld()
{
	Node::OnRemovedFromWorld();

	mEntity = nullptr;
}

void EntityNode::OnTransformUpdated()
{
	mEntity->SetTransform(GetWorldTransform());
}
