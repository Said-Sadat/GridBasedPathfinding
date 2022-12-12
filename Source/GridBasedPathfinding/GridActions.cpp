// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActions.h"

#include "AStarPathfinding.h"
#include "GridUtilities.h"
#include "Kismet/GameplayStatics.h"

void AGridActions::FindPath(FTileData s, FTileData n)
{
	if(GridManager->GridTiles.Contains(s.Index) && GridManager->GridTiles.Contains(n.Index))
	{
		for (auto Node : Path)
		{
			GridManager->RemoveStateFromTile(Node.Index, ETileStates::Neighbour);
		}
		
		Path.Empty();
		Path = AStarPathfinding->AStarPathfinding(s, n, GridManager->GridTiles);
		
		for (auto Node : Path)
		{
			GridManager->AddStateToTile(Node.Index, ETileStates::Neighbour);
		}
	}
}

// Sets default values
AGridActions::AGridActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGridActions::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	TArray<AActor*> OutActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridManager::StaticClass(), OutActors);
	
	if(OutActors.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("NO ACTOR FOUND"));
		return;
	}

	GridManager = Cast<AGridManager>(OutActors[0]);

	AStarPathfinding = NewObject<UAStarPathfinding>();

	PlayerController->InputComponent->BindAction("LMB", EInputEvent::IE_Pressed, this, &AGridActions::LClickOnTile);
	PlayerController->InputComponent->BindAction("RMB", EInputEvent::IE_Pressed, this, &AGridActions::RClickOnTile);
}

// Called every frame
void AGridActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector2D TileIndex = UGridUtilities::GetTileIndexUnderCursor(PlayerController, GridManager);
	UE_LOG(LogTemp, Error, TEXT("X: %f Y: %f"), TileIndex.X, TileIndex.Y);
	
	UpdateTileCursor();
}

void AGridActions::UpdateTileCursor()
{
	if(!GridManager) return;
	
	FVector2D TileIndex = UGridUtilities::GetTileIndexUnderCursor(PlayerController, GridManager);

	if(HoveredTile == TileIndex) return;

	if(!GridManager->GridTiles.Contains(TileIndex)) return;

	GridManager->RemoveStateFromTile(HoveredTile, ETileStates::Hovered);

	for (auto Tile : NeighbourTiles)
		GridManager->RemoveStateFromTile(Tile, ETileStates::Neighbour);

	NeighbourTiles.Empty();
		
	HoveredTile = TileIndex;
	GridManager->AddStateToTile(HoveredTile, ETileStates::Hovered);
}

void AGridActions::LClickOnTile()
{
	if(!GridManager) return;
	
	FVector2D TileIndex = UGridUtilities::GetTileIndexUnderCursor(PlayerController, GridManager);
	
	if(SelectedTile == TileIndex) return;
	if(!GridManager->GridTiles.Contains(TileIndex)) return;

	GridManager->RemoveStateFromTile(SelectedTile, ETileStates::Selected);
		
	SelectedTile = TileIndex;
	GridManager->AddStateToTile(SelectedTile, ETileStates::Selected);

	StartNode = *GridManager->GridTiles.Find(TileIndex);
}

void AGridActions::RClickOnTile()
{
	if(!GridManager) return;
	
	FVector2D TileIndex = UGridUtilities::GetTileIndexUnderCursor(PlayerController, GridManager);
	
	if(EndNode.Index == TileIndex) return;
	if(!GridManager->GridTiles.Contains(TileIndex)) return;

	GridManager->RemoveStateFromTile(EndNode.Index, ETileStates::Hovered);
		
	EndNode.Index = TileIndex;
	GridManager->AddStateToTile(EndNode.Index, ETileStates::Hovered);

	EndNode = *GridManager->GridTiles.Find(TileIndex);

	FindPath(StartNode, EndNode);
}