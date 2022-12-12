// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commons.h"
#include "Components/ActorComponent.h"
#include "GridModifierComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDBASEDPATHFINDING_API UGridModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridModifierComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	TEnumAsByte<ETileTypes> TileType;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
