// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterPuddle.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AWaterPuddle::AWaterPuddle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(Mesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	
	BoxCollision->SetupAttachment(GetRootComponent());
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWaterPuddle::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AWaterPuddle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaterPuddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaterPuddle::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBoolIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UKismetSystemLibrary::PrintString(this, FString("Overlap happened") );
}

