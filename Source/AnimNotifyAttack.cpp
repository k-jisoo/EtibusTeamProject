// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyAttack.h"
#include "BaseCharacter.h"
#include "Weapon.h"

void UAnimNotifyAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ABaseCharacter* pChar = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (IsValid(pChar) == false)
		return;
	
	AWeapon* weapon = pChar->Weapon;
	if (!IsValid(weapon))
		return;

	weapon->SetSphereCollisionState(true);
}
