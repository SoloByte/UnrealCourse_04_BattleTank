// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

 //Forward Declaration
class UTankAimingComponent;

/**
 *
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float) override;

	UPROPERTY(EditDefaultsOnly, Category = Pathfinding)
	float AcceptanceRadius = 10000;
};
