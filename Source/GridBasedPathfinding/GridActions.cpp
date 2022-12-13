// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActions.h"

#include "AStarPathfinding.h"
#include "GridUtilities.h"
#include "Kismet/GameplayStatics.h"

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
}

// Called every frame
void AGridActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateTileCursor();
}

void AGridActions::UpdateTileCursor()
{
	if(!GridManager) return;
	
	FVector2D TileIndex = UGridUtilities::GetTileIndexUnderCursor(PlayerController, GridManager);

	if(HoveredTile == TileIndex) return;

	if(!GridManager->GridTiles.Contains(TileIndex)) return;

	GridManager->RemoveStateFromTile(HoveredTile, ETileStates::Hovered);

	HoveredTile = TileIndex;
	GridManager->AddStateToTile(HoveredTile, ETileStates::Hovered);
}