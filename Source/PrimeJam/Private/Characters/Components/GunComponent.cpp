// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/GunComponent.h"

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

void UGunComponent::FireProjectile() const
{
	FireProjectile(DefaultProjectile);
}

void UGunComponent::FireProjectile(const TSubclassOf<AProjectile>& ProjectileClass) const
{
	FTransform Transform;
	Transform.SetLocation(GetProjectileSpawnLocation());
	Transform.SetRotation(FQuat::FindBetweenNormals(FVector::ForwardVector, GetFireDirection()));
	
	AProjectile* NewProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, Transform);
	
	if (!NewProjectile)
	{
		NewProjectile->FinishSpawning(Transform);
		return;
	}
	
	// NewProjectile->SetProjectileOwner(HealthComponent);
	NewProjectile->FinishSpawning(Transform);
}


