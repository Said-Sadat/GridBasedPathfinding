#include "TileData.h"

FTileData::FTileData()
{
	TileType = ETileTypes::Normal;
	
}

FTileData::FTileData(FVector2D Index, ETileTypes TileType, FTransform Transform)
{
	this->Index = Index;
	this->TileType = TileType;
	this->Transform = Transform;
}