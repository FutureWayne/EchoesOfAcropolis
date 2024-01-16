// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EchoPlayerController.generated.h"

struct FGameplayTag;
class UEchoInputConfig;
class UEchoAbilitySystemComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API AEchoPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UEchoAbilitySystemComponent* GetEchoAbilitySystemComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	UEchoInputConfig* EchoInputConfig;

	UPROPERTY()
	UEchoAbilitySystemComponent* EchoAbilitySystemComponent;


protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for jumping input */
	void Jump(const FInputActionValue& Value);

	/** Called for stopping jumping input */
	void StopJumping(const FInputActionValue& Value);
};
