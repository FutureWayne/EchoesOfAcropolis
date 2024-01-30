// Copyright Echo of Acropolis. All Rights Reserved.


#include "Equipment/EchoEquipmentInstance.h"

#include "Equipment/EchoEquipmentDefinition.h"
#include "GameFramework/Character.h"

UEchoEquipmentInstance::UEchoEquipmentInstance()
{
}

UWorld* UEchoEquipmentInstance::GetWorld() const
{
	if (const APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

APawn* UEchoEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void UEchoEquipmentInstance::SpawnEquipmentActors(const TArray<FEchoEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachToComponent = OwningPawn->GetRootComponent();
		if (const ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachToComponent = Char->GetMesh();
		}

		for (const FEchoEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			NewActor->AttachToComponent(AttachToComponent, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void UEchoEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UEchoEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UEchoEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}
