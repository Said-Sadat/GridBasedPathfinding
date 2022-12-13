// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "GridUtilities.h"
#include "GridActor.h"
#include "Commons.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridManager::SpawnGrid(FVector CenterLocation, FVector TileSize, FVector2D TileCount)
{
	GridTiles.Empty();
	InitializeGridMeshInstance(GridShapesStruct.FlatMesh);
	
	GridBottomLeftCornerLocation = CalculateGridBottomLeftCorner(CenterLocation, TileSize, TileCount);

	TileCount.X = UKismetMathLibrary::Round(TileCount.X);
	TileCount.Y = UKismetMathLibrary::Round(TileCount.Y);

	for(int x = 0; x < TileCount.X; x++)
	{
		for(int y = 0; y < TileCount.Y; y++)
		{
			FTransform tile;
			FVector TileLocation = GridBottomLeftCornerLocation + FVector(x,y,0) * TileSize;
			tile.SetLocation(TileLocation);
			tile.SetScale3D(TileSize / GridShapesStruct.MeshSize); 

			if(ScanFloor)
				SnapTileToFloor(tile, TileSize, FVector2D(x,y));
			else
				AddGridTile(FTileData(FVector2D(x,y), ETileTypes::Normal, tile)); 
		}
	}
}

void AGridManager::AddGridTile(FTileData TileData)
{
	GridTiles.Add(TileData.Index, TileData);
	UpdateGrid(TileData);
}

void AGridManager::AddStateToTile(FVector2D Index, ETileStates TileState)
{
	if(!GridTiles.Find(Index)) return;

	FTileData TileData = *GridTiles.Find(Index);
	TileData.TileStates.AddUnique(TileState);

	AddGridTile(TileData);
}

void AGridManager::AddOccupierToTile(FVector2D Index, AActor* Actor)
{
	FTileData TileData = *GridTiles.Find(Index);
	TileData.OccupyingActor = Actor;
	AddGridTile(TileData);
}

void AGridManager::RemoveStateFromTile(FVector2D Index, ETileStates TileState)
{
	if(!GridTiles.Find(Index)) return;

	FTileData TileData = *GridTiles.Find(Index);
	TileData.TileStates.Remove(TileState);

	AddGridTile(TileData);
}

TArray<FVector2D> AGridManager::GetTilesInRange(FVector2D StartingIndex, int Range)
{
	TArray<FVector2D> InRangeTiles;
	int StepCount = 0;

	InRangeTiles.Add(StartingIndex);
	
	while (StepCount < Range)
	{
		TArray<FVector2D> SurroundingTiles;

		for (auto Tile : InRangeTiles)
			for (auto NeighbourTile : GridTiles[Tile].GetTileNeigbours(GridTiles))
				SurroundingTiles.Add(NeighbourTile);

		for (auto SurroundingTile : SurroundingTiles)
			InRangeTiles.AddUnique(SurroundingTile);

		StepCount++;
	}
	
	return InRangeTiles;
}

void AGridManager::SnapTileToFloor(FTransform TileTransform, FVector TileSize, FVector2D TileIndex)
{
	TArray<FHitResult> OutHits;
	TArray<AActor*> IgnoreArray;
				
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TileTransform.GetLocation() + FVector(0,0,1000),
		TileTransform.GetLocation() + FVector(0,0,-1000),
		TileSize.X / 3,
		UEngineTypes::ConvertToTraceType(Ground),
		false,
		IgnoreArray,
		EDrawDebugTrace::None,
		OutHits,
		false);

	for(auto OutHit : OutHits)
	{
		AGridActor* GridActor = Cast<AGridActor>(OutHit.GetActor());
		
		if(GridActor)
			if(!UGridUtilities::IsTileWalkable(GridActor->GridModifierComponent->TileType))
				return;
		
		FVector TileLocation = TileTransform.GetLocation();
		TileLocation.Z = OutHit.Location.Z - TileSize.X/3;
		TileTransform.SetLocation(TileLocation);

		FTileData TileData = FTileData(TileIndex, ETileTypes::Normal, TileTransform);
		AddGridTile(TileData);
	}
}
