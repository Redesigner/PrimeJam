// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PrimePlayerState.generated.h"

class UBaseAttributeSet;
class UAbilitySet;
/**
 * 
 */
UCLASS()
class PRIMEJAM_API APrimePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;	
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBaseAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilitySet> AbilitySet;
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	APrimePlayerState(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
};
