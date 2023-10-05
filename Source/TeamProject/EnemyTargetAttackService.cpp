// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTargetAttackService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy.h"
#include "EnemyAIController.h"
#include "GameFramework/Actor.h"

UEnemyTargetAttackService::UEnemyTargetAttackService()
{
	NodeName = TEXT("BTServiceAttackTarget");
	Interval = 1.5f;
}

void UEnemyTargetAttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (Controller == nullptr) 
	{
		Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
		if(Controller ==nullptr) {
			Controller->GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::Ible));
			return;
		}
	}

	//if (anim == null	//if (anim == nullptr)
	//{
	//	anim = Controller->anim;
	//}

	//UE_LOG(LogTemp, Warning, TEXT("%s"), anim->bAttacking ? TEXT("True") : TEXT("False"));
	//
	//if (anim->bAttacking)
	//	return;

	//AEnemy* me = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	me = Controller->me;
	if (me == nullptr) 
	{
		Controller->GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::Ible));
		return;
	}

	//AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject("Target"));
	AActor* Target = Controller->Target;
	if (Target == nullptr)
	{
		Controller->GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::Ible));
		return;
	}

	float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());

	if ( Target == nullptr) 
	{
		Controller->GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::Ible));
		return;
	}
	if (distance > me->AttackRange && Target != nullptr) {
		Controller->GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::MoveTarget));
		return;
	}
}
