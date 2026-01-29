// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Projectiles/Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectile::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AddActorLocalOffset(FVector(DeltaSeconds * ProjectileSpeed, 0.0f, 0.0f), true);
}

void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// HealthComponent->TakeDamage(BaseDamageValue, ProjectileOwner.Get());
}

float AProjectile::CalculateDamage_Implementation() const
{
	return BaseDamageValue;
}

