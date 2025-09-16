// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstaclePlaform.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class GAMEENGINE3_SOLOPROJ_API AObstaclePlaform : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	//collider to check if player is on platform
	UPROPERTY(EditAnywhere)
	USphereComponent *SphereComp;

	
	//Collider to check if player dies when touching
	UPROPERTY(EditAnywhere)
	UBoxComponent *BoxComp;
	
	UPROPERTY(EditAnywhere)
	USpringArmComponent *SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent *Camera;
	
	
public:	
	// Sets default values for this actor's properties
	AObstaclePlaform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnEnterPlaform(UPrimitiveComponent *OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent *OtherComp, int32 OtherBoolIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnExitPlaform(UPrimitiveComponent *OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent *OtherComp, int32 OtherBoolIndex);

	UFUNCTION(BlueprintCallable)
	void OnCrossParameters(UPrimitiveComponent *OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent *OtherComp, int32 OtherBoolIndex, bool bFromSweep, const FHitResult &SweepResult);
};
