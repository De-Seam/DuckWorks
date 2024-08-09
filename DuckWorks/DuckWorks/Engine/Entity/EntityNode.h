#pragma once
// Engine includes
#include <Engine/Entity/Entity.h>
#include <Engine/Node/Node.h>


class EntityNode : public Node
{
	RTTI_CLASS(EntityNode, Node)
public:

protected:
	virtual void OnAddedToWorld() override;
	virtual void OnRemovedFromWorld() override;
	virtual void OnTransformUpdated() override;

private:
	Ref<Entity> mEntity;
};
