#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PrimePlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class PRIMEJAM_API APrimePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	virtual void BeginPlay() override;
};
