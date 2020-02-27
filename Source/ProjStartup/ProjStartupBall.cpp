// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjStartupBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "PickableObject.h"
#include "Engine.h"

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

	//Create sphere collider
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	sphere->InitSphereRadius(800.0f);
	sphere->SetupAttachment(RootComponent);

	// Set up forces
	Ball->SetMassOverrideInKg("Ball", 0.5f, true);
	RollTorque = 50000.0f;
	JumpImpulse = 300.0f;
	bCanJump = true; // Start being able to jump


	//disable AI
	//AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;
	bReplicates = false;
}

// Called every frame
void AProjStartupBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	TArray<AActor*> overlappingActorsSphere;
	sphere->GetOverlappingActors(overlappingActorsSphere);


	for (int32 ActorIndex = 0; ActorIndex < overlappingActorsSphere.Num(); ActorIndex++)
	{
		APickableObject* object = Cast<APickableObject>(overlappingActorsSphere[ActorIndex]);
		if (object)
		{
			if (!object->isAttracting)
			{
				object->SetPlayer(this);
			}
		}
	}

	TArray<AActor*> overlappingActors;
	Ball->GetOverlappingActors(overlappingActors);
	for (int32 ActorIndex = 0; ActorIndex < overlappingActors.Num(); ActorIndex++)
	{
		APickableObject* object = Cast<APickableObject>(overlappingActors[ActorIndex]);
		if (object)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(object->GetName()));

			object->isSticked = true;
			object->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}
	}



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
	const FVector Torque = FVector(-1.f * Val * RollTorque, 0.f, 0.f);
	Ball->AddTorqueInRadians(Torque);
}

void AProjStartupBall::MoveForward(float Val)
{
	const FVector Torque = FVector(0.f, Val * RollTorque, 0.f);
	Ball->AddTorqueInRadians(Torque);	
}

void AProjStartupBall::Jump()
{
	if(bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}
}

void AProjStartupBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bCanJump = true;
}
