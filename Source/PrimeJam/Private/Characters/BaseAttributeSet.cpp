// Copyright (c) 2025 sleepysunset


#include "Characters/BaseAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Logging/StructuredLog.h"

void UBaseAttributeSet::InitializeHealth(float HealthValue)
{
	SetMaxHealth(HealthValue);
	SetHealth(HealthValue);
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		ApplyDamage(Data);
		return;
	}
	
	if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		ApplyHealing(Data);
		return;
	}
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UBaseAttributeSet::ApplyDamage(const FGameplayEffectModCallbackData& Data)
{
	if (!bAlive)
	{
		return;
	}
	
	// No negative damage, should be applied using healing instead
	if (Data.EvaluatedData.Magnitude <= 0.0f)
	{
		return;
	}
	
	// No self-damage allowed, right now
	/*if (Data.EffectSpec.GetEffectContext().GetInstigatorAbilitySystemComponent() == GetOwningAbilitySystemComponentChecked())
	{
		return;
	}*/
	
	SetHealth(FMath::Clamp(GetHealth() - Damage.GetCurrentValue(), 0.0f, GetMaxHealth()));
	SetDamage(0.0f);
	
	if (GetHealth() == 0.0f)
	{
		bAlive = false;
		OnDeath.Broadcast(Data.EffectSpec);
	}
}

void UBaseAttributeSet::ApplyHealing(const FGameplayEffectModCallbackData& Data)
{
	// No negative healing, should be applied using damage instead
	if (Data.EvaluatedData.Magnitude <= 0.0f)
	{
		return;
	}
	
	SetHealth(FMath::Clamp(GetHealth() + GetHealing(), 0.0f, GetMaxHealth()));
	SetHealing(0.0f);
}