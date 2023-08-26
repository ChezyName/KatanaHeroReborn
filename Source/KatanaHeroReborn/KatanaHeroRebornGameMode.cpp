// Copyright Epic Games, Inc. All Rights Reserved.

#include "KatanaHeroRebornGameMode.h"
#include "KatanaHeroRebornCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKatanaHeroRebornGameMode::AKatanaHeroRebornGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
