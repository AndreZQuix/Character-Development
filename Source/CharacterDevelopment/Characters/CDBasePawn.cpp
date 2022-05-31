// Fill out your copyright notice in the Description page of Project Settings.


#include "CDBasePawn.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"
#include "../Components/MovementComponents/CDPawnMovementComponent.h"

// Sets default values
ACDBasePawn::ACDBasePawn()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->SetSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UCDPawnMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);
}

// Called to bind functionality to input
void ACDBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACDBasePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACDBasePawn::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACDBasePawn::Jump);
}

void ACDBasePawn::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACDBasePawn::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACDBasePawn::Jump()
{
	checkf(MovementComponent->IsA<UCDPawnMovementComponent>(), TEXT("Jump can only work with UCDPawnMovementComponent"));
	UCDPawnMovementComponent* BaseMovement = StaticCast<UCDPawnMovementComponent*>(MovementComponent);
	BaseMovement->JumpStart();
}

