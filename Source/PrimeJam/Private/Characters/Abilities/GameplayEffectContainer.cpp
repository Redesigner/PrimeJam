// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Abilities/GameplayEffectContainer.h"

#include "PrimeJam.h"
#include "Abilities/GameplayAbility.h"
#include "Logging/StructuredLog.h"

TArray<FGameplayEffectSpecHandle> FGameplayEffectContainer::MakeEffectSpecs(UGameplayAbility* OwningAbility) const
{
	TArray<FGameplayEffectSpecHandle> NewEffectSpecs;

	if (!OwningAbility)
	{
		UE_LOGFMT(LogPrimeJam, Error, "GameplayEffectContainer failed to generate EffectSpecs. The owning ability was null.");
		return NewEffectSpecs;
	}

	for (FGameplayEffectMagnitudeEntry MagnitudeEntry : Effects)
	{
		if (!MagnitudeEntry.Effect)
		{
			UE_LOGFMT(LogPrimeJam, Error, "GameplayEffectContainer failed to create a spec for GameplayAbility '{AbilityName}'. The EffectClass was invalid.", OwningAbility->GetName());
			continue;
		}

		FGameplayEffectSpecHandle NewEffectSpec = OwningAbility->MakeOutgoingGameplayEffectSpec(MagnitudeEntry.Effect);
		if (!NewEffectSpec.IsValid())
		{
			UE_LOGFMT(LogPrimeJam, Error, "GameplayEffectContainer failed to create spec of GameplayEffect '{GameplayEffectName}'.", MagnitudeEntry.Effect->GetName());
			continue;
		}

		FGameplayEffectSpec* Spec = NewEffectSpec.Data.Get();
		if (Spec)
		{
			for (const FGameplayEffectMagnitudePair& Pair : MagnitudeEntry.Magnitudes)
			{
				Spec->SetSetByCallerMagnitude(Pair.SetByCallerTag, Pair.Magnitude);
			}
		}

		NewEffectSpecs.Add(NewEffectSpec);
	}
	return NewEffectSpecs;
}
