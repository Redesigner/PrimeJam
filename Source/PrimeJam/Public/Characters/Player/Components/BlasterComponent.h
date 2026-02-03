// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/GunComponent.h"
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

UCLASS(ClassGroup=(MegaPrime), meta=(BlueprintSpawnableComponent))
class PRIMEJAM_API UBlasterComponent : public UGunComponent
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Charging, meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float ChargeTime = 1.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Firing, meta = (AllowPrivateAccess))
	TSubclassOf<AProjectile> LemonProjectile;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Firing, meta = (AllowPrivateAccess))
	TSubclassOf<AProjectile> ChargeProjectile;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Firing, meta = (AllowPrivateAccess))
	EBlasterMode BlasterMode;
	
public:
	UBlasterComponent();
	
	virtual void BeginPlay() override;
	
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
	
protected:
	virtual FVector GetFireDirection() const override;
	
	virtual FVector GetProjectileSpawnLocation() const override;
	
private:
	void SetBlasterMode(EBlasterMode BlasterModeIn);
	
	TWeakObjectPtr<UTargetingComponent> TargetingComponent;
	
	FTimerHandle ChargeTimer;
};
