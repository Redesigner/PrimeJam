// Copyright (c) 2026 sleepysunset


#include "Characters/Abilities/BlasterAbility.h"

#include "AbilitySystemComponent.h"
#include "PrimeJam.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/MovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Logging/StructuredLog.h"

#include "Props/Projectiles/Projectile.h"
#include "Characters/Components/EffectApplicationComponent.h"
#include "Characters/Components/GunComponent.h"


void UBlasterAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ProjectileClass)
	{
		UE_LOGFMT(LogPrimeJam, Warning, "BlasterAbility '{AbilityName}' failed to spawn projectile, Projectile Class is invalid.", GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOGFMT(LogPrimeJam, Error, "BlasterAbility '{AbilityName}' failed to spawn projectile, world is invalid.", GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!ActorInfo->AvatarActor.IsValid())
	{
		UE_LOGFMT(LogPrimeJam, Warning, "BlasterAbility '{AbilityName}' failed to spawn projectile '{ProjectileName}', AbilitySystemComponent does not have a valid AvatarActor.", GetName(), ProjectileClass->GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Only spawn the projectile on the server, and replicate out from there
	if (ActorInfo->IsNetAuthority())
	{
		SpawnProjectile(ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorRotation().RotateVector(ProjectileOffset));
	}

	// CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true);
	// CommitExecute(Handle, ActorInfo, ActivationInfo);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (bEndAbilityOnFire)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UBlasterAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (IsActive())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}


void UBlasterAbility::SpawnProjectile(const FVector& Location)
{
	if (!ProjectileClass)
	{
		UE_LOGFMT(LogPrimeJam, Error, "BlasterAbility '{AbilityName}' failed to spawn projectile. The projectile class was invalid.", GetName());
		return;
	}

	if (!CurrentActorInfo->AvatarActor.IsValid())
	{
		return;
	}
	
	UGunComponent* GunComponent = CurrentActorInfo->AvatarActor->GetComponentByClass<UGunComponent>();
	if (!GunComponent)
	{
		return;
	}

	FTransform ProjectileTransform;
	ProjectileTransform.SetLocation(GunComponent->GetProjectileSpawnLocation());
	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass.Get(), ProjectileTransform);

	if (UEffectApplicationComponent* EffectApplier = Projectile->GetComponentByClass<UEffectApplicationComponent>())
	{
		EffectApplier->SetEffectHandles(CurrentActorInfo->AvatarActor.Get(), EffectContainer.MakeEffectSpecs(this));
	}
	Projectile->SetProjectileOwner(CurrentActorInfo->AvatarActor.Get());
	Projectile->FinishSpawning(ProjectileTransform);
	Projectile->SetVelocity(GunComponent->GetFireDirection() * ProjectileSpeed);
}
