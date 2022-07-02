// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CDBaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "CDPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CHARACTERDEVELOPMENT_API ACDPlayerCharacter : public ACDBaseCharacter
{
	GENERATED_BODY()

public:
	ACDPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	virtual void Turn(float Value) override;
	virtual void LookUp(float Value) override;
	virtual void TurnAtRate(float Value) override;
	virtual void LookUpAtRate(float Value) override;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;	// adjust camera socket
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnJumped_Implementation() override;

	void OnSprintStart_Implementation() override;
	void OnSprintEnd_Implementation() override;

	virtual void OnProne(float HeightAdjust, float ScaledHeightAdjust) override;

	virtual void OnUnProne(float HeightAdjust, float ScaledHeightAdjust) override;

	UPROPERTY(EditAnywhere, Category = "Character | Camera")
	UCurveFloat* SpringArmSprintCurve;

	UPROPERTY(EditAnywhere, Category = "Character | Camera", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SpringArmLength = 400.0f;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
	class USpringArmComponent* SpringArmComponent;

private:
	FTimeline SpringArmTimeline;
	float DefaultSpringArmLength;

	void HandleSpringArm(float Value); // timeline function
};
