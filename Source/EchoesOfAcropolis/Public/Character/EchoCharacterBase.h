// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "NinjaCharacter.h"
#include "Interface/CombatInterface.h"
#include "EchoCharacterBase.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API AEchoCharacterBase : public ANinjaCharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	explicit AEchoCharacterBase(const FObjectInitializer& ObjectInitializer);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/* Begin Combat Interface */
	
	virtual FVector GetCombatAimLocation() override;

	virtual FVector GetCombatAimDirection() override;

	virtual FVector GetCombatSocketLocation_Implementation(FGameplayTag SocketTag) override;

	/* End Combat Interface */

protected:
	virtual void InitAbilityActorInfo() {}

	void InitDefaultAttributes() const;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;

	void AddCharacterAbilities() const;

	virtual void Die() override;
	
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Echo|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(EditAnywhere, Category = "Echo|Weapon")
	FName MainWeaponSocketName;

	UPROPERTY(EditAnywhere, Category = "Echo|Weapon")
	FName SideWeaponSocketName;

private:
	UPROPERTY(EditAnywhere, Category = "Echo|Ability")
	TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;
};
