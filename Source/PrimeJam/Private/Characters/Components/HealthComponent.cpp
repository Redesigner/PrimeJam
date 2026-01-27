// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::SetHealth(const float NewHealth)
{
	if (Health == 0.0f)
	{
		return;
	}
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(NewHealth);
	if (Health == 0.0f)
	{
		OnDeath.Broadcast();
	}
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

void UHealthComponent::SetMaxHealth(const float NewHealth)
{
	MaxHealth = FMath::Max(0.0f, NewHealth);
	OnMaxHealthChanged.Broadcast(MaxHealth);
	
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
		OnHealthChanged.Broadcast(Health);
	}
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::TakeDamage(const float DamageAmount, UHealthComponent* Source)
{
	if (Health == 0.0f)
	{
		return;
	}
	
	// Don't take damage if self or source is unaffiliated, or we're on the same team
	if (Source && Team != ETeam::Unaffiliated && Source->Team == Team)
	{
		return;
	}
	SetHealth(Health - DamageAmount);
	OnTakeDamage.Broadcast(DamageAmount, Source);
}
