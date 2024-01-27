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
	UFUNCTION(BlueprintCallable, Category = "Echo|Ability")
	AController* GetControllerFromActorInfo() const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Echo|Ability")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;
};
