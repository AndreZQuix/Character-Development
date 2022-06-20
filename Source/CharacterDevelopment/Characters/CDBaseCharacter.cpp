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

	//IKScale = GetActorScale3D().Z;
	//IKTraceDistance *= IKScale;
}

void ACDBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateIKSettings(DeltaTime);
	TryChangeSprintState();
}

void ACDBaseCharacter::UpdateIKSettings(float DeltaTime)
{
	IKLeftFootOffset = FMath::FInterpTo(IKLeftFootOffset, GetIKOffsetForASocket(LeftFootSocketName), DeltaTime, IKInterpSpeed);
	IKRightFootOffset = FMath::FInterpTo(IKRightFootOffset, GetIKOffsetForASocket(RightFootSocketName), DeltaTime, IKInterpSpeed);
	IKPelvisOffset = FMath::FInterpTo(IKPelvisOffset, GetIKPelvisOffset(), DeltaTime, IKInterpSpeed);
}

void ACDBaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else if(!CDBaseCharacterMovementComponent->IsSprinting())
	{
		Crouch();
	}
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
	if (bIsSprintRequested && !CDBaseCharacterMovementComponent->IsSprinting() && CanStandUp())
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

float ACDBaseCharacter::GetIKOffsetForASocket(const FName& SocketName)
{
	float Result = 0.0f;
	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
	FVector TraceStart(SocketLocation.X + 5.0f, SocketLocation.Y - 5.0f, GetActorLocation().Z + 5.0f);
	FVector TraceEnd = TraceStart - (CapsuleHalfHeight + IKTraceDistance) * FVector::UpVector;

	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	const FVector FootSizeBox = FVector(2.5f, 15.0f, 6.0f);
	if (UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEnd, FootSizeBox, GetMesh()->GetSocketRotation(SocketName), TraceType, true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true)) // shape cast
	{
		Result = TraceStart.Z - CapsuleHalfHeight - HitResult.Location.Z;
	}

	return Result;
}

float ACDBaseCharacter::GetIKPelvisOffset()
{
	return -FMath::Abs(IKRightFootOffset - IKLeftFootOffset); // if character can't reach the surface with his leg
}

bool ACDBaseCharacter::CanStandUp() const
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