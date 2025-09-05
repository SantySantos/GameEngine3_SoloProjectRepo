// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AOrder.generated.h"

class UInputAction;
class UFloatingPawnMovement;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UStaticMeshComponent;

UCLASS()
class GAMEENGINE3_SOLOPROJ_API AAOrder : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	//settings actions
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> XAxis;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> YAxis;

public:
	// Sets default values for this pawn's properties
	AAOrder();

	UFUNCTION()
	void Move(const FInputActionValue& InputActionValue);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
