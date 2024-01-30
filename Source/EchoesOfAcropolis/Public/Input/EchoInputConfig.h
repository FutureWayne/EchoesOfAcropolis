// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "EchoInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FEchoInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API UEchoInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
    const UInputAction* FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FEchoInputAction> AbilityInputActions;
};
