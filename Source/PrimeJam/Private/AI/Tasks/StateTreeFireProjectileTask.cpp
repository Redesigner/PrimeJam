#include "AI/Tasks/StateTreeFireProjectileTask.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "Characters/Components/GunComponent.h"

#define LOCTEXT_NAMESPACE "StateTree"

EStateTreeRunStatus FStateTreeFireProjectileTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.Pawn)
	{
		return EStateTreeRunStatus::Failed;
	}

	const UGunComponent* Gun = InstanceData.Pawn->GetComponentByClass<UGunComponent>();
	if (!Gun)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	Gun->FireProjectile();
	return EStateTreeRunStatus::Succeeded;
}

#if WITH_EDITOR
FText FStateTreeFireProjectileTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);
	
	if (Formatting == EStateTreeNodeFormatting::RichText)
	{
		return LOCTEXT("FireProjectile", "<b>Fire a projectile</>");	
	}
	return LOCTEXT("FireProjectile", "Fire a projectile");
}
#endif
#undef LOCTEXT_NAMESPACE