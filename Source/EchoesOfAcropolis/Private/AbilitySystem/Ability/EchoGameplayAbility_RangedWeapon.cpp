// Copyright Echo of Acropolis. All Rights Reserved.


#include "AbilitySystem/Ability/EchoGameplayAbility_RangedWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Interface/CombatInterface.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "EchoesOfAcropolis/EchoesOfAcropolis.h"
#include "Equipment/EchoRangedWeaponInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Singleton/EchoGameplayTags.h"

namespace EchoConsoleVariables
{
	static float DrawBulletTracesDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBulletTraceDuration(
		TEXT("Echo.Weapon.DrawBulletTraceDuration"),
		DrawBulletTracesDuration,
		TEXT("Should we do debug drawing for bullet traces (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawBulletHitDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBulletHits(
		TEXT("Echo.Weapon.DrawBulletHitDuration"),
		DrawBulletHitDuration,
		TEXT("Should we do debug drawing for bullet impacts (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawBulletHitRadius = 3.0f;
	static FAutoConsoleVariableRef CVarDrawBulletHitRadius(
		TEXT("Echo.Weapon.DrawBulletHitRadius"),
		DrawBulletHitRadius,
		TEXT("When bullet hit debug drawing is enabled (see DrawBulletHitDuration), how big should the hit radius be? (in uu)"),
		ECVF_Default);
}

//////////////////////////////////////////////////////////////////////

FVector VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent)
{
	if (ConeHalfAngleRad > 0.f)
	{
		const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);
		
		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
		const float AngleAround = FMath::FRand() * 360.0f;

		const FRotator Rot = Dir.Rotation();
		const FQuat DirQuat(Rot);
		const FQuat FromCenterQuat(FRotator(0.0f, AngleFromCenter, 0.0f));
		const FQuat AroundQuat(FRotator(0.0f, 0.0, AngleAround));
		FQuat FinalDirectionQuat = DirQuat * AroundQuat * FromCenterQuat;
		FinalDirectionQuat.Normalize();

		return FinalDirectionQuat.RotateVector(FVector::ForwardVector);
	}
	else
	{
		return Dir.GetSafeNormal();
	}
}


void UEchoGameplayAbility_RangedWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UEchoGameplayAbility_RangedWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bRet = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	//TODO: Check associated weapon instance, need to figure out how to get ability spec before ability activates.
	if (bRet)
	{
		if (GetWeaponInstance() == nullptr)
		{
			if (GetWeaponInstance() == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Weapon ability %s cannot be activated because there is no associated ranged weapon (equipment instance=%s but needs to be derived from %s)"),
				*GetPathName(),
				*GetPathNameSafe(GetAssociatedEquipmentInstance()),
				*UEchoRangedWeaponInstance::StaticClass()->GetName());
				bRet = false;
			}
		}
	}

	return bRet;
}

UEchoRangedWeaponInstance* UEchoGameplayAbility_RangedWeapon::GetWeaponInstance() const
{
	return Cast<UEchoRangedWeaponInstance>(GetAssociatedEquipmentInstance());
}

void UEchoGameplayAbility_RangedWeapon::StartShootTargeting()
{
	check(CurrentActorInfo)

	const AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	check(AvatarActor);

	const UAbilitySystemComponent* SourceASC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(SourceASC);

	const AController* Controller = GetControllerFromActorInfo();
	check(Controller);

	TArray<FHitResult> OutHits;
	PerformTargeting(/*out*/ OutHits);

	for (const FHitResult& CurHitResult : OutHits)
	{
		// Add damage to the target
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CurHitResult.GetActor()))
		{
			const FEchoGameplayTags GameplayTags = FEchoGameplayTags::Get();
			const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), SourceASC->MakeEffectContext());
			const float DamageValue = Damage; // TODO: Use scalable value with level
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, DamageValue);
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

int32 UEchoGameplayAbility_RangedWeapon::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
	for (int32 Idx = 0; Idx < HitResults.Num(); ++Idx)
	{
		const FHitResult& CurHitResult = HitResults[Idx];
		AActor* HitActor = CurHitResult.GetActor();
		if (Cast<APawn>(HitActor) != nullptr)
		{
			// If we hit a pawn, we're good
			return Idx;
		}
		else
		{
			if ((HitActor != nullptr) && (HitActor->GetAttachParentActor() != nullptr) && (Cast<APawn>(HitActor->GetAttachParentActor()) != nullptr))
			{
				// If we hit something attached to a pawn, we're good
				return Idx;
			}
		}
	}

	return INDEX_NONE;
}


void UEchoGameplayAbility_RangedWeapon::DoSingleBulletTrace(const FVector& TraceStartPos, const FVector& TraceEndPos,

                                               float SweepRadius, bool bIsSimulated, OUT FHitResult& OutHitResult) const

{
#if ENABLE_DRAW_DEBUG
	if (EchoConsoleVariables::DrawBulletTracesDuration > 0.0f)
	{
		static float DebugThickness = 1.0f;
		DrawDebugLine(GetWorld(), TraceStartPos, TraceEndPos, FColor::Red, false, EchoConsoleVariables::DrawBulletTracesDuration, 0, DebugThickness);
	}
#endif // ENABLE_DRAW_DEBUG
	
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetAvatarActorFromActorInfo());
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	bool bHit;
	
	if (SweepRadius > 0.0f)
	{
		bHit = GetWorld()->SweepSingleByChannel(/*out*/ OutHitResult, TraceStartPos, TraceEndPos, FQuat::Identity, Echo_TraceChannel_Weapon, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		bHit = GetWorld()->LineTraceSingleByChannel(/*out*/ OutHitResult, TraceStartPos, TraceEndPos, Echo_TraceChannel_Weapon, TraceParams);
	}

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	const FVector TrailStartPos = CombatInterface->GetCombatSocketLocation_Implementation(WeaponSourceSocketTag);
	UNiagaraComponent* TrailEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, TrailEffect, TrailStartPos, FRotator::ZeroRotator);
	
	if (bHit)
	{
		TrailEffectComponent->SetVectorParameter(FName("User.Hit"), OutHitResult.ImpactPoint);
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, OutHitResult.ImpactPoint);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, OutHitResult.ImpactPoint, OutHitResult.ImpactNormal.Rotation());
	}
	else
	{
		TrailEffectComponent->SetVectorParameter(FName("User.Hit"), TraceEndPos);
	}
}

void UEchoGameplayAbility_RangedWeapon::TraceBulletsInCartridge(const FRangeWeaponShootingInput& ShootingInput, TArray<FHitResult>& OutHits) const
{
	const UEchoRangedWeaponInstance* WeaponData = ShootingInput.WeaponData;
	check(WeaponData);

	const int32 BulletsPerCartridge = WeaponData->GetBulletsPerCartridge();
	for (int32 BulletIndex = 0; BulletIndex < BulletsPerCartridge; ++BulletIndex)
	{
		const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(WeaponData->GetSpreadAngle() * 0.5f);
		const FVector BulletDir = VRandConeNormalDistribution(ShootingInput.TraceDir, HalfSpreadAngleInRadians, WeaponData->GetSpreadExponent());
		const FVector EndTrace = ShootingInput.TraceStartPos + (BulletDir * WeaponData->GetMaxDamageRange());
		
		FHitResult SingleImpact;
		DoSingleBulletTrace(ShootingInput.TraceStartPos, EndTrace, WeaponData->GetBulletTraceSweepRadius(), false, /*out*/ SingleImpact);

		const AActor* HitActor = SingleImpact.GetActor();
		if (HitActor != nullptr)
		{
#if ENABLE_DRAW_DEBUG
			if (EchoConsoleVariables::DrawBulletHitDuration > 0.0f)
			{
				DrawDebugPoint(GetWorld(), SingleImpact.ImpactPoint, EchoConsoleVariables::DrawBulletHitRadius, FColor::Red, false, EchoConsoleVariables::DrawBulletHitRadius);
			}
#endif

			OutHits.Add(SingleImpact);
		}

		if (OutHits.Num() == 0)
		{
			if (!SingleImpact.bBlockingHit)
			{
				SingleImpact.Location = EndTrace;
				SingleImpact.ImpactPoint = EndTrace;
			}

			OutHits.Add(SingleImpact);
		}
	}
}


void UEchoGameplayAbility_RangedWeapon::PerformTargeting(TArray<FHitResult>& OutHits) const
{
	APawn* AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);

	const FTransform TargetingTransform = GetTargetingTransform(AvatarPawn, EEchoAbilityTargetingSource::CameraTowardsFocus);
	UEchoRangedWeaponInstance* WeaponInstance = GetWeaponInstance();

	if (AvatarPawn && WeaponInstance)
	{
		FRangeWeaponShootingInput ShootingInput;
		ShootingInput.TraceDir = TargetingTransform.GetUnitAxis(EAxis::X);
		ShootingInput.TraceStartPos = TargetingTransform.GetTranslation();
		ShootingInput.TraceEndPos = ShootingInput.TraceStartPos + (ShootingInput.TraceDir * WeaponInstance->GetMaxDamageRange());
		ShootingInput.WeaponData = WeaponInstance;

#if ENABLE_DRAW_DEBUG
		if (EchoConsoleVariables::DrawBulletTracesDuration > 0.0f)
		{
			static float DebugThickness = 2.0f;
			DrawDebugLine(GetWorld(), ShootingInput.TraceStartPos, ShootingInput.TraceEndPos, FColor::Yellow, false, EchoConsoleVariables::DrawBulletTracesDuration, 0, DebugThickness);
		}
#endif

		TraceBulletsInCartridge(ShootingInput, /*out*/ OutHits);
	}
}

FTransform UEchoGameplayAbility_RangedWeapon::GetTargetingTransform(APawn* SourcePawn, EEchoAbilityTargetingSource Source) const
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(SourcePawn);
	check(CombatInterface);

	AController* Controller = SourcePawn->GetController();
	check(Controller)

	const APlayerController* PC = Cast<APlayerController>(Controller);

	FVector SourceLoc;
	const double FocalDistance = 1024.0f;
	FVector FocalLoc;
	FVector CamLoc;
	FRotator CamRot;

	if (PC != nullptr)
	{
		PC->GetPlayerViewPoint(/*out*/ CamLoc, /*out*/ CamRot);
	}
	else
	{
		SourceLoc = CombatInterface->GetCombatSocketLocation_Implementation(WeaponSourceSocketTag);
		CamLoc = SourceLoc;
		CamRot = Controller->GetControlRotation();
	}

	// Determine initial focal point
	const FVector AimDir = CamRot.Vector().GetSafeNormal();
	FocalLoc = CamLoc + (AimDir * FocalDistance);
	
	if (PC)
	{
		// Since the camera is behind the player, move the trace start to the front of the player
		const FVector PlayerLoc = SourcePawn->GetActorLocation();
		// Use the project point of weapon loc onto the aim dir as new cam loc
		CamLoc = FocalLoc + (((PlayerLoc - FocalLoc) | AimDir) * AimDir);
		// Move the focal point based on new cam loc
		FocalLoc = CamLoc + (AimDir * FocalDistance);
	}
	// Move the start to the head position of AI
	else if(Cast<AAIController>(Controller))
	{
		CamLoc = SourcePawn->GetActorLocation() + FVector(0.0f, 0.0f, SourcePawn->BaseEyeHeight);
	}

	if (Source == EEchoAbilityTargetingSource::CameraTowardsFocus)
	{
		return FTransform(CamRot, CamLoc);
	}
	else
	{
		SourceLoc = CombatInterface->GetCombatSocketLocation_Implementation(WeaponSourceSocketTag);
		return FTransform((FocalLoc - SourceLoc).Rotation(), SourceLoc);
	}
}
