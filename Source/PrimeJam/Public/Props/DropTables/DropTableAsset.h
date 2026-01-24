// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DropTableAsset.generated.h"


class APickup;

USTRUCT(BlueprintType)
struct PRIMEJAM_API FDropTableAssetEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APickup> Pickup;
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float DropChance = 1.0f;
};

UCLASS()
class PRIMEJAM_API UDropTableAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDropTableAssetEntry> Entries;
	
	UFUNCTION(BlueprintCallable)
	TArray<TSubclassOf<APickup>> MakeRandomDrops() const;
};
