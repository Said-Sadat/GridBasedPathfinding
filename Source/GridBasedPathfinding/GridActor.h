// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridManager.h"
#include "GridModifierComponent.h"
#include "GameFramework/Actor.h"
#include "GridActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRequestMovement, class AGridActor*, Actor, FVector2D, Destination, TArray<FTileData>&, OutPath, bool&, Successful);


UCLASS()
class GRIDBASEDPATHFINDING_API AGridActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Grid", meta=(AllowPrivateAccess = true))
	FVector2D LocationOnGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid", meta=(AllowPrivateAccess = true))
	int Range;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Grid", meta=(AllowPrivateAccess = true))
	TArray<FVector2D> TilesInRange;
	
public:	
	// Sets default values for this actor's properties
	AGridActor();
	
	UPROPERTY(VisibleAnywhere)
	UGridModifierComponent* GridModifierComponent;

	FRequestMovement RequestMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(BlueprintReadWrite)
	AGridManager* GridManager;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateGridActorOnGrid();

	UFUNCTION()
	void SetLocationOnGrid(FVector2D Index);

	UFUNCTION()
	FVector2D GetLocationOnGrid();

	UFUNCTION()
	void SetTilesInRange(FVector2D Index);
};
