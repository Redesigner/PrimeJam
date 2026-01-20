// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Player/PrimePlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void APrimePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMappingContext.IsNull())
			{
				InputSystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 1);
			}
		}
	}
}