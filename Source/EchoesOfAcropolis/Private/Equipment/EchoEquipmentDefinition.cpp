// Copyright Echo of Acropolis. All Rights Reserved.


#include "Equipment/EchoEquipmentDefinition.h"
#include "Equipment/EchoEquipmentInstance.h"

UEchoEquipmentDefinition::UEchoEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
{
	InstanceType = UEchoEquipmentInstance::StaticClass();
}
