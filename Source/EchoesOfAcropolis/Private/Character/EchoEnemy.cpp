// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EchoEnemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EchoAbilitySystemComponent.h"

AEchoEnemy::AEchoEnemy(const FObjectInitializer& ObjectInitializer)
{
}

void AEchoEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

void AEchoEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UEchoAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();
}
