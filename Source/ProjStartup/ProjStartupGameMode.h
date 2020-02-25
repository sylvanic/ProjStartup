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
	
	std::vector<AProjStartupBall*> players;

	UPROPERTY()
	int maxPlayerCount = 4;

private:
	void SpawnPlayers();
};



