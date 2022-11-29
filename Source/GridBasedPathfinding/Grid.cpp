// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

#include "BFLUtilities.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	RootComponent = InstancedStaticMesh;

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::InitializeGridMeshInstance(UStaticMesh* StaticMesh, UMaterial* Material, FColor Color,
	ECollisionChannel Collision)
{
	SetOffsetFromGround(GridOffset);
}

void AGrid::InitializeGridMeshInstance(UStaticMesh* StaticMesh)
{
	ClearInstances();
	
	InstancedStaticMesh->SetStaticMesh(StaticMesh);

	SetOffsetFromGround(GridOffset);
}

void AGrid::SetOffsetFromGround(float Offset)
{
	GridOffset = Offset;
	
	FVector GridLocation = GetActorLocation();
	GridLocation.Z += Offset;

	SetActorLocation(GridLocation);
}

void AGrid::AddInstance(FVector2D Index, FTransform InstanceTransform)
{
	RemoveInstance(Index);
	
	InstancedStaticMesh->AddInstance(InstanceTransform, true);
	InstanceIndex.Add(Index);
}

void AGrid::RemoveInstance(FVector2D Index)
{
	if(!InstanceIndex.Contains(Index)) return;
	
	InstancedStaticMesh->RemoveInstance(InstanceIndex.Find(Index));
	InstanceIndex.Remove(Index);
}

void AGrid::ClearInstances()
{
	InstancedStaticMesh->ClearInstances();
	InstanceIndex.Empty();
}

void AGrid::UpdateGrid(FTileData TileData)
{
	RemoveInstance(TileData.Index);
	
	if(!UBFLUtilities::IsTileWalkable(TileData.TileType))
	{
		UE_LOG(LogTemp, Error, TEXT("RETURN"));
		return;
	}

	AddInstance(TileData.Index, TileData.Transform);
}

