// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	ATank();

	virtual float TakeDamage
	(
		float DamageAmount, 
		struct FDamageEvent const & DamageEvent, 
		class AController * EventInstigator, 
		AActor* DamageCauser
	) override;

	FTankDelegate TankDeath;

	UFUNCTION(BlueprintPure, Category = Health)
	float GetHealthPercentage() const;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	int32 MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = Health)
	int32 CurrentHealth;//Initialized in begin play
};
