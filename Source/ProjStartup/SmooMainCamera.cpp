// Fill out your copyright notice in the Description page of Project Settings.


#include "SmooMainCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "ProjStartupGameMode.h"
#include "ProjStartupBall.h"
#include "TimerManager.h"
#include <vector>


// Sets default values
ASmooMainCamera::ASmooMainCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetUsingAbsoluteRotation(true); // Rotation of the ball should not affect rotation of boom
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
}

// Called when the game starts or when spawned
void ASmooMainCamera::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AProjStartupGameMode>(GetWorld()->GetAuthGameMode());

	for (FConstPlayerControllerIterator pcIter = GetWorld()->GetPlayerControllerIterator(); pcIter; ++pcIter)
	{
		APlayerController* pc = Cast<APlayerController>(*pcIter);

		pc->SetViewTargetWithBlend(this);
	}

	GetWorld()->GetTimerManager().SetTimer(updateCameraHandle, this, &ASmooMainCamera::UpdateCamera, 0.01f, true);
}

void ASmooMainCamera::UpdateCamera() {
	if (GameMode->HasInitialized == false)
	{
		return;
	}

	float currentFarthestDistance = TNumericLimits<float>::Min();
	FVector newPosition = FVector::ZeroVector;

	std::vector<AProjStartupBall*> players = GameMode->Players;

	for (int i = 0; i < players.size(); i++)
	{
		for (size_t y = i; y < players.size(); y++)
		{
			currentFarthestDistance = GetFarthestDistance(players, i, y, currentFarthestDistance);
		}
		// add all player positions together
		newPosition += players.at(i)->GetActorLocation();
	}
	// get the average of all player positions
	newPosition /= players.size();
	// move the camera a bit lower so we don't lose track of the player who's closest to the camera
	newPosition += FVector::BackwardVector * 500.0f;

	// update the camera position based on the average position of all the players (smoothly)
	SetActorLocation(UKismetMathLibrary::VLerp(GetActorLocation(), newPosition, 0.02f));
	// apply the distance from the players based on the distance of the players furthest from each other
	SpringArm->TargetArmLength = FMath::Lerp(minDistance, maxDistance, currentFarthestDistance / (maxDistance - maxDistance/3.0f));
}

float ASmooMainCamera::GetFarthestDistance(std::vector<AProjStartupBall*> players, int x, int y, float currentFarthestDistance) {
	float distance = FVector::Dist(players.at(x)->GetActorLocation(), players.at(y)->GetActorLocation());
	if (distance > currentFarthestDistance)
	{
		currentFarthestDistance = distance;
	}

	return currentFarthestDistance;
}

// Called every frame
void ASmooMainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

