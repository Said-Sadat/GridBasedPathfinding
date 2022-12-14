#include "TileData.h"

FTileData::FTileData()
{
	InitializeNode();
}

FTileData::FTileData(FVector2D Index, ETileTypes TileType, FTransform Transform)
{
	this->Index = Index;
	this->TileType = TileType;
	this->Transform = Transform;

	InitializeNode();
}

void FTileData::InitializeNode()
{
	TileType = ETileTypes::Normal;
	
	FCost = 0;
	GCost = 10000;
	HCost = 0;

	ParentIndex = FVector2D(-100,-100);
}

// Gets the neighbours on 4 directions of the tile.
TArray<FTileData> FTileData::GetTileNeigbours(TMap<FVector2D, FTileData> GridMap, int filler)
{
	TArray<FTileData> Neighbours;

	if(GridMap.Contains(Index + FVector2D(0,1)))
		Neighbours.Add(*GridMap.Find(Index + FVector2D(0,1)));
		
	if(GridMap.Contains(Index + FVector2D(0,-1)))
		Neighbours.Add(*GridMap.Find(Index + FVector2D(0,-1)));

	if(GridMap.Contains(Index + FVector2D(-1,0)))
		Neighbours.Add(*GridMap.Find(Index + FVector2D(-1,0)));

	if(GridMap.Contains(Index + FVector2D(1,0)))
		Neighbours.Add(*GridMap.Find(Index + FVector2D(1,0)));

	return Neighbours;
}
