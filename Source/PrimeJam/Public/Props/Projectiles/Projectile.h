// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UHealthComponent;

UCLASS()
class PRIMEJAM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess))
	float ProjectileSpeed = 10.0f;	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess))
	float BaseDamageValue = 1.0f;

public:
	AProjectile();
	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	float CalculateDamage() const;
};
