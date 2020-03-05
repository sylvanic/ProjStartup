// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJSTARTUP_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Var")
	FLinearColor colorPlayer1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Var")
	FLinearColor colorPlayer2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Var")
		bool player1Won;
};
