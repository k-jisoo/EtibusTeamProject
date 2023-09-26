// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyInputPossible.h"
#include "BaseCharacter.h"

void UAnimNotifyInputPossible::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ABaseCharacter* pChar = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (IsValid(pChar) == false)
		return;

	pChar->SetInputPossible();
}
