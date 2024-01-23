// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EchoCharacterBase.h"

#include "AbilitySystem/EchoAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

AEchoCharacterBase::AEchoCharacterBase(const FObjectInitializer& ObjectInitializer)
	: ANinjaCharacter(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAbilitySystemComponent* AEchoCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FVector AEchoCharacterBase::GetCombatAimLocation()
{
	return ICombatInterface::GetCombatAimLocation();
}

FVector AEchoCharacterBase::GetCombatAimDirection()
{
	return ICombatInterface::GetCombatAimDirection();
}

FVector AEchoCharacterBase::GetWeaponTargetingSourceLocation(int WeaponIndex)
{
	return ICombatInterface::GetWeaponTargetingSourceLocation(WeaponIndex);
}

void AEchoCharacterBase::AddCharacterAbilities()
{
	UEchoAbilitySystemComponent* EchoAbilitySystemComponent = Cast<UEchoAbilitySystemComponent>(AbilitySystemComponent);
	if (EchoAbilitySystemComponent)
	{
		EchoAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	}
}
