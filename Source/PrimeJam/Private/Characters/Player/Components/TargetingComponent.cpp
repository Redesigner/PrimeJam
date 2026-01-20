// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Player/Components/TargetingComponent.h"

#include "Kismet/GameplayStatics.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TargetingMode = ETargetingMode::Relative;
	ReticlePosition = FVector2D(0.5f);
}

void UTargetingComponent::AbsoluteInput()
{
	TargetingMode = ETargetingMode::Absolute;
	const FViewport* Viewport = GetWorld()->GetGameViewport()->Viewport;
	FVector2D MousePosition = FVector2D(Viewport->GetMouseX(), Viewport->GetMouseY());
	SetReticlePosition(MousePosition / FVector2D(Viewport->GetSizeXY()));
	SetCursorPosition(MousePosition);
	
	if (bTimingOut)
	{
		GetWorld()->GetTimerManager().ClearTimer(LookTimer);
		bTimingOut = false;
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
		
		if (!bTimingOut)
		{
			bTimingOut = true;
			GetWorld()->GetTimerManager().SetTimer(LookTimer, FTimerDelegate::CreateLambda([this]()
			{
				SetReticlePosition(FVector2D(0.5f));
				const FViewport* Viewport = GetWorld()->GetGameViewport()->Viewport;
				SetCursorPosition(FVector2D(0.5f) * Viewport->GetSizeXY());
				bTimingOut = false;
			}), RelativeLookTimeout, false);
		}
		return;
	}
	
	if (bTimingOut)
	{
		GetWorld()->GetTimerManager().ClearTimer(LookTimer);
		bTimingOut = false;
	}
	
	TargetingMode = ETargetingMode::Relative;
	LastRelativeInput = Position;
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TargetingMode == ETargetingMode::Relative)
	{
		const FViewport* Viewport = GetWorld()->GetGameViewport()->Viewport;
		SetReticlePosition(ReticlePosition + LastRelativeInput * LookSpeed * DeltaTime);
		SetCursorPosition(ReticlePosition * Viewport->GetSizeXY());
	}
}

void UTargetingComponent::SetCursorPosition(const FVector2D ScreenPositionPixels)
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPosition, Direction;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, ScreenPositionPixels, WorldPosition, Direction);
	SetWorldLocation(WorldPosition + (Direction * 500.0f));
	OnCursorPositionChanged.Broadcast(ScreenPositionPixels);
}

void UTargetingComponent::SetReticlePosition(const FVector2D Position)
{
	ReticlePosition.X = FMath::Clamp(Position.X, 0.0f, 1.0f);
	ReticlePosition.Y = FMath::Clamp(Position.Y, 0.0f, 1.0f);
}
