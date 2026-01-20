// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"

#include "PrimeCharacter.generated.h"

class UTargetingComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionInstance;

UCLASS()
class PRIMEJAM_API APrimeCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess))
	TObjectPtr<UCameraComponent> FirstPersonCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> TankAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> StrafeAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> AimAbsoluteAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> AimRelativeAction;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float TurnSpeed = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
	TObjectPtr<UTargetingComponent> TargetingComponent;
	
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
