// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "MaverickCharacter.generated.h"

class UMHHitboxComponent;
class UAbilitySet;
class UBaseAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class PRIMEJAM_API AMaverickCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;	
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBaseAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilitySet> AbilitySet;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	float DefaultHealth = 5.0f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Components, meta = (AllowPrivateAccess = true))
	TObjectPtr<UMHHitboxComponent> HitboxComponent;

public:
	AMaverickCharacter();
	
	virtual void BeginPlay() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void Die(FGameplayEffectSpec DeathCause);
};
