// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Projectiles/Projectile.h"

#include "AbilitySystemInterface.h"
#include "Characters/Components/EffectApplicationComponent.h"


// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	EffectApplication = CreateDefaultSubobject<UEffectApplicationComponent>(TEXT("EffectApplication"));
}

void AProjectile::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AddActorLocalOffset(FVector(DeltaSeconds * ProjectileSpeed, 0.0f, 0.0f), true);
}

void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const FHitResult FakeHit = FHitResult(OtherActor, nullptr, GetActorLocation(), -GetActorForwardVector());

	if (HitActors.Contains(OtherActor))
	{
		return;
	}
	if (!OtherActor->Implements<UAbilitySystemInterface>())
	{
		return;
	}
	
	EffectApplication->ApplyGameplayEffectsToTarget(OtherActor, FakeHit);
	HitActors.Add(OtherActor);

	if (bDestroyOnEffectApplied)
	{
		Destroy();
	}
}

void AProjectile::SetVelocity(const FVector& Velocity)
{
	SetActorRotation(FQuat::FindBetweenNormals(FVector::ForwardVector, Velocity.GetSafeNormal()));
	ProjectileSpeed = Velocity.Length();
}

void AProjectile::SetProjectileOwner(AActor* ProjectileOwner)
{
	HitActors.Add(ProjectileOwner);
}

float AProjectile::CalculateDamage_Implementation() const
{
	return BaseDamageValue;
}

