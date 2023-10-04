// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskAttackCrystal.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTTaskAttackCrystal : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTaskAttackCrystal();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	class AEnemyAIController* Controller;

	class UEnemyAnim* anim;	

	class AEnemy* me;
};
