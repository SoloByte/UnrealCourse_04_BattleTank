// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheelConstraint"));
	SetRootComponent(MassWheelConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(MassWheelConstraint);

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle);

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
	AxleWheelConstraint->SetupAttachment(Axle);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	SetUpConstraint();
	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);
}

void ASprungWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	ApplyForce();
}

void ASprungWheel::SetUpConstraint()
{
	UE_LOG(LogTemp, Warning, TEXT("Parent: %s"), *GetAttachParentActor()->GetName());
	if (!GetAttachParentActor()) return;

	UPrimitiveComponent* TankBody =
		Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
	UE_LOG(LogTemp, Warning, TEXT("TankBody: %s"), *TankBody->GetName());
	if (!TankBody) return;

	MassWheelConstraint->SetConstrainedComponents
	(
		TankBody,
		NAME_None,
		Axle,
		NAME_None
	);

	AxleWheelConstraint->SetConstrainedComponents
	(
		Axle,
		NAME_None,
		Wheel,
		NAME_None
	);

	UE_LOG(LogTemp, Warning, TEXT("Axle: %s"), *Axle->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Wheel: %s"), *Wheel->GetName());
}

void ASprungWheel::ApplyForce()
{
	Wheel->AddForce(Axle->GetForwardVector() * TotalForceMagnitudeThisFrame);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->TickGroup == TG_PostPhysics) {
		TotalForceMagnitudeThisFrame = 0;
	}
}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}



