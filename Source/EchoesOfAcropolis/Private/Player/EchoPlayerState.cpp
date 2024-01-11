// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EchoPlayerState.h"

#include "AbilitySystem/EchoAbilitySystemComponent.h"
#include "AbilitySystem/EchoAttributeSet.h"

AEchoPlayerState::AEchoPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UEchoAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UEchoAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AEchoPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
