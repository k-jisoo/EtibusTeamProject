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

    AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

    if (Enemy && Enemy->bAttacking == false)
    {
        Enemy->Attack();
    }

    return EBTNodeResult::Failed;

}