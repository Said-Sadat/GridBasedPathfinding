#pragma once

#include "CoreMinimal.h"
#include "GridModifier.h"
#include "TileData.Generated.h"

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

	FTileData();
	FTileData(FVector2D Index, ETileTypes TileType, FTransform Transform);
};
