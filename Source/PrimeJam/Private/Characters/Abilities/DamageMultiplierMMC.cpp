// Copyright (c) 2025 sleepysunset


#include "Characters/Abilities/DamageMultiplierMMC.h"

float UDamageMultiplierMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	return BaseValue * Spec.GetSetByCallerMagnitude(Tag, false, 1.0f);
}
