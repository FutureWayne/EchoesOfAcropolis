// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EchoGameplayAbility.h"
#include "EchoDashAbility.generated.h"

class UNiagaraSystem;
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
	
	UAbilityTask_PlayMontageAndWait* PlayDashMontage();

	UAbilityTask_ApplyRootMotionConstantForce* ApplyDashForce();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float Strength = 1850.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float RootMotionDuration = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float AbilityDuration = 0.55f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	UAnimMontage* DashAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	UNiagaraSystem* DashEffect;

private:
	static EDashDirection FindDashDirection(const FVector& FacingDirection, const FVector& LastMovementDirection);

	void PlayDashEffect() const;
	
	FVector Direction;
};
