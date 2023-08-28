// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "KatanaHeroRebornGameMode.generated.h"

USTRUCT(BlueprintType)
struct FPlayerUpgrade : public FTableRowBase
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int PercentUpgrade = 100;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int GoldCost = 250;

	
};

UCLASS(minimalapi)
class AKatanaHeroRebornGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKatanaHeroRebornGameMode();

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	int HealthUpgrade = 100;

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	int StrengthUpgrade = 100;

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	int SpeedUpgrade = 100;

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	int ManaUpgrade = 100;

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	int APUpgrade = 100;

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	int PotionUpgrade = 100;

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	int GoldUpgrade = 100;

	//===============================================================================================
	
	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	FString HealthName = "1";

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	FString StrengthName = "1";

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	FString SpeedName = "1";

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	FString ManaName = "1";

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	FString APName = "1";

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	FString PotionName = "1";

	UPROPERTY(EditAnywhere,Category="Categories",BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	FString GoldName = "1";
};



