// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UEffectApplicationComponent;
class UHealthComponent;

UCLASS()
class PRIMEJAM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components, meta = (AllowPrivateAccess))
	TObjectPtr<UEffectApplicationComponent> EffectApplication;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess))
	float ProjectileSpeed = 10.0f;	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess))
	float BaseDamageValue = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess))
	bool bDestroyOnEffectApplied = true;
	
public:
	AProjectile();
	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	void SetGameplayEffectSpec(FGameplayEffectSpec EffectSpec);
	
	void SetVelocity(const FVector& Velocity);
	
	void SetProjectileOwner(AActor* ProjectileOwner);
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	float CalculateDamage() const;
	
private:
	FGameplayEffectSpec DamageEffectSpec;
	
	TArray<FObjectKey> HitActors;
};
