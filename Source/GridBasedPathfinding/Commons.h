#pragma once

#include "CoreMinimal.h"
#include "Commons.generated.h"

#define Ground ECC_GameTraceChannel1
#define Grid ECC_GameTraceChannel2

UENUM(BlueprintType)
enum ETileTypes
{
	None,
	Normal,
	Obstacle,
	Occupied
};

UENUM(BlueprintType)
enum ETileStates
{
	NoState,
	Hovered,
	Selected,
	Neighbour
};