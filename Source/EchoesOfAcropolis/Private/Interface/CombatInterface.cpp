// Copyright Echo of Acropolis. All Rights Reserved.


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

void ICombatInterface::Die()
{
}
