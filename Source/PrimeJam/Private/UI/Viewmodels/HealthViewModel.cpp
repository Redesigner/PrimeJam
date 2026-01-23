// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Viewmodels/HealthViewModel.h"

#include "Characters/Components/HealthComponent.h"

void UHealthViewModel::BindHealthComponent(UHealthComponent* HealthComponent)
{
	if (!HealthComponent)
	{
		return;
	}
	
	UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, HealthComponent->GetHealth());
	UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, HealthComponent->GetMaxHealth());

	
	HealthComponent->OnHealthChanged.AddLambda([this](const float NewHealth)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, NewHealth);
	});
	HealthComponent->OnMaxHealthChanged.AddLambda([this](const float NewMaxHealth)
	{
		UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth);
	});
}
