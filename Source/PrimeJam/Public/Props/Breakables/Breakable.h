// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "Breakable.generated.h"

class UDropTableComponent;
struct FGameplayEffectSpec;
class UBaseAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class PRIMEJAM_API ABreakable : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;	
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBaseAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Drops, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDropTableComponent> DropTable;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Health, meta = (AllowPrivateAccess = true))
	float StartingHealth = 3.0f;
	
public:
	ABreakable();
	
	virtual void BeginPlay() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
private:
	void OnDeath(FGameplayEffectSpec Spec);
};
