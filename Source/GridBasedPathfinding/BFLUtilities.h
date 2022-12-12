// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Commons.h"
#include "BFLUtilities.generated.h"

/**
 * 
 */
UCLASS()
class GRIDBASEDPATHFINDING_API UBFLUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FVector SnapVectors(FVector v1, FVector v2);

	UFUNCTION(BlueprintCallable)
	static bool IsFloatEven(float testfloat);

	UFUNCTION(BlueprintCallable)
	static bool IsTileWalkable(ETileTypes TileType);
};
