// Fill out your copyright notice in the Description page of Project Settings.


#include "CDPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

ACDPlayerCharacter::ACDPlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = 1;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
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

bool ACDPlayerCharacter::CanJumpInternal_Implementation() const
{
	ACharacter* DefaultCharacter = GetClass()->GetDefaultObject<ACharacter>();	// get standing capsule collision to calculate it's half height
	const FCollisionShape StandingCapsuleShape = DefaultCharacter->GetCapsuleComponent()->GetCollisionShape();
	float CapsuleRadius, CapsuleHalfHeight;
	GetCapsuleComponent()->GetScaledCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	const FVector CharacterLocation = GetCharacterMovement()->UpdatedComponent->GetComponentLocation();
	const float SweepInflation = KINDA_SMALL_NUMBER * 10.0f;
	float MinFloorDistHalf = UCharacterMovementComponent::MIN_FLOOR_DIST / 2.0f;
	float NewZ = CharacterLocation.Z - CapsuleHalfHeight + StandingCapsuleShape.Capsule.HalfHeight;	// get new Z at the point of character location with an error
	NewZ += SweepInflation + MinFloorDistHalf;

	const FVector NewLocation = FVector(CharacterLocation.X, CharacterLocation.Y, NewZ);
	FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(CrouchTrace), false, this);
	FCollisionResponseParams ResponseParam;
	ECollisionChannel CollisionChannel = GetCharacterMovement()->UpdatedComponent->GetCollisionObjectType();

	bool bEncroached = GetWorld()->OverlapBlockingTestByChannel(NewLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);	// raycast test if ray overlaps with object on standing capsule height

	if (!bEncroached && bIsCrouched)
	{
		return true;
	}

	return Super::CanJumpInternal_Implementation();
}

void ACDPlayerCharacter::OnJumped_Implementation()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
}
