// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CDBaseCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERDEVELOPMENT_API UCDBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsSprinting() { return bIsSprinting; }
	virtual float GetMaxSpeed() const override;
	void StartSprint();
	void StopSprint();

	FORCEINLINE bool IsOutOfStamina() const { return bIsOutOfStamina; }
	void SetOutOfStamina(bool bIsOutOfStamina_In)
	{
		bIsOutOfStamina = bIsOutOfStamina_In;
	}

	FORCEINLINE bool IsProning() const { return bIsProned; }
	void SetIsProning(bool bIsProning_In);
	virtual void Prone();
	virtual void UnProne();

	bool CanProneInCurrentState() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character movement: prone")
	float ProneCapsuleRadius = 40.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character movement: prone")
	float ProneCapsuleHalfHeight = 40.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float OutOfStaminaSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: prone", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxProneSpeed = 150.0f;
	
private:
	bool bIsSprinting;
	bool bIsOutOfStamina;
	bool bIsProned;
};
