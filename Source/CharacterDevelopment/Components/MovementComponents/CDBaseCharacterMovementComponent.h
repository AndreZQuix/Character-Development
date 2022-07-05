// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../LedgeDetectorComponent.h"
#include "CDBaseCharacterMovementComponent.generated.h"

struct FMantlingMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;
	FRotator InitialRotation = FRotator::ZeroRotator;

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;

	FVector InitialAnimationLocation = FVector::ZeroVector;

	float Duration = 1.0f;
	float StartTime = 0.0f;

	UCurveVector* MantlingCurve;
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_Max UMETA(Hidden)
};

UCLASS()
class CHARACTERDEVELOPMENT_API UCDBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsSprinting() { return bIsSprinting; }
	virtual float GetMaxSpeed() const override;
	void StartSprint();
	void StopSprint();

	void StartMantle(const FMantlingMovementParameters& MantlingParameters);
	void EndMantle();
	bool IsMantling();

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
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float OutOfStaminaSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: prone", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxProneSpeed = 150.0f;

	UPROPERTY(Category = "Character Movement: swimming", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SwimmingCapsuleRadius = 40.0f;

	UPROPERTY(Category = "Character Movement: swimming", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SwimmingCapsuleHalfHeight = 40.0f;
	
private:
	bool bIsSprinting;
	bool bIsOutOfStamina;
	bool bIsProned;

	FMantlingMovementParameters CurrentMantlingParameters;

	FTimerHandle MantlingTimer;
};
