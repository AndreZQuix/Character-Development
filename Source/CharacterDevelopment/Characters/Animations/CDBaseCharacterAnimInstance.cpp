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
}
