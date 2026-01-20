// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"

#include "PrimeCharacter.generated.h"

class UBlasterComponent;
class UTargetingComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionInstance;

UCLASS()
class PRIMEJAM_API APrimeCharacter : public ACharacter
{
	GENERATED_BODY()
	
	// INPUT ACTIONS
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> TankAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> StrafeAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> AimAbsoluteAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> AimRelativeAction;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> FireAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> JumpAction;	
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess))
	TObjectPtr<UCameraComponent> FirstPersonCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float TurnSpeed = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess, ClampMin = 0.0f, ClampMax = 90.0f))
	float MaxVerticalRotation = 60.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
	TObjectPtr<UTargetingComponent> TargetingComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
	TObjectPtr<UBlasterComponent> BlasterComponent;
	
public:
	APrimeCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	void BindActions(UInputComponent* PlayerInputComponent);
	
	void AimAbsolute(const FInputActionInstance& Instance);
	
	void AimRelative(const FInputActionInstance& Instance);
	
	void Tank(const FInputActionInstance& Instance);
	
	void Strafe(const FInputActionInstance& Instance);
	
	void AddMovementRotated(FVector2D Movement);
};
