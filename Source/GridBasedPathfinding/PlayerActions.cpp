// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActions.h"

#include "Kismet/GameplayStatics.h"

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

	PlayerController->InputComponent->BindAction("LMB", EInputEvent::IE_Pressed, this, &APlayerActions::ClickOnTile);
}

// Called every frame
void APlayerActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTileCursor();
}

void APlayerActions::UpdateTileCursor()
{
	if(!GridManager) return;
	
	FVector2D TileIndex = GridManager->GetTileIndexUnderCursor();

	if(HoveredTile == TileIndex) return;

	GridManager->RemoveStateFromTile(HoveredTile, ETileStates::Hovered);
		
	HoveredTile = TileIndex;
	GridManager->AddStateToTile(HoveredTile, ETileStates::Hovered);
}

void APlayerActions::ClickOnTile()
{
	UE_LOG(LogTemp, Error, TEXT("CLICK"));

	if(!GridManager) return;
	
	FVector2D TileIndex = GridManager->GetTileIndexUnderCursor();

	if(SelectedTile == TileIndex) return;

	GridManager->RemoveStateFromTile(SelectedTile, ETileStates::Selected);
		
	SelectedTile = TileIndex;
	GridManager->AddStateToTile(SelectedTile, ETileStates::Selected);
}

