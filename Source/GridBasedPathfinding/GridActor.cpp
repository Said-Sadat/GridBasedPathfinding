// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActor.h"

#include "GridManager.h"
#include "GridUtilities.h"
#include "Kismet/GameplayStatics.h"

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

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass());

	GridManager = Cast<AGridManager>(FoundActor);

	UGameplayStatics::GetPlayerController(GetWorld(),0)->InputComponent->BindAction(
		"LMB",
		EInputEvent::IE_Pressed,
		this,
		&AGridActor::UpdateGridActorOnGrid);
}

// Called every frame
void AGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridActor::UpdateGridActorOnGrid()
{
	FVector2D Index = UGridUtilities::GetTileIndexUnderCursor(
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		GridManager);

	if(!GridManager->GridTiles.Contains(Index)) return;

	FTileData Tile = *GridManager->GridTiles.Find(Index);
	if(Tile.OccupyingActor == this)
	{
		SetTilesInRange(Index);
		return;
	}

	if(TilesInRange.IsEmpty()) return;
	if(!TilesInRange.Contains(Index)) return;
	
	GridManager->CalculatePath(*GridManager->GridTiles.Find(LocationOnGrid), Tile);
	
	bool success;
	TArray<FTileData> OutPath;
	
	if(RequestMovement.IsBound())
		RequestMovement.Broadcast(this, Index, OutPath, success);
	
	if(success)
		SetActorLocation(Tile.Transform.GetLocation());

	
	ClearTilesInRange();
}

void AGridActor::SetLocationOnGrid(FVector2D Index)
{
	LocationOnGrid = Index;
}

FVector2D AGridActor::GetLocationOnGrid()
{
	return LocationOnGrid;
}

void AGridActor::SetTilesInRange(FVector2D Index)
{
	for (auto tile : TilesInRange)
		GridManager->RemoveStateFromTile(tile, ETileStates::Available);

	TilesInRange.Empty();

	TilesInRange = GridManager->GetTilesInRange(Index, Range);

	for (auto tile : TilesInRange)
		GridManager->AddStateToTile(tile, ETileStates::Available);
}

void AGridActor::ClearTilesInRange()
{
	for (auto tile : TilesInRange)
		GridManager->RemoveStateFromTile(tile, ETileStates::Available);

	TilesInRange.Empty();
}

