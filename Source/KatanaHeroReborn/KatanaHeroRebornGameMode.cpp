// Copyright Epic Games, Inc. All Rights Reserved.

#include "KatanaHeroRebornGameMode.h"
#include "KatanaHeroRebornCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

class AKatanaHeroRebornCharacter;

AKatanaHeroRebornGameMode::AKatanaHeroRebornGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AKatanaHeroRebornGameMode::BeginPlay()
{
	Super::BeginPlay();
	AKatanaHeroRebornCharacter* C = Cast<AKatanaHeroRebornCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	baseMaxSpeed = C->GetCharacterMovement()->MaxWalkSpeed;
}

void AKatanaHeroRebornGameMode::OnHealthUpgrade()
{
	AKatanaHeroRebornCharacter* C = Cast<AKatanaHeroRebornCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	C->MaxHealth = HealthUpgrade;
	C->Health = HealthUpgrade;
}

void AKatanaHeroRebornGameMode::OnDamageUpgrade()
{
	AKatanaHeroRebornCharacter* C = Cast<AKatanaHeroRebornCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	C->KatanaDamage = (15*(StrengthUpgrade/100));
}

void AKatanaHeroRebornGameMode::OnSpeedUpgrade()
{
	AKatanaHeroRebornCharacter* C = Cast<AKatanaHeroRebornCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	C->GetCharacterMovement()->MaxWalkSpeed = (baseMaxSpeed*(SpeedUpgrade/100));
}

void AKatanaHeroRebornGameMode::OnManaUpgrade()
{
	AKatanaHeroRebornCharacter* C = Cast<AKatanaHeroRebornCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	C->MaxMana = ManaUpgrade;
	C->Mana = ManaUpgrade;
}

void AKatanaHeroRebornGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SpeedrunTime += DeltaSeconds;
}
