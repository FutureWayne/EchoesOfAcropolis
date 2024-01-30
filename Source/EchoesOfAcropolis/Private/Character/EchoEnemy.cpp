// Copyright Echo of Acropolis. All Rights Reserved.


#include "Character/EchoEnemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EchoAbilitySystemComponent.h"
#include "AbilitySystem/EchoAttributeSet.h"
#include "Components/WidgetComponent.h"

AEchoEnemy::AEchoEnemy(const FObjectInitializer& ObjectInitializer)
	: AEchoCharacterBase(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UEchoAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UEchoAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

void AEchoEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	// Bind attribute change callbacks
	if (const UEchoAttributeSet* EchoAttributeSet = CastChecked<UEchoAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EchoAttributeSet->GetHealthAttribute()).
		                        AddLambda([this](const FOnAttributeChangeData& Data)
		                        {
			                        OnHealthChanged.Broadcast(Data.NewValue);
		                        });

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EchoAttributeSet->GetMaxHealthAttribute()).
		                        AddLambda([this](const FOnAttributeChangeData& Data)
		                        {
			                        OnMaxHealthChanged.Broadcast(Data.NewValue);
		                        });

		// Broadcast initial values
		OnHealthChanged.Broadcast(EchoAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(EchoAttributeSet->GetMaxHealth());
	}
}

void AEchoEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UEchoAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();

	InitDefaultAttributes();
}

void AEchoEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}
