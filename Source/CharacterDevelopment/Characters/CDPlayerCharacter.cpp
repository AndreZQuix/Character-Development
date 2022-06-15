// Fill out your copyright notice in the Description page of Project Settings.


#include "CDPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ACDPlayerCharacter::ACDPlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ACDPlayerCharacter::MoveForward(float Value)
{
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
	}
}

void ACDPlayerCharacter::MoveRight(float Value)
{
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightdVector = YawRotator.RotateVector(FVector::RightVector);
		AddMovementInput(RightdVector, Value);
	}
}

void ACDPlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ACDPlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ACDPlayerCharacter::Jump()
{

}
