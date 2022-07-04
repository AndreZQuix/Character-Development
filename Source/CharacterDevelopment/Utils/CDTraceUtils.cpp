#include "CDTraceUtils.h"
#include "DrawDebugHelpers.h"

bool CDTraceUtils::SweepCapsuleSingleByChannel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, float CapsuleRadius, float CapsuleHalfHeight, const FQuat& Rot, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params /*= FCollisionQueryParams::DefaultQueryParam*/, const FCollisionResponseParams& ResponseParam /*= FCollisionResponseParams::DefaultResponseParam*/, bool bDrawDebug /*= false*/, float DrawTime /*= -1.0f*/, FColor TraceColor /*= FColor::Black*/, FColor HitColor /*= FColor::Red*/)
{
	bool bResult = false;
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	bResult = World->SweepSingleByChannel(OutHit, Start, End, Rot, TraceChannel, CollisionShape, Params, ResponseParam);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		DrawDebugCapsule(World, Start, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, TraceColor, false, DrawTime);
		DrawDebugCapsule(World, End, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, TraceColor, false, DrawTime);
		DrawDebugLine(World, Start, End, TraceColor, false, DrawTime);

		if (bResult)
		{
			DrawDebugCapsule(World, OutHit.Location, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, HitColor, false, DrawTime);
			DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, HitColor, false, DrawTime);
		}
	}
#endif

	return bResult;
}

bool CDTraceUtils::SweepSphereSingleByChannel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, float SphereRadius, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params /*= FCollisionQueryParams::DefaultQueryParam*/, const FCollisionResponseParams& ResponseParam /*= FCollisionResponseParams::DefaultResponseParam*/, bool bDrawDebug /*= false*/, float DrawTime /*= -1.0f*/, FColor TraceColor /*= FColor::Black*/, FColor HitColor /*= FColor::Red*/)
{
	bool bResult = false;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SphereRadius);
	bResult = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, TraceChannel, CollisionShape, Params, ResponseParam);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		FVector DebugDrawCapsuleLocation = (Start + End) * 0.5f;
		FVector TraceVector = End - Start;
		float DebugDrawCapsuleHalfHeight = TraceVector.Size() * 0.5f;
		FQuat DebugCapsuleRotation = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();
		DrawDebugCapsule(World, DebugDrawCapsuleLocation, DebugDrawCapsuleHalfHeight, SphereRadius, DebugCapsuleRotation, TraceColor, false, DrawTime);
		DrawDebugSphere(World, OutHit.Location, SphereRadius, 32, TraceColor, false, DrawTime);
		DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, HitColor, false, DrawTime);
	}
#endif

	return bResult;
}
bool CDTraceUtils::OverlapCapsuleAnyByProfile(const UWorld* World, const FVector& Pos, float CapsuleRadius, float CapsuleHalfHeight, FQuat Rotation, FName ProfileName, const FCollisionQueryParams& QueryParams, bool bDrawDebug /*= false*/, float DrawTime /*= -1.0f*/, FColor HitColor /*= FColor::Red*/)
{
	bool bResult = false;
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	bResult = World->OverlapAnyTestByProfile(Pos, Rotation, ProfileName, CollisionShape, QueryParams);
	
#if ENABLE_DRAW_DEBUG
	if (bDrawDebug && bResult)
	{
		DrawDebugCapsule(World, Pos, CapsuleHalfHeight, CapsuleRadius, Rotation, HitColor, false, DrawTime);
	}
#endif

	return bResult;
}