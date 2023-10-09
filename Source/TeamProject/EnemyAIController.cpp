// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameFramework/Controller.h"
#include "Enemy.h"
#include "EnemyAnim.h"

const FName AEnemyAIController::CrysytalVector(TEXT("CrysytalVector"));
const FName AEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyAIController::TargetKey(TEXT("Target"));
const FName AEnemyAIController::AttackTargetKey(TEXT("AttackTargetKey"));
const FName AEnemyAIController::State(TEXT("State"));


AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BlackboardData'/Game/MyContent/Blueprints/AI/BB_Enemy.BB_Enemy'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/MyContent/Blueprints/AI/BT_Enemy.BT_Enemy'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	//class UEnemyAnim* anim;
	SetPerceptionSystem();
	
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//애님 인스턴드 불러오기
	me = Cast<AEnemy>(GetPawn());
	if (me == nullptr) 
	{
		return;
	}

	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	if (anim != nullptr) 
	{
		anim->animState = EEnemyState::MoveCrystal;
	}
	
	//블랙보드 텀포넌트 기본설정
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsVector("CrysytalVector", FVector(0, 0, 0));
		if (!RunBehaviorTree(BTAsset))
		{
			return;
		}
		BlackboardComp->SetValueAsObject(TargetKey, nullptr);
	}

	// # Ible로 변경 후 자동 Move설정
	BlackboardComp->SetValueAsEnum(State, static_cast<uint8>(EEnemyState::Ible));

	BlackboardComp->SetValueAsBool(TEXT("bLive"), true);

}

void AEnemyAIController::SetPerceptionSystem()
{

	//플래이어 발견 Perception
	SightPerception = CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("SightPerception"));
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = Sight;
	SightConfig->LoseSightRadius = SightConfig->SightRadius;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightPerception->ConfigureSense(*SightConfig);
	SightPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);

	//플래이어 공격 범위 Perception
	AttackPerception = CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AttackPerception"));
	AttackConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("AttackConfig"));
	AttackConfig->SightRadius = AttackRange;
	AttackConfig->LoseSightRadius = AttackConfig->SightRadius;
	AttackConfig->PeripheralVisionAngleDegrees = 180.0f;
	AttackConfig->DetectionByAffiliation.bDetectEnemies = true;
	AttackConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AttackConfig->DetectionByAffiliation.bDetectFriendlies = true;
	AttackPerception->ConfigureSense(*AttackConfig);
	AttackPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetAttack);

}

void AEnemyAIController::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (BlackboardComp->GetValueAsEnum("State") == static_cast<uint8>(EEnemyState::AttackCrystal))
		return;
	//만났을 때 Player일 경우 지속
	TArray<FName> ActorTags = actor->Tags;
	for (FName Tag : ActorTags) 
	{
		if (Tag == "Player")
		{
			UpdatedTarget(actor);
		}
		if (Tag == "Crystal")
		{
			BlackboardComp->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::AttackCrystal));
			BlackboardComp->SetValueAsObject(TargetKey, actor);
			Target = actor;
		}
	}
	
}

void AEnemyAIController::OnTargetAttack(AActor* actor, FAIStimulus const Stimulus)
{
	TArray<FName> ActorTags = actor->Tags;
	for (FName Tag : ActorTags) {
		if (Tag == "Player") {
			BlackboardComp->SetValueAsEnum(State, static_cast<uint8>(EEnemyState::AttackTarget));
			BlackboardComp->SetValueAsObject(TargetKey, actor);
			Target = actor;
		}
	}
}

void AEnemyAIController::UpdatedTarget(AActor* actor)
{
	// 한번 발견한 대상을 잊어버리기 까지 쫒아가고 잊어버리면 바로 크리스탈쪽으로 이동하는 로직

	if (BlackboardComp->GetValueAsObject(TargetKey) == nullptr) 
	{
		BlackboardComp->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::MoveTarget));
		BlackboardComp->SetValueAsObject(TargetKey, actor);
	}
	else if (actor == BlackboardComp->GetValueAsObject(TargetKey))
	{
		BlackboardComp->SetValueAsObject(TargetKey, nullptr);
		BlackboardComp->SetValueAsEnum("State", static_cast<uint8>(EEnemyState::Ible));
	}
	
}
