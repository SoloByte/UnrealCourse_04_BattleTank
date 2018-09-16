// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::TickComponent
(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ApplySidewaysFriction(DeltaTime);
}

void UTankTrack::ApplySidewaysFriction(float DeltaTime)
{
	auto SlippageSpeed = FVector::DotProduct
	(GetComponentVelocity(), GetRightVector());

	auto CorrectionAcceleration = -(SlippageSpeed / DeltaTime) * GetRightVector();


	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
													//halfed because there are two tracks
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2;
	TankRoot->AddForce(CorrectionForce * SidewaysFrictionCoefficient);
}

void UTankTrack::SetThrottle(float Throttle)
{
	Throttle = FMath::Clamp<float>(Throttle, -1, +1);

	auto ForceApplied = GetForwardVector() * TrackMaxDrivingForce * Throttle;
	auto ForceLocation = GetComponentLocation();

	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);

	/*auto name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s Throttle: %s"), *name, *ForceApplied.ToString());*/
}


