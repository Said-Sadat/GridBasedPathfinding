// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TileData.h"
#include "AStarPathfinding.generated.h"

/**
 * 
 */
UCLASS()
class GRIDBASEDPATHFINDING_API UAStarPathfinding : public UObject
{
	GENERATED_BODY()


	bool IsDestination(FTileData Node, FTileData EndNode);

	float CalculateHCost(FTileData Node, FTileData EndNode);
	
	float CalculateFCost(float G, float H);

	FTileData GetLowestFCostNode(TArray<FTileData> NodeList);

	TArray<FTileData> CalculatePath(FTileData EndNode);
public:
	UAStarPathfinding();

	TMap<FVector2D, FTileData> GridMap;

	TArray<FTileData> AStarPathfinding(FTileData StartNode, FTileData EndNode);

};
