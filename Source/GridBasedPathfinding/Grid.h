// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileData.h"
#include "Grid.generated.h"

UCLASS()
class GRIDBASEDPATHFINDING_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	UPROPERTY(BlueprintReadWrite, Category= "Grid")
	TArray<FVector2D> InstanceIndex;

protected:
	UPROPERTY(BlueprintReadWrite, Category= "Grid")
	FVector GridCenterLocation;

	UPROPERTY(BlueprintReadWrite, Category= "Grid")
	FVector GridBottomLeftCornerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	FVector GridTileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	FVector2D GridTileCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	float GridOffset;

	UPROPERTY(EditAnywhere, Category= "Grid")
	TMap<TEnumAsByte<ETileStates>, FLinearColor> StateColour;
	
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector CalculateGridBottomLeftCorner(FVector CenterLocation, FVector TileSize, FVector2D TileCount);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void InitializeGridMeshInstance(UStaticMesh* StaticMesh, UMaterial* Material);

	UFUNCTION(BlueprintCallable)
	FTransform GetOffsetFromGround(FTransform Transform, float Offset);

	UFUNCTION(BlueprintCallable)
	void AddInstance(FVector2D Index, FTransform InstanceTransform, TArray<TEnumAsByte<ETileStates>> States);

	UFUNCTION(BlueprintCallable)
	void RemoveInstance(FVector2D Index); 

	UFUNCTION(BlueprintCallable)
	void ClearInstances();

	UFUNCTION(BlueprintCallable)
	void UpdateGrid(FTileData TileData);

	UFUNCTION(BlueprintCallable)
	FLinearColor GetColourFromStates(TArray<TEnumAsByte<ETileStates>> States);

	UFUNCTION(BlueprintCallable)
	FVector GetGridBottomLeftLocation();

	UFUNCTION(BlueprintCallable)
	FVector GetGridTileSize();
};