// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EchoGameplayAbility.h"
#include "EchoGameplayAbility_FromEquip.generated.h"

class UEchoEquipmentInstance;
/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API UEchoGameplayAbility_FromEquip : public UEchoGameplayAbility
{
	GENERATED_BODY()

public:
	UEchoGameplayAbility_FromEquip(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Echo|Ability")
	UEchoEquipmentInstance* GetAssociatedEquipmentInstance() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
};
