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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid", meta=(AllowPrivateAccess = true))
	float GridOffset;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void InitializeGridMeshInstance(UStaticMesh* StaticMesh);
	
	void InitializeGridMeshInstance(UStaticMesh* StaticMesh, UMaterial* Material, FColor Color, ECollisionChannel Collision);

	UFUNCTION(BlueprintCallable)
	void SetOffsetFromGround(float Offset);

	UFUNCTION(BlueprintCallable)
	void AddInstance(FVector2D Index, FTransform InstanceTransform);

	UFUNCTION(BlueprintCallable)
	void RemoveInstance(FVector2D Index); 

	UFUNCTION(BlueprintCallable)
	void ClearInstances();

	UFUNCTION(BlueprintCallable)
	void UpdateGrid(FTileData TileData);

};
