// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "TP_ThirdPerson/TP_ThirdPersonPlayerController.h"

// Sets default values
ATurret::ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(GetRootComponent());

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(GetRootComponent());

	UseTurretRangeCollision = CreateDefaultSubobject<USphereComponent>(TEXT("UseTurretRangeSphere"));
	UseTurretRangeCollision->SetupAttachment(GetRootComponent());
	UseTurretRangeCollision->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnPlayerEnterTurret);
	UseTurretRangeCollision->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnPlayerExitTurret);


	OwningPlayerSeat = CreateDefaultSubobject<USceneComponent>(TEXT("OwningPlayerSeat"));
	OwningPlayerSeat->SetupAttachment(GetRootComponent());

	TurretNuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("TurretNuzzle"));
	TurretNuzzle->SetupAttachment(TurretMesh);

	OwningPlayer = nullptr;
	OwningPlayerController = nullptr;

	minAnglePitch = -45.0f;
	maxAnglePitch = 45.0f;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwningPlayer)
	{
		if (OwningPlayer->bInTurret)
		{
			float OwningPlayerPitch = FMath::ClampAngle(OwningPlayer->GetControlRotation().Pitch, minAnglePitch,
			                                            maxAnglePitch);
			SetActorRotation(FRotator(GetActorRotation().Roll, OwningPlayerPitch,
			                          OwningPlayer->GetControlRotation().Yaw));
		}
	}
}

void ATurret::OnPlayerEnterTurret(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBoolIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	OwningPlayer = Cast<ATP_ThirdPersonCharacter>(OtherActor);

	//check if cast worked
	if (OwningPlayer)
	{
		OwningPlayerController = Cast<ATP_ThirdPersonPlayerController>(OwningPlayer->GetController());

		//check if cast worked
		if (OwningPlayerController)
		{
			//enable input for turret
			EnableInput(OwningPlayerController);

			//Input component is in all actors, which is used to bind input actions,
			//it is created with the actor, but is null until we create it
			//we create it here, when the player enters the turret range
			//InputComponent = NewObject<UInputComponent>(this);
			if (!InputComponent)
			{
				InputComponent = NewObject<UInputComponent>(this);
				InputComponent->RegisterComponent();
				InputComponent->bBlockInput = false;
				InputComponent->Priority = 1;

				InputComponent->BindAction("UseTurret", IE_Pressed, this, &ATurret::JoinLeaveTurret);
				InputComponent->BindAction("Fire", IE_Pressed, this, &ATurret::UseTurret);
				AddOwnedComponent(InputComponent);
			}
		}
	}
}

void ATurret::OnPlayerExitTurret(UPrimitiveComponent* OverlappedComp,
                                 AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex)
{
	if (OwningPlayer)
	{
		if (OwningPlayerController)
		{
			//enable input for turret
			DisableInput(OwningPlayerController);

			if (InputComponent)
			{
				InputComponent->DestroyComponent();
				InputComponent = nullptr;
				OwningPlayer = nullptr;
				OwningPlayerController = nullptr;
			}
		}
	}
}

void ATurret::UseTurret()
{
	GetWorld()->SpawnActor<AActor>(ProjectileClass, TurretNuzzle->GetComponentLocation(), GetActorRotation());
}

void ATurret::JoinLeaveTurret()
{
	FVector LastPlayerLocation = GetActorLocation();
	if (OwningPlayer)
	{
		if (!OwningPlayer->bInTurret)
		{
			LastPlayerLocation = OwningPlayer->GetActorLocation();
			OwningPlayer->GetCharacterMovement()->DisableMovement();

			if (OwningPlayerController)
			{
				OwningPlayerController->SetViewTargetWithBlend(this, 0.5, EViewTargetBlendFunction::VTBlend_Linear);
				OwningPlayer->bInTurret = true;
				OwningPlayer->SetActorRotation(GetActorRotation());
				OwningPlayer->AttachToComponent(OwningPlayerSeat,
				                                FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
		}
		else
		{
			if (OwningPlayerController)
			{
				OwningPlayerController->SetViewTargetWithBlend(OwningPlayer, 0.5,
				                                               EViewTargetBlendFunction::VTBlend_Linear);
				OwningPlayer->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				OwningPlayer->SetActorLocation(LastPlayerLocation);
				OwningPlayer->bInTurret = false;
			}
		}
	}
}
