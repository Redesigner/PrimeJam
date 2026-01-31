// Copyright (c) 2025 sleepysunset

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "StructUtils/InstancedStruct.h"
#include "MeleeAbility.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEJAM_API UMeleeAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Melee, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UAnimMontage> MeleeAnimation;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Melee, meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> HitEffect;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void ForceEndAbility();
	
	UFUNCTION()
	void HitEnemy(UPrimitiveComponent* OverlappedHitbox, UPrimitiveComponent* OtherComponent, AActor* OtherActor, const FInstancedStruct& OptionalData);
};
