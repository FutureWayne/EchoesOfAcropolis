// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NinjaCharacter.h"
#include "EchoCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API AEchoCharacterBase : public ANinjaCharacter
{
	GENERATED_BODY()

public:
	explicit AEchoCharacterBase(const FObjectInitializer& ObjectInitializer);
	
};
