// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AStarPathfinding.h"
#include "GridManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "PlayerActions.generated.h"

UCLASS()
class GRIDBASEDPATHFINDING_API APlayerActions : public AActor
{
	GENERATED_BODY()

	FVector2D HoveredTile;
	FVector2D SelectedTile;
	TArray<FVector2D> NeighbourTiles;

	FTileData StartNode;
	FTileData EndNode;
	TArray<FTileData> Path;

	APlayerController* PlayerController;
	UAStarPathfinding* AStarPathfinding;

	void FindPath(FTileData s, FTileData n);
	
public:	
	// Sets default values for this actor's properties
	APlayerActions();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	AGridManager* GridManager;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpdateTileCursor();

	void ClickOnTile();

	void RClickOnTile();

};
