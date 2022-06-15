// Fill out your copyright notice in the Description page of Project Settings.


#include "CDPlayerController.h"
#include "../CDBaseCharacter.h"

void ACDPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<ACDBaseCharacter>(InPawn);
}

void ACDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ACDPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACDPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ACDPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ACDPlayerController::LookUp);
	InputComponent->BindAxis("TurnAtRate", this, &ACDPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUpAtRate", this, &ACDPlayerController::LookUpAtRate);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACDPlayerController::Jump);
}

void ACDPlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}

void ACDPlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}

void ACDPlayerController::Turn(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(Value);
	}
}

void ACDPlayerController::TurnAtRate(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->TurnAtRate(Value);
	}
}

void ACDPlayerController::LookUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(Value);
	}
}

void ACDPlayerController::LookUpAtRate(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUpAtRate(Value);
	}
}

void ACDPlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}
