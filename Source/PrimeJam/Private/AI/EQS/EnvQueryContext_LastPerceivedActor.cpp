// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/EnvQueryContext_LastPerceivedActor.h"

#include "AIController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "Perception/AIPerceptionComponent.h"

void UEnvQueryContext_LastPerceivedActor::ProvideContext(FEnvQueryInstance& QueryInstance,
														 FEnvQueryContextData& ContextData) const
{
	AAIController* Controller = Cast<AAIController>(QueryInstance.Owner.Get());
	if (!Controller)
	{
		UE_LOGFMT(LogTemp, Warning, "Controller missing");
		return;
	}

	if (!Controller->GetAIPerceptionComponent())
	{
		UE_LOGFMT(LogTemp, Warning, "Perception missing");
		return;
	}

	TArray<AActor*> PerceivedActors;
	Controller->GetAIPerceptionComponent()->GetKnownPerceivedActors(nullptr, PerceivedActors);
	if (PerceivedActors.IsEmpty() || !PerceivedActors.IsValidIndex(0))
	{
		UE_LOGFMT(LogTemp, Warning, "No perceived actors");
		return;
	}
	
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, PerceivedActors[0]);
}
