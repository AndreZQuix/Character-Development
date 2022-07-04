// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CDDebugSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERDEVELOPMENT_API UCDDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	bool IsCategoryEnabled(const FName& CategoryName) const;

private:
	UFUNCTION(exec)
	void EnableCategory(const FName& CategoryName, bool bIsEnabled);

	TMap<FName, bool> EnabledCategories;
};
