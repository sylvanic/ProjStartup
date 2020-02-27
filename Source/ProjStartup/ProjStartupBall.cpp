// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjStartupBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AProjStartupBall::AProjStartupBall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Meshes/1M_Cube"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	// Set up forces
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> SmooPhysics(TEXT("/Game/Materials/SmooPhysics"));
	Ball->SetPhysMaterialOverride(SmooPhysics.Object);

	Ball->SetMassOverrideInKg("Ball", 0.5f, true);
	RollTorque = 50000.0f;
	AirTorque = 5000.0f;
	JumpImpulse = 300.0f;
	bCanJump = true; // Start being able to jump

	//disable AI
	//AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;
	bReplicates = false;
}


void AProjStartupBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjStartupBall::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &AProjStartupBall::MoveForward);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProjStartupBall::Jump);
}

void AProjStartupBall::MoveRight(float Val)
{
	const FVector Torque = FVector(-1.f * Val * currentTorque, 0.f, 0.f);
	Ball->AddTorqueInRadians(Torque);
}

void AProjStartupBall::MoveForward(float Val)
{
	const FVector Torque = FVector(0.f, Val * currentTorque, 0.f);
	Ball->AddTorqueInRadians(Torque);	
}

void AProjStartupBall::Jump()
{
	if(bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		currentTorque = AirTorque;
		bCanJump = false;
	}
}

void AProjStartupBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	currentTorque = RollTorque;
	bCanJump = true;
}
