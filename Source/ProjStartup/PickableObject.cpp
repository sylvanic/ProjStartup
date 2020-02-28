// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableObject.h"
#include "Editor.h"

// Sets default values
APickableObject::APickableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickableObject::BeginPlay()
{
	Super::BeginPlay();
	attractionSpeed = 0.08f;
	
}

// Called every frame
void APickableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isAttracting && !isSticked)
	{
		FVector direction = player->GetActorLocation() - GetActorLocation();
		

		SetActorLocation(GetActorLocation() + (direction * attractionSpeed));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(isSticked));
}

void APickableObject::SetPlayer(AActor* playerP)
{
	this->player = playerP;
	isAttracting = true;
}

