// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceAttackCrystal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy.h"
#include "EnemyAIController.h"


UBTServiceAttackCrystal::UBTServiceAttackCrystal()
{
	NodeName = TEXT("BTServiceAttackCrystal");
	Interval = 5.0;
}

void UBTServiceAttackCrystal::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (Controller == nullptr)
	{
		Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
		if (Controller == nullptr) {
			return;
		}
	}

	me = Controller->me;
	if (me == nullptr)
	{
		return;
	}

	AActor* Target = Controller->Target;
	if (Target == nullptr)
	{
		return;
	}

	float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());

	if (Target == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("%f"), distance);

	FVector distination = Target->GetActorLocation();
	FVector dir = distination - me->GetActorLocation();
	if (distance > 300.0f && Target != nullptr) {
		me->AddMovementInput(dir.GetSafeNormal());
	}

	UE_LOG(LogTemp, Warning, TEXT("Attack!!"));
	
}
