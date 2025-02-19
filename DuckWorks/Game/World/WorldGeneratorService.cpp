#include <Game/World/WorldGeneratorService.h>

#include <Game/Entity/EntityService.h>
#include <Game/Grid/Grid.h>
#include <Game/Grid/GridService.h>
#include <Game/World/World.h>

WorldGeneratorService::WorldGeneratorService(World& inWorld) : Base(inWorld) { }

void WorldGeneratorService::Generate()
{
	Grid& grid = GetWorld().GetService<GridService>().GetGrid();
	const DC::UVec2& size = grid.GetSize();
	for (uint y = 0; y < size.mY; y++)
	{
		for (uint x = 0; x < size.mX; x++)
		{
			GridTile& tile = grid.GetTile({ x, y });
			DC::Ref<Entity> entity = new Entity(GetWorld());
			GetWorld().GetService<EntityService>().AddEntity(entity);
			tile.SetEntity(*entity);
		}
	}
}
