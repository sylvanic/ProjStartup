// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenLevel.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AOpenLevel::AOpenLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOpenLevel::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::OpenLevel(this, "RollingExampleMap");
	
}

// Called every frame
void AOpenLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

