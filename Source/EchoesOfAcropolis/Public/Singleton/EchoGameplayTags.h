// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */


struct FEchoGameplayTags
{
	static const FEchoGameplayTags& Get()
	{
		return GameplayTags;
	}

	static void InitializeGameplayTags();

	FGameplayTag InputTag_Aim;
	FGameplayTag InputTag_Fire;
	FGameplayTag InputTag_Dash;

	FGameplayTag Damage;

	FGameplayTag Effect_HitReact;

	FGameplayTag Cooldown_Dash;
	
private:
	static FEchoGameplayTags GameplayTags;
};
