// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mavericks/MaverickAIController.h"

#include "AI/AIGameplayTags.h"
#include "Components/StateTreeAIComponent.h"
#include "Logging/StructuredLog.h"
#include "Perception/AIPerceptionComponent.h"

AMaverickAIController::AMaverickAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTree"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
}

void AMaverickAIController::BeginPlay()
{
	Super::BeginPlay();
	
	StateTreeComponent->StartLogic();
	AIPerception->OnTargetPerceptionInfoUpdated.AddUniqueDynamic(this, &ThisClass::TargetPerceptionInfoUpdated);
	AIPerception->OnTargetPerceptionForgotten.AddUniqueDynamic(this, &ThisClass::TargetPerceptionForgotten);

	TargetLocationProxy = GetWorld()->SpawnActor(TargetProxyClass);
}

void AMaverickAIController::Destroyed()
{
	Super::Destroyed();
	
	if (TargetLocationProxy.IsValid())
	{
		TargetLocationProxy->Destroy();
	}
}

AActor* AMaverickAIController::GetTargetLocationProxy() const
{
	return TargetLocationProxy.Get();
}

void AMaverickAIController::TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if (UpdateInfo.Stimulus.IsActive())
	{
		StateTreeComponent->SendStateTreeEvent(AIGameplayTags::StateTree_Perception_Sensed, FConstStructView::Make(UpdateInfo.Stimulus));
	}
}

void AMaverickAIController::TargetPerceptionForgotten(AActor* Actor)
{
	if (!AIPerception)
	{
		return;
	}

	TArray<AActor*> PerceivedActors;
	AIPerception->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);
	// We're still perceiving some actors, so don't do anything
	if (!PerceivedActors.IsEmpty())
	{
		return;
	}

	AIPerception->GetKnownPerceivedActors(nullptr, PerceivedActors);
	StateTreeComponent->SendStateTreeEvent(AIGameplayTags::StateTree_Perception_Forgotten);
}
