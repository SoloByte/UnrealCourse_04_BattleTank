// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GetControlledTank();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	ATank* tank = Cast<ATank>(GetPawn());
	if (tank) { UE_LOG(LogTemp, Warning, TEXT("PlayerController possesed Tank Pawn: %s"), *(tank->GetName())); }
	else { UE_LOG(LogTemp, Warning, TEXT("PlayerController not possesing a tank pawn!!!")); }
	return tank;
}
