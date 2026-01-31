// Copyright (c) 2025 sleepysunset

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySet.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class UGameplayAbility;
/**
 * 
 */
UCLASS()
class PRIMEJAM_API UAbilitySet : public UDataAsset
{
	GENERATED_BODY()
	
public:
	void GiveToAbilitySystem(UAbilitySystemComponent* Component);
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;
};
