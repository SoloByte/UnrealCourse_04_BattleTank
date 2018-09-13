// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "BattleTank/Public/Tank.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATank* PlayerTank = GetPlayerTank();
	ATank* ControlledTank = GetControlledTank();
	
	if (ControlledTank && PlayerTank) {
		ControlledTank->AimAt(PlayerTank->GetActorLocation());
	}
}

ATank* ATankAIController::GetControlledTank() const
{
	ATank* AITank = Cast<ATank>(GetPawn());
	if (!AITank) { UE_LOG(LogTemp, Warning, TEXT("AIController %s not possesing a tank pawn!!!"), *(GetName())); }
	return AITank;
}

ATank* ATankAIController::GetPlayerTank() const
{
	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayerTank)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("AI Tank did not find any Player Tank in the world!!!")); 
		return nullptr;
	}
	return Cast<ATank>(PlayerTank);
}
