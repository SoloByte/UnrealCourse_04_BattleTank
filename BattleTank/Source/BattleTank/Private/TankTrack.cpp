// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
	Throttle = FMath::Clamp<float>(Throttle, -1, +1);

	auto ForceApplied = GetForwardVector() * TrackMaxDrivingForce * Throttle;
	auto ForceLocation = GetComponentLocation();

	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);

	auto name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s Throttle: %s"), *name, *ForceApplied.ToString());
}

