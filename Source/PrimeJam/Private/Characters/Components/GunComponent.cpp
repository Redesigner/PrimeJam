// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/GunComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Characters/Components/EffectApplicationComponent.h"
#include "Props/Projectiles/Projectile.h"


// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UGunComponent::GetFireDirection() const
{
	return GetForwardVector();
}

FVector UGunComponent::GetProjectileSpawnLocation() const
{
	return GetComponentLocation();
}

FRotator UGunComponent::GetFireRotator() const
{
	return FRotator(FQuat::FindBetweenNormals(FVector::ForwardVector, GetForwardVector()));
}
