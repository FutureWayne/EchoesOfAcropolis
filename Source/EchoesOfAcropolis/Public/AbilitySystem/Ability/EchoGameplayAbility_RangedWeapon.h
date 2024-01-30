// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EchoGameplayAbility_FromEquip.h"
#include "Equipment/EchoRangedWeaponInstance.h"
#include "EchoGameplayAbility_RangedWeapon.generated.h"

class UNiagaraSystem;
class USoundBase;

/** Defines where an ability starts its trace from and where it should face */
UENUM(BlueprintType)
enum class EEchoAbilityTargetingSource : uint8
{
	// From the player's camera towards camera focus
	CameraTowardsFocus,
	// From the weapon's muzzle or location, towards camera focus
	WeaponTowardsFocus,
};

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API UEchoGameplayAbility_RangedWeapon : public UEchoGameplayAbility_FromEquip
{
	GENERATED_BODY()

public:
	//~UGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	//~End of UGameplayAbility interface

	UFUNCTION(BlueprintCallable, Category = "Echo|Ability")
	UEchoRangedWeaponInstance* GetWeaponInstance() const;
	
	/* Ability System Parameters */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shooting Ability|Ability System")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shooting Ability|Ability System")
	FGameplayTag WeaponSourceSocketTag;

	/* VFX & SFX Parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooting Ability|Effects")
	UNiagaraSystem* TrailEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooting Ability|Effects")
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooting Ability|Effects")
	USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooting Ability|Effects")
	USoundBase* ShootingSound;

protected:
	UFUNCTION(BlueprintCallable)
	void StartShootTargeting();

	static int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults);

	void DoSingleBulletTrace(const FVector& TraceStartPos, const FVector& TraceEndPos, float SweepRadius, bool bIsSimulated, FHitResult&
							 OutHitResult) const;

private:
	struct FRangeWeaponShootingInput
	{
		// The direction of the trace if aim were perfect
		FVector TraceDir;
		
		// Start of the trace
		FVector TraceStartPos;

		// End of the trace if aim were perfect
		FVector TraceEndPos;

		// The weapon instance / source of weapon data
		UEchoRangedWeaponInstance* WeaponData = nullptr;

		FRangeWeaponShootingInput()
			: TraceDir(ForceInitToZero)
			, TraceStartPos(ForceInitToZero)
			, TraceEndPos(ForceInitToZero)
		{
		}
	};


	FTransform GetTargetingTransform(APawn* SourcePawn, EEchoAbilityTargetingSource Source) const;
	void TraceBulletsInCartridge(const FRangeWeaponShootingInput& ShootingInput, TArray<FHitResult>& OutHits) const;
	void PerformTargeting(OUT TArray<FHitResult>& OutHits) const;

};
