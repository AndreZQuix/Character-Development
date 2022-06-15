// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CDBaseCharacter.h"
#include "CDPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CHARACTERDEVELOPMENT_API ACDPlayerCharacter : public ACDBaseCharacter
{
	GENERATED_BODY()

public:
	ACDPlayerCharacter();

	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	virtual void Turn(float Value) override;
	virtual void LookUp(float Value) override;
	virtual void Jump() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
	class USpringArmComponent* SpringArmComponent;
};