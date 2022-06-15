// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CDBaseCharacter.generated.h"

UCLASS(Abstract, NotBlueprintable)
class CHARACTERDEVELOPMENT_API ACDBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};
	virtual void TurnAtRate(float Value) {};
	virtual void LookUpAtRate(float Value) {};

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Control")
	float BaseTurnRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Control")
	float BaseLookUpRate = 45.0f;
};
