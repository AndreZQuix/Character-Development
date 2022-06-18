// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CDBaseCharacter.generated.h"

class UCDBaseCharacterMovementComponent;

UCLASS(Abstract, NotBlueprintable)
class CHARACTERDEVELOPMENT_API ACDBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACDBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};
	virtual void TurnAtRate(float Value) {};
	virtual void LookUpAtRate(float Value) {};

	virtual void ChangeCrouchState();
	virtual void StartSprint();
	virtual void StopSprint();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UCDBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() { return CDBaseCharacterMovementComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Control")
	float BaseTurnRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Control")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float SprintSpeed = 800.0f;

	virtual bool CanSprint();

	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Movement")
	void OnSprintStart();

	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Movement")
	void OnSprintEnd();

	UCDBaseCharacterMovementComponent* CDBaseCharacterMovementComponent;

private:
	bool bIsSprintRequested = false;

	void TryChangeSprintState();
};
