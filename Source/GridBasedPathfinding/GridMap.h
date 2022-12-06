// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileData.h"
#include "UObject/NoExportTypes.h"
#include "GridMap.generated.h"

/**
 * 
 */
UCLASS()
class GRIDBASEDPATHFINDING_API UGridMap : public  UActorComponent
{
	GENERATED_BODY()


public:
	UGridMap();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Map", meta=(AllowPrivateAccess = true))
	TMap<FVector2D, FTileData> GridTileMap;
	
	UFUNCTION(BlueprintCallable)
	void AddTileToGrid(FVector2D Index, FTileData Tile);

	UFUNCTION(BlueprintCallable)
	FTileData GetTileAtIndex(FVector2D Index);

	UFUNCTION(BlueprintCallable)
	void RemoveTileAtIndex(FVector2D Index);

	UFUNCTION(BlueprintCallable)
	bool IsIndexValid(FVector2D Index);

	UFUNCTION(BlueprintCallable)
	void ClearGridMap();
};
