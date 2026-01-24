// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DropTableComponent.generated.h"


class UDropTableAsset;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PRIMEJAM_API UDropTableComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Drops, meta = (AllowPrivateAccess))
	TObjectPtr<UDropTableAsset> DropTable;

public:
	UDropTableComponent();
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void DropItems();
};
