// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"

#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "MapObject.h"

#include "PickableObject.generated.h"


UCLASS()
class PROJSTARTUP_API APickableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPlayer(AActor* player);
	bool isAttracting;
	bool isSticked;
	bool launched;

	AActor* owner;

	UStaticMeshComponent* staticComp;

	bool isNotMoving() 
	{
		if (physicsVelocity <= 0) {
			return true;
		}
		return false;
	}

private:
	AActor* player;

	FVector vector = AActor::GetVelocity();
	float physicsVelocity = vector.Size();
	float timerDelay;
	float velocity;
	USphereComponent* sphereComponent;

	float attractionSpeed;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
