// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "EchoAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API UEchoAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UEchoAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};
