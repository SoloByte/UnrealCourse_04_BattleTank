// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

class ASprungWheel;
/**
 * TankTrack is used to set maximum driving force, and to apply forces to the tank
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);
	
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float TrackMaxDrivingForce = 55000000;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float SidewaysFrictionCoefficient = 1;

private:
	UTankTrack();

	//void ApplySidewaysFriction();

	void DriveTrack(float CurrentThrottle);

	TArray<ASprungWheel*>GetWheels() const;
	
};
