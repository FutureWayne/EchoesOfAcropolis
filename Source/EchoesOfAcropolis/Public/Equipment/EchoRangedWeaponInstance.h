// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Equipment/EchoEquipmentInstance.h"
#include "EchoRangedWeaponInstance.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API UEchoRangedWeaponInstance : public UEchoEquipmentInstance
{
	GENERATED_BODY()

public:
	UEchoRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FORCEINLINE int32 GetBulletsPerCartridge() const { return BulletsPerCartridge; }

	FORCEINLINE float GetSpreadExponent() const { return SpreadExponent; }

	FORCEINLINE float GetSpreadAngle() const { return SpreadAngle; }

	FORCEINLINE float GetMaxDamageRange() const { return MaxDamageRange; }

	FORCEINLINE float GetBulletTraceSweepRadius() const { return BulletTraceSweepRadius; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.1), Category="Weapon Config")
	float SpreadExponent = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.0, ClampMax=180.0), Category="Weapon Config")
	float SpreadAngle = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Config")
	int32 BulletsPerCartridge = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Config", meta=(ForceUnits=cm))
	float MaxDamageRange = 25000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Config", meta=(ForceUnits=cm))
	float BulletTraceSweepRadius = 0.0f;
};
