// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Slime.generated.h"

UCLASS()
class PROJSTARTUP_API ASlime : public APawn
{
	GENERATED_BODY()

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Slime, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Slime;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Slime, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Slime, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

public:
	// Sets default values for this pawn's properties
	ASlime();
	
	UPROPERTY(EditAnywhere, Category = Slime)
	float rollTorque;

	UPROPERTY(EditAnywhere, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, Category = Camera)
	float BaseLookUpRate;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveRight(float value);

	void MoveForward(float value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	
	// Called to bind functionality to input
	/** Returns Slime subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Slime; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

};
