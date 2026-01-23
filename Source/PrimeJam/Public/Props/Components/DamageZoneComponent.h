// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "DamageZoneComponent.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent))
class PRIMEJAM_API UDamageZoneComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Damage, meta = (AllowPrivateAccess))
	float Damage = 1.0f;
	
public:
	UDamageZoneComponent();
	
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
