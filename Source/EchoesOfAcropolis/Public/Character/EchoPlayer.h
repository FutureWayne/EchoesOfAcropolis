// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EchoCharacterBase.h"
#include "EchoPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API AEchoPlayer : public AEchoCharacterBase
{
	GENERATED_BODY()

public:
	explicit AEchoPlayer(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "ADS")
	void SetAimingStatus(bool bNewAimingStatus);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ADS")
	float ZoomFOV;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ADS")
	float ZoomSpeed;

protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
	
	virtual void InitAbilityActorInfo() override;

	virtual FVector GetCombatAimLocation() override;

    virtual FVector GetCombatAimDirection() override;

	virtual FVector GetWeaponTargetingSourceLocation(int WeaponIndex = 0) override;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	bool bIsAiming = false;
	float DefaultFOV;
	float CurrentFOV;

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
