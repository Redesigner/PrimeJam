// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Breakables/Breakable.h"

#include "Characters/BaseAttributeSet.h"
#include "Props/DropTables/Components/DropTableComponent.h"


ABreakable::ABreakable()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));
	AttributeSet->OnDeath.AddUObject(this, &ThisClass::OnDeath);

	DropTable = CreateDefaultSubobject<UDropTableComponent>(TEXT("DropTable"));
}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();
	
	AttributeSet->InitializeHealth(StartingHealth);
}

UAbilitySystemComponent* ABreakable::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABreakable::OnDeath(FGameplayEffectSpec Spec)
{
	DropTable->DropItems();
	Destroy();
}
