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

	bool success;
	TArray<FTileData> OutPath;
	
	if(RequestMovement.IsBound())
		RequestMovement.Broadcast(this, Index, OutPath, success);

	UE_LOG(LogTemp,Warning,TEXT("BOG"));
	
	if(success)
	{
		FTileData Tile = *GridManager->GridTiles.Find(Index);
		SetActorLocation(Tile.Transform.GetLocation());
	}
}

void AGridActor::SetLocationOnGrid(FVector2D Index)
{
	LocationOnGrid = Index;
}

FVector2D AGridActor::GetLocationOnGrid()
{
	return LocationOnGrid;
}


