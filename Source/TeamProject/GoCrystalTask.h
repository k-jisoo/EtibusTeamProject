// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GoCrystalTask.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UGoCrystalTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UGoCrystalTask();

	class AEnemy* me;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	//class AEnemyAIController* Controller;

};
