// Copyright Echo of Acropolis. All Rights Reserved.


#include "Character/EchoCharacterBase.h"

#include "AbilitySystem/EchoAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Singleton/EchoGameplayTags.h"

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

FVector AEchoCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag SocketTag)
{
	const FEchoGameplayTags& GameplayTags = FEchoGameplayTags::Get();
	if (SocketTag.MatchesTagExact(GameplayTags.Combat_Socket_Main))
	{
		return GetMesh()->GetSocketLocation(MainWeaponSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.Combat_Socket_Side))
	{
		return GetMesh()->GetSocketLocation(SideWeaponSocketName);
	}
	
	return GetActorLocation();
}


void AEchoCharacterBase::InitDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AEchoCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> EffectClass, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void AEchoCharacterBase::AddCharacterAbilities() const
{
	UEchoAbilitySystemComponent* EchoAbilitySystemComponent = Cast<UEchoAbilitySystemComponent>(AbilitySystemComponent);
	if (EchoAbilitySystemComponent)
	{
		EchoAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	}
}

void AEchoCharacterBase::Die()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);
}