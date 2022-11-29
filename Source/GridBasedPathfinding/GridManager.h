// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "map"
#include "GridShapesStruct.h"
#include "TileData.h"
#include "GridManager.generated.h"

UCLASS()
class GRIDBASEDPATHFINDING_API AGridManager : public AGrid
{
	GENERATED_BODY()

	FVector GridBottomLeftCornerLocation;

	FVector CalculateGridBottomLeftCorner(FVector CenterLocation, FVector TileSize, FVector2D TileCount);
	void SnapTileToFloor(FTransform TileTransform, FVector TileSize, FVector2D TileIndex);
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	FVector GridCenterLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	FVector GridTileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	FVector2D GridTileCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	bool ScanFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	float ScanRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	float OffsetFromGround;

	std::map<FVector2D, FTileData> GridTiles;

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

	void AddGridTile(FTileData TileData);
	
	UFUNCTION(BlueprintCallable)
	FVector GetCursorLocationOnGrid(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	FVector2D WorldPositionToGrid(FVector WorldPosition);

};
