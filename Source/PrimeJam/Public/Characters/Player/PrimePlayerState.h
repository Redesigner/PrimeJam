// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Characters/Components/HealthComponent.h"
#include "PrimePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEJAM_API APrimePlayerState : public APlayerState, public IHealthInterface
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Components, meta = (AllowPrivateAccess))
	TObjectPtr<UHealthComponent> HealthComponent;
	
public:
	APrimePlayerState(const FObjectInitializer& ObjectInitializer);	
	
	virtual UHealthComponent* GetHealthComponent_Implementation() override;
};
