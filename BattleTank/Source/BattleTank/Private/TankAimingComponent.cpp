// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include"Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Engine/World.h"
#include "Projectile.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	//every tank needs to wait for initial reload before firing
	LastFireTime = GetWorld()->GetTimeSeconds();
}

void UTankAimingComponent::InitializeComponent
(UTankBarrel * TankBarrelToSet, UTankTurret * TankTurretToSet)
{
	Turret = TankTurretToSet;
	Barrel = TankBarrelToSet;
}

// Called every frame
void UTankAimingComponent::TickComponent
(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if ((GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTimeInSeconds){

		FiringStatus = EFiringStatus::Reloading;
	}
	else if (IsBarrelMoving()) {
		FiringStatus = EFiringStatus::Aiming;
	}
	else {
		FiringStatus = EFiringStatus::Locked;
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	return !Barrel->GetForwardVector().Equals(DesiredAimDirection, TargetLockedTolerance);
}

void UTankAimingComponent::AimAt(FVector WorldLocationToAimAt)
{
	if (!ensure(Barrel)) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("BarrelEnd"));

	bool bHaveAimSolution =
		UGameplayStatics::SuggestProjectileVelocity(
			this,
			OutLaunchVelocity,
			StartLocation,
			WorldLocationToAimAt,
			LaunchSpeed,
			false,
			0,
			0,
			ESuggestProjVelocityTraceOption::DoNotTrace
		);

	if (bHaveAimSolution)
	{
		DesiredAimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(DesiredAimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel && Turret)) { return; }

	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimToRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimToRotator - BarrelRotator;

	Turret->Rotate(DeltaRotator.Yaw);
	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::Fire()
{
	if (FiringStatus != EFiringStatus::Reloading)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }

		LastFireTime = GetWorld()->GetTimeSeconds();

		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation("BarrelEnd"),
			Barrel->GetSocketRotation("BarrelEnd")
			);

		Projectile->LaunchProjectile(LaunchSpeed);
	}
}

