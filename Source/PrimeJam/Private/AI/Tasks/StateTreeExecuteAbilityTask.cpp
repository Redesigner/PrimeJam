// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Tasks/StateTreeExecuteAbilityTask.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Logging/StructuredLog.h"
#include "StateTreeExecutionContext.h"

#include "PrimeJam.h"

#define LOCTEXT_NAMESPACE "StateTree"

EStateTreeRunStatus FStateTreeExecuteAbilityTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	auto& [ActivatedAbility, AbilityEndDelegate, Tag, Actor, bRunning] = Context.GetInstanceData(*this);

	const IAbilitySystemInterface* AbilitySystem = Cast<IAbilitySystemInterface>(Actor);
	if (!AbilitySystem)
	{
		UE_LOGFMT(LogPrimeJam, Warning, "'{AIName}' Failed to execute attack: Pawn '{PawnName}' does not implement AbilitySystemInterface.",
			GetNameSafe(Context.GetOwner()), GetNameSafe(Actor));
		return EStateTreeRunStatus::Failed;
	}

	UAbilitySystemComponent* Asc = AbilitySystem->GetAbilitySystemComponent();
	if (!Asc)
	{
		UE_LOGFMT(LogPrimeJam, Warning, "'{AIName}' Failed to execute attack: Pawn '{PawnName}' does not have a valid ASC.",
			GetNameSafe(Context.GetOwner()), GetNameSafe(Actor));
		return EStateTreeRunStatus::Failed;
	}

	TArray<FGameplayAbilitySpecHandle> AbilityHandles;
	Asc->FindAllAbilitiesWithTags(AbilityHandles, Tag.GetSingleTagContainer());
	if (AbilityHandles.IsEmpty())
	{
		UE_LOGFMT(LogPrimeJam, Warning, "'{AIName}' Failed to execute attack: Pawn '{PawnName}' does not have an ability matching tag '{TagName}'.",
			GetNameSafe(Context.GetOwner()), GetNameSafe(Actor), Tag.ToString());
		return EStateTreeRunStatus::Failed;
	}

	TWeakObjectPtr OwningAsc = Asc;
	AbilityEndDelegate = Asc->OnAbilityEnded.AddLambda([this, &Context, OwningAsc](const FAbilityEndedData& AbilityEndedData)
	{
		FInstanceDataType& Data = Context.GetInstanceData(*this);
		if (AbilityEndedData.AbilitySpecHandle != Data.ActivatedAbility || !OwningAsc.IsValid())
		{
			return;
		}

		UE_LOGFMT(LogPrimeJam, Display, "STT_ExecuteAbility: Ability '{AbilityName}' ended.", AbilityEndedData.AbilityThatEnded->GetName());
		OwningAsc->OnAbilityEnded.Remove(Data.AbilityEndDelegate);
		Data.ActivatedAbility = FGameplayAbilitySpecHandle();
		Data.bRunning = false;
	});
	
	bRunning = true;
	ActivatedAbility = AbilityHandles[0];
	if (Asc->TryActivateAbility(AbilityHandles[0]))
	{
		UE_LOGFMT(LogPrimeJam, Display, "STT_ExecuteAbility: Ability '{AbilityName}' activated successfully.", GetNameSafe(Asc->FindAbilitySpecFromHandle(AbilityHandles[0])->Ability));
		return EStateTreeRunStatus::Running;
	}

	bRunning = false;
	Asc->OnAbilityEnded.Remove(AbilityEndDelegate);
	ActivatedAbility = FGameplayAbilitySpecHandle();
	return EStateTreeRunStatus::Failed;
}

void FStateTreeExecuteAbilityTask::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Super::ExitState(Context, Transition);
}

EStateTreeRunStatus FStateTreeExecuteAbilityTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	return InstanceData.bRunning ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Succeeded;
}

#if WITH_EDITOR
FText FStateTreeExecuteAbilityTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType Data = InstanceDataView.Get<FInstanceDataType>();
	
	const FText Format = (Formatting == EStateTreeNodeFormatting::RichText)
		? LOCTEXT("ExecuteAbilityRich", "<b>Execute Ability</> \"{AbilityName}\"")
		: LOCTEXT("ExecuteAbility", "ExecuteAbility \"{AbilityName}\"");

	return FText::FormatNamed(Format,
		TEXT("AbilityName"), FText::FromString(Data.AbilityTag.ToString()));
}
#endif
#undef LOCTEXT_NAMESPACE
