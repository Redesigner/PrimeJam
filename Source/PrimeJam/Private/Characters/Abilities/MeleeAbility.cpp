// Copyright (c) 2025 sleepysunset


#include "Characters/Abilities/MeleeAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/Abilities/CallerTagSet.h"
#include "Hitboxes/MHHitboxComponent.h"

void UMeleeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (MeleeAnimation.IsNull())
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}
	
	UMHHitboxComponent* HitboxComponent = ActorInfo->AvatarActor.Get()->GetComponentByClass<UMHHitboxComponent>();
	if (!HitboxComponent)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("MeleeAnimation"), MeleeAnimation.LoadSynchronous());

	PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::ForceEndAbility);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::ForceEndAbility);
	PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::ForceEndAbility);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::ForceEndAbility);
	PlayMontageTask->ReadyForActivation();
	
	HitboxComponent->OnHitboxOverlapped.AddDynamic(this, &ThisClass::HitEnemy);
	UE_LOG(LogTemp, Display, TEXT("Bound attack callback"));
}

void UMeleeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	UMHHitboxComponent* HitboxComponent = ActorInfo->AvatarActor.Get()->GetComponentByClass<UMHHitboxComponent>();
	if (!HitboxComponent)
	{
		return;
	}
	
	HitboxComponent->OnHitboxOverlapped.RemoveDynamic(this, &ThisClass::HitEnemy);
	UE_LOG(LogTemp, Display, TEXT("Unbound attack callback"));
}

void UMeleeAbility::ForceEndAbility()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UMeleeAbility::HitEnemy(UPrimitiveComponent* OverlappedHitbox, UPrimitiveComponent* OtherComponent,
	AActor* OtherActor, const FInstancedStruct& OptionalData)
{
	const UAbilitySystemComponent* AbilitySystemComponent = GetActorInfo().AbilitySystemComponent.Get();
	check(AbilitySystemComponent);
	
	const IAbilitySystemInterface* EnemyAbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (!EnemyAbilitySystemInterface)
	{
		return;
	}

	const FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(HitEffect, 1.0f, AbilitySystemComponent->MakeEffectContext());
	if (OptionalData.IsValid())
	{
		if (const FCallerTagSet* Set = OptionalData.GetPtr<FCallerTagSet>())
		{
			for (const auto& [Tag, Value] : Set->Tags)
			{
				EffectSpec.Data->SetSetByCallerMagnitude(Tag, Value);
			}
		}
	}
	EnemyAbilitySystemInterface->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}
