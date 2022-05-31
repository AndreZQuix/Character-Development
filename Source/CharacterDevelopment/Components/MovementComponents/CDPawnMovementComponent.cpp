// Fill out your copyright notice in the Description page of Project Settings.


#include "CDPawnMovementComponent.h"

void UCDPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))	// optimization
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PendingInput = GetPendingInputVector().GetClampedToMaxSize(1.0f);
	Velocity = PendingInput * MaxSpeed;
	ConsumeInputVector();	// adjust velocity (prevent from eternal summing with previous inputs)

	if (bEnableGravity)
	{
		FHitResult HitResult;
		FVector StartPoint = UpdatedComponent->GetComponentLocation();
		float LineTraceLength = 50.0f + GetGravityZ() * DeltaTime;
		FVector EndPoint = StartPoint - LineTraceLength * FVector::UpVector;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		bool bWasFalling = bIsFalling;	// cache previous state
		bIsFalling = !GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, CollisionParams);
		if (bIsFalling)	// if pawn is falling..
		{
			VerticalVelocity += GetGravityZ() * FVector::UpVector * DeltaTime;	// ..increase velocity
			Velocity += VerticalVelocity;
		}
		else if(bWasFalling)
		{
			VerticalVelocity = FVector::ZeroVector;	// prevent from eternal velocity increasing
		}
	}

	FVector Delta = Velocity * DeltaTime;
	if (!Delta.IsNearlyZero(1e-6f))
	{
		FQuat Rot = UpdatedComponent->GetComponentQuat();
		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rot, true, Hit);	// surface sliding (from UFloatingPawnMovement)

		if (Hit.IsValidBlockingHit())
		{
			HandleImpact(Hit, DeltaTime, Delta);
			SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		}
	}
	UpdateComponentVelocity();
}
