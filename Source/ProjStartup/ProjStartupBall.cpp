// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjStartupBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"

#include "Engine.h"

#include "PhysicalMaterials/PhysicalMaterial.h"


AProjStartupBall::AProjStartupBall()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BallMesh(TEXT("/Game/Meshes/TestPlayer"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Ball0"));
	Ball->SetSkeletalMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	//Create sphere collider
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	sphere->InitSphereRadius(400.0f);
	sphere->SetupAttachment(RootComponent);

	sphere2 = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider2"));
	sphere2->InitSphereRadius(60);
	sphere2->SetupAttachment(RootComponent);

	// Set up forces
	//static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> SmooPhysics(TEXT("/Game/Materials/SmooPhysics"));
	//Ball->SetPhysMaterialOverride(SmooPhysics.Object);

	Ball->SetMassOverrideInKg("Ball", 100.0f, true);
	RollTorque = 50000000.0f;
	AirTorque = 500000.0f;
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
	sphere2->GetOverlappingActors(overlappingActors);
	for (int32 ActorIndex = 0; ActorIndex < overlappingActors.Num(); ActorIndex++)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(overlappingActors[ActorIndex]->GetName()));

		APickableObject* object = Cast<APickableObject>(overlappingActors[ActorIndex]);
		if (object)
		{
			if (!object->isSticked)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(object->GetName()));
				attachedActors.Add(object);
				object->isSticked = true;
				object->AttachToComponent(Ball, FAttachmentTransformRules::KeepWorldTransform);
				//for (UStaticMesh* Component : object->GetComponentByClass(UB)
				//{
				//	Component->SetSimulatePhysics(false);
				//}
				//object->staticme
			}
		}
	}
}


void AProjStartupBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjStartupBall::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &AProjStartupBall::MoveForward);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProjStartupBall::Jump);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AProjStartupBall::Attack);
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
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		currentTorque = AirTorque;
		bCanJump = false;
	}
}

void AProjStartupBall::Attack()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(attachedActors.Num()));
	for (int32 ActorIndex = 0; ActorIndex < attachedActors.Num(); ActorIndex++)
	{
		APickableObject* object = attachedActors[ActorIndex];
		object->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}


void AProjStartupBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	currentTorque = RollTorque;
	bCanJump = true;
	UE_LOG(LogTemp, Warning, TEXT("Collision with ground!"));

}
