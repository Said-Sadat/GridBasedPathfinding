// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "GridShapesStruct.h"
#include "TileData.h"
#include "GridManager.generated.h"

UCLASS()
class GRIDBASEDPATHFINDING_API AGridManager : public AGrid
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	bool ScanFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	float ScanRadius;

	UPROPERTY(BlueprintReadWrite, Category= "Grid")
	TMap<FVector2D, FTileData> GridTiles;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	FGridShapesStruct GridShapesStruct;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnGrid(FVector CenterLocation, FVector TileSize, FVector2D TileCount);

	UFUNCTION(BlueprintCallable)
	void AddGridTile(FTileData TileData);

	UFUNCTION(BlueprintCallable)
	void AddStateToTile(FVector2D Index, ETileStates TileState);

	UFUNCTION(BlueprintCallable)
	void AddOccupierToTile(FVector2D Index, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void RemoveStateFromTile(FVector2D Index, ETileStates TileState);

	UFUNCTION(BlueprintCallable)
	void SnapTileToFloor(FTransform TileTransform, FVector TileSize, FVector2D TileIndex);
};
