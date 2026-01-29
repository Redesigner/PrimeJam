// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mavericks/MaverickCharacter.h"

#include "AbilitySystemComponent.h"
#include "Characters/BaseAttributeSet.h"
#include "Characters/Abilities/AbilitySet.h"
#include "Hitboxes/MHHitboxComponent.h"


// Sets default values
AMaverickCharacter::AMaverickCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));
	HitboxComponent = CreateDefaultSubobject<UMHHitboxComponent>(TEXT("HitboxComponent"));
}

void AMaverickCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AttributeSet->InitializeHealth(DefaultHealth);
	AttributeSet->OnDeath.AddUObject(this, &ThisClass::Die);
	
	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent);
	}
}

UAbilitySystemComponent* AMaverickCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMaverickCharacter::Die_Implementation(FGameplayEffectSpec DeathCause)
{
	Destroy();
}

