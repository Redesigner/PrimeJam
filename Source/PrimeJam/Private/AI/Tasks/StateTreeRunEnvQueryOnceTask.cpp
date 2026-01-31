#include "AI/Tasks/StateTreeRunEnvQueryOnceTask.h"

#include "Characters/Mavericks/MaverickAIController.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Tasks/StateTreeRunEnvQueryTask.h"

#define LOCTEXT_NAMESPACE "StateTree"

EStateTreeRunStatus FStateTreeRunEnvQueryOnceTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.QueryTemplate)
	{
		return EStateTreeRunStatus::Failed;
	}

	StartQuery(Context, InstanceData);
	return InstanceData.RequestId != INDEX_NONE ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FStateTreeRunEnvQueryOnceTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);	
	if (!InstanceData.QueryResult)
	{
		return EStateTreeRunStatus::Running;
	}
	
	if (!InstanceData.QueryResult->IsSuccessful())
	{
		return EStateTreeRunStatus::Running;
	}
	
	FVector Result = InstanceData.QueryResult->GetItemAsLocation(0);
	if (AMaverickAIController* AIController = Cast<AMaverickAIController>(Context.GetOwner()))
	{
		if (AIController->GetTargetLocationProxy())
		{
			AIController->GetTargetLocationProxy()->SetActorLocation(Result);
		}
	}
	return EStateTreeRunStatus::Succeeded;
}


void FStateTreeRunEnvQueryOnceTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	CancelQuery(Context, Context.GetInstanceData(*this));
}

void FStateTreeRunEnvQueryOnceTask::StartQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const
{
	AActor* Owner = InstanceData.QueryOwner;
	FEnvQueryRequest Request(InstanceData.QueryTemplate, Owner);

	for (FAIDynamicParam& DynamicParam : InstanceData.QueryConfig)
	{
		Request.SetDynamicParam(DynamicParam, nullptr);
	}

	InstanceData.RequestId = Request.Execute(InstanceData.RunMode,
		FQueryFinishedSignature::CreateLambda([InstanceDataRef = Context.GetInstanceDataStructRef(*this)](TSharedPtr<FEnvQueryResult> QueryResult) mutable
			{
				if (FInstanceDataType* InstanceData = InstanceDataRef.GetPtr())
				{
					InstanceData->QueryResult = QueryResult;
					InstanceData->RequestId = INDEX_NONE;
				}
			}));
}

void FStateTreeRunEnvQueryOnceTask::CancelQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const
{
	if (InstanceData.RequestId != INDEX_NONE)
	{
		if (UEnvQueryManager* QueryManager = UEnvQueryManager::GetCurrent(Context.GetOwner()))
		{
			QueryManager->AbortQuery(InstanceData.RequestId);
		}
		InstanceData.RequestId = INDEX_NONE;
	}
	// InstanceData.QueryResult.Reset();
}

void FStateTreeRunEnvQueryOnceTask::ResetQuery(FStateTreeExecutionContext& Context,
	FInstanceDataType& InstanceData) const
{
	CancelQuery(Context, InstanceData);
	StartQuery(Context, InstanceData);
}

#if WITH_EDITOR
void FStateTreeRunEnvQueryOnceTask::PostEditInstanceDataChangeChainProperty(const FPropertyChangedChainEvent& PropertyChangedEvent, FStateTreeDataView InstanceDataView)
{
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(FStateTreeRunEnvQueryOnceInstanceData, QueryTemplate))
	{
		FInstanceDataType& InstanceData = InstanceDataView.GetMutable<FInstanceDataType>();
		if (InstanceData.QueryTemplate)
		{
			InstanceData.QueryTemplate->CollectQueryParams(*InstanceData.QueryTemplate, InstanceData.QueryConfig);
			for (FAIDynamicParam& DynamicParam : InstanceData.QueryConfig)
			{
				DynamicParam.bAllowBBKey = false;
			}
		}
		else
		{
			InstanceData.QueryConfig.Reset();
		}
	}
	else if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(FAIDynamicParam, bAllowBBKey))
	{
		FInstanceDataType& InstanceData = InstanceDataView.GetMutable<FInstanceDataType>();
		const int32 ChangedIndex = PropertyChangedEvent.GetArrayIndex(GET_MEMBER_NAME_CHECKED(FStateTreeRunEnvQueryInstanceData, QueryConfig).ToString());
		if (InstanceData.QueryConfig.IsValidIndex(ChangedIndex))
		{
			if (!InstanceData.QueryConfig[ChangedIndex].bAllowBBKey)
			{
				InstanceData.QueryConfig[ChangedIndex].BBKey.InvalidateResolvedKey();
			}
		}
	}
}

FText FStateTreeRunEnvQueryOnceTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);

	FText QueryTemplateValue = BindingLookup.GetBindingSourceDisplayName(FPropertyBindingPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, QueryTemplate)), Formatting);
	if (QueryTemplateValue.IsEmpty())
	{
		QueryTemplateValue = FText::FromString(GetNameSafe(InstanceData->QueryTemplate));
	}
	
	FText TargetProxyValue = BindingLookup.GetBindingSourceDisplayName(FPropertyBindingPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, TargetLocationProxy)), Formatting);
	if (TargetProxyValue.IsEmpty())
	{
		TargetProxyValue = FText::FromString(GetNameSafe(InstanceData->TargetLocationProxy.Get()));
	}
	

	if (Formatting == EStateTreeNodeFormatting::RichText)
	{
		return FText::Format(LOCTEXT("RunEQSOnceRich", "<b>Run EQS Query Once</> {0}, output to {1}"), QueryTemplateValue, TargetProxyValue);	
	}
	return FText::Format(LOCTEXT("RunEQSOnce", "Run EQS Query Once {0}, output to {1}"), QueryTemplateValue, TargetProxyValue);
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE