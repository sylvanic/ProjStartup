// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnArea.generated.h"

UCLASS()
class PROJSTARTUP_API ASpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnArea();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* Capsule;

public:	

	/** Returns Camera subobject **/
	FORCEINLINE class UCapsuleComponent* GetCapsule() const { return Capsule; }
};
