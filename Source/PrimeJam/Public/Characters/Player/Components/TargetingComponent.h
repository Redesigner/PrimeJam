// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TargetingComponent.generated.h"

class UPrimeMovementComponent;
class APrimeCharacter;

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
	
	/// Vertical look speed, in degrees/sec
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Looking, meta = (AllowPrivateAccess))
	float VerticalLookSpeed = 45.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Looking, meta = (AllowPrivateAccess))
	float DeadZone = 0.2f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Looking, meta = (AllowPrivateAccess))
	float RelativeLookTimeout = 2.0f;

	/**
	 * If the cursor is in this percentage of either the top or the bottom of the screen,
	 * the camera will look up or down
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Cursor, meta = (AllowPrivateAccess, ClampMin = 0.0f, ClampMax = 0.5f))
	float LookRegion = 0.2f;

	/// Percent of the screen the cursor is limited to  when strafing
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Cursor, meta = (AllowPrivateAccess, ClampMin = 0.0f, ClampMax = 0.5f))
	float StrafeLimit = 0.2f;
	
	/// How fast the cursor moves, in screens/sec
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Cursor, meta = (AllowPrivateAccess))
	float CursorMoveSpeed = 1.0f;
	
	// How long it takes to fully constrain the cursor to the strafe limit, in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Cursor, meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float CursorClampTime = 1.0f;
	
	// How long it takes to recenter the cursor, in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Cursor, meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float CursorResetTime = 1.0f;
	
public:
	UTargetingComponent();
	
	UFUNCTION(BlueprintCallable)
	FVector GetLookDirection() const;
	
	void AbsoluteInput();
	
	void RelativeInput(const FVector2D Position);
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetMovementComponent(UPrimeMovementComponent* PrimeMovementComponent);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCursorPositionChanged, FVector2D, Position);
	UPROPERTY(BlueprintAssignable, Category = Cursor)
	FOnCursorPositionChanged OnCursorPositionChanged;
	
	DECLARE_DELEGATE_OneParam(FOnVerticalLookAngleChanged, float);
	FOnVerticalLookAngleChanged OnLookAngleChanged;

	DECLARE_DELEGATE(FOnVerticalLookReset);
	FOnVerticalLookReset OnVerticalLookReset;
	
private:
	void SetCursorPosition(const FVector2D ScreenPositionPixels);
	
	void SetReticlePosition(const FVector2D Position);
	
	void RequestLookVertical(float Degrees);
	
	void ResetLookVertical();
	
	float GetStrafeLimit() const;
	
	FTimerHandle LookTimer;
	
	FVector2D ReticlePosition;
	
	FVector2D LastRelativeInput;
	
	FVector LookDirection;
	
	bool bTimingOut = false;
	
	
	FVector2D CursorPositionBeforeReset;
	
	UPROPERTY(VisibleAnywhere, Transient, meta = (AllowPrivateAccess))
	float CurrentCursorResetTime = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Transient, meta = (AllowPrivateAccess))
	bool bResettingCursor = false;
	
	
	bool bConstrained = false;
	
	float CurrentClampTime = 0.0f;
	
	ETargetingMode TargetingMode;
	
	// This probably isn't the best way to structure the interaction between
	// these two components, but it should be fine for now
	TWeakObjectPtr<UPrimeMovementComponent> MovementComponent;
};
