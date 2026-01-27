// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Player/Components/TargetingComponent.h"

#include "Characters/PrimeCharacter.h"
#include "Characters/Player/Components/PrimeMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TargetingMode = ETargetingMode::Relative;
	ReticlePosition = FVector2D(0.5f);
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
	
	bConstrained = MovementComponent->GetControlMode() == EControlMode::Strafe;
	CurrentClampTime = FMath::Clamp(CurrentClampTime += bConstrained ? DeltaTime : -DeltaTime, 0.0f, CursorClampTime);
	
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

void UTargetingComponent::SetMovementComponent(UPrimeMovementComponent* PrimeMovementComponent)
{
	MovementComponent = PrimeMovementComponent;
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
	const float Limit = TankLimit + GetStrafeLimit();
	ReticlePosition.X = FMath::Clamp(Position.X, Limit, 1.0f - Limit);
	ReticlePosition.Y = FMath::Clamp(Position.Y, 0.0f, 1.0f);
	
	SetCursorPosition(ReticlePosition * GetWorld()->GetGameViewport()->Viewport->GetSizeXY());
}

void UTargetingComponent::RequestLookVertical(float Degrees)
{
	OnLookAngleChanged.ExecuteIfBound(Degrees);
}

void UTargetingComponent::ResetLookVertical()
{
	OnVerticalLookReset.ExecuteIfBound();
}

float UTargetingComponent::GetStrafeLimit() const
{
	float Alpha = CurrentClampTime / CursorClampTime;
	Alpha *= Alpha;
	return FMath::Lerp(0.0f, StrafeLimit, Alpha);
}
