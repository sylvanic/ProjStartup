// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjStartupGameMode.h"
#include "ProjStartupBall.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerStart.h"
#include "Misc/Timespan.h"
#include "Templates/Casts.h"

AProjStartupGameMode::AProjStartupGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerStateClass = nullptr;
	GameStateClass = nullptr;
	SpectatorClass = nullptr;
}

void AProjStartupGameMode::BeginPlay() {
	Super::BeginPlay();

	SpawnPlayers();
}

void AProjStartupGameMode::SpawnPlayers() {
	TArray<AActor*> playerStartPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStartPoints);

	UE_LOG(LogTemp, Warning, TEXT("ps size: %i"), playerStartPoints.Num());
	if (playerStartPoints.Num() > 0)
	{
		for (size_t i = 0; i < playerStartPoints.Num(); i++)
		{
			APlayerController* pc = UGameplayStatics::CreatePlayer(GetWorld(), i);
			APawn* player = GetWorld()->SpawnActor<APawn>(AProjStartupBall::StaticClass(), playerStartPoints[i]->GetTransform());
			if (pc != nullptr)
			{
				pc->Possess(player);
			}
		}
	}
}
