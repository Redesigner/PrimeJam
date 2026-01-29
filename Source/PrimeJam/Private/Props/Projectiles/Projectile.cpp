// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Projectiles/Projectile.h"

#include "Characters/Components/HealthComponent.h"


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
	
	if (!OtherActor->Implements<UHealthInterface>())
	{
		return;
	}
	
	UHealthComponent* HealthComponent = IHealthInterface::Execute_GetHealthComponent(OtherActor);
	
	if (!HealthComponent)
	{
		return;
	}
	
	// Don't damage self!
	if (HealthComponent == ProjectileOwner)
	{
		return;
	}
	
	HealthComponent->TakeDamage(BaseDamageValue, ProjectileOwner.Get());
}

void AProjectile::SetProjectileOwner(UHealthComponent* NewOwner)
{
	ProjectileOwner = NewOwner;
}

float AProjectile::CalculateDamage_Implementation() const
{
	return BaseDamageValue;
}

