// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Components/DamageZoneComponent.h"


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
	// HealthComponent->TakeDamage(Damage, nullptr);
}

