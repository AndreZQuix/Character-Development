// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CDBasePawn.generated.h"

UCLASS()
class CHARACTERDEVELOPMENT_API ACDBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACDBasePawn();

	UPROPERTY(VisibleAnywhere)
	class UPawnMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComponent;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Jump();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBlendComplete();

	AActor* CurrentViewActor;
};
