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
#include <ProjStartup\SmooMainCamera.h>
#include "ProjStartupGameMode.h"




AProjStartupBall::AProjStartupBall()
{
	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BallMesh(TEXT("/Game/Meshes/TestPlayer"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Ball0"));
	Ball->SetSkeletalMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	Ball->SetSimulatePhysics(true);
	//Ball->SetAngularDamping(0.1f);
	//Ball->SetLinearDamping(0.1f);
	//Ball->BodyInstance.MassScale = 3.5f;
	//Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	//Ball->SetNotifyRigidBodyCollision(true);
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

	//Ball->SetMassOverrideInKg("Smoo", 100.0f, true);
	//Ball->Density = 1.0f;
	//Ball->bLocalSpaceSimulation = true;
	//GroundSpeed = 90.0f;
	//AirSpeed = 90.0f;
	//JumpImpulse = 30.0f;
	//MaxSpeed = 450.0f;
	bCanJump = true; // Start being able to jump


	//disable AI
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;
	bReplicates = false;

	//MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("Movement Component"));
	//MovementComponent->UpdatedComponent = RootComponent;


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
			if (!object->isAttracting)
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
			if (!object->isSticked)
			{
				attachedActors.Add(object);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString(object->GetName()));
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(attachedActors.Num()));
				object->owner = this;
				object->isSticked = true;
				object->K2_AttachToActor(this, "", EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
				object->staticComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
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
	PlayerInputComponent->BindAction("Die", IE_Pressed, this, &AProjStartupBall::Die);
}

void AProjStartupBall::MoveRight(float Val)
{
	const FVector Impulse = FVector(-Val * 40, 0.f, 0.f);

	Ball->AddImpulse(Impulse);

	//AddMovementInput(GetActorRightVector(), Val);
}

void AProjStartupBall::MoveForward(float Val)
{
	const FVector Impulse = FVector(0.f, Val * 40, 0.f);
	Ball->AddImpulse(Impulse);

	//AddMovementInput(GetActorForwardVector(), Val, true );
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(Val));

}

void AProjStartupBall::Jump()
{
	if (true)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		//bCanJump = false;
	}
}

void AProjStartupBall::Die()
{
	if (IsPlayerControlled())
	{


		TArray<AActor*> players;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProjStartupBall::StaticClass(), players);

		for (size_t i = 0; i < players.Num(); i++)
		{
			AProjStartupBall* playerBall = Cast<AProjStartupBall>(players[i]);
			if (playerBall)
			{
				playerBall->SetActorLocation(playerBall->startingPosition, false, nullptr, ETeleportType::TeleportPhysics);
				playerBall->Ball->SetSimulatePhysics(false);
				playerBall->Ball->SetSimulatePhysics(true);
			}
		}
		
	AProjStartupGameMode* mymode = Cast<AProjStartupGameMode>(GetWorld()->GetAuthGameMode());
	if (mymode)
	{
		mymode->ScorePlayer1++;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(mymode->ScorePlayer1));
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
}


void AProjStartupBall::Attack()
{
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
		}
		//object->isAttracting = false;
		object->launched = true;

	}
	attachedActors.Empty();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(attachedActors.Num()));

}


void AProjStartupBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	currentSpeed = GroundSpeed;
	bCanJump = true;
	UE_LOG(LogTemp, Warning, TEXT("Collision with ground!"));
}


void AProjStartupBall::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APickableObject* object = Cast<APickableObject>(OtherActor);

	if (object)
	{
		if (object->owner != this && object->owner != nullptr)
		{
			//if(object->owner->GetAttachParentActor())
			if (object->GetAttachParentActor() == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("attack"));
				object->owner = nullptr;
			}
		}
	}
}