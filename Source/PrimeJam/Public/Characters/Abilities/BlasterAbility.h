// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectContainer.h"
#include "Abilities/GameplayAbility.h"
#include "UObject/Object.h"
#include "BlasterAbility.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class PRIMEJAM_API UBlasterAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect, meta = (AllowPrivateAccess = true))
	FGameplayEffectContainer EffectContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = true))
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = true))
	FVector ProjectileOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = true, ClampMin = 0.0f))
	float ProjectileSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Activation, meta = (AllowPrivateAccess = true))
	bool bEndAbilityOnFire = true;


	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)  override;

	void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	void SpawnProjectile(const FVector& Location);
};