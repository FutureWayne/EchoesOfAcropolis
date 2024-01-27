// Fill out your copyright notice in the Description page of Project Settings.


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

	GameplayTags.InputTag_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Fire"),
		FString("Input Tag for Firing")
		);

	GameplayTags.InputTag_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Dash"),
		FString("Input Tag for Dashing")
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
}
