// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableObject.h"
#include "Editor.h"	
#include "Engine/CollisionProfile.h"
//#include "DestructibleComponent.h"

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
	}

	attractionSpeed = 0.08f;

	TArray<UStaticMeshComponent*> staticComps;
	GetComponents<UStaticMeshComponent>(staticComps);

	for (int32 ActorIndex2 = 0; ActorIndex2 < 1; ActorIndex2++)
	{

		staticComp = staticComps[ActorIndex2];

	}
}

// Called every frame
void APickableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(isNotMoving()));

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(isAttracting) + "-" + FString::FromInt(isSticked));

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
			isAttracting = false;
			launched = false;
			isSticked = false;
			timerDelay = 0;


			staticComp->SetSimulatePhysics(false);
			staticComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		}
	}
}

void APickableObject::SetPlayer(AActor* playerP)
{
	this->player = playerP;
	isAttracting = true;
}


void APickableObject::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("COLLIDING!!!!!"));

	AMapObject* object = Cast<AMapObject>(OtherActor);
	if (object)
	{
		//UDestructibleComponent* destructibleComponent = FindComponentByClass<UDestructibleComponent>();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(test.GetName()));
	}
	
}
