// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "PickableObject.h"
#include "ProjStartupBall.generated.h" 

UCLASS(config=Game)
class AProjStartupBall : public APawn
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Ball;

public:
	AProjStartupBall();

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(EditAnywhere, Category=Ball)
	float JumpImpulse;

	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditAnywhere, Category=Ball)
	float GroundAcceleration;

	UPROPERTY(EditAnywhere, Category=Ball)
	float AirAcceleration;

	/** Indicates whether we can currently jump, use to prevent double jumping */
	UPROPERTY(EditAnywhere, Category = Ball)
	bool bCanJump;

	UPROPERTY(EditAnywhere, Category = Ball)
	float MaxSpeed;

	UPROPERTY(EditAnywhere, Category = Ball)
	float JumpDelay;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:


	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

	/** Handle jump action. */
	void Jump();

	void Attack();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	void Die();

public:
	/** Returns Ball subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetBall() const { return Ball; }

	FVector startingPosition;

private:

	USphereComponent* sphere;
	USphereComponent* sphere2;
	TArray<APickableObject*> attachedActors;

	float currentAcceleration;
	float jumpTimer;

	UPawnMovementComponent* MovementComponent;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
