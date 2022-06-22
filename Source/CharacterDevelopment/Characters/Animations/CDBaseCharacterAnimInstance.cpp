// Fill out your copyright notice in the Description page of Project Settings.


#include "CDBaseCharacterAnimInstance.h"
#include "../CDBaseCharacter.h"
#include "../../Components/MovementComponents/CDBaseCharacterMovementComponent.h"

void UCDBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<ACDBaseCharacter>(), TEXT("UCDBaseCharacterAnimInstance::NativeBeginPlay() can only work with ACDBaseCharacter class"));
	CachedBaseCharacter = StaticCast<ACDBaseCharacter*>(TryGetPawnOwner());
}

void UCDBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	UCDBaseCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent();
	Speed = CharacterMovement->Velocity.Size();
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = CharacterMovement->IsCrouching();
	bIsSprinting = CharacterMovement->IsSprinting();
	bIsOutOfStamina = CharacterMovement->IsOutOfStamina();
	bIsProne = CharacterMovement->IsProne();

	LeftFootEffectorLocation = FVector(-(CachedBaseCharacter->GetIKLeftFootOffset() + CachedBaseCharacter->GetIKPelvisOffset()), 0.0f, 0.0f);
	RightFootEffectorLocation = FVector(CachedBaseCharacter->GetIKRightFootOffset() + CachedBaseCharacter->GetIKPelvisOffset(), 0.0f, 0.0f);
	PelvisEffectorLocation = FVector(0.0f, 0.0f, CachedBaseCharacter->GetIKPelvisOffset());
}