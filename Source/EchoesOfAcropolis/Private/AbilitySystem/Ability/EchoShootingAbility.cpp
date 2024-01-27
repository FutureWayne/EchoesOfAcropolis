// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/EchoShootingAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interface/CombatInterface.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "EchoesOfAcropolis/EchoesOfAcropolis.h"
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


void UEchoShootingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	StartShootTargeting();
}

void UEchoShootingAbility::StartShootTargeting()
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
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, CurHitResult.ImpactPoint);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, CurHitResult.ImpactPoint, FRotator::ZeroRotator);

		// Add damage to the target
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CurHitResult.GetActor()))
		{
			const FEchoGameplayTags GameplayTags = FEchoGameplayTags::Get();
			const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), SourceASC->MakeEffectContext());
			const float DamageValue = Damage.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, DamageValue);
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

int32 UEchoShootingAbility::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
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


void UEchoShootingAbility::DoSingleBulletTrace(const FVector& TraceStartPos, const FVector& TraceEndPos,

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
	
	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepSingleByChannel(/*out*/ OutHitResult, TraceStartPos, TraceEndPos, FQuat::Identity, Echo_TraceChannel_Weapon, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceSingleByChannel(/*out*/ OutHitResult, TraceStartPos, TraceEndPos, Echo_TraceChannel_Weapon, TraceParams);
	}
}

void UEchoShootingAbility::TraceBulletsInCartridge(const FRaytracingInput& RaytracingInput, TArray<FHitResult>& OutHits)
{
	for (int32 BulletIndex = 0; BulletIndex < BulletsPerCartridge; ++BulletIndex)
	{
		const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(BulletSpreadAngle * 0.5f);
		const FVector BulletDir = VRandConeNormalDistribution(RaytracingInput.TraceDir, HalfSpreadAngleInRadians, BulletSpreadExponent);
		const FVector EndTrace = RaytracingInput.TraceStartPos + (BulletDir * ShootingRange);
		
		FHitResult SingleImpact;
		DoSingleBulletTrace(RaytracingInput.TraceStartPos, EndTrace, BulletSweepRadius, false, /*out*/ SingleImpact);

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


void UEchoShootingAbility::PerformTargeting(TArray<FHitResult>& OutHits)
{
	APawn* AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);

	const FTransform TargetingTransform = GetTargetingTransform(AvatarPawn);

	FRaytracingInput RaytracingInput;
	RaytracingInput.TraceDir = TargetingTransform.GetUnitAxis(EAxis::X);
	RaytracingInput.TraceStartPos = TargetingTransform.GetTranslation();
	RaytracingInput.TraceEndPos = RaytracingInput.TraceStartPos + (RaytracingInput.TraceDir * ShootingRange);

#if ENABLE_DRAW_DEBUG
	if (EchoConsoleVariables::DrawBulletTracesDuration > 0.0f)
	{
		static float DebugThickness = 2.0f;
		DrawDebugLine(GetWorld(), RaytracingInput.TraceStartPos, RaytracingInput.TraceEndPos, FColor::Yellow, false, EchoConsoleVariables::DrawBulletTracesDuration, 0, DebugThickness);
	}
#endif

	TraceBulletsInCartridge(RaytracingInput, /*out*/ OutHits);
}

FTransform UEchoShootingAbility::GetTargetingTransform(APawn* SourcePawn) const

{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(SourcePawn);
	check(CombatInterface);

	AController* Controller = SourcePawn->GetController();
	check(Controller)

	const APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC != nullptr)
	{
		// Get player camera transform
		FVector CamLoc;
		FRotator CamRot;

		PC->GetPlayerViewPoint(/*out*/ CamLoc, /*out*/ CamRot);
		
		// Determine initial focal point based on camera
		const FVector AimDir = CamRot.Vector().GetSafeNormal();

		FVector FocalLoc = CamLoc + (AimDir * ShootingRange);

		
		// Since the camera is behind the player, move the trace start to the front of the player
		const FVector WeaponLoc = CombatInterface->GetWeaponTargetingSourceLocation();
		// Use the project point of weapon loc onto the aim dir as new cam loc
		CamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir) * AimDir);
		// Move the focal point based on new cam loc

		FocalLoc = CamLoc + (AimDir * ShootingRange);


		return FTransform((FocalLoc - CamLoc).Rotation(), CamLoc);
	}
	else
	{
		return FTransform(Controller->GetControlRotation(), CombatInterface->GetWeaponTargetingSourceLocation());
	}
}
