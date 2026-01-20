// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TargetingComponent.generated.h"

UENUM()
enum class ETargetingMode : uint8
{
	Relative,
	Absolute
};

UCLASS()
class PRIMEJAM_API UTargetingComponent : public USceneComponent
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Looking, meta = (AllowPrivateAccess))
	float LookSpeed = 1.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Looking, meta = (AllowPrivateAccess))
	float DeadZone = 0.2f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Looking, meta = (AllowPrivateAccess))
	float RelativeLookTimeout = 2.0f;
	
public:
	UTargetingComponent();
	
	void AbsoluteInput();
	
	void RelativeInput(const FVector2D Position);
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCursorPositionChanged, FVector2D, Position);
	UPROPERTY(BlueprintAssignable, Category = Cursor)
	FOnCursorPositionChanged OnCursorPositionChanged;

private:
	void SetCursorPosition(const FVector2D ScreenPositionPixels);
	
	void SetReticlePosition(const FVector2D Position);
	
	FTimerHandle LookTimer;
	
	FVector2D ReticlePosition;
	
	FVector2D LastRelativeInput;
	
	bool bTimingOut = false;
	
	ETargetingMode TargetingMode;
};
