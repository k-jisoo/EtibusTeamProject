// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTackAttack.h"
#include "Enemy.h"
#include "EnemyAnim.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Crystal.h"


UBTTackAttack::UBTTackAttack()
{
	NodeName = TEXT("BTTackAttackTarget");
}

EBTNodeResult::Type UBTTackAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (Controller == nullptr) 
	{
		Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
		if (Controller ==nullptr) 
		{
			return EBTNodeResult::Failed;
		}
	}

	if (anim == nullptr) 
	{
		anim = Controller->anim;
	}

	if (anim->bAttacking)
	{
		return EBTNodeResult::Failed;
	}

	anim->bAttacking = true;

	anim->PlayAttackMontage();

	return EBTNodeResult::Succeeded;

}