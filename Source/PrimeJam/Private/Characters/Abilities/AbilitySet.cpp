// Copyright (c) 2025 sleepysunset


#include "Characters/Abilities/AbilitySet.h"

#include "AbilitySystemComponent.h"

void UAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* Component)
{
	for (const TSubclassOf Effect : DefaultEffects)
	{
		const UGameplayEffect* GameplayEffect = Effect->GetDefaultObject<UGameplayEffect>();
		Component->ApplyGameplayEffectToSelf(GameplayEffect, 1.0f, Component->MakeEffectContext());
	}
	
	for (const auto Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability, 1.0f);
		AbilitySpec.SourceObject = Component;
		Component->GiveAbility(AbilitySpec);
	}
}
