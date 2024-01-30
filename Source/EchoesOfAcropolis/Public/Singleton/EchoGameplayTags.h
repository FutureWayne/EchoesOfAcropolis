// Copyright Echo of Acropolis. All Rights Reserved.

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
	FGameplayTag InputTag_Fire_Main;
	FGameplayTag InputTag_Fire_Side;
	FGameplayTag InputTag_Dash;

	FGameplayTag Combat_Socket_Main;
	FGameplayTag Combat_Socket_Side;

	FGameplayTag Damage;

	FGameplayTag Effect_HitReact;

	FGameplayTag Cooldown_Dash;
	
private:
	static FEchoGameplayTags GameplayTags;
};
