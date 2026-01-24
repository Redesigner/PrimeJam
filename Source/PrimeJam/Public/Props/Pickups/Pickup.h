// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class APrimeCharacter;
class APrimePlayerState;
class USphereComponent;

UCLASS()
class PRIMEJAM_API APickup : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Components, meta = (AllowPrivateAccess))
	TObjectPtr<USphereComponent> OverlapSphere;
	
public:
	APickup();
	
private:
	UFUNCTION()
	void PickedUp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerPickedUp(APrimePlayerState* PlayerState, APrimeCharacter* Character);
};
