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
    AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

    if (Enemy && Enemy->bAttacking == false)
    {
        Enemy->Attack();
    }

    return EBTNodeResult::Failed;

}
