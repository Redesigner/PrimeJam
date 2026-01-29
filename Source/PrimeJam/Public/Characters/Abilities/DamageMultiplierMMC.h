// Copyright (c) 2025 sleepysunset

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DamageMultiplierMMC.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEJAM_API UDamageMultiplierMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess, Categories = "Attribute.SetByCaller"))
	FGameplayTag Tag;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess))
	float BaseValue = 0.0f;
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
