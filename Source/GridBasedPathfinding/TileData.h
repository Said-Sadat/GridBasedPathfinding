#pragma once

#include "CoreMinimal.h"
#include "GridModifier.h"
#include "TileData.Generated.h"

UENUM(BlueprintType)
enum ETileStates
{
	NoState,
	Hovered,
	Selected
};

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Index;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETileTypes> TileType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<ETileStates>> TileStates;

	FTileData();
	FTileData(FVector2D Index, ETileTypes TileType, FTransform Transform);
};
