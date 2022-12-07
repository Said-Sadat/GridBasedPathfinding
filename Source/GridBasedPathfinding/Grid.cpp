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

void AGrid::InitializeGridMeshInstance(UStaticMesh* StaticMesh)
{
	ClearInstances();
	
	InstancedStaticMesh->SetStaticMesh(StaticMesh);
}

FTransform AGrid::GetOffsetFromGround(FTransform Transform, float Offset)
{
	FVector Location = Transform.GetLocation();
	Location.Z += GridOffset;
	Transform.SetLocation(Location);	
	
	return Transform;
}

void AGrid::AddInstance(FVector2D Index, FTransform InstanceTransform, TArray<TEnumAsByte<ETileStates>> States)
{
	RemoveInstance(Index);
	
	InstancedStaticMesh->AddInstance(InstanceTransform, true);
	InstanceIndex.Add(Index);

	FLinearColor Color = GetColourFromStates(States);

	InstancedStaticMesh->SetCustomDataValue(InstanceIndex.Find(Index), 0, Color.R);
	InstancedStaticMesh->SetCustomDataValue(InstanceIndex.Find(Index), 1, Color.G);
	InstancedStaticMesh->SetCustomDataValue(InstanceIndex.Find(Index), 2, Color.B);

	float fill;
	
	if(States.Contains(Selected) || States.Contains(Hovered) || States.Contains(Neighbour))
		fill = 1;
	else
		fill = 0;
	
	InstancedStaticMesh->SetCustomDataValue(InstanceIndex.Find(Index), 3, fill);
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

	TileData.Transform = GetOffsetFromGround(TileData.Transform, GridOffset);
	AddInstance(TileData.Index, TileData.Transform, TileData.TileStates);
}

FLinearColor AGrid::GetColourFromStates(TArray<TEnumAsByte<ETileStates>> States)
{
	if(States.IsEmpty())
		return FLinearColor::Black;

	if(States.Contains(ETileStates::Selected))
		return FLinearColor::Green;
	
	if(States.Contains(ETileStates::Hovered))
		 return FLinearColor::Red;

	if(States.Contains(ETileStates::Neighbour))
		return FLinearColor::Yellow;
	
	return  FLinearColor::Black;
}

FVector AGrid::CalculateGridBottomLeftCorner(FVector CenterLocation, FVector TileSize, FVector2D TileCount)
{
	FVector GridTileCount3D = FVector(TileCount.X, TileCount.Y, 0);

	if(!UBFLUtilities::IsFloatEven(TileCount.X))
		GridTileCount3D.X -= 1;
	
	if(!UBFLUtilities::IsFloatEven(TileCount.Y))
		GridTileCount3D.Y -= 1;
	
	return CenterLocation - TileSize * (GridTileCount3D / 2);
}
