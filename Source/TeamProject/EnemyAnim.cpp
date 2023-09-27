// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "EnemyAIController.h"

UEnemyAnim::UEnemyAnim()
{

}

void UEnemyAnim::OnAttackEndAnimation()
{
	bAttackPlay = false;
}

void UEnemyAnim::PlayAttackMontage()
{
	if (AttackMontage && AttackSequence) {
		UAnimMontage* pAnimationMtg = PlaySlotAnimationAsDynamicMontage(AttackSequence, TEXT("DefaultSlot"), 0.3f, 0.3f, 1.0f);

		Montage_Play(AttackMontage, 1);
	}
	

	if (AttackMontage != nullptr) {
		
	}
	
}




