// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BlasterComponent.generated.h"


struct FInputActionInstance;
class UTargetingComponent;


UENUM()

enum class EBlasterMode : uint8
{
	None,
	Lemon,
	Charge
};

UCLASS()
class PRIMEJAM_API UBlasterComponent : public USceneComponent
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Charging, meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float ChargeTime = 1.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Firing, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> LemonProjectile;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Firing, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> ChargeProjectile;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Firing, meta = (AllowPrivateAccess))
	EBlasterMode BlasterMode;
	
public:
	UBlasterComponent();
	
	void SetTargetingComponent(UTargetingComponent* TargetingComponentIn);
	
	void StartFiring();
	
	void ReleaseFire();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EBlasterMode GetBlasterMode() const;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlasterModeChanged, EBlasterMode, Mode);

	/**
	 * Delegate for whenever the blaster mode is changed (only fires if the new value is different from the old value)
	 * Mostly intended here as a hook for animations
	 */
	UPROPERTY(BlueprintAssignable)
	FOnBlasterModeChanged OnBlasterModeChanged;
	
private:
	void SetBlasterMode(EBlasterMode BlasterModeIn);
	
	FVector GetFireDirection() const;
	
	FVector GetProjectileSpawnLocation() const;
	
	void FireProjectile(const TSubclassOf<AActor>& ProjectileClass) const;
	
	TWeakObjectPtr<UTargetingComponent> TargetingComponent;
	
	FTimerHandle ChargeTimer;
};
