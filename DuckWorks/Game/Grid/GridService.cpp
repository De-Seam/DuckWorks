#include <Game/Grid/GridService.h>

using namespace DC;

GridService::GridService(World& inWorld) :
	Base(inWorld)
{
	mGrid = MakeUnique<Grid>(128, 128);
}
