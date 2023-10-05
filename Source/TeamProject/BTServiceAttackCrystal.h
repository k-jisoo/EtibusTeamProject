// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceAttackCrystal.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTServiceAttackCrystal : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTServiceAttackCrystal();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	class AEnemyAIController* Controller;

	class AEnemy* me;
};
