// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))//, hidecategories = ("Collision"))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	//-1 max speed downwards, 1 max speed upwards
	void Elevate(float RelativSpeed);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MaxDegreesPerSecond = 5;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MaxElevation = 40;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MinElevation = -1;
	
};