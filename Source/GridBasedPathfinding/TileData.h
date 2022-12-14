#pragma once

#include "CoreMinimal.h"
#include "Commons.h"
#include "TileData.Generated.h"



USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

public:
	float FCost;
	float GCost;
	float HCost;
	FVector2D ParentIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Index;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETileTypes> TileType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<ETileStates>> TileStates;

	UPROPERTY(BlueprintReadWrite)
	AActor* OccupyingActor = nullptr;

	FTileData();
	FTileData(FVector2D Index, ETileTypes TileType, FTransform Transform);

	void InitializeNode();
	
	TArray<FTileData> GetTileNeigbours(TMap<FVector2D, FTileData> GridMap, int filler);

	inline bool operator==(FTileData a) const
	{
		if(a.Index == Index)
			return true;

		return false;
	}
};
