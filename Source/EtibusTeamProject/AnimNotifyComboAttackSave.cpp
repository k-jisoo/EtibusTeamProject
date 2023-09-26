// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyComboAttackSave.h"
#include "BaseCharacter.h"
#include "Engine/DataTable.h"

void UAnimNotifyComboAttackSave::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ABaseCharacter* pChar = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (IsValid(pChar) == false)
		return;

	pChar->ComboAttackSave();
}