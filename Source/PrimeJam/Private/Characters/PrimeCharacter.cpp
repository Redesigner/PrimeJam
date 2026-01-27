// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PrimeCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/PrimePlayerState.h"
#include "Characters/Player/Components/BlasterComponent.h"
#include "Characters/Player/Components/PrimeMovementComponent.h"
#include "Characters/Player/Components/TargetingComponent.h"

APrimeCharacter::APrimeCharacter(const FObjectInitializer& ObjectInitializer) :
	ACharacter(ObjectInitializer.SetDefaultSubobjectClass(CharacterMovementComponentName, UPrimeMovementComponent::StaticClass()))
{
	PrimaryActorTick.bCanEverTick = true;
	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FirstPersonCamera->SetupAttachment(GetMesh(), TEXT("HeadSocket"));
	FirstPersonCamera->bUsePawnControlRotation;
	
	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting"));
	TargetingComponent->SetupAttachment(FirstPersonCamera);
	TargetingComponent->OnLookAngleChanged.BindLambda([this](const float LookSpeed)
	{
		FRotator CameraRotation = FirstPersonCamera->GetRelativeRotation();
		CameraRotation.Pitch = FMath::Clamp(CameraRotation.Pitch - LookSpeed, -MaxVerticalRotation, MaxVerticalRotation);
		FirstPersonCamera->SetRelativeRotation(CameraRotation);
		bResettingCamera = false;
	});
	TargetingComponent->OnVerticalLookReset.BindLambda([this]()
	{
		CameraInitialPitch = FirstPersonCamera->GetRelativeRotation().Pitch;
		bResettingCamera = true;
		CurrentCameraResetTime = 0.0f;
	});
	
	BlasterComponent = CreateDefaultSubobject<UBlasterComponent>(TEXT("Blaster"));
	BlasterComponent->SetupAttachment(GetMesh(), TEXT("BlasterSocket"));
	BlasterComponent->SetTargetingComponent(TargetingComponent);
	
	PrimeMovementComponent = Cast<UPrimeMovementComponent>(ACharacter::GetMovementComponent());
	TargetingComponent->SetMovementComponent(PrimeMovementComponent.Get());
}

void APrimeCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APrimeCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FRotator CameraWorldRotation = FirstPersonCamera->GetComponentRotation();
	FRotator CharacterRotation = GetActorRotation();
	CharacterRotation.Pitch = CameraWorldRotation.Pitch;
	FirstPersonCamera->SetWorldRotation(CharacterRotation);
	
	if (bResettingCamera)
	{
		FRotator CurrentCameraAngle = FirstPersonCamera->GetRelativeRotation();
		
		CurrentCameraResetTime += DeltaTime;
		if (CurrentCameraResetTime > CameraResetTime)
		{
			CurrentCameraResetTime = 0.0f;
			bResettingCamera = false;
			CurrentCameraAngle.Pitch = 0.0f;
			FirstPersonCamera->SetRelativeRotation(CurrentCameraAngle);
		}
		else
		{
			CurrentCameraAngle.Pitch = FMath::Lerp(CameraInitialPitch, 0.0f, CurrentCameraResetTime / CameraResetTime);
			FirstPersonCamera->SetRelativeRotation(CurrentCameraAngle);
		}
	}
}

void APrimeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	BindActions(PlayerInputComponent);
}

UHealthComponent* APrimeCharacter::GetHealthComponent()
{
	if (APrimePlayerState* PrimePlayerState = Cast<APrimePlayerState>(GetPlayerState()))
	{
		return  PrimePlayerState->GetHealthComponent();
	}
	
	return nullptr;
}

void APrimeCharacter::BindActions(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		
	EnhancedInputComponent->BindAction(TankAction, ETriggerEvent::Triggered, this, &ThisClass::Tank);
	EnhancedInputComponent->BindAction(AimAbsoluteAction, ETriggerEvent::Triggered, this, &ThisClass::AimAbsolute);
	EnhancedInputComponent->BindAction(AimRelativeAction, ETriggerEvent::Triggered, this, &ThisClass::AimRelative);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, BlasterComponent.Get(), &UBlasterComponent::StartFiring);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, BlasterComponent.Get(), &UBlasterComponent::ReleaseFire);
	EnhancedInputComponent->BindAction(ToggleStrafeAction, ETriggerEvent::Started, this, &ThisClass::PressStrafe);
	EnhancedInputComponent->BindAction(ToggleStrafeAction, ETriggerEvent::Completed, this, &ThisClass::ReleaseStrafe);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
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
	
	if (PrimeMovementComponent->GetControlMode() == EControlMode::Tank)
	{
		const UWorld* World = GetWorld();
		AddControllerYawInput(Input.X * World->GetDeltaSeconds() * TurnSpeed);
		AddMovementRotated(FVector2D(0.0f, Input.Y));
	}
	if (PrimeMovementComponent->GetControlMode() == EControlMode::Strafe)
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

void APrimeCharacter::PressStrafe()
{
	PrimeMovementComponent->SetControlMode(EControlMode::Strafe);
}

void APrimeCharacter::ReleaseStrafe()
{
	PrimeMovementComponent->SetControlMode(EControlMode::Tank);
}


