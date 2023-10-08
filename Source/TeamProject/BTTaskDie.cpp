// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskDie.h"
#include "Enemy.h"
#include "EnemyAIController.h"


UBTTaskDie::UBTTaskDie()
{
	NodeName = TEXT("BBTackDie");
}


EBTNodeResult::Type UBTTaskDie::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


	//Controller 불러오기
	if (Controller == nullptr)
	{
		Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
		if (Controller == nullptr)
		{
			return EBTNodeResult::Failed;
		}
	}

	AEnemy* me = Controller->me;

	if (me == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("BTTackDie : ExactCuteTask Can not Cast AEnemy"))
		return EBTNodeResult::Failed;
	}

	



	//me->ReqPlayAnimMontage(me->DieMontage);

	return EBTNodeResult::Succeeded;
}


