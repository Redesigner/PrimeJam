// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/DropTables/Components/DropTableComponent.h"

#include "Characters/Components/HealthComponent.h"
#include "Props/Pickups/Pickup.h"
#include "PrimeJam.h"
#include "Props/DropTables/DropTableAsset.h"

UDropTableComponent::UDropTableComponent()
{
}

void UDropTableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	IHealthInterface* HealthInterface = Cast<IHealthInterface>(GetOwner());
	if (!HealthInterface)
	{
		return;
	}
	
	HealthInterface->GetHealthComponent()->OnDeath.AddUniqueDynamic(this, &ThisClass::DropItems);
}

void UDropTableComponent::DropItems()
{
	if (!DropTable)
	{
		UE_LOGFMT(LogPrimeJam, Warning, "DropTableComponent '{}' did not have a drop table. Did you forget to set it?", GetNameSafe(this));
		return;
	}
	
	for (const auto& PickupClass : DropTable->MakeRandomDrops())
	{
		GetWorld()->SpawnActor<APickup>(PickupClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	}
}

