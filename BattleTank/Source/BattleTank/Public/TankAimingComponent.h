// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"


UENUM()
enum class EFiringStatus : uint8
{
	None,
	NoAmmo,
	Reloading,
	Aiming,
	Locked
};

//Forward Declaration
class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTankAimingComponent();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void InitializeComponent(UTankBarrel* TankBarrelToSet, UTankTurret* TankTurretToSet);

	void AimAt(FVector WorldLocationToAimAt);

	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();

	EFiringStatus GetFiringStatues() const;

	UFUNCTION(BlueprintCallable, Category = Firing)
	int32 GetCurAmmo() const;

	UFUNCTION(BlueprintCallable, Category = Firing)
	int32 GetMaxAmmo() const;

	UFUNCTION(BlueprintCallable, Category = Firing)
	float GetReloadFinishedPercentage() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = State)
	EFiringStatus FiringStatus = EFiringStatus::Reloading;

	UPROPERTY(EditAnywhere, Category = Firing)
	int32 MaxAmmo = 15;

private:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsBarrelMoving();

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 10000;

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	void MoveBarrelTowards(FVector AimDirection);

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeInSeconds = 3;

	UPROPERTY(EditDefaultsOnly, Category = Aiming)
	float TargetLockedTolerance = 0.025f;

	double LastFireTime = 0;

	FVector DesiredAimDirection = FVector(0);

	int32 CurAmmo = 0;

	float ReloadFinishedPercentage = 0;
};
