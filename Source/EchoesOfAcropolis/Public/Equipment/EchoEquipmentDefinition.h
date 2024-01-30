// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EchoEquipmentDefinition.generated.h"

class UEchoAbilitySet;
class UEchoGameplayAbility;
class UEchoEquipmentInstance;

USTRUCT()
struct FEchoEquipmentActorToSpawn
{
	GENERATED_BODY()

	FEchoEquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};

/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class ECHOESOFACROPOLIS_API UEchoEquipmentDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UEchoEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UEchoEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<const UEchoAbilitySet*> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FEchoEquipmentActorToSpawn> ActorsToSpawn;
};
