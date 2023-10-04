// Fill out your copyright notice in the Description page of Project Settings.


#include "GoCrystalTask.h"
#include "Enemy.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

UGoCrystalTask::UGoCrystalTask()
{
	NodeName = TEXT("BBTackMoveCrystal");
}

EBTNodeResult::Type UGoCrystalTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (Controller==nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Controller->GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::State, static_cast<uint8>(EEnemyState::MoveCrystal));

	return EBTNodeResult::Succeeded;

}

