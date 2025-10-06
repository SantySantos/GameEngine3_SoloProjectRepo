// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class USceneComponent;
class UInputAction;
class ATP_ThirdPersonPlayerController;

UCLASS()
class GAMEENGINE3_SOLOPROJ_API ATurret : public AActor
{
	GENERATED_BODY()

	/** The base mesh of the turret */
	UPROPERTY(VisibleAnywhere,  Category = "Components")
	UStaticMeshComponent* BaseMesh;

	/** The turret mesh that rotates to aim */
	UPROPERTY(VisibleAnywhere,  Category = "Components")
	UStaticMeshComponent* TurretMesh;

	/** Sphere collision to determine if player is in range to use turret */
	UPROPERTY(VisibleAnywhere,  Category = "Components")
	USphereComponent* UseTurretRangeCollision;

	/** Scene component representing the seat where the player will sit when using the turret */
	UPROPERTY(VisibleAnywhere,  Category = "Components")
	USceneComponent* OwningPlayerSeat;

	/** Scene component representing the nuzzle where projectiles will spawn from */
	UPROPERTY(VisibleAnywhere,  Category = "Components")
	USceneComponent* TurretNuzzle;

	/** Projectile class to spawn when firing the turret */
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AActor> ProjectileClass;

	/** Temp Player that will be using the turret */
	UPROPERTY(EditAnywhere, Category="Player")
	ATP_ThirdPersonCharacter* OwningPlayer;

	/** Temp PlayerController that will be using the turret */
	UPROPERTY(EditAnywhere, Category="Player")
	APlayerController *OwningPlayerController;

	/** Max Turret Rotation on Y */
	UPROPERTY(EditAnywhere, Category = "Turret")
	float minAnglePitch;
	
	UPROPERTY(EditAnywhere, Category = "Turret")
	float maxAnglePitch;
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Use Turret Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction *UseTurretAction;

	/** Join/Leave Turret */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction *JoinLeaveTurretAction;

	/**Spring Arm to hold the camera when using the turret*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = "Components")
	USpringArmComponent *SpringArm;

	/** Camera to use when player is using the turret */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent *TurretCamera;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** check if player is in bounds of sphere to use the turret
	 * @param OtherActor = (actor joining the bounds of the sphere), must be a player*/
	UFUNCTION(BlueprintCallable)
	void OnPlayerEnterTurret(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBoolIndex, bool bFromSweep, const FHitResult& SweepResult);


	/** make sure when player leaves the turret bounds they can no longer use it
	 * @param OtherActor = (actor leaving the bounds of the sphere), must be a player*/
	UFUNCTION(BlueprintCallable)
	void OnPlayerExitTurret(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex);

	/** Handles using the turret when player is in range and presses the use turret input action */
	UFUNCTION(BlueprintCallable)
	void UseTurret();

	/** Handles joining and leaving the turret */
	UFUNCTION(BlueprintCallable)
	void JoinLeaveTurret();
	
};

