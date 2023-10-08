// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskDie.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTTaskDie : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskDie();

public:

	class AEnemyAIController* Controller;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
