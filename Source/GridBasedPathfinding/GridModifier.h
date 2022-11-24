// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridModifier.generated.h"

UENUM(BlueprintType)
enum ETileTypes
{
	None,
	Normal,
	Obstacle
};

UCLASS()
class GRIDBASEDPATHFINDING_API AGridModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridModifier();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Tile")
	TEnumAsByte<ETileTypes> TileType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
