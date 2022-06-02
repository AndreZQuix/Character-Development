// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ASpiderPawn::ASpiderPawn()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Spider mesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	
	IKScale = GetActorScale3D().Z;
	IKTraceDistance = CollisionSphereRadius * IKScale;
}

void ASpiderPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	IKRightFrontFootOffset = GetIKOffsetForASocket(RightFrontFootSocketName);
	IKRightRearFootOffset = GetIKOffsetForASocket(RightRearFootSocketName);
	IKLeftFrontFootOffset = GetIKOffsetForASocket(LeftFrontFootSocketName);
	IKLeftRearFootOffset = GetIKOffsetForASocket(LeftRearFootSocketName);

}

float ASpiderPawn::GetIKOffsetForASocket(const FName& SocketName)
{
	float Result = 0.0f; 

	FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(SocketName);
	FVector TraceStart(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z); // ray cast from socket
	FVector TraceEnd = TraceStart - IKTraceDistance * FVector::UpVector;

	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, TraceType, true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true))	// if ray cast is successful
	{
		Result = (TraceEnd.Z - HitResult.Location.Z) / IKScale;	// get the distance between hit result and trace end (floor is above the foot)
	}
	else if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceEnd, TraceEnd - IKTraceExtendDistance * FVector::UpVector, TraceType, true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		Result = (TraceEnd.Z - HitResult.Location.Z) / IKScale;	// get the distance between hit result and trace end (floor is below the foot)
	}

	return Result;
}
