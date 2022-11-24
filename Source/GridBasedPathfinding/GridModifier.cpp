// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifier.h"

// Sets default values
AGridModifier::AGridModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AGridModifier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridModifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

