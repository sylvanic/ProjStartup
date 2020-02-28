// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableObject.h"
#include "Editor.h"
#include "D:/Programs/Epic Games/UE_4.24/Engine/Plugins/Runtime/ApexDestruction/Source/ApexDestructionEditor/Private/SDestructibleMeshEditorViewport.h"


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

	sphereComponent = FindComponentByClass<USphereComponent>();

	if (sphereComponent != nullptr)
	{
		sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickableObject::BeginOverlap);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(sphereComponent->GetName()));
	}


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
	
	if (launched)
	{
		timerDelay += DeltaTime;

		if (timerDelay >= 2)
		{
			launched = false;
			//isSticked = false;
			timerDelay = 0;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(isSticked));
}

void APickableObject::SetPlayer(AActor* playerP)
{
	this->player = playerP;
	isAttracting = true;
}


void APickableObject::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMapObject* object = Cast<AMapObject>(OtherActor);
	if (object)
	{
		
		//UDestructibleComponent* destructibleComponent = FindComponentByClass<UDestructibleComponent>();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(test.GetName()));

	}
	
}