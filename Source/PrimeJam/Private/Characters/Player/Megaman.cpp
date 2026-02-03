// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Player/Megaman.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "PrimeJam.h"
#include "Characters/Player/PrimePlayerState.h"
#include "Characters/Player/Components/PrimeMovementComponent.h"
#include "Characters/Player/Components/TargetingComponent.h"

AMegaman::AMegaman(const FObjectInitializer& ObjectInitializer) :
	ACharacter(ObjectInitializer.SetDefaultSubobjectClass(CharacterMovementComponentName, UPrimeMovementComponent::StaticClass()))
{
	PrimaryActorTick.bCanEverTick = true;
	PrimeMovementComponent = Cast<UPrimeMovementComponent>(ACharacter::GetMovementComponent());
}

void AMegaman::BeginPlay()
{
	Super::BeginPlay();
}

void AMegaman::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	TargetingComponent = GetComponentByClass<UTargetingComponent>();
	if (!TargetingComponent.IsValid())
	{
		UE_LOGFMT(LogPrimeJam, Error, "Character {CharacterName} does not have a valid targeting component", GetNameSafe(this));
	}
}

void AMegaman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	BindActions(PlayerInputComponent);
}

void AMegaman::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	const IAbilitySystemInterface* AbilitySystem = Cast<IAbilitySystemInterface>(GetPlayerState());
	if (AbilitySystem)
	{
		AbilitySystem->GetAbilitySystemComponent()->SetAvatarActor(this);
	}
}

UAbilitySystemComponent* AMegaman::GetAbilitySystemComponent() const
{
	const IAbilitySystemInterface* AbilitySystem = Cast<IAbilitySystemInterface>(GetPlayerState());
	if (!AbilitySystem)
	{
		return nullptr;
	}
	
	return AbilitySystem->GetAbilitySystemComponent();
}

void AMegaman::BindActions(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		
	EnhancedInputComponent->BindAction(TankAction, ETriggerEvent::Triggered, this, &ThisClass::Tank);
	EnhancedInputComponent->BindAction(AimAbsoluteAction, ETriggerEvent::Triggered, this, &ThisClass::AimAbsolute);
	EnhancedInputComponent->BindAction(AimRelativeAction, ETriggerEvent::Triggered, this, &ThisClass::AimRelative);
	// EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, BlasterComponent.Get(), &UBlasterComponent::StartFiring);
	// EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, BlasterComponent.Get(), &UBlasterComponent::ReleaseFire);
	EnhancedInputComponent->BindAction(ToggleStrafeAction, ETriggerEvent::Started, this, &ThisClass::PressStrafe);
	EnhancedInputComponent->BindAction(ToggleStrafeAction, ETriggerEvent::Completed, this, &ThisClass::ReleaseStrafe);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
}

void AMegaman::AimAbsolute(const FInputActionInstance& Instance)
{
	// If the mouse has moved
	if (!TargetingComponent.IsValid())
	{
		return;
	}
	TargetingComponent->AbsoluteInput();
}

void AMegaman::AimRelative(const FInputActionInstance& Instance)
{
	if (!TargetingComponent.IsValid())
	{
		return;
	}
	TargetingComponent->RelativeInput(Instance.GetValue().Get<FVector2D>());
}

void AMegaman::Tank(const FInputActionInstance& Instance)
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

void AMegaman::AddMovementRotated(const FVector2D Movement)
{
	const float Rotation = GetActorRotation().Yaw + 90.0f;
	const FVector2D RotatedMovement2D = Movement.GetRotated(-Rotation);
	const FVector RotatedMovement = FVector(RotatedMovement2D.X, -RotatedMovement2D.Y, 0.0);
	AddMovementInput(RotatedMovement);
}

void AMegaman::PressStrafe()
{
	PrimeMovementComponent->SetControlMode(EControlMode::Strafe);
}

void AMegaman::ReleaseStrafe()
{
	PrimeMovementComponent->SetControlMode(EControlMode::Tank);
}


