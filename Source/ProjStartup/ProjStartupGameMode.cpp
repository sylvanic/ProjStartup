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
#include "Materials/MaterialInstanceDynamic.h"
#include "MyGameInstance.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UObject/ConstructorHelpers.h"

AProjStartupGameMode::AProjStartupGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerStateClass = nullptr;
	GameStateClass = nullptr;
	SpectatorClass = nullptr;

	static ConstructorHelpers::FObjectFinder<UClass> SmooPhysics(TEXT("Blueprint'/Game/MyAssets/Player/Player.Player_C'"));
	FloorCellClass = SmooPhysics.Object;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString(FloorCellClass.Get()->GetName()));

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

			UObject* test = FindObject<UObject>(UObject::StaticClass(), TEXT("/Game/NewBlueprint.NewBlueprint'"));
			UBlueprint* GeneratedBP = Cast<UBlueprint>(test);

			AProjStartupBall* player = GetWorld()->SpawnActor<AProjStartupBall>(FloorCellClass, playerStartPoints[i]->GetTransform());

			FLinearColor* linearColor = nullptr;
			UMyGameInstance* gameinstance = Cast<UMyGameInstance>(GetGameInstance());

			if (player != nullptr)
			{
				Players.push_back(player);
			}

			if (pc != nullptr)
			{
				//Player 2
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("posses1"));

				pc->Possess(player);
				if (gameinstance)
				{
					linearColor = &gameinstance->colorPlayer2;
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Casting Error"));
				}
			}
			else
			{
				//Player1
				UE_LOG(LogTemp, Warning, TEXT("player controller already exists: %i"), i);
				UE_LOG(LogTemp, Warning, TEXT("Assigning player controller %i to player %i"), i, i);

				APlayerController* existingController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), i);
				existingController->Possess(player);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString(player->GetName()));
				linearColor = &gameinstance->colorPlayer1;


			}

			if (linearColor != nullptr)
			{
				UMaterialInstanceDynamic* dynamicMaterial = player->GetBall()->CreateDynamicMaterialInstance(0, player->GetBall()->GetMaterial(0));
				dynamicMaterial->SetVectorParameterValue("Color", *linearColor);
				player->GetBall()->SetMaterial(0, dynamicMaterial);
				player->startingPosition = playerStartPoints[i]->GetActorLocation();
			}
		}

		ASmooMainCamera* camera = GetWorld()->SpawnActor<ASmooMainCamera>(ASmooMainCamera::StaticClass(), playerStartPoints[0]->GetTransform());
	}
}
