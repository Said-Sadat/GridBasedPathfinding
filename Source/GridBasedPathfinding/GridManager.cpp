// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "BFLUtilities.h"
#include "Kismet/KismetMathLibrary.h"

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

			InstancedStaticMesh->AddInstance(tile, true);
		}
	}
}

FVector AGridManager::CalculateGridBottomLeftCorner(FVector CenterLocation, FVector TileSize, FVector2D TileCount)
{
	FVector GridTileCount3D = FVector(TileCount.X, TileCount.Y, 0);

	if(!UBFLUtilities::IsFloatEven(TileCount.X))
		GridTileCount3D.X -= 1;
	
	if(!UBFLUtilities::IsFloatEven(TileCount.Y))
		GridTileCount3D.Y -= 1;
	
	return CalculateGridSnappedCenter(CenterLocation, TileSize) - TileSize * (GridTileCount3D / 2);
}

FVector AGridManager::CalculateGridSnappedCenter(FVector CenterLocation, FVector TileSize)
{
	return UBFLUtilities::SnapVectors(CenterLocation, GridTileSize);
}

