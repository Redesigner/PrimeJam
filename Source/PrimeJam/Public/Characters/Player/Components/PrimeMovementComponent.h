// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PrimeMovementComponent.generated.h"

UENUM()
enum class EControlMode : uint8
{
	Tank,
	Strafe
};

UCLASS()
class PRIMEJAM_API UPrimeMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Curves, meta = (AllowPrivateAccess))
	TObjectPtr<UCurveFloat> TankWalkSpeedCurve;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Curves, meta = (AllowPrivateAccess))
	TObjectPtr<UCurveFloat> StrafeWalkSpeedCurve;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual float GetMaxSpeed() const override;
	
	void SetControlMode(EControlMode ControlModeIn);
	
private:
	float GetWalkSpeedCurveMultiplier() const;
	
	float CurrentWalkTime = 0.0f;
	
	EControlMode ControlMode;
};
