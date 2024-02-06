// Copyright Echo of Acropolis. All Rights Reserved.


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
