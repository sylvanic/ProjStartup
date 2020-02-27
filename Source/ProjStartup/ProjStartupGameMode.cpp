// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjStartupGameMode.h"
#include "ProjStartupBall.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerStart.h"
#include "Misc/Timespan.h"
#include "Templates/Casts.h"
#include "SpawnArea.h"
#include "SmooMainCamera.h"

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
	HasInitialized = true;
}

void AProjStartupGameMode::SpawnPlayers() {
	TArray<AActor*> playerStartPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), playerStartPoints);
	Players = std::vector<AProjStartupBall*>();

	UE_LOG(LogTemp, Warning, TEXT("ps size: %i"), playerStartPoints.Num());
	if (playerStartPoints.Num() > 0)
	{
		for (size_t i = 0; i < playerStartPoints.Num(); i++)
		{
			APlayerController* pc = UGameplayStatics::CreatePlayer(GetWorld(), i);
			AProjStartupBall* player = GetWorld()->SpawnActor<AProjStartupBall>(AProjStartupBall::StaticClass(), playerStartPoints[i]->GetTransform());
			if (player != nullptr)
			{
				Players.push_back(player);
			}

			if (pc != nullptr)
			{
				pc->Possess(player);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("player controller already exists: %i"), i);
				UE_LOG(LogTemp, Warning, TEXT("Assigning player controller %i to player %i"), i, i);

				APlayerController* existingController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), i);
				existingController->Possess(player);
			}
		}

		ASmooMainCamera* camera = GetWorld()->SpawnActor<ASmooMainCamera>(ASmooMainCamera::StaticClass(), playerStartPoints[0]->GetTransform());
	}
}
