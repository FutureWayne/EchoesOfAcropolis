// Copyright Echo of Acropolis. All Rights Reserved.

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

	UFUNCTION(BlueprintCallable, Category = "Ability|ADS")
	void SetAimingStatus(bool bNewAimingStatus);

	UFUNCTION(BlueprintCallable, Category = "Ability|Dash")
	void SetDashStatus(bool bNewDashingStatus);

	UFUNCTION(BlueprintCallable, Category = "Ability|Dash")
	void ResetDashCooldown();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Dash")
	void OnDashCooldownReset();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability|ADS")
	float ZoomFOV;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability|ADS")
	float ZoomSpeed;

protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
	
	virtual void InitAbilityActorInfo() override;

	virtual FVector GetCombatAimLocation() override;

    virtual FVector GetCombatAimDirection() override;

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
