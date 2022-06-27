// Fill out your copyright notice in the Description page of Project Settings.


#include "CDPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACDPlayerCharacter::ACDPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	DefaultSpringArmLength = SpringArmComponent->TargetArmLength;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = 1;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

void ACDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (SpringArmSprintCurve)
	{
		FOnTimelineFloatStatic SprintProgressFunction;	// for camera adjusting
		SprintProgressFunction.BindUObject(this, &ACDPlayerCharacter::HandleSpringArm);
		SpringArmTimeline.AddInterpFloat(SpringArmSprintCurve, SprintProgressFunction);
	}
}

void ACDPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpringArmTimeline.TickTimeline(DeltaTime);
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

void ACDPlayerCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACDPlayerCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACDPlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	SpringArmComponent->TargetOffset += FVector(0.0f, 0.0f, HalfHeightAdjust);
}

void ACDPlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	SpringArmComponent->TargetOffset -= FVector(0.0f, 0.0f, HalfHeightAdjust);
}

//bool ACDPlayerCharacter::CanJumpInternal_Implementation() const
//{
//	if (CanStandUpWhileCrouch() && bIsCrouched)
//	{
//		return true;
//	}
//
//	return Super::CanJumpInternal_Implementation();
//}

void ACDPlayerCharacter::OnJumped_Implementation()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void ACDPlayerCharacter::OnSprintStart_Implementation()
{
	SpringArmTimeline.PlayFromStart();
}

void ACDPlayerCharacter::OnSprintEnd_Implementation()
{
	SpringArmTimeline.Reverse();
}

void ACDPlayerCharacter::HandleSpringArm(float Value)
{
	float Result = FMath::Lerp(DefaultSpringArmLength, SpringArmLength, Value);
	SpringArmComponent->TargetArmLength = Result;
}
