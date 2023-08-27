// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultEnemy.h"

#include "KatanaHeroRebornCharacter.h"

// Sets default values
ADefaultEnemy::ADefaultEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageHitbox = CreateDefaultSubobject<UBoxComponent>("Damage Hitbox");
	DamageHitbox->SetupAttachment(RootComponent);
	DamageHitbox->SetGenerateOverlapEvents(true);
	DamageHitbox->BodyInstance.SetCollisionProfileName("OverlapAll");
	DamageHitbox->OnComponentBeginOverlap.AddDynamic(this, &ADefaultEnemy::OnHit);
}

// Called when the game starts or when spawned
void ADefaultEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefaultEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADefaultEnemy::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,OtherActor->GetName());
	AKatanaHeroRebornCharacter* Yasuo = Cast<AKatanaHeroRebornCharacter>(OtherActor);
	if(Yasuo)
	{
		//Deal Damage To Yasuo
		Yasuo->TakeDamage();
	}
}

// Called to bind functionality to input
void ADefaultEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

