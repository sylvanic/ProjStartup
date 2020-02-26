// Fill out your copyright notice in the Description page of Project Settings.


#include "SmooMainCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "ProjStartupGameMode.h"
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
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
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

	// Get a refernce to the game mode
	GameMode = Cast<AProjStartupGameMode>(GetWorld()->GetAuthGameMode());

	std::vector<APawn*> players = GameMode->Players;

	//for (size_t i = 0; i < players.size(); i++)
	//{
	//	players.at(0);
	//}
	
}

// Called every frame
void ASmooMainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

