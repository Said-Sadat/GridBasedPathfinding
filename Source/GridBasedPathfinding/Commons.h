#pragma once

#include "CoreMinimal.h"
#include "Commons.generated.h"

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