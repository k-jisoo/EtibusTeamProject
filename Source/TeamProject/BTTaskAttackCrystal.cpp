// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskAttackCrystal.h"
#include "Enemy.h"
#include "EnemyAnim.h"
#include "EnemyAIController.h"



UBTTaskAttackCrystal::UBTTaskAttackCrystal()
{
	NodeName = TEXT("BTTaskAttackCrystal");
}

EBTNodeResult::Type UBTTaskAttackCrystal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Controller == nullptr)
	{
		Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
		if (Controller == nullptr)
		{
			return EBTNodeResult::Failed;
		}
	}

	if (anim == nullptr)
	{
		anim = Controller->anim;
	}

	if (me == nullptr)
	{
		me = Controller->me;
	}

	if (anim->bAttacking)
	{
		return EBTNodeResult::Failed;
	}

	anim->bAttacking = true;

	me->Attack();

	return EBTNodeResult::Succeeded;

}
