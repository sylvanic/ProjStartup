// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ProjStartupBall.h"
#include <vector>
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjStartupGameMode.generated.h"
UCLASS(minimalapi)
class AProjStartupGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjStartupGameMode();
	virtual void BeginPlay() override;
	
	std::vector<AProjStartupBall*> Players;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Var")
	int ScorePlayer1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Var")
	int ScorePlayer2;

	bool HasInitialized = false;
private:
	void SpawnPlayers();
};

