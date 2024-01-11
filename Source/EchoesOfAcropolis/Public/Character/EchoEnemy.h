// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EchoCharacterBase.h"
#include "EchoEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API AEchoEnemy : public AEchoCharacterBase
{
	GENERATED_BODY()

public:
	explicit AEchoEnemy(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

protected:
	virtual void InitAbilityActorInfo() override;
};
