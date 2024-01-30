// Copyright Echo of Acropolis. All Rights Reserved.


#include "Equipment/EchoEquipmentManagerComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/EchoAbilitySystemComponent.h"
#include "Equipment/EchoEquipmentDefinition.h"
#include "Equipment/EchoEquipmentInstance.h"


//////////////////////////////////////////////////////////////////////
// FEchoEquipmentList

UEchoEquipmentInstance* FEchoEquipmentList::AddEntry(TSubclassOf<UEchoEquipmentDefinition> EquipmentDefinition)
{
	UEchoEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent != nullptr);
	check(OwnerComponent->GetOwner() != nullptr);

	const UEchoEquipmentDefinition* EquipmentDefinitionCDO = GetDefault<UEchoEquipmentDefinition>(EquipmentDefinition);
	TSubclassOf<UEchoEquipmentInstance> InstanceType = EquipmentDefinitionCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UEchoEquipmentInstance::StaticClass();
	}

	FEchoAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UEchoEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	if (UEchoAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const UEchoAbilitySet* AbilitySet : EquipmentDefinitionCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UEchoEquipmentManagerComponent::AddEntry: No ability system component found!"))
	}

	Result->SpawnEquipmentActors(EquipmentDefinitionCDO->ActorsToSpawn);
	
	return Result;
}

void FEchoEquipmentList::RemoveEntry(UEchoEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FEchoAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UEchoAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();

			EntryIt.RemoveCurrent();
		}
	}
}

UEchoAbilitySystemComponent* FEchoEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	const AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UEchoAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

//////////////////////////////////////////////////////////////////////
// UEchoEquipmentManagerComponent

UEchoEquipmentManagerComponent::UEchoEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
{
	EquipmentList = FEchoEquipmentList(this);
}

UEchoEquipmentInstance* UEchoEquipmentManagerComponent::EquipItem(
	TSubclassOf<UEchoEquipmentDefinition> EquipmentDefinition)
{
	UEchoEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result != nullptr)
		{
			Result->OnEquipped();
		}
	}

	return Result;
}

void UEchoEquipmentManagerComponent::UnequipItem(UEchoEquipmentInstance* EquipmentInstance)
{
	if (EquipmentInstance != nullptr)
	{
		EquipmentInstance->OnUnequipped();
		EquipmentList.RemoveEntry(EquipmentInstance);
	}
}

void UEchoEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UEchoEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UEchoEquipmentInstance*> InstancesToRemove;
	for (const FEchoAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		InstancesToRemove.Add(Entry.Instance);
	}

	for (UEchoEquipmentInstance* Instance : InstancesToRemove)
	{
		UnequipItem(Instance);
	}
	
	Super::UninitializeComponent();
}

