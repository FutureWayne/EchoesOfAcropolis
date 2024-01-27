// Fill out your copyright notice in the Description page of Project Settings.


#include "Singleton/EchoAssetManager.h"

#include "Singleton/EchoGameplayTags.h"

UEchoAssetManager& UEchoAssetManager::Get()
{
	check(GEngine);
	UEchoAssetManager* EchoAssetManager = Cast<UEchoAssetManager>(GEngine->AssetManager);
	return *EchoAssetManager;
}

void UEchoAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FEchoGameplayTags::InitializeGameplayTags();
}
