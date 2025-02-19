#pragma once
#include <DuckCore/Containers/UniquePtr.h>

#include <Game/Grid/Grid.h>
#include <Game/World/Service.h>

class GridService : public Service
{
	RTTI_CLASS(GridService, Service)
public:
	explicit GridService(World& inWorld);

	Grid& GetGrid() { return *mGrid; }
	const Grid& GetGrid() const { return *mGrid; }

private:
	DC::UniquePtr<Grid> mGrid;
};
