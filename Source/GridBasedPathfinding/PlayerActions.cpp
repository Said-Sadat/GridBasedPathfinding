// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActions.h"

#include "AStarPathfinding.h"
#include "Kismet/GameplayStatics.h"

void APlayerActions::FindPath(FTileData s, FTileData n)
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
APlayerActions::APlayerActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerActions::BeginPlay()
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

	PlayerController->InputComponent->BindAction("LMB", EInputEvent::IE_Pressed, this, &APlayerActions::ClickOnTile);
	PlayerController->InputComponent->BindAction("RMB", EInputEvent::IE_Pressed, this, &APlayerActions::RClickOnTile);
}

// Called every frame
void APlayerActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UpdateTileCursor();
}

void APlayerActions::UpdateTileCursor()
{
	if(!GridManager) return;
	
	FVector2D TileIndex = GridManager->GetTileIndexUnderCursor();

	if(HoveredTile == TileIndex) return;

	GridManager->RemoveStateFromTile(HoveredTile, ETileStates::Hovered);

	for (auto Tile : NeighbourTiles)
		GridManager->RemoveStateFromTile(Tile, ETileStates::Neighbour);

	NeighbourTiles.Empty();
		
	HoveredTile = TileIndex;
	GridManager->AddStateToTile(HoveredTile, ETileStates::Hovered);

	if(!GridManager->GridTiles.Find(TileIndex)) return;
	
	FTileData TileData = *GridManager->GridTiles.Find(TileIndex);

	for (auto TileNeighbour : TileData.GetTileNeigbours(GridManager->GridTiles))
	{
		NeighbourTiles.Add(TileNeighbour);
		GridManager->AddStateToTile(TileNeighbour, ETileStates::Neighbour);
	}
}

void APlayerActions::ClickOnTile()
{
	if(!GridManager) return;
	
	FVector2D TileIndex = GridManager->GetTileIndexUnderCursor();
	
	if(SelectedTile == TileIndex) return;

	GridManager->RemoveStateFromTile(SelectedTile, ETileStates::Selected);
		
	SelectedTile = TileIndex;
	GridManager->AddStateToTile(SelectedTile, ETileStates::Selected);

	StartNode = *GridManager->GridTiles.Find(TileIndex);
}

void APlayerActions::RClickOnTile()
{
	if(!GridManager) return;
	
	FVector2D TileIndex = GridManager->GetTileIndexUnderCursor();
	
	if(EndNode.Index == TileIndex) return;

	GridManager->RemoveStateFromTile(EndNode.Index, ETileStates::Hovered);
		
	EndNode.Index = TileIndex;
	GridManager->AddStateToTile(EndNode.Index, ETileStates::Hovered);

	EndNode = *GridManager->GridTiles.Find(TileIndex);

	FindPath(StartNode, EndNode);
}

