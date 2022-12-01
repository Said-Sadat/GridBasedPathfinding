// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EShapesEnum.h"
#include "GridShapesStruct.generated.h"

USTRUCT(BlueprintType)
struct FGridShapesStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EShapesEnum ShapeType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MeshMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* FlatMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* FlatBoarderMaterial;
};
