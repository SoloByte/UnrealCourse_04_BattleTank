// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleTank/Public/Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATank* GetControlledTank() const;
	
protected:
	virtual void BeginPlay() override;
	
	
};