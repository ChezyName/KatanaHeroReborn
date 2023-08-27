// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "KatanaHeroRebornCharacter.generated.h"

UCLASS(config=Game)
class AKatanaHeroRebornCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Katana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* KatanaHitbox;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);
	void LookUp(float Val);
	FVector2D DashDir;
	void StartJump();
	bool StopMovement = false;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	//Abilities / Combat
	void KatanaAttack();
	bool Attacking = false;

	void Dash();

	void LookAtMouse();
	void StopLookingAtMouse();

public:
	AKatanaHeroRebornCharacter();

	FVector GetMouseLocation();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(BlueprintReadOnly)
	bool SwordActive = false;

	float DashCD = 0;
	bool Dashing = false;
	float X;

	void DisplayAfterImages();

	UPROPERTY(BlueprintReadOnly)
	int Health = 3;

	void TakeDamage();

	//Primary Sword Attack
	UPROPERTY(EditAnywhere,Category="Yasuo")
	TSubclassOf<AActor> AfterImages;
	UPROPERTY(EditAnywhere,Category="Yasuo|MainAttack")
	UAnimMontage* SwordAttachAnimation;
	UPROPERTY(EditAnywhere,Category="Yasuo|MainAttack")
	float HitboxActiveT = 0.1;
	UPROPERTY(EditAnywhere,Category="Yasuo|MainAttack")
	float DashSpeed = 25;
	UPROPERTY(EditAnywhere,Category="Yasuo|Dash")
	float DodgeSpeed = 35;
	UPROPERTY(EditAnywhere,Category="Yasuo|Dash")
	float DashTime = 0.2;

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySlashVFX();
};
