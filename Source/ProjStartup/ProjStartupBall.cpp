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
#include "ProjStartup\SmooMainCamera.h"
#include "ProjStartupGameMode.h"

AProjStartupBall::AProjStartupBall()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BallMesh(TEXT("/Game/Meshes/TestPlayer"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Ball0"));
	Ball->SetSkeletalMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	Ball->SetSimulatePhysics(true);
	RootComponent = Ball;

	//Create sphere collider
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	sphere->InitSphereRadius(100.0f);
	sphere->SetupAttachment(RootComponent);

	sphere2 = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider2"));
	sphere2->InitSphereRadius(20);
	sphere2->SetupAttachment(RootComponent);
	sphere2->OnComponentBeginOverlap.AddDynamic(this, &AProjStartupBall::BeginOverlap);


	//UCapsuleComponent* sphere3 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sphere Collider3"));
	//sphere3->SetRelativeLocation(FVector(130, 0, 0), false);
	//sphere3->SetCapsuleRadius(40);
	//sphere3->SetCapsuleHalfHeight(80);
	//sphere3->SetupAttachment(RootComponent);
	//sphere3->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	//sphere3->bHiddenInGame = false;

	// Set up forces
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> SmooPhysics(TEXT("/Game/Materials/SmooPhysics"));
	Ball->SetPhysMaterialOverride(SmooPhysics.Object);

	GroundAcceleration = 50;
	AirAcceleration = 70.0f;
	JumpImpulse = 3000.0f;
	MaxSpeed = 550.0f;
	currentAcceleration = GroundAcceleration;
	bCanJump = true; // Start being able to jump
	JumpDelay = 3.0f;

	//Other settings
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;
	bReplicates = false;
}


// Called every frame
void AProjStartupBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> overlappingActorsSphere;
	sphere->GetOverlappingActors(overlappingActorsSphere);

	for (size_t ActorIndex = 0; ActorIndex < overlappingActorsSphere.Num(); ActorIndex++)
	{
		APickableObject* object = Cast<APickableObject>(overlappingActorsSphere[ActorIndex]);
		if (object)
		{
			if (!object->isAttracting && !object->launched)
			{
				object->SetPlayer(this);
			}
		}
	}

	TArray<AActor*> overlappingActors;

	sphere2->GetOverlappingActors(overlappingActors);

	for (size_t ActorIndex = 0; ActorIndex < overlappingActors.Num(); ActorIndex++)
	{
		APickableObject* object = Cast<APickableObject>(overlappingActors[ActorIndex]);

		if (object)
		{
			if (!object->isSticked && object->owner == nullptr && !object->launched)
			{

				attachedActors.Add(object);
				object->owner = this;
			

				object->isSticked = true;
				object->K2_AttachToActor(this, "", EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
				object->staticComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
			}
		}
	}

	if (jumpTimer < JumpDelay)
	{
		bCanJump = false;
		jumpTimer += DeltaTime;
	}
	else
	{
		bCanJump = true;
	}

	Ball->SetPhysicsLinearVelocity(ClampVector(Ball->GetPhysicsLinearVelocity(), -FVector(1.0f, 999.0f, 1.0f) * MaxSpeed, FVector(1.0f, 999.0f, 1.0f) * MaxSpeed));
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
	if (Ball->IsSimulatingPhysics())
	{
		const FVector Impulse = FVector(0.f, FMath::Clamp(Val, -1.0f, 1.0f) * currentAcceleration, 0.f);
		Ball->AddImpulse(Impulse);
	}
}

void AProjStartupBall::MoveForward(float Val)
{
	if (Ball->IsSimulatingPhysics())
	{
		const FVector Impulse = FVector(FMath::Clamp(Val, -1.0f, 1.0f) * currentAcceleration, 0.f, 0.f);
		Ball->AddImpulse(Impulse);
	}
}

// Try to jump
void AProjStartupBall::Jump()
{
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		currentAcceleration = AirAcceleration;
		Ball->AddImpulse(Impulse);
		jumpTimer = 0.0f;
	}
}

void AProjStartupBall::Die(FVector impulse)
{
		Ball->AddImpulse(impulse * 10);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("DIEEEE ") + FString::FromInt(impulse.Size()));

		AProjStartupGameMode* myMode = Cast<AProjStartupGameMode>(GetWorld()->GetAuthGameMode());

		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProjStartupBall::StaticClass(), Players);

		for (size_t i = 0; i < Players.Num(); i++)
		{
			AProjStartupBall* playerBall = Cast<AProjStartupBall>(Players[i]);
			if (playerBall)
			{
				//playerBall->SetActorLocation(playerBall->startingPosition, false, nullptr, ETeleportType::TeleportPhysics);
				//playerBall->Ball->SetSimulatePhysics(true);
			}
		}

		if (myMode)
		{
			if (this == myMode->player1)
			{
				myMode->ScorePlayer2++;

			}
			else if (this == myMode->player2)
			{
				myMode->ScorePlayer1++;

			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("error"));
			}
			MyBlueprintEventFunction();
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("error"));
		}


		//FTransform test = GetTransform();
		//GetWorld()->DestroyActor(this);
		//if (camera != nullptr)
		//{
		//	GetWorld()->DestroyActor(camera);
		//}
		//AProjStartupBall* player = GetWorld()->SpawnActor<AProjStartupBall>(AProjStartupBall::StaticClass(), startingPosition);
		//APlayerController* existingController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
		//existingController->Possess(player);
		//camera = GetWorld()->SpawnActor<ASmooMainCamera>(ASmooMainCamera::StaticClass(), startingPosition);
		//player->startingPosition = startingPosition;
	
}


void AProjStartupBall::Attack()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(attachedActors.Num()));

	for (int32 ActorIndex = 0; ActorIndex < attachedActors.Num(); ActorIndex++)
	{
		APickableObject* object = attachedActors[ActorIndex];
		object->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		TArray<UStaticMeshComponent*> staticComps;
		object->GetComponents<UStaticMeshComponent>(staticComps);

		for (int32 ActorIndex2 = 0; ActorIndex2 < 1; ActorIndex2++)
		{
			UStaticMeshComponent* staticComp = staticComps[ActorIndex2];
			staticComp->SetSimulatePhysics(true);
			staticComp->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
			staticComp->AddImpulse(GetVelocity() / 2.0f);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Impulse"));

		}
		//object->isAttracting = false;
		object->launched = true;
		object->Landed = false;

	}

	attachedActors.Empty();

}

void AProjStartupBall::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
//	APickableObject* object = Cast<APickableObject>(OtherActor);
//
//	if (object)
//	{
//		if (object->owner != this && object->owner != nullptr)
//		{
//			//if(object->owner->GetAttachParentActor())
//			if (object->GetAttachParentActor() == nullptr)
//			{
//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("attack"));
//				object->owner = nullptr;
//			}
//		}
//	}
}