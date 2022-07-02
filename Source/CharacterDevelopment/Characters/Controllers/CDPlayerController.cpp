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
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ACDPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Prone", EInputEvent::IE_DoubleClick, this, &ACDPlayerController::ChangeProneState);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACDPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACDPlayerController::StopSprint);
	InputComponent->BindAction("Mantle", EInputEvent::IE_Pressed, this, &ACDPlayerController::Mantle);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACDPlayerController::Jump);
	InputComponent->BindAxis("SwimForward", this, &ACDPlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &ACDPlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &ACDPlayerController::SwimUp);
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

void ACDPlayerController::ChangeProneState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeProneState();
	}
}

void ACDPlayerController::Mantle()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Mantle();
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

void ACDPlayerController::SwimForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimForward(Value);
	}
}

void ACDPlayerController::SwimRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimRight(Value);
	}
}

void ACDPlayerController::SwimUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimUp(Value);
	}
}
