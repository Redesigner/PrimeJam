// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Components/BlasterComponent.h"

#include "Characters/Player/Components/TargetingComponent.h"


UBlasterComponent::UBlasterComponent()
{

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
	switch (BlasterMode)
	{
	default:
	case EBlasterMode::None:
		break;
		
	case EBlasterMode::Lemon:
		FireProjectile(LemonProjectile);
		break;
		
	case EBlasterMode::Charge:
		FireProjectile(ChargeProjectile);
		break;
	}
	
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
	if (!TargetingComponent.IsValid())
	{
		return FVector::ZeroVector;
	}
	
	return TargetingComponent->GetAttachParent()->GetComponentLocation();
}

void UBlasterComponent::FireProjectile(const TSubclassOf<AActor>& ProjectileClass) const
{
	const FVector Position = GetProjectileSpawnLocation();
	const FRotator Rotator = FRotator(FQuat::FindBetweenNormals(FVector::ForwardVector, GetFireDirection()));
	
	GetWorld()->SpawnActor(ProjectileClass, &Position, &Rotator);
}
