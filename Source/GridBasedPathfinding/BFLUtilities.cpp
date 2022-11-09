// Fill out your copyright notice in the Description page of Project Settings.


#include "BFLUtilities.h"
#include "Kismet/KismetMathLibrary.h"

FVector UBFLUtilities::SnapVectors(FVector v1, FVector v2)
{
	FVector SnappedVector;
	SnappedVector.X = UKismetMathLibrary::GridSnap_Float(v1.X, v2.X);
	SnappedVector.Y = UKismetMathLibrary::GridSnap_Float(v1.Y, v2.Y);
	SnappedVector.Z = UKismetMathLibrary::GridSnap_Float(v1.Z, v2.Z);

	return SnappedVector;
}

bool UBFLUtilities::IsFloatEven(float testfloat)
{
	double result;
	UKismetMathLibrary::FMod64(testfloat, 2.0f, result);

	if(result == 0)
		return true;

	return  false;
}
