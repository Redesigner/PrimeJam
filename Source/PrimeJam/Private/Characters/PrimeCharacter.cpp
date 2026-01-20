// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PrimeCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/Components/TargetingComponent.h"

APrimeCharacter::APrimeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation;
	
	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting"));
	TargetingComponent->SetupAttachment(FirstPersonCamera);
}

void APrimeCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APrimeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrimeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	BindActions(PlayerInputComponent);
}

void APrimeCharacter::BindActions(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		
	EnhancedInputComponent->BindAction(TankAction, ETriggerEvent::Triggered, this, &ThisClass::Tank);
	EnhancedInputComponent->BindAction(StrafeAction, ETriggerEvent::Triggered, this, &ThisClass::Strafe);
	EnhancedInputComponent->BindAction(AimAbsoluteAction, ETriggerEvent::Triggered, this, &ThisClass::AimAbsolute);
	EnhancedInputComponent->BindAction(AimRelativeAction, ETriggerEvent::Triggered, this, &ThisClass::AimRelative);
}

void APrimeCharacter::AimAbsolute(const FInputActionInstance& Instance)
{
	// If the mouse has moved
	TargetingComponent->AbsoluteInput();
}

void APrimeCharacter::AimRelative(const FInputActionInstance& Instance)
{
	TargetingComponent->RelativeInput(Instance.GetValue().Get<FVector2D>());
}

void APrimeCharacter::Tank(const FInputActionInstance& Instance)
{
	const FVector2D Input = Instance.GetValue().Get<FVector2D>();
	
	if (Input.IsNearlyZero())
	{
		return;
	}
	
	const UWorld* World = GetWorld();
	AddControllerYawInput(Input.X * World->GetDeltaSeconds() * TurnSpeed);
	AddMovementRotated(FVector2D(0.0f, Input.Y));
}

void APrimeCharacter::Strafe(const FInputActionInstance& Instance)
{
	if (const FVector2D Input = Instance.GetValue().Get<FVector2D>(); Controller && !Input.IsNearlyZero())
	{
		AddMovementRotated(Input);
	}
}

void APrimeCharacter::AddMovementRotated(const FVector2D Movement)
{
	const float Rotation = FirstPersonCamera->GetComponentRotation().Yaw + 90.0f;
	const FVector2D RotatedMovement2D = Movement.GetRotated(-Rotation);
	const FVector RotatedMovement = FVector(RotatedMovement2D.X, -RotatedMovement2D.Y, 0.0);
	AddMovementInput(RotatedMovement);
}


