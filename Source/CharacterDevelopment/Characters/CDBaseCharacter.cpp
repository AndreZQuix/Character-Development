// Fill out your copyright notice in the Description page of Project Settings.


#include "CDBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/MovementComponents/CDBaseCharacterMovementComponent.h"

ACDBaseCharacter::ACDBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCDBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CDBaseCharacterMovementComponent = StaticCast< UCDBaseCharacterMovementComponent*>(GetCharacterMovement());
}

void ACDBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryChangeSprintState();
}

void ACDBaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ACDBaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void ACDBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

bool ACDBaseCharacter::CanSprint()
{
	return true;
}

void ACDBaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !CDBaseCharacterMovementComponent->IsSprinting() && CanSprint())
	{
		CDBaseCharacterMovementComponent->StartSprint();
		OnSprintStart();
	}

	if (!bIsSprintRequested && CDBaseCharacterMovementComponent->IsSprinting())
	{
		CDBaseCharacterMovementComponent->StopSprint();
		OnSprintEnd();
	}
}