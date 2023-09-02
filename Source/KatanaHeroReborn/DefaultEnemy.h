// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "DefaultEnemy.generated.h"

UCLASS()
class KATANAHEROREBORN_API ADefaultEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultEnemy();

	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true))
	UBoxComponent* DamageHitbox;

	UPROPERTY(EditAnywhere,Category="Coins")
	TSubclassOf<AActor> Coin;
	UPROPERTY(EditAnywhere,Category="Coins")
	int CoinsDropped = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,Category="Enemy")
	float Damage = 12;
	UPROPERTY(EditAnywhere,Category="Enemy",BlueprintReadOnly)
	float MaxHealth = 25;

	UPROPERTY(BlueprintReadWrite)
	bool CanTakeDamage = true;

	UPROPERTY(BlueprintReadWrite)
	bool AlrHalfHealth = false;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHalfHealth();

	UPROPERTY(BlueprintReadOnly)
	float Health = 0;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void TakeSomeDamage(float _Damage);
};
