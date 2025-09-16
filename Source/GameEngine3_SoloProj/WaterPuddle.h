// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterPuddle.generated.h"

class UStaticMeshComponent;
class UBoxComponent;


UCLASS()
class GAMEENGINE3_SOLOPROJ_API AWaterPuddle : public AActor
{
	GENERATED_BODY()
	
	//UENUM(BlueprintType)
	enum puddleType
	{
		ORDER UMETA(DisplayName = "Order"),
		CHAOS UMETA(DisplayName = "Chaos")
	};
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	//detecting collision
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	
		
public:	
	// Sets default values for this actor's properties
	AWaterPuddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor* OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBoolIndex, bool bFromSweep, const FHitResult &SweepResult);

};
