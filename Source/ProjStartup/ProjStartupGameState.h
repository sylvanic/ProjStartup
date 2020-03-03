// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ProjStartupGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJSTARTUP_API AProjStartupGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Helper")
		bool prova;
	
};
