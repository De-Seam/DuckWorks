#include <Game/Grid/GridService.h>

using namespace DC;

GridService::GridService(World& inWorld) :
	Base(inWorld)
{
	mGrid = gMakeUnique<Grid>(128, 128);
}
