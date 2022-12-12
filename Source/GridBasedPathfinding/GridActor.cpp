// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActor.h"

#include "GridUtilities.h"

// Sets default values
AGridActor::AGridActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMeshComponent);
	
	GridModifierComponent = CreateDefaultSubobject<UGridModifierComponent>(TEXT("Grid Modifier"));
}

// Called when the game starts or when spawned
void AGridActor::BeginPlay()
{
	Super::BeginPlay();

	if(GridModifierComponent->TileType == ETileTypes::Occupied)
		UpdateGridActorOnGrid();
}

// Called every frame
void AGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGridActor::UpdateGridActorOnGrid()
{
	FVector2D TileIndex = UGridUtilities::WorldToGridPosition(
		GetActorLocation(),
		GridManager->GetGridBottomLeftLocation(),
		GridManager->GetGridTileSize());
	
	GridManager->AddStateToTile(TileIndex, ETileStates::Neighbour);
	GridManager->AddOccupierToTile(TileIndex, this);
}


