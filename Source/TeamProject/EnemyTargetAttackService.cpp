// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTargetAttackService.h"
#include "EnemyAIController.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UEnemyTargetAttackService::UEnemyTargetAttackService()
{
	NodeName = TEXT("AttackTarget");
	Interval = 1.0f;
}

void UEnemyTargetAttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemy* me = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	testint++;

	UE_LOG(LogTemp, Warning, TEXT("%d"), testint);
}
