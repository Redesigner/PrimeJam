// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Properties/StateTreeGetTargetLocationProxyPropertyFunction.h"

#include "StateTreeExecutionContext.h"
#include "StateTreeNodeDescriptionHelpers.h"
#include "Characters/Mavericks/MaverickAIController.h"
#include "Logging/StructuredLog.h"

#define LOCTEXT_NAMESPACE "StateTree"

void FStateTreeGetTargetLocationProxyPropertyFunction::Execute(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	InstanceData.Output = InstanceData.Input->GetTargetLocationProxy();
}

#if WITH_EDITOR
FText FStateTreeGetTargetLocationProxyPropertyFunction::GetDescription(const FGuid& ID,FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup,
	EStateTreeNodeFormatting Formatting) const
{
	return UE::StateTree::DescHelpers::GetDescriptionForSingleParameterFunc<FInstanceDataType>(LOCTEXT("StateTreeGetTargetLocationProxy", "GetTargetLocationProxy"), ID, InstanceDataView, BindingLookup, Formatting);
}
#endif

#undef LOCTEXT_NAMESPACE
