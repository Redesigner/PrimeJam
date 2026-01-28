// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_LastPerceivedActor.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEJAM_API UEnvQueryContext_LastPerceivedActor : public UEnvQueryContext
{
	GENERATED_BODY()
	
	void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
