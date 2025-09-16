// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstaclePlaform.h"

#include "Camera/CameraComponent.h"
#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AObstaclePlaform::AObstaclePlaform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setting basic components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(Mesh);
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetupAttachment(GetRootComponent());
	//calling this function when the sphere overlaps with something
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AObstaclePlaform::OnEnterPlaform);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AObstaclePlaform::OnExitPlaform);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(GetRootComponent());
	//calling this function when the box overlaps with something
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AObstaclePlaform::OnCrossParameters);
	BoxComp->SetActive(false);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);

	
}

// Called when the game starts or when spawned
void AObstaclePlaform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstaclePlaform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstaclePlaform::OnEnterPlaform(UPrimitiveComponent *OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent *OtherComp, int32 OtherBoolIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (OtherActor == Player)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Player on platform"));
		BoxComp->SetActive(true);
	}
}

void AObstaclePlaform::OnExitPlaform(UPrimitiveComponent *OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent *OtherComp, int32 OtherBoolIndex)
{
	
}

void AObstaclePlaform::OnCrossParameters(UPrimitiveComponent *OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent *OtherComp, int32 OtherBoolIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	
}

