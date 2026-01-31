// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "MaverickAIController.generated.h"

struct FActorPerceptionUpdateInfo;
class UStateTreeAIComponent;

UCLASS()
class PRIMEJAM_API AMaverickAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UStateTreeAIComponent> StateTreeComponent;
	
	/// Actor to be used as the destination for the state tree
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Desination, Meta = (AllowPrivateAccess))
	TSubclassOf<AActor> TargetProxyClass;
	
public:
	AMaverickAIController();

	virtual void BeginPlay() override;
	
	virtual void Destroyed() override;

	AActor* GetTargetLocationProxy() const;
	
private:
	UFUNCTION()
	void TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);

	UFUNCTION()
	void TargetPerceptionForgotten(AActor* Actor);

	TWeakObjectPtr<AActor> TargetLocationProxy;
};
