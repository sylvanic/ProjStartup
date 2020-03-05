// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableObject.h"
#include "Editor.h"	
#include "Engine/CollisionProfile.h"
#include <ProjStartup\ProjStartupBall.h>
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

	sphereComponent = FindComponentByClass<UCapsuleComponent>();



	attractionSpeed = 0.20f;

	TArray<UStaticMeshComponent*> staticComps;
	GetComponents<UStaticMeshComponent>(staticComps);

	for (int32 ActorIndex2 = 0; ActorIndex2 < 1; ActorIndex2++)
	{

		staticComp = staticComps[ActorIndex2];

	}

	staticComp->OnComponentHit.AddDynamic(this, &APickableObject::OnHit);


}

void APickableObject::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(OtherActor->GetName()));

	if (OtherActor->GetName().Contains("Ground")) {
		Landed = true;
	}
	else if(OtherActor->GetName().Contains("Player") && OtherActor != owner)
	{

		AProjStartupBall* playerCasted = Cast<AProjStartupBall>(OtherActor);

		for (int32 ActorIndex = 0; ActorIndex < playerCasted->attachedActors.Num(); ActorIndex++)
		{
			APickableObject* object = playerCasted->attachedActors[ActorIndex];
			object->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			TArray<UStaticMeshComponent*> staticComps;
			object->GetComponents<UStaticMeshComponent>(staticComps);

			for (int32 ActorIndex2 = 0; ActorIndex2 < 1; ActorIndex2++)
			{
				UStaticMeshComponent* staticComp2 = staticComps[ActorIndex2];
				staticComp2->SetSimulatePhysics(true);
				staticComp2->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

			}
			//object->isAttracting = false;
			object->launched = true;
			object->Landed = false;

		}

		if (playerCasted)
		{

		
			if (owner != OtherActor && owner != nullptr)
			{
				if (staticComp->GetPhysicsLinearVelocity().Size() > 300)
				{
					playerCasted->Die(staticComp->GetPhysicsLinearVelocity());
					owner = nullptr;
				}
			}
		}

	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(OtherActor->GetName()));
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

	TArray<AActor*> overlappingActors;

	//staticComp->GetOverlappingActors(overlappingActors);

	//for (size_t i = 0; i < overlappingActors.Num(); i++)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(overlappingActors[i]->GetName()));
	//}


	if (staticComp->GetPhysicsLinearVelocity().Size() < 10 && launched && Landed)
	{
		isAttracting = false;
		launched = false;
		isSticked = false;
		owner = nullptr;
		
		staticComp->SetSimulatePhysics(false);
		staticComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("rest"));

	}


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
