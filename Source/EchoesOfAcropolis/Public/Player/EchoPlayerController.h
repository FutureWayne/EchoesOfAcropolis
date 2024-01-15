// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EchoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API AEchoPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AEchoPlayerController();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	virtual void SetupInputComponent() override;

	void Jump();

	void StopJumping();

	void MoveForward(float Value);
	
	void MoveRight(float Value);
	
	void TurnAtRate(float Rate);
	
	void LookUpAtRate(float Rate);
};
