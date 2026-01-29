// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthComponent;

UENUM()
enum class ETeam : uint8
{
	Unaffiliated,
	Light,
	Wily
};

UINTERFACE()
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for pawns that want to have health components
 * allows for the health component to live in different places,
 * such as in the player state
 */
class IHealthInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	UHealthComponent* GetHealthComponent();
};

UCLASS(ClassGroup=(Prime), meta=(BlueprintSpawnableComponent))
class PRIMEJAM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Health, BlueprintSetter = SetHealth, BlueprintGetter = GetHealth, meta = (AllowPrivateAcess, ClampMin = 0.0f))
	float Health = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = Health, BlueprintSetter = SetMaxHealth, BlueprintGetter = GetMaxHealth, meta = (AllowPrivateAcess, ClampMin = 0.0f))
	float MaxHealth = 10.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = Team, meta = (AllowPrivateAccess))
	ETeam Team;
	
public:
	UHealthComponent();

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() const;
	
	
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float NewHealth);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageAmount, UHealthComponent* Source);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);
	FOnHealthChanged OnHealthChanged;
	
	FOnHealthChanged OnMaxHealthChanged;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeDamage, float, Damage, const UHealthComponent*, Source);
	UPROPERTY(BlueprintAssignable)
	FOnTakeDamage OnTakeDamage;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
};
