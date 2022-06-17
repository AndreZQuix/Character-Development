// Fill out your copyright notice in the Description page of Project Settings.


#include "CDBaseCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../CDBaseCharacter.h"

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

	UCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetCharacterMovement();
	Speed = CharacterMovement->Velocity.Size();
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = CharacterMovement->IsCrouching();
}
