// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"

#include "HealthViewModel.generated.h"

struct FOnAttributeChangeData;
class UBaseAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class PRIMEJAM_API UHealthViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = true))
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = true))
	float MaxHealth;

public:
	UFUNCTION(BlueprintCallable)
	void BindAttributeSet(UAbilitySystemComponent* AbilitySystemComponent, UBaseAttributeSet* AttributeSet);
	
	void HealthChanged(const FOnAttributeChangeData& Data);

	void MaxHealthChanged(const FOnAttributeChangeData& Data);
};
