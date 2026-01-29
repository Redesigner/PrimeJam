// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GunComponent.generated.h"


class AProjectile;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PRIMEJAM_API UGunComponent : public USceneComponent
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Firing, meta = (AllowPrivateAccess))
	TSubclassOf<AProjectile> DefaultProjectile;
	
public:
	// Sets default values for this component's properties
	UGunComponent();

protected:
	virtual FVector GetFireDirection() const;
	
	virtual FVector GetProjectileSpawnLocation() const;
	
	void FireProjectile() const;
	
	void FireProjectile(const TSubclassOf<AProjectile>& ProjectileClass) const;
};
