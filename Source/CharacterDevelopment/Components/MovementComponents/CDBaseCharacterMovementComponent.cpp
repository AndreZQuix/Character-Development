// Fill out your copyright notice in the Description page of Project Settings.


#include "CDBaseCharacterMovementComponent.h"
#include "../../Characters/CDBaseCharacter.h"
#include "Components/CapsuleComponent.h"

float UCDBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		Result = SprintSpeed;
	}

	if (bIsOutOfStamina)
	{
		Result = OutOfStaminaSpeed;
	}

	if (bIsProned)
	{
		Result = MaxProneSpeed;
	}

	return Result;
}

void UCDBaseCharacterMovementComponent::StartSprint()
{
	bIsSprinting = true;
	bForceMaxAccel = 1;
}

void UCDBaseCharacterMovementComponent::StopSprint()
{
	bIsSprinting = false;
	bForceMaxAccel = 0;
}

void UCDBaseCharacterMovementComponent::SetIsProning(bool bIsProne_In)
{
	bIsProned = bIsProne_In;
}

bool UCDBaseCharacterMovementComponent::CanProneInCurrentState() const
{
	return IsMovingOnGround() && UpdatedComponent && !UpdatedComponent->IsSimulatingPhysics();
}

void UCDBaseCharacterMovementComponent::Prone()
{
	if (!CanProneInCurrentState())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Prone"));
	ACDBaseCharacter* CachedBaseCharacter = Cast<ACDBaseCharacter>(CharacterOwner);
	if (CachedBaseCharacter)
	{
		bIsProned = true;
		if (CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() == ProneCapsuleHalfHeight)
		{
			CachedBaseCharacter->OnProne(0.f, 0.f);
			return;
		}

		const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
		const float OldUnscaledHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		const float OldUnscaledRadius = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
		// Height is not allowed to be smaller than radius.
		const float ClampedPronedHalfHeight = FMath::Max3(0.f, OldUnscaledRadius, ProneCapsuleHalfHeight);
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(OldUnscaledRadius, ClampedPronedHalfHeight);
		float HalfHeightAdjust = (OldUnscaledHalfHeight - ClampedPronedHalfHeight);
		float ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;

		if (ClampedPronedHalfHeight > OldUnscaledHalfHeight)
		{
			FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(CrouchTrace), false, CharacterOwner);
			FCollisionResponseParams ResponseParam;
			InitCollisionParams(CapsuleParams, ResponseParam);
			const bool bEncroached = GetWorld()->OverlapBlockingTestByChannel(UpdatedComponent->GetComponentLocation() - FVector(0.f, 0.f, ScaledHalfHeightAdjust), FQuat::Identity,
				UpdatedComponent->GetCollisionObjectType(), GetPawnCapsuleCollisionShape(SHRINK_None), CapsuleParams, ResponseParam);

			// If encroached, cancel
			if (bEncroached)
			{
				CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(OldUnscaledRadius, OldUnscaledHalfHeight);
				return;
			}
		}

		if (MovementMode == MOVE_Walking)
		{
			UpdatedComponent->MoveComponent(FVector(0.f, 0.f, -ScaledHalfHeightAdjust), UpdatedComponent->GetComponentQuat(), true, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
		}
		bWantsToCrouch = false;
		CachedBaseCharacter->bIsCrouched = false;
		bForceNextFloorCheck = true;

		// OnProne takes the change from the Default size, not the current one (though they are usually the same).
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		HalfHeightAdjust = (DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - CrouchedHalfHeight - ClampedPronedHalfHeight);
		ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;

		AdjustProxyCapsuleSize();
		CachedBaseCharacter->OnProne(HalfHeightAdjust, ScaledHalfHeightAdjust);
	}
}

void UCDBaseCharacterMovementComponent::UnProne()
{
	UE_LOG(LogTemp, Warning, TEXT("UnProne"));
	ACDBaseCharacter* CachedBaseCharacter = Cast<ACDBaseCharacter>(CharacterOwner);
	if (CachedBaseCharacter)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();

		// See if collision is already at desired size.
		if (CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() == CrouchedHalfHeight)
		{
			CachedBaseCharacter->OnUnProne(0.f, 0.f);
			return;
		}

		const float CurrentPronedHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
		const float OldUnscaledHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		float HalfHeightAdjust = CrouchedHalfHeight - OldUnscaledHalfHeight;
		float ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;
		const FVector PawnLocation = UpdatedComponent->GetComponentLocation();

		// Grow to uncrouched size.
		check(CharacterOwner->GetCapsuleComponent());
		const UWorld* MyWorld = GetWorld();
		const float SweepInflation = KINDA_SMALL_NUMBER * 10.f;
		FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(ProneTrace), false, CharacterOwner);
		FCollisionResponseParams ResponseParam;
		InitCollisionParams(CapsuleParams, ResponseParam);

		// Compensate for the difference between current capsule size and standing size
		const FCollisionShape StandingCapsuleShape = GetPawnCapsuleCollisionShape(SHRINK_HeightCustom, -SweepInflation - ScaledHalfHeightAdjust); // Shrink by negative amount, so actually grow it.
		const ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();
		bool bEncroached = true;

		if (MovementMode != MOVE_Walking)
		{
			// Expand in place
			bEncroached = MyWorld->OverlapBlockingTestByChannel(PawnLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);

			if (bEncroached)
			{
				// Try adjusting capsule position to see if we can avoid encroachment.
				if (ScaledHalfHeightAdjust > 0.f)
				{
					// Shrink to a short capsule, sweep down to base to find where that would hit something, and then try to stand up from there.
					float PawnRadius, PawnHalfHeight;
					CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleSize(PawnRadius, PawnHalfHeight);
					const float ShrinkHalfHeight = PawnHalfHeight - PawnRadius;
					const float TraceDist = PawnHalfHeight - ShrinkHalfHeight;
					const FVector Down = FVector(0.f, 0.f, -TraceDist);

					FHitResult Hit(1.f);
					const FCollisionShape ShortCapsuleShape = GetPawnCapsuleCollisionShape(SHRINK_HeightCustom, ShrinkHalfHeight);
					const bool bBlockingHit = MyWorld->SweepSingleByChannel(Hit, PawnLocation, PawnLocation + Down, FQuat::Identity, CollisionChannel, ShortCapsuleShape, CapsuleParams);
					if (Hit.bStartPenetrating)
					{
						bEncroached = true;
					}
					else
					{
						// Compute where the base of the sweep ended up, and see if we can stand there
						const float DistanceToBase = (Hit.Time * TraceDist) + ShortCapsuleShape.Capsule.HalfHeight;
						const FVector NewLoc = FVector(PawnLocation.X, PawnLocation.Y, PawnLocation.Z - DistanceToBase + StandingCapsuleShape.Capsule.HalfHeight + SweepInflation + MIN_FLOOR_DIST / 2.f);
						bEncroached = MyWorld->OverlapBlockingTestByChannel(NewLoc, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);
						if (!bEncroached)
						{
							// Intentionally not using MoveUpdatedComponent, where a horizontal plane constraint would prevent the base of the capsule from staying at the same spot.
							UpdatedComponent->MoveComponent(NewLoc - PawnLocation, UpdatedComponent->GetComponentQuat(), false, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
						}
					}
				}
			}
		}
		else
		{
			// Expand while keeping base location the same.
			FVector StandingLocation = PawnLocation + FVector(0.f, 0.f, StandingCapsuleShape.GetCapsuleHalfHeight() - CurrentPronedHalfHeight);
			bEncroached = MyWorld->OverlapBlockingTestByChannel(StandingLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);

			if (bEncroached)
			{
				if (IsMovingOnGround())
				{
					// Something might be just barely overhead, try moving down closer to the floor to avoid it.
					const float MinFloorDist = KINDA_SMALL_NUMBER * 10.f;
					if (CurrentFloor.bBlockingHit && CurrentFloor.FloorDist > MinFloorDist)
					{
						StandingLocation.Z -= CurrentFloor.FloorDist - MinFloorDist;
						bEncroached = MyWorld->OverlapBlockingTestByChannel(StandingLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);
					}
				}
			}

			if (!bEncroached)
			{
				// Commit the change in location.
				UpdatedComponent->MoveComponent(StandingLocation - PawnLocation, UpdatedComponent->GetComponentQuat(), false, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
				bForceNextFloorCheck = true;
			}

			// If still encroached then abort.
			if (bEncroached)
			{
				return;
			}

			bIsProned = false;
			CharacterOwner->bIsCrouched = false;
		}

		// Now call SetCapsuleSize() to cause touch/untouch events and actually grow the capsule
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), CrouchedHalfHeight, true);
		HalfHeightAdjust = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - CrouchedHalfHeight - ProneCapsuleHalfHeight;
		ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;
		AdjustProxyCapsuleSize();
		CachedBaseCharacter->OnUnProne(HalfHeightAdjust, ScaledHalfHeightAdjust);
	}
}