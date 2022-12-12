// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridModifierComponent.h"
#include "GameFramework/Actor.h"
#include "GridActor.generated.h"

UCLASS()
class GRIDBASEDPATHFINDING_API AGridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridActor();

	UPROPERTY(VisibleAnywhere)
	UGridModifierComponent* GridModifierComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
