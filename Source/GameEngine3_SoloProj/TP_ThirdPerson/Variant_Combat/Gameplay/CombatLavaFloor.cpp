// Copyright Epic Games, Inc. All Rights Reserved.


#include "CombatLavaFloor.h"
#include "Components/StaticMeshComponent.h"
#include "GameEngine3_SoloProj/TP_ThirdPerson/Variant_Combat/Interfaces/CombatDamageable.h"

ACombatLavaFloor::ACombatLavaFloor()
{
	PrimaryActorTick.bCanEverTick = false;

	// create the mesh
	RootComponent = Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// bind the hit handler
	Mesh->OnComponentHit.AddDynamic(this, &ACombatLavaFloor::OnFloorHit);
}

void ACombatLavaFloor::OnFloorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// check if the hit actor is damageable by casting to the interface
	if (ICombatDamageable* Damageable = Cast<ICombatDamageable>(OtherActor))
	{
		// damage the actor
		Damageable->ApplyDamage(Damage, this, Hit.ImpactPoint, FVector::ZeroVector);
	}
}
