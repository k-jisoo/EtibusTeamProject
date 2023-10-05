// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EnemyTargetAttackService.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UEnemyTargetAttackService : public UBTService
{
	GENERATED_BODY()
	
public:

	UEnemyTargetAttackService();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	class AEnemyAIController* Controller;

	class UEnemyAnim* anim;

	class AEnemy* me;
};
