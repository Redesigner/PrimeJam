// Copyright (c) 2025 sleepysunset

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CallerTagSet.generated.h"

USTRUCT(BlueprintType)
struct PRIMEJAM_API FCallerTagEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Attribute.SetByCaller"))
	FGameplayTag Tag;
	
	UPROPERTY(EditDefaultsOnly)
	float Value = 1.0f;
};

USTRUCT(BlueprintType)
struct PRIMEJAM_API FCallerTagSet
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FCallerTagEntry> Tags;
};
