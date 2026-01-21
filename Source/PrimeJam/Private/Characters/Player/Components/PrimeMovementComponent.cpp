// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Components/PrimeMovementComponent.h"


void UPrimeMovementComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	if (GetPendingInputVector().IsNearlyZero())
	{
		CurrentWalkTime = 0.0f;
	}
	else
	{
		CurrentWalkTime += DeltaTime;
	}
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UPrimeMovementComponent::GetMaxSpeed() const
{
	switch(MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		return IsCrouching() ? MaxWalkSpeedCrouched : MaxWalkSpeed * GetWalkSpeedCurveMultiplier();
	case MOVE_Falling:
		return MaxWalkSpeed;
	case MOVE_Swimming:
		return MaxSwimSpeed;
	case MOVE_Flying:
		return MaxFlySpeed;
	case MOVE_Custom:
		return MaxCustomMovementSpeed;
	case MOVE_None:
	default:
		return 0.f;
	}
}

void UPrimeMovementComponent::SetControlMode(const EControlMode ControlModeIn)
{
	ControlMode = ControlModeIn;
}

float UPrimeMovementComponent::GetWalkSpeedCurveMultiplier() const
{
	switch (ControlMode)
	{
	case EControlMode::Strafe:
		return StrafeWalkSpeedCurve->GetFloatValue(CurrentWalkTime);
	
	default:
	case EControlMode::Tank:
		return TankWalkSpeedCurve->GetFloatValue(CurrentWalkTime);
	}
}
