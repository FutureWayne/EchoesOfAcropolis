// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
FVector ICombatInterface::GetCombatAimLocation()
{
	return FVector();
}

FVector ICombatInterface::GetCombatAimDirection()
{
	return FVector();
}

FVector ICombatInterface::GetWeaponTargetingSourceLocation(int WeaponIndex)
{
	return FVector();
}

void ICombatInterface::Die()
{
}
