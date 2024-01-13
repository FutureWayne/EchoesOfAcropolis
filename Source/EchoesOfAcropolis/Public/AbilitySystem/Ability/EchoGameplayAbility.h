// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EchoGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API UEchoGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag StartupInputTag;
};
