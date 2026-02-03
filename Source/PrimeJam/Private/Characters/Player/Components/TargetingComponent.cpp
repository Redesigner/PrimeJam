// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Player/Components/TargetingComponent.h"

#include "PrimeJam.h"
#include "Characters/Player/Megaman.h"
#include "Characters/Player/Components/PrimeMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TargetingMode = ETargetingMode::Relative;
	ReticlePosition = FVector2D(0.5f);
	ReticleArea.X = GetStrafeLimit();
}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	MovementComponent = GetOwner()->GetComponentByClass<UPrimeMovementComponent>();
	if (!MovementComponent.IsValid())
	{
		UE_LOGFMT(LogPrimeJam, Error,
			"TargetingComponent could not find a relevant movement component on actor {OwnerName}. Make sure one exists, or call SetMovementComponent manually",
			GetNameSafe(GetOwner()));
	}
}

FVector UTargetingComponent::GetLookDirection() const
{
	return LookDirection;
}

void UTargetingComponent::AbsoluteInput()
{
	TargetingMode = ETargetingMode::Absolute;	
	if (bTimingOut)
	{
		GetWorld()->GetTimerManager().ClearTimer(LookTimer);
		bTimingOut = false;
		bResettingCursor = false;
	}
}

void UTargetingComponent::RelativeInput(const FVector2D Position)
{
	if (Position.SquaredLength() < DeadZone * DeadZone)
	{
		if (TargetingMode != ETargetingMode::Relative)
		{
			return;
		}
		
		LastRelativeInput = FVector2D(0.0f);
		
		if (!bTimingOut && ReticlePosition.X != 0.5f && ReticlePosition.Y != 0.5f)
		{
			bTimingOut = true;
			GetWorld()->GetTimerManager().SetTimer(LookTimer, FTimerDelegate::CreateLambda([this]()
			{
				ResetLookVertical();
				bTimingOut = false;
				bResettingCursor = true;
				CurrentCursorResetTime = 0.0f;
				CursorPositionBeforeReset = ReticlePosition;
			}), RelativeLookTimeout, false);
		}
		return;
	}
	
	bResettingCursor = false;
	if (bTimingOut)
	{
		GetWorld()->GetTimerManager().ClearTimer(LookTimer);
		bTimingOut = false;
	}
	
	TargetingMode = ETargetingMode::Relative;
	LastRelativeInput = Position;
}

void UTargetingComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TargetingMode == ETargetingMode::Relative)
	{
		if (bResettingCursor)
		{
			CurrentCursorResetTime += DeltaTime;
			if (CurrentCursorResetTime >= CursorResetTime)
			{
				ReticlePosition = FVector2D(0.5f);
				CurrentCursorResetTime = 0.0f;
				bResettingCursor = false;
			}
			else
			{
				const float Alpha = CurrentCursorResetTime / CursorResetTime;
				ReticlePosition = FMath::Lerp(CursorPositionBeforeReset, FVector2D(0.5f), Alpha * Alpha);
			}
			
		}
		
		SetReticlePosition(ReticlePosition + LastRelativeInput * CursorMoveSpeed * DeltaTime);
	}
	else if (TargetingMode == ETargetingMode::Absolute)
	{
		const FViewport* Viewport = GetWorld()->GetGameViewport()->Viewport;
		const FVector2D MousePosition = FVector2D(Viewport->GetMouseX(), Viewport->GetMouseY());
		SetReticlePosition(MousePosition / FVector2D(Viewport->GetSizeXY()));
	}
	if (MovementComponent.IsValid())
	{
		bConstrained = MovementComponent->GetControlMode() == EControlMode::Strafe;
	}
	
	CurrentClampTime += bConstrained ? DeltaTime : -DeltaTime;
	if (CurrentClampTime > CursorClampTime)
	{
		CurrentClampTime = CursorClampTime;
	}
	else if (CurrentClampTime < 0.0f)
	{
		CurrentClampTime = 0.0f;
	}
	else
	{
		ReticleArea.X = GetStrafeLimit();
		OnReticleAreaChanged.Broadcast(ReticleArea);
	}
	
	if (!bTimingOut)
	{
		if (ReticlePosition.Y < LookRegion)
		{
			if (TargetingMode == ETargetingMode::Absolute || LastRelativeInput.Y < 0.0f)
			{
				RequestLookVertical(-DeltaTime * VerticalLookSpeed);
			}
		}
		else if (ReticlePosition.Y > 1.0f -LookRegion)
		{
			if (TargetingMode == ETargetingMode::Absolute || LastRelativeInput.Y > 0.0f)
			{
				RequestLookVertical(DeltaTime * VerticalLookSpeed);
			}
		}
	}
}

#if WITH_EDITOR
void UTargetingComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, TankLimit) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, StrafeLimit))
	{
		ReticleArea.X = GetStrafeLimit();
		OnReticleAreaChanged.Broadcast(ReticleArea);
	}
}
#endif

void UTargetingComponent::SetMovementComponent(UPrimeMovementComponent* PrimeMovementComponent)
{
	MovementComponent = PrimeMovementComponent;
}

FVector2D UTargetingComponent::GetReticleArea() const
{
	return ReticleArea;
}

void UTargetingComponent::SetCursorPosition(const FVector2D ScreenPositionPixels)
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, ScreenPositionPixels, WorldPosition, LookDirection);
	SetWorldLocation(WorldPosition + (LookDirection * 500.0f));
	OnCursorPositionChanged.Broadcast(ScreenPositionPixels);
}

void UTargetingComponent::SetReticlePosition(const FVector2D Position)
{
	ReticlePosition.X = FMath::Clamp(Position.X, ReticleArea.X, 1.0f - ReticleArea.X);
	ReticlePosition.Y = FMath::Clamp(Position.Y, ReticleArea.Y, 1.0f - ReticleArea.Y);
	
	SetCursorPosition(ReticlePosition * GetWorld()->GetGameViewport()->Viewport->GetSizeXY());
}

void UTargetingComponent::RequestLookVertical(float Degrees)
{
	OnLookAngleChanged.Broadcast(Degrees);
}

void UTargetingComponent::ResetLookVertical()
{
	OnVerticalLookReset.Broadcast();
}

float UTargetingComponent::GetStrafeLimit() const
{
	float Alpha = CurrentClampTime / CursorClampTime;
	Alpha *= Alpha;
	return TankLimit + FMath::Lerp(0.0f, StrafeLimit, Alpha);
}
