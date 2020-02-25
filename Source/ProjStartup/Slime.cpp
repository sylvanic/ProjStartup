// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

// Sets default values
ASlime::ASlime()
{
 	//create mesh for the ball
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));
	Slime = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Slime0"));
	Slime->SetupAttachment(RootComponent);
	Slime->SetStaticMesh(BallMesh.Object);
	RootComponent = Slime;

	//spring arm for the camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);

	//create camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; 
	
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

