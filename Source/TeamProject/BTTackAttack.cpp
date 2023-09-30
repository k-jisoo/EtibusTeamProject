// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTackAttack.h"
#include "Enemy.h"
#include "EnemyAnim.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Crystal.h"


UBTTackAttack::UBTTackAttack()
{
}

EBTNodeResult::Type UBTTackAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	AEnemy* me = Cast<AEnemy>(Controller->GetPawn());
	if (me == nullptr)
	{
		Controller->GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::Move));
		return EBTNodeResult::Failed;
	}

	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	anim->bAttackPlay = true;


	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject("Target"));
	if (nullptr == Target)
	{
		Controller->GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::Move));
		return EBTNodeResult::Failed;
	}

	float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());

	bAttackPlay = anim->bAttackPlay;

	if (distance > 100.0f || Target == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("distance = %f"), distance);
		Controller->GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::Move));
		return EBTNodeResult::Failed;
	}

	anim->PlayAttackMontage();
	return EBTNodeResult::Succeeded;
}


