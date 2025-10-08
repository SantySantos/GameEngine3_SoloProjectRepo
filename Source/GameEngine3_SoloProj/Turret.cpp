// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Kismet/GameplayStatics.h"
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

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(GetRootComponent());

	TurretCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TurretCamera"));
	TurretCamera->SetupAttachment(SpringArm);

	OwningPlayer = nullptr;
	OwningPlayerController = nullptr;

	UseTurretAction = nullptr;
	JoinLeaveTurretAction = nullptr;

	minAnglePitch = -45.0f;
	maxAnglePitch = 45.0f;

	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 500.0f);
	TurretCamera->SetRelativeRotation(FRotator(0.0f, -20.0f, 0.0f));
	TurretCamera->FieldOfView = 120.0f;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	SpringArm->Activate();
	TurretCamera->Activate();

	if (TurretCamera)
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Turret Camera Exists");

	/*
	if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent); 
			IsValid(EnhancedInput))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Enhanced Input Exists");
		EnhancedInput->BindAction(UseTurretAction, ETriggerEvent::Started, this, &ATurret::UseTurret);
		EnhancedInput->BindAction(JoinLeaveTurretAction, ETriggerEvent::Started, this, &ATurret::JoinLeaveTurret);
	
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Enhanced Input Does Not Exist");
	}
	*/
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


			SetActorRotation(FRotator(TurretMesh->GetComponentRotation().Pitch, OwningPlayer->GetControlRotation().Yaw,
			                          TurretMesh->GetComponentRotation().Roll));
		}
	}
}

void ATurret::OnPlayerEnterTurret(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBoolIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Player Enter");
	OwningPlayer = Cast<ATP_ThirdPersonCharacter>(OtherActor);

	//check if cast worked
	if (OwningPlayer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Player Cast Worked");
		OwningPlayerController = Cast<APlayerController>(OwningPlayer->GetController());

		//check if cast worked
		if (OwningPlayerController)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Player Controller Cast Worked");

			EnableInput(OwningPlayerController);


				UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(
				OwningPlayerController->GetLocalPlayer());

			if (Subsystem)
			{
				Subsystem->AddMappingContext(TurretMappingContext, 1);
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Subsystem Worked");
			}

			if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
			{
				Input->BindAction(UseTurretAction, ETriggerEvent::Started, this, &ATurret::UseTurret);
				Input->BindAction(JoinLeaveTurretAction, ETriggerEvent::Started, this, &ATurret::JoinLeaveTurret);
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Input Binding Worked");
			}
		}
	}
}

void ATurret::OnPlayerExitTurret(UPrimitiveComponent* OverlappedComp,
                                 AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Player Leave");
	if (OwningPlayer)
	{
		if (OwningPlayerController)
		{
			//disable input for turret
			DisableInput(OwningPlayerController);
		}
	}
}

void ATurret::UseTurret()
{
	if (OwningPlayer->bInTurret)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Use Turret");
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor *tempProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, FVector::ZeroVector, GetActorRotation(), SpawnParams);
			IsValid(tempProjectile))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "yay");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "noy");
		}
		
	}
}


void ATurret::JoinLeaveTurret()
{
	if (OwningPlayer)
	{
		FVector LastPlayerLocation = GetActorLocation();
		FVector OldPlayerVelocity = OwningPlayer->GetCharacterMovement()->Velocity;

		if (!OwningPlayer->bInTurret)
		{
			LastPlayerLocation = OwningPlayer->GetActorLocation();
			OwningPlayer->GetCharacterMovement()->Velocity = FVector::ZeroVector;

			if (OwningPlayerController)
			{
				OwningPlayerController->SetViewTargetWithBlend(this, 0.5, EViewTargetBlendFunction::VTBlend_Linear);
				OwningPlayer->SetActorRotation(GetActorRotation());
				OwningPlayer->bInTurret = true;
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
				OwningPlayer->GetCharacterMovement()->Velocity = OldPlayerVelocity;
			}
		}
	}
}
