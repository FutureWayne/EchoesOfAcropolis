// Copyright Echo of Acropolis. All Rights Reserved.


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
