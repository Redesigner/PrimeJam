// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Components/DamageZoneComponent.h"

#include "Characters/Components/HealthComponent.h"


UDamageZoneComponent::UDamageZoneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDamageZoneComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwner()->OnActorBeginOverlap.AddUniqueDynamic(this, &ThisClass::UDamageZoneComponent::ActorBeginOverlap);
}

void UDamageZoneComponent::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	IHealthInterface* HealthInterface = Cast<IHealthInterface>(OtherActor);
	if (!HealthInterface)
	{
		return;
	}
	
	UHealthComponent* HealthComponent = HealthInterface->GetHealthComponent();
	if (!HealthComponent)
	{
		return;
	}
	
	HealthComponent->TakeDamage(Damage, nullptr);
}

