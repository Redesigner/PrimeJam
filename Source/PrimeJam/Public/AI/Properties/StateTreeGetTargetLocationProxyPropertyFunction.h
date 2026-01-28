// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StateTreePropertyFunctionBase.h"
#include "CoreMinimal.h"
#include "StateTreeGetTargetLocationProxyPropertyFunction.generated.h"

class AMaverickAIController;

USTRUCT()
struct PRIMEJAM_API FStateTreeGetTargetLocationProxyPropertyFunctionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<AMaverickAIController> Input = nullptr;

	UPROPERTY(EditAnywhere, Category = Output)
	TObjectPtr<AActor> Output = nullptr;
};

USTRUCT(DisplayName = "Get Target Location Proxy")
struct PRIMEJAM_API FStateTreeGetTargetLocationProxyPropertyFunction : public FStateTreePropertyFunctionCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FStateTreeGetTargetLocationProxyPropertyFunctionInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual void Execute(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const;
#endif
};