// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EchoCharacterBase.h"

#include "Components/CapsuleComponent.h"

AEchoCharacterBase::AEchoCharacterBase(const FObjectInitializer& ObjectInitializer)
	: ANinjaCharacter(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAbilitySystemComponent* AEchoCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
