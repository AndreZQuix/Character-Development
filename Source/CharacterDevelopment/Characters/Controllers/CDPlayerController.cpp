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
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ACDPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Prone", EInputEvent::IE_DoubleClick, this, &ACDPlayerController::Prone);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACDPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACDPlayerController::StopSprint);
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

void ACDPlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void ACDPlayerController::Prone()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Prone();
	}
}

void ACDPlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}

void ACDPlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}