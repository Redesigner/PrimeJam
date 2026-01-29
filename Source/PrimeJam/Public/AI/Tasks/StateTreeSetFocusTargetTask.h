#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "UObject/Object.h"
#include "StateTreeSetFocusTargetTask.generated.h"

class AAIController;

USTRUCT()
struct PRIMEJAM_API FStateTreeSetFocusTargetInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AAIController> Controller = nullptr;
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<AActor> FocusTarget = nullptr;
};

USTRUCT(meta = (DisplayName = "Set Focus Target", Category = "Common"))
struct PRIMEJAM_API FStateTreeSetFocusTargetTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeSetFocusTargetInstanceData;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
	FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Task");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Grey;
	}
#endif // WITH_EDITOR
};