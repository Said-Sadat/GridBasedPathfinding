// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "BFLUtilities.h"
#include "GridModifier.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#define Ground ECC_GameTraceChannel1
#define Grid ECC_GameTraceChannel2

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	RootComponent = InstancedStaticMesh;


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
	InstancedStaticMesh->ClearInstances();
	InstancedStaticMesh->SetStaticMesh(GridShapesStruct.FlatMesh);

	GridBottomLeftCornerLocation = CalculateGridBottomLeftCorner(CenterLocation, TileSize, TileCount);

	TileCount.X = UKismetMathLibrary::Round(TileCount.X);
	TileCount.Y = UKismetMathLibrary::Round(TileCount.Y);

	for(int x = 0; x < TileCount.X; x++)
	{
		for(int y = 0; y < TileCount.Y; y++)
		{
			FTransform tile;
			tile.SetLocation(GridBottomLeftCornerLocation + FVector(x,y,0) * TileSize);
			tile.SetScale3D(TileSize / GridShapesStruct.MeshSize);

			if(ScanFloor)
				SnapTileToFloor(tile, TileSize);
			else
				InstancedStaticMesh->AddInstance(tile, true);
		}
	}
}

bool AGridManager::IsWalkable(ETileTypes TileType)
{
	switch (TileType)
	{
	case None:
		return false;
	case Normal:
		return true;
	case Obstacle:
		return false;
	default:
		return false;
	}
}

FVector AGridManager::GetCursorLocationOnGrid(APlayerController* PlayerController)
{
	FHitResult HitResult;
	
	if(!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is null"));
		return FVector(0,0,0);
	}
	
	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(Grid), false, HitResult);

	if(HitResult.GetActor())
		return HitResult.Location;

	FVector WorldLocation;
	FVector WorldDirection;
	
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FVector Intersection = FMath::LinePlaneIntersection(WorldLocation,
		(WorldDirection * 1000) + WorldLocation,
		UKismetMathLibrary::MakePlaneFromPointAndNormal(GridCenterLocation, FVector(0,0,1)));

	return Intersection;
}

FVector AGridManager::CalculateGridBottomLeftCorner(FVector CenterLocation, FVector TileSize, FVector2D TileCount)
{
	FVector GridTileCount3D = FVector(TileCount.X, TileCount.Y, 0);

	if(!UBFLUtilities::IsFloatEven(TileCount.X))
		GridTileCount3D.X -= 1;
	
	if(!UBFLUtilities::IsFloatEven(TileCount.Y))
		GridTileCount3D.Y -= 1;
	
	return CenterLocation - TileSize * (GridTileCount3D / 2);
}

void AGridManager::SnapTileToFloor(FTransform TileTransform, FVector TileSize)
{
	TArray<FHitResult> OutHits;
	TArray<AActor*> IgnoreArray;
				
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TileTransform.GetLocation() + FVector(0,0,1000),
		TileTransform.GetLocation() + FVector(0,0,-1000),
		TileSize.X / 3,
		UEngineTypes::ConvertToTraceType(Ground),
		false,
		IgnoreArray,
		EDrawDebugTrace::ForDuration,
		OutHits,
		false);

	for(auto OutHit : OutHits)
	{
		AGridModifier* GridModifier = Cast<AGridModifier>(OutHit.GetActor());
		if(GridModifier)
			if(!IsWalkable(GridModifier->TileType))
				return;
	}

	if(!OutHits.IsEmpty())
	{
		FVector TileLocation = TileTransform.GetLocation();
		TileLocation.Z = OutHits[0].Location.Z - TileSize.X/3 + OffsetFromGround;
		TileTransform.SetLocation(TileLocation);

		InstancedStaticMesh->AddInstance(TileTransform, true);
	}
}