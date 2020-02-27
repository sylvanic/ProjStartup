// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProjStartupGameMode.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmooMainCamera.generated.h"

UCLASS()
class PROJSTARTUP_API ASmooMainCamera : public AActor
{
	GENERATED_BODY()

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UFUNCTION()
	void UpdateCamera();

	UPROPERTY(EditAnywhere)
	float minDistance = 700.0f;

	UPROPERTY(EditAnywhere)
	float maxDistance = 5000.0f;
	
public:	
	// Sets default values for this actor's properties
	ASmooMainCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

private:
	AProjStartupGameMode* GameMode;

	FTimerHandle updateCameraHandle;

	float GetFarthestDistance(std::vector<AProjStartupBall*> players, int x, int y, float currentFarthestDistance);
};
