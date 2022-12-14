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

	AStarPathfinding = NewObject<UAStarPathfinding>();
	
	AssignGridActorsToTiles();
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
	if(!GridTiles.Contains(Index)) return;

	FTileData TileData = *GridTiles.Find(Index);
	TileData.TileStates.AddUnique(TileState);

	AddGridTile(TileData);
}

void AGridManager::AddOccupierToTile(FVector2D Index, AActor* Actor)
{
	if(!GridTiles.Contains(Index)) return;
	
	FTileData TileData = *GridTiles.Find(Index);
	TileData.OccupyingActor = Actor;
	TileData.TileStates.AddUnique(ETileStates::Available);
	AddGridTile(TileData);
}

void AGridManager::RemoveOccupierFromTile(FVector2D Index, AActor* Actor)
{
	if(!GridTiles.Contains(Index)) return;

	FTileData TileData = *GridTiles.Find(Index);
	TileData.OccupyingActor = nullptr;
	TileData.TileStates.Remove(ETileStates::Available);
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

void AGridManager::AssignGridActorsToTiles()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridActor::StaticClass(), OutActors);

	for(auto Actor : OutActors)
	{
		AGridActor* GridActor = Cast<AGridActor>(Actor);

		FVector2D TileIndex = UGridUtilities::WorldToGridPosition(
			Actor->GetActorLocation(),
			GridBottomLeftCornerLocation,
			GridTileSize);

		GridActor->SetLocationOnGrid(TileIndex);
		
		AddStateToTile(TileIndex, ETileStates::Available);
		AddOccupierToTile(TileIndex, Actor);

		GridActor->GridActorPath.AddDynamic(this, &AGridManager::GridActorPathToLocation);
	}
}

void AGridManager::CalculatePath(FTileData StartNode, FTileData EndNode)
{
	if(!GridTiles.Contains(StartNode.Index) || !GridTiles.Contains(EndNode.Index)) return;

	for(auto Node : Path)
		RemoveStateFromTile(Node.Index, ETileStates::Neighbour);

	Path.Empty();
	Path = AStarPathfinding->AStarPathfinding(StartNode, EndNode, GridTiles);

	for(auto Node : Path)
		AddStateToTile(Node.Index, ETileStates::Neighbour);
}

void AGridManager::GridActorPathToLocation(FVector2D StartIndex, FVector2D EndIndex, TArray<FTileData>& OutList)
{
	FTileData StartNode;
	FTileData EndNode;

	if(!GridTiles.Contains(StartIndex) || !GridTiles.Contains(EndIndex)) return;

	StartNode = *GridTiles.Find(StartIndex);
	EndNode = *GridTiles.Find(EndIndex);

	CalculatePath(StartNode, EndNode);

	OutList = Path;
}