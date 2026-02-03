// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Components/BlasterComponent.h"

#include "PrimeJam.h"
#include "Characters/Player/Components/TargetingComponent.h"


UBlasterComponent::UBlasterComponent()
{
}

void UBlasterComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UTargetingComponent* NewTargetingComponent = GetOwner()->GetComponentByClass<UTargetingComponent>();
	if (!NewTargetingComponent)
	{
		UE_LOGFMT(LogPrimeJam, Warning,
			"BlasterComponent could not find a relevant targeting component on actor {OwnerName}. Make sure one exists, or call SetTargetingComponent manually",
			GetNameSafe(GetOwner()));
		return;
	}
	
	SetTargetingComponent(NewTargetingComponent);
}

void UBlasterComponent::SetTargetingComponent(UTargetingComponent* TargetingComponentIn)
{
	TargetingComponent = TargetingComponentIn;
}

void UBlasterComponent::StartFiring()
{
	if (BlasterMode != EBlasterMode::None)
	{
		return;
	}
	
	BlasterMode = EBlasterMode::Lemon;
	GetWorld()->GetTimerManager().SetTimer(ChargeTimer, FTimerDelegate::CreateLambda([this]()
	{
		SetBlasterMode(EBlasterMode::Charge);
	}), ChargeTime, false);
}

void UBlasterComponent::ReleaseFire()
{	
	SetBlasterMode(EBlasterMode::None);
	GetWorld()->GetTimerManager().ClearTimer(ChargeTimer);
}

EBlasterMode UBlasterComponent::GetBlasterMode() const
{
	return BlasterMode;
}

void UBlasterComponent::SetBlasterMode(const EBlasterMode BlasterModeIn)
{
	if (BlasterModeIn != BlasterMode)
	{
		BlasterMode = BlasterModeIn;
		OnBlasterModeChanged.Broadcast(BlasterMode);
	}
}

FVector UBlasterComponent::GetFireDirection() const
{
	if (!TargetingComponent.IsValid())
	{
		return FVector::ZeroVector;
	}
	
	return TargetingComponent->GetLookDirection();
}

FVector UBlasterComponent::GetProjectileSpawnLocation() const
{
	return GetComponentLocation();
}
