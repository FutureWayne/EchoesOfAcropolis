// Copyright Echo of Acropolis. All Rights Reserved.


#include "Singleton/EchoGameplayTags.h"

#include "GameplayTagsManager.h"

FEchoGameplayTags FEchoGameplayTags::GameplayTags;

void FEchoGameplayTags::InitializeGameplayTags()
{
	/*
	 * Input Tags
	 */

	GameplayTags.InputTag_Aim = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Aim"),
		FString("Input Tag for Aiming")
		);

	GameplayTags.InputTag_Fire_Main = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Fire.Main"),
		FString("Input Tag for Firing Main Weapon")
		);

	GameplayTags.InputTag_Fire_Side = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Fire.Side"),
		FString("Input Tag for Firing Side Weapon")
		);

	GameplayTags.InputTag_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Dash"),
		FString("Input Tag for Dashing")
		);

	/*
	 * Combat Tags
	 */

	GameplayTags.Combat_Socket_Main = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Combat.Socket.Main"),
		FString("Combat Tag for Main Socket")
		);

	GameplayTags.Combat_Socket_Side = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Combat.Socket.Side"),
		FString("Combat Tag for Side Socket")
		);

	/* 
	 * Damage Tags
	 */

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage Tag")
		);

	/*
	 * Effect Tags
	 */
	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.HitReact"),
		FString("Effect Tag for Hit React")
		);

	/*
	 * Cooldown Tags
	 */
	GameplayTags.Cooldown_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Dash"),
		FString("Cooldown Tag for Dash")
		);
}
