// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CDBasePawnAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERDEVELOPMENT_API UCDBasePawnAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base pawn animation instance")
	float InputForward;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base pawn animation instance")
	float InputRight;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base pawn animation instance")
	bool bIsInAir;

private:
	TWeakObjectPtr<class ACDBasePawn> CachedPawn;
};
