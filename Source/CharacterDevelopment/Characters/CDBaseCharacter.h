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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetIKLeftFootOffset() const { return IKLeftFootOffset; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetIKRightFootOffset() const { return IKRightFootOffset; }

	float GetIKPelvisOffset();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Control")
	float BaseTurnRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Control")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float SprintSpeed = 800.0f;

	bool CanStandUp() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintStart();
	virtual void OnSprintStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintEnd();
	virtual void OnSprintEnd_Implementation();

	UCDBaseCharacterMovementComponent* CDBaseCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | IK settings")
	FName LeftFootSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | IK settings")
	FName RightFootSocketName;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Character | IK settings", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float IKInterpSpeed = 20.0f;

private:
	bool bIsSprintRequested = false;

	void TryChangeSprintState();

	float GetIKOffsetForASocket(const FName& SocketName);

	void UpdateIKSettings(float DeltaTime);

	float IKLeftFootOffset = 0.0f;
	float IKRightFootOffset = 0.0f;
	float IKPelvisOffset = 0.0f;

	float IKTraceDistance = 50.0f;
};
