// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMap.h"

UGridMap::UGridMap()
{
}

void UGridMap::AddTileToGrid(FVector2D Index, FTileData Tile)
{
	UE_LOG(LogTemp, Warning, TEXT("!"));
	GridTileMap[Index] = Tile;
}

FTileData UGridMap::GetTileAtIndex(FVector2D Index)
{
	return *GridTileMap.Find(Index);
}

void UGridMap::RemoveTileAtIndex(FVector2D Index)
{
	GridTileMap.Remove(Index);
}

bool UGridMap::IsIndexValid(FVector2D Index)
{
	if(GridTileMap.Find(Index))
		return true;
	
	return false;
}

void UGridMap::ClearGridMap()
{
	GridTileMap.Empty();
}
