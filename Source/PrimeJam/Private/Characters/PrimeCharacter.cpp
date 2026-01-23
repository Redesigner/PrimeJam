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
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation;
	
	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting"));
	TargetingComponent->SetupAttachment(FirstPersonCamera);
	TargetingComponent->OnLookAngleChanged.BindLambda([this](const float LookSpeed)
	{
		const FRotator CameraRotation = FirstPersonCamera->GetRelativeRotation();
		FirstPersonCamera->SetRelativeRotation(FRotator(
			FMath::Clamp(CameraRotation.Pitch - LookSpeed, -MaxVerticalRotation, MaxVerticalRotation),
			0.0f,
			0.0f));
		bResettingCamera = false;
	});
	TargetingComponent->OnVerticalLookReset.BindLambda([this]()
	{
		CameraInitialPitch = FirstPersonCamera->GetRelativeRotation().Pitch;
		bResettingCamera = true;
		CurrentCameraResetTime = 0.0f;
	});
	
	BlasterComponent = CreateDefaultSubobject<UBlasterComponent>(TEXT("Blaster"));
	BlasterComponent->SetupAttachment(RootComponent);
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
	EnhancedInputComponent->BindAction(StrafeAction, ETriggerEvent::Triggered, this, &ThisClass::Strafe);
	EnhancedInputComponent->BindAction(AimAbsoluteAction, ETriggerEvent::Triggered, this, &ThisClass::AimAbsolute);
	EnhancedInputComponent->BindAction(AimRelativeAction, ETriggerEvent::Triggered, this, &ThisClass::AimRelative);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, BlasterComponent.Get(), &UBlasterComponent::StartFiring);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, BlasterComponent.Get(), &UBlasterComponent::ReleaseFire);
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
	
	PrimeMovementComponent->SetControlMode(EControlMode::Tank);
	
	const UWorld* World = GetWorld();
	AddControllerYawInput(Input.X * World->GetDeltaSeconds() * TurnSpeed);
	AddMovementRotated(FVector2D(0.0f, Input.Y));
}

void APrimeCharacter::Strafe(const FInputActionInstance& Instance)
{
	if (const FVector2D Input = Instance.GetValue().Get<FVector2D>(); Controller && !Input.IsNearlyZero())
	{
		AddMovementRotated(Input);
		
		PrimeMovementComponent->SetControlMode(EControlMode::Strafe);
	}
}

void APrimeCharacter::AddMovementRotated(const FVector2D Movement)
{
	const float Rotation = FirstPersonCamera->GetComponentRotation().Yaw + 90.0f;
	const FVector2D RotatedMovement2D = Movement.GetRotated(-Rotation);
	const FVector RotatedMovement = FVector(RotatedMovement2D.X, -RotatedMovement2D.Y, 0.0);
	AddMovementInput(RotatedMovement);
}


