// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EchoGameplayAbility.h"
#include "EchoDashAbility.generated.h"

class UAbilityTask_ApplyRootMotionConstantForce;
class UAbilityTask_PlayMontageAndWait;

enum class EDashDirection
{
	Forward,
	Backward,
	Left,
	Right
};

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API UEchoDashAbility : public UEchoGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnDashCancelled();

	UFUNCTION()
	void OnDashCompleted();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float Strength = 1850.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float RootMotionDuration = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float AbilityDuration = 0.55f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	UAnimMontage* DashAnimMontage;

	UAbilityTask_PlayMontageAndWait* PlayDashMontage();

	UAbilityTask_ApplyRootMotionConstantForce* ApplyDashForce();
	
private:
	static EDashDirection FindDashDirection(const FVector& FacingDirection, const FVector& LastMovementDirection);
	
	FVector Direction;
};
