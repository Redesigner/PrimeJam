#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreePropertyRef.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "StateTreeRunEnvQueryOnceTask.generated.h"

struct FEnvQueryResult;
struct FAIDynamicParam;
class UEnvQuery;

USTRUCT()
struct PRIMEJAM_API FStateTreeRunEnvQueryOnceInstanceData
{
	GENERATED_BODY()

	// The query will be run with this actor has the owner object.
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AActor> QueryOwner = nullptr;

	// The query template to run
	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<UEnvQuery> QueryTemplate;

	// Query config associated with the query template.
	UPROPERTY(EditAnywhere, EditFixedSize, Category = Parameter)
	TArray<FAIDynamicParam> QueryConfig;

	/** determines which item will be stored (All = only first matching) */
	UPROPERTY(EditAnywhere, Category = Parameter)
	TEnumAsByte<EEnvQueryRunMode::Type> RunMode = EEnvQueryRunMode::SingleResult;

	/** To use dynamic pathfinding, set the location of an empty actor instead*/
	UPROPERTY(EditAnywhere, Category = Parameter)
	TWeakObjectPtr<AActor> TargetLocationProxy;

	TSharedPtr<FEnvQueryResult> QueryResult = nullptr;

	int32 RequestId = INDEX_NONE;	
};

// This cannot inherit from FStateTreeRunEnvQueryTask because the editor methods are not exported
USTRUCT(meta = (DisplayName = "Run Env Query Once", Category = "Query"))
struct PRIMEJAM_API FStateTreeRunEnvQueryOnceTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeRunEnvQueryOnceInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	void StartQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const;
	void CancelQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const;
	void ResetQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const;

#if WITH_EDITOR
	virtual void PostEditInstanceDataChangeChainProperty(const FPropertyChangedChainEvent& PropertyChangedEvent, FStateTreeDataView InstanceDataView) override;
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;

	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Find");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Grey;
	}
#endif // WITH_EDITOR
};