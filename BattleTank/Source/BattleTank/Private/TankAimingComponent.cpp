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
	CurAmmo = MaxAmmo;
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
	
	auto CurReloadTime = GetWorld()->GetTimeSeconds() - LastFireTime;

	if (CurAmmo <= 0) {
		FiringStatus = EFiringStatus::NoAmmo;
		ReloadFinishedPercentage = 0;
	}
	else if (CurReloadTime < ReloadTimeInSeconds){
		FiringStatus = EFiringStatus::Reloading;
		ReloadFinishedPercentage =
			FMath::Clamp<float>(CurReloadTime / ReloadTimeInSeconds, 0, 1);
	}
	else if (IsBarrelMoving()) {
		FiringStatus = EFiringStatus::Aiming;
	}
	else {
		FiringStatus = EFiringStatus::Locked;
	}
}


int32 UTankAimingComponent::GetMaxAmmo() const
{
	return MaxAmmo;
}

int32 UTankAimingComponent::GetCurAmmo() const
{
	return CurAmmo;
}

float UTankAimingComponent::GetReloadFinishedPercentage() const
{
	return ReloadFinishedPercentage;
}

EFiringStatus UTankAimingComponent::GetFiringStatues() const
{
	return FiringStatus;
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

	//So the turret always rotates the shortest way
	if (FMath::Abs(DeltaRotator.Yaw) < 180) {
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else {
		Turret->Rotate(-DeltaRotator.Yaw);
	}
	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::Fire()
{
	if (FiringStatus == EFiringStatus::Locked || FiringStatus == EFiringStatus::Aiming)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }

		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation("BarrelEnd"),
			Barrel->GetSocketRotation("BarrelEnd")
			);

		Projectile->LaunchProjectile(LaunchSpeed);

		LastFireTime = GetWorld()->GetTimeSeconds();
		FiringStatus = EFiringStatus::Reloading;

		CurAmmo--;
		CurAmmo = FMath::Clamp<int>(CurAmmo, 0, MaxAmmo);
	}
}



