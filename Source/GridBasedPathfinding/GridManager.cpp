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

void AGridManager::SpawnGrid()
{
	InstancedStaticMesh->ClearInstances();
	InstancedStaticMesh->SetStaticMesh(GridShapesStruct.FlatMesh);

	GridBottomLeftCornerLocation = CalculateGridBottomLeftCorner();

	GridTileCount.X = UKismetMathLibrary::Round(GridTileCount.X);
	GridTileCount.Y = UKismetMathLibrary::Round(GridTileCount.Y);

	for(int x = 0; x < GridTileCount.X; x++)
	{
		for(int y = 0; y < GridTileCount.Y; y++)
		{
			FTransform tile;
			tile.SetLocation(GridBottomLeftCornerLocation + FVector(x,y,0) * GridTileSize);
			tile.SetScale3D(GridTileSize / GridShapesStruct.MeshSize);

			InstancedStaticMesh->AddInstance(tile, true);
		}
	}
}

FVector AGridManager::CalculateGridBottomLeftCorner()
{
	FVector GridTileCount3D = FVector(GridTileCount.X, GridTileCount.Y, 0);

	if(!UBFLUtilities::IsFloatEven(GridTileCount.X))
		GridTileCount3D.X -= 1;
	
	if(!UBFLUtilities::IsFloatEven(GridTileCount.Y))
		GridTileCount3D.Y -= 1;
	
	return CalculateGridSnappedCenter() - GridTileSize * (GridTileCount3D / 2);
}

FVector AGridManager::CalculateGridSnappedCenter()
{
	return UBFLUtilities::SnapVectors(GridCenterLocation, GridTileSize);
}

