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
		float TraceDepth = 1.0f;
		float SphereRadius = 50.0f;
		FVector EndPoint = StartPoint - TraceDepth * FVector::UpVector;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		bool bWasFalling = bIsFalling;	// cache previous state
		FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);
		bIsFalling = !GetWorld()->SweepSingleByChannel(HitResult, StartPoint, EndPoint, FQuat::Identity, ECC_Visibility, Sphere, CollisionParams);
		if (bIsFalling)	// if pawn is falling..
		{
			VerticalVelocity += GetGravityZ() * FVector::UpVector * DeltaTime;	// ..increase velocity
		}
		else if(bWasFalling)
		{
			VerticalVelocity = FVector::ZeroVector;	// prevent from eternal velocity increasing
		}
	}

	Velocity += VerticalVelocity;

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

void UCDPawnMovementComponent::JumpStart()
{
	if (!bIsFalling)
	{
		VerticalVelocity = InitialJumpVelocity * FVector::UpVector;
	}
}