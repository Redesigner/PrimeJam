// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PrimePlayerState.h"

APrimePlayerState::APrimePlayerState(const FObjectInitializer& ObjectInitializer)
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

UHealthComponent* APrimePlayerState::GetHealthComponent()
{
	return HealthComponent;
}
