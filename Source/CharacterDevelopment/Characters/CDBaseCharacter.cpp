// Fill out your copyright notice in the Description page of Project Settings.


#include "CDBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/MovementComponents/CDBaseCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ACDBaseCharacter::ACDBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCDBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CDBaseCharacterMovementComponent = StaticCast<UCDBaseCharacterMovementComponent*>(GetCharacterMovement());
	CDBaseCharacterMovementComponent->bCanWalkOffLedgesWhenCrouching = 1;
}

void ACDBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentStamina = MaxStamina;
}

void ACDBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateIKSettings(DeltaTime);
	TryChangeSprintState();
	UpdateStamina(DeltaTime);
}

void ACDBaseCharacter::UpdateIKSettings(float DeltaTime)
{
	IKLeftFootOffset = FMath::FInterpTo(IKLeftFootOffset, GetIKOffsetForASocket(LeftFootSocketName), DeltaTime, IKInterpSpeed);
	IKRightFootOffset = FMath::FInterpTo(IKRightFootOffset, GetIKOffsetForASocket(RightFootSocketName), DeltaTime, IKInterpSpeed);
	IKPelvisOffset = FMath::FInterpTo(IKPelvisOffset, GetIKPelvisOffset(), DeltaTime, IKInterpSpeed);
}

void ACDBaseCharacter::ChangeCrouchState()
{
	if (!CDBaseCharacterMovementComponent->IsCrouching() && !CDBaseCharacterMovementComponent->IsSprinting() && !CDBaseCharacterMovementComponent->IsProning())
	{
		Crouch();
	}
	else if (CDBaseCharacterMovementComponent->IsCrouching() && !CDBaseCharacterMovementComponent->IsProning())
	{
		UnCrouch();
	}
}

void ACDBaseCharacter::ChangeProneState()
{
	if (!CDBaseCharacterMovementComponent->IsProning())
	{
		CDBaseCharacterMovementComponent->Prone();
	}
	else if (CDBaseCharacterMovementComponent->IsProning() && !CDBaseCharacterMovementComponent->IsCrouching())
	{
		CDBaseCharacterMovementComponent->UnProne();
	}
}

void ACDBaseCharacter::OnProne(float HeightAdjust, float ScaledHeightAdjust)
{
	RecalculateBaseEyeHeight();

	K2_OnProne(HeightAdjust, ScaledHeightAdjust);
}

void ACDBaseCharacter::OnUnProne(float HeightAdjust, float ScaledHeightAdjust)
{

	RecalculateBaseEyeHeight();

	const ACharacter* DefaultChar = GetDefault<ACharacter>(GetClass());
	const float HeightDifference = CDBaseCharacterMovementComponent->CrouchedHalfHeight - CDBaseCharacterMovementComponent->ProneCapsuleHalfHeight;
	if (GetMesh() && DefaultChar->GetMesh())
	{
		FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
		MeshRelativeLocation.Z = DefaultChar->GetMesh()->GetRelativeLocation().Z + HeightDifference;
		BaseTranslationOffset.Z = MeshRelativeLocation.Z;
	}
	else
	{
		BaseTranslationOffset.Z = DefaultChar->GetBaseTranslationOffset().Z + HeightDifference;
	}

	if (!CDBaseCharacterMovementComponent->IsProning())
	{
		Crouch();
	}

	K2_OnUnProne(HeightAdjust, ScaledHeightAdjust);
}

void ACDBaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	if (bIsCrouched)
	{
		ChangeCrouchState();
	}
}

void ACDBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

void ACDBaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !CDBaseCharacterMovementComponent->IsOutOfStamina() && !CDBaseCharacterMovementComponent->IsSprinting() && CanStandUpWhileCrouch())
	{
		CDBaseCharacterMovementComponent->StartSprint();
		OnSprintStart();
	}

	if (!bIsSprintRequested && CDBaseCharacterMovementComponent->IsSprinting())
	{
		CDBaseCharacterMovementComponent->StopSprint();
		OnSprintEnd();
	}
}

void ACDBaseCharacter::UpdateStamina(float DeltaTime)
{
	if (CDBaseCharacterMovementComponent->IsSprinting())
	{
		CurrentStamina -= SprintStaminaConsumptionVelocity * DeltaTime;	// consume stamina
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
	}
	else
	{
		CurrentStamina += StaminaRestoreVelocity * DeltaTime;	// restore stamina
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
	}

	if (CurrentStamina == 0.0f) // character is exhausted
	{
		CDBaseCharacterMovementComponent->SetOutOfStamina(true);
		StopSprint();
	}

	if (CDBaseCharacterMovementComponent->IsOutOfStamina() && CurrentStamina == MaxStamina) // character stamina is fully restored
	{
		CDBaseCharacterMovementComponent->SetOutOfStamina(false);
	}

	if (CurrentStamina < MaxStamina)	// simple ui for debugging
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Stamina: %.2f"), CurrentStamina));
		if (CDBaseCharacterMovementComponent->IsOutOfStamina())
		{
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString::Printf(TEXT("Stamina: %.2f"), CurrentStamina));
		}
	}
}

float ACDBaseCharacter::GetIKOffsetForASocket(const FName& SocketName)
{
	float Result = 0.0f;
	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
	FVector TraceStart(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z + 5.0f);
	FVector TraceEnd = TraceStart - (CapsuleHalfHeight + IKTraceDistance) * FVector::UpVector;

	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, TraceType, true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		Result = TraceStart.Z - CapsuleHalfHeight - HitResult.Location.Z;
	}

	return Result;
}

float ACDBaseCharacter::GetIKPelvisOffset()
{
	return -FMath::Abs(IKRightFootOffset - IKLeftFootOffset); // if character can't reach the surface with his leg
}

bool ACDBaseCharacter::CanStandUpWhileCrouch() const
{
	ACharacter* DefaultCharacter = GetClass()->GetDefaultObject<ACharacter>();	// get standing capsule collision to calculate it's half height
	const FCollisionShape StandingCapsuleShape = DefaultCharacter->GetCapsuleComponent()->GetCollisionShape();
	float CapsuleRadius, CapsuleHalfHeight;
	GetCapsuleComponent()->GetScaledCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	const FVector CharacterLocation = GetCharacterMovement()->UpdatedComponent->GetComponentLocation();
	const float SweepInflation = KINDA_SMALL_NUMBER * 10.0f;
	float MinFloorDistHalf = UCharacterMovementComponent::MIN_FLOOR_DIST / 2.0f;
	float NewZ = CharacterLocation.Z - CapsuleHalfHeight + StandingCapsuleShape.Capsule.HalfHeight;	// get new Z at the point of character location with an error
	NewZ += SweepInflation + MinFloorDistHalf;

	const FVector NewLocation = FVector(CharacterLocation.X, CharacterLocation.Y, NewZ);
	FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(CrouchTrace), false, this);
	FCollisionResponseParams ResponseParam;
	ECollisionChannel CollisionChannel = GetCharacterMovement()->UpdatedComponent->GetCollisionObjectType();

	bool bEncroached = !GetWorld()->OverlapBlockingTestByChannel(NewLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);	// raycast test if ray overlaps with object on standing capsule height
	return bEncroached;
}

void ACDBaseCharacter::OnSprintStart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSprintStart_Implementation"));
}

void ACDBaseCharacter::OnSprintEnd_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSprintEnd_Implementation"));
}

bool ACDBaseCharacter::CanJumpInternal_Implementation() const
{
	if (CDBaseCharacterMovementComponent->IsOutOfStamina() || CDBaseCharacterMovementComponent->IsProning())
	{
		return false;
	}

	if (bIsCrouched && CanStandUpWhileCrouch())
	{
		return true;
	}

	return Super::CanJumpInternal_Implementation();
}
