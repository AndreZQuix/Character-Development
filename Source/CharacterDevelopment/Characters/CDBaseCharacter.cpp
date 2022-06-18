// Fill out your copyright notice in the Description page of Project Settings.


#include "CDBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/MovementComponents/CDBaseCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

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
	else if(!CDBaseCharacterMovementComponent->IsSprinting())
	{
		Crouch();
	}
}

void ACDBaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	if (bIsCrouched)
	{
		ChangeCrouchState();
	}
}

void ACDBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

void ACDBaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !CDBaseCharacterMovementComponent->IsSprinting() && CanStandUp())
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

bool ACDBaseCharacter::CanStandUp() const
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

	bool bEncroached = !GetWorld()->OverlapBlockingTestByChannel(NewLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);	// raycast test if ray overlaps with object on standing capsule height
	return bEncroached;
}

void ACDBaseCharacter::OnSprintStart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSprintStart_Implementation"));
}

void ACDBaseCharacter::OnSprintEnd_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSprintEnd_Implementation"));
}