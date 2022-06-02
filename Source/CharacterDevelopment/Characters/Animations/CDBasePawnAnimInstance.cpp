// Fill out your copyright notice in the Description page of Project Settings.


#include "CDBasePawnAnimInstance.h"
#include "../CDBasePawn.h"
#include "../../Components/MovementComponents/CDPawnMovementComponent.h"

void UCDBasePawnAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<ACDBasePawn>(), TEXT("UCDBasePawnAnimInstance::NativeBeginPlay() can only work with CDBasePawn class"));
	CachedPawn = StaticCast<ACDBasePawn*>(TryGetPawnOwner());
}

void UCDBasePawnAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedPawn.IsValid())
	{
		return;
	}

	InputForward = CachedPawn->GetInputForward();
	InputRight = CachedPawn->GetInputRight();

	bIsInAir = CachedPawn->GetMovementComponent()->IsFalling();
}