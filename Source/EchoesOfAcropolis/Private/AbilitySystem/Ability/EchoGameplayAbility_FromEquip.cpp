// Copyright Echo of Acropolis. All Rights Reserved.


#include "AbilitySystem/Ability/EchoGameplayAbility_FromEquip.h"

#include "AbilitySystemComponent.h"
#include "Equipment/EchoEquipmentInstance.h"

UEchoGameplayAbility_FromEquip::UEchoGameplayAbility_FromEquip(const FObjectInitializer& ObjectInitializer)
{
}

UEchoEquipmentInstance* UEchoGameplayAbility_FromEquip::GetAssociatedEquipmentInstance() const
{
	const FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	if (Spec)
	 {
	 	return Cast<UEchoEquipmentInstance>(Spec->SourceObject);
	 }
	
	return nullptr;
}

#if WITH_EDITOR
EDataValidationResult UEchoGameplayAbility_FromEquip::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = Super::IsDataValid(ValidationErrors);

	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		ValidationErrors.Add(NSLOCTEXT("Echo", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}
	
	return Result;
}
#endif
