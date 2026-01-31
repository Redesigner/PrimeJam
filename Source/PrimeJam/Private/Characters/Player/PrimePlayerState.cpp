// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PrimePlayerState.h"

#include "Characters/BaseAttributeSet.h"
#include "Characters/Abilities/AbilitySet.h"

UAbilitySystemComponent* APrimePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

APrimePlayerState::APrimePlayerState(const FObjectInitializer&)
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));
}

void APrimePlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent);
	}
}
