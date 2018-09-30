// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"
#include "SprungWheel.h"
#include "Components/SphereComponent.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

//void UTankTrack::ApplySidewaysFriction()
//{
//	auto SlippageSpeed = FVector::DotProduct
//	(GetComponentVelocity(), GetRightVector());
//
//	auto DeltaTime = GetWorld()->GetDeltaSeconds();
//	auto CorrectionAcceleration = -(SlippageSpeed / DeltaTime) * GetRightVector();
//
//
//	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
//													//halfed because there are two tracks
//	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2;
//	TankRoot->AddForce(CorrectionForce * SidewaysFrictionCoefficient);
//}
//
//void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, 
//	UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
//{
//	DriveTrack();
//	ApplySidewaysFriction();
//	CurrentThrottle = 0;
//}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto ForceApplied = TrackMaxDrivingForce * CurrentThrottle;

	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();
	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> ResultArray;
	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);

	for (USceneComponent* Child : Children) {
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild)continue;

		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
		if (!SprungWheel)continue;

		ResultArray.Add(SprungWheel);
	}
	return ResultArray;
}

void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, +1);
	DriveTrack(CurrentThrottle);
}


