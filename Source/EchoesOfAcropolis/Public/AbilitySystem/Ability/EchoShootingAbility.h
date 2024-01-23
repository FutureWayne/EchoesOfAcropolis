// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EchoGameplayAbility.h"
#include "EchoShootingAbility.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API UEchoShootingAbility : public UEchoGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Echo|Shooting Ability")
	UGameplayEffect* DamageEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Echo|Shooting Ability")
	float ShootingRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Echo|Shooting Ability")
	float BulletSpreadAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Echo|Shooting Ability")
	float BulletSpreadExponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Echo|Shooting Ability")
	float BulletSweepRadius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Echo|Shooting Ability")
	int32 BulletsPerCartridge;

protected:
	UFUNCTION(BlueprintCallable)
	void StartShootTargeting();

	static int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults);

	void DoSingleBulletTrace(const FVector& TraceStartPos, const FVector& TraceEndPos, float SweepRadius, bool bIsSimulated, FHitResult&
							 OutHitResult) const;

private:
	
	struct FRaytracingInput
	{
		// The direction of the trace if aim were perfect
		FVector TraceDir;
		
		// Start of the trace
		FVector TraceStartPos;

		// End of the trace if aim were perfect
		FVector TraceEndPos;
		

		FRaytracingInput()
			: TraceDir(ForceInitToZero)
			, TraceStartPos(ForceInitToZero)
			, TraceEndPos(ForceInitToZero)
		{
		}
	};

	FTransform GetTargetingTransform(APawn* SourcePawn) const;
	void TraceBulletsInCartridge(const FRaytracingInput& RaytracingInput, TArray<FHitResult> OutHits);
	void PerformTargeting(OUT const TArray<FHitResult>& OutHits);
};
