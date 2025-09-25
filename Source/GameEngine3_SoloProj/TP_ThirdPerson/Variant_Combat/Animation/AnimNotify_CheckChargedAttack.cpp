// Copyright Epic Games, Inc. All Rights Reserved.


#include "AnimNotify_CheckChargedAttack.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameEngine3_SoloProj/TP_ThirdPerson/Variant_Combat/Interfaces/CombatAttacker.h"

void UAnimNotify_CheckChargedAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// cast the owner to the attacker interface
	if (ICombatAttacker* AttackerInterface = Cast<ICombatAttacker>(MeshComp->GetOwner()))
	{
		// tell the actor to check for a charged attack loop
		AttackerInterface->CheckChargedAttack();
	}
}

FString UAnimNotify_CheckChargedAttack::GetNotifyName_Implementation() const
{
	return FString("Check Charged Attack");
}
