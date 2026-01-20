// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"

#include "PrimeCharacter.generated.h"

class UCameraComponent;
class UInputAction;
struct FInputActionInstance;

UCLASS()
class PRIMEJAM_API APrimeCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> FirstPersonCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> TankAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> StrafeAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> AimAction;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = true, ClampMin = 0.0f))
	float TurnSpeed = 50.0f;
	
public:
	APrimeCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	void BindActions(UInputComponent* PlayerInputComponent);
	
	void Aim(const FInputActionInstance& Instance);
	
	void Tank(const FInputActionInstance& Instance);
	
	void Strafe(const FInputActionInstance& Instance);
	
	void AddMovementRotated(FVector2D Movement);
};
