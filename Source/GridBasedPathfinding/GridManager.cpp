// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Components/InstancedStaticMeshComponent.h"

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

	FVector GridTileCount3D = FVector(GridTileCount.X, GridTileCount.Y, 0);
	
	GridBottomLeftCornerLocation = GridCenterLocation - GridTileSize * (GridTileCount3D / 2);

	for(int x = 0; x < GridTileCount.X; x++)
	{
		for(int y = 0; y < GridTileCount.Y; y++)
		{
			FTransform tile;
			tile.SetLocation(GridBottomLeftCornerLocation + FVector(x,y,0) * GridTileSize);
			tile.SetScale3D(GridTileSize / GridShapesStruct.MeshSize);

			InstancedStaticMesh->AddInstance(tile);
		}
	}
}

