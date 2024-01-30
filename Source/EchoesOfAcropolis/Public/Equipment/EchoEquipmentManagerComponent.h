// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/EchoAbilitySet.h"
#include "Components/ActorComponent.h"
#include "EchoEquipmentManagerComponent.generated.h"


struct FEchoEquipmentList;
class UEchoEquipmentManagerComponent;
class UEchoAbilitySystemComponent;
class UEchoEquipmentDefinition;
class UEchoEquipmentInstance;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FEchoAppliedEquipmentEntry
{
	GENERATED_BODY()

	FEchoAppliedEquipmentEntry()
	{}

private:
	friend FEchoEquipmentList;
	friend UEchoEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UEchoEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	UEchoEquipmentInstance* Instance = nullptr;
	
	UPROPERTY()
	FEchoAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FEchoEquipmentList
{
	GENERATED_BODY()

	FEchoEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FEchoEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	UEchoEquipmentInstance* AddEntry(TSubclassOf<UEchoEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UEchoEquipmentInstance* Instance);

private:
	UEchoAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UEchoEquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FEchoAppliedEquipmentEntry> Entries;

	UPROPERTY()
	UActorComponent* OwnerComponent;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOESOFACROPOLIS_API UEchoEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEchoEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	UEchoEquipmentInstance* EquipItem(TSubclassOf<UEchoEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable)
	void UnequipItem(UEchoEquipmentInstance* EquipmentInstance);

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	//~End of UActorComponent interface

private:
	UPROPERTY()
	FEchoEquipmentList EquipmentList;
};
