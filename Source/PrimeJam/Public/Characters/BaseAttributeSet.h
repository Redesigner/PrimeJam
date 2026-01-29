// Copyright (c) 2025 sleepysunset

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PRIMEJAM_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Health, meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Health, meta = ( AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	
	UPROPERTY(BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;
	
public:
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Damage)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Healing)
	
private:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	void ApplyDamage(const FGameplayEffectModCallbackData& Data);
	
	void ApplyHealing(const FGameplayEffectModCallbackData& Data);
	
};
