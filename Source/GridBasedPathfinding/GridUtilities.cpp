// Fill out your copyright notice in the Description page of Project Settings.


#include "GridUtilities.h"
#include "Kismet/KismetMathLibrary.h"

FVector UGridUtilities::SnapVectors(FVector v1, FVector v2)
{
	FVector SnappedVector;
	SnappedVector.X = UKismetMathLibrary::GridSnap_Float(v1.X, v2.X);
	SnappedVector.Y = UKismetMathLibrary::GridSnap_Float(v1.Y, v2.Y);
	SnappedVector.Z = UKismetMathLibrary::GridSnap_Float(v1.Z, v2.Z);

	return SnappedVector;
}

bool UGridUtilities::IsFloatEven(float testfloat)
{
	double result;
	UKismetMathLibrary::FMod64(testfloat, 2.0f, result);

	if(result == 0)
		return true;

	return  false;
}

bool UGridUtilities::IsTileWalkable(ETileTypes TileType)
{
	switch (TileType)
	{
	case None:
		return false;
	case Obstacle:
		return false;
	default:
		return true;
	}
}

FVector2D UGridUtilities::WorldToGridPosition(FVector WorldPosition, FVector GridBottomLeft, FVector TileSize)
{
	FVector LocationOnGrid;
	LocationOnGrid = WorldPosition - GridBottomLeft;
	LocationOnGrid = UGridUtilities::SnapVectors(LocationOnGrid, TileSize);
	LocationOnGrid /= TileSize;
	
	return FVector2D(LocationOnGrid);
}

FVector2D UGridUtilities::GetTileIndexUnderCursor(APlayerController* PlayerController, AGridManager* GridManager)
{
	FHitResult HitResult;
	
	if(!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is null"));
		return FVector2D(-1,-1);
	}
	
	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(Grid), false, HitResult);

	if(HitResult.GetActor())
		return WorldToGridPosition(
			HitResult.Location,
			GridManager->GetGridBottomLeftLocation(),
			GridManager->GetGridTileSize());

	return FVector2D(-1,-1);
}
