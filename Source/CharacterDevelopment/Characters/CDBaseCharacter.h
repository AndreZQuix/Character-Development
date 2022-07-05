// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CDBaseCharacter.generated.h"

USTRUCT(BlueprintType)
struct FMantlingSettings
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MantlingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* MantlingCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionXY = 65.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionZ = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeightStartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeightStartTime = 0.5f;
};

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
	virtual void ChangeProneState();

	virtual void StartSprint();
	virtual void StopSprint();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SwimForward(float Value) {};
	virtual void SwimRight(float Value) {};
	virtual void SwimUp(float Value) {};

	virtual void Mantle();

	FORCEINLINE UCDBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() { return CDBaseCharacterMovementComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetIKLeftFootOffset() const { return IKLeftFootOffset; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetIKRightFootOffset() const { return IKRightFootOffset; }

	float GetIKPelvisOffset();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetPronePelvisOffset() const { return PronePelvisOffset; }

	bool CanStandUpWhileCrouch() const;

	virtual void OnProne(float HeightAdjust, float ScaledHeightAdjust);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnProne", ScriptName = "OnProne"))
	void K2_OnProne(float HeightAdjust, float ScaledHeightAdjust);

	virtual void OnUnProne(float HeightAdjust, float ScaledHeightAdjust);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUnProne", ScriptName = "OnUnProne"))
	void K2_OnUnProne(float HeightAdjust, float ScaledHeightAdjust);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Control")
	float BaseTurnRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Control")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float SprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement | Sprint")
	float MaxStamina = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement | Sprint")
	float StaminaRestoreVelocity = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement | Sprint")
	float SprintStaminaConsumptionVelocity = 5.0f;

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintStart();
	virtual void OnSprintStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintEnd();
	virtual void OnSprintEnd_Implementation();

	virtual bool CanJumpInternal_Implementation() const override;

	UCDBaseCharacterMovementComponent* CDBaseCharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	class ULedgeDetectorComponent* LedgeDetectorComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | IK settings")
	FName LeftFootSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | IK settings")
	FName RightFootSocketName;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Character | IK settings", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float IKInterpSpeed = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings HighMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings LowMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float LowMantleMaxHeight = 125.0f;

private:
	void TryChangeSprintState();
	bool bIsSprintRequested = false;

	void UpdateStamina(float DeltaTime);
	float CurrentStamina = 0.0f;

	const FMantlingSettings& GetMantlingSettings(float LedgeHeight);

	float GetIKOffsetForASocket(const FName& SocketName);
	void UpdateIKSettings(float DeltaTime);

	float IKLeftFootOffset = 0.0f;
	float IKRightFootOffset = 0.0f;
	float IKPelvisOffset = 0.0f;
	float IKTraceDistance = 50.0f;

	float PronePelvisOffset = -60.0f;

};
