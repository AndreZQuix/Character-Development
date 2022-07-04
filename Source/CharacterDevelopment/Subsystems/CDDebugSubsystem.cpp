// Fill out your copyright notice in the Description page of Project Settings.


#include "CDDebugSubsystem.h"

bool UCDDebugSubsystem::IsCategoryEnabled(const FName& CategoryName) const
{
	const bool* bIsEnabled = EnabledCategories.Find(CategoryName);
	return bIsEnabled != nullptr && *bIsEnabled;
}

void UCDDebugSubsystem::EnableCategory(const FName& CategoryName, bool bIsEnabled)
{
	EnabledCategories.FindOrAdd(CategoryName);
	EnabledCategories[CategoryName] = bIsEnabled;
}