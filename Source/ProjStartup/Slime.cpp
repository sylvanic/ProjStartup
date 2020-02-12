// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

// Sets default values
ASlime::ASlime()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));
	Slime->SetStaticMesh(BallMesh.Object);
}

// Called when the game starts or when spawned
void ASlime::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called to bind functionality to input
void ASlime::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &ASlime::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASlime::MoveForward);
}

void ASlime::MoveRight(float Value) {
	const FVector Torque = FVector(-1.f * Value * rollTorque, 0.f, 0.f);
	Slime->AddTorqueInRadians(Torque);
}

void ASlime::MoveForward(float Value) {
	const FVector Torque = FVector(0.f, Value * rollTorque, 0.f);
	Slime->AddTorqueInRadians(Torque);
}

