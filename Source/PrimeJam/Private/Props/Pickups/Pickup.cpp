// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Pickups/Pickup.h"

#include "Characters/Player/Megaman.h"
#include "Characters/Player/PrimePlayerState.h"
#include "Components/SphereComponent.h"


APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;
	
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	RootComponent = OverlapSphere;
	OverlapSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::PickedUp);
}

void APickup::PickedUp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMegaman* Player = Cast<AMegaman>(OtherActor);
	if (!Player)
	{
		return;
	}
	
	APrimePlayerState* PrimePlayerState = Cast<APrimePlayerState>(Player->GetPlayerState());
	if (!PrimePlayerState)
	{
		return;
	}
	
	PlayerPickedUp(PrimePlayerState, Player);
	
	Destroy();
}

