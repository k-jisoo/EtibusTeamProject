// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Ible,
	Damage,
	Die,
	MoveCrystal,
	AttackCrystal,
	MoveTarget,
	AttackTarget
};
/**
 * 
 */
UCLASS()

class TEAMPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:	

	AEnemyAIController();


// Anim
public:

	UPROPERTY()
	class AEnemy* me;

	UPROPERTY()
	class UEnemyAnim* anim;

	UPROPERTY()
	class AActor* Target;

	virtual void OnPossess(APawn* InPawn) override;

	static const FName CrysytalVector;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName AttackTargetKey;
	static const FName State;

// BT && BB
private:

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UBlackboardComponent* BlackboardComp = Blackboard;

// Perception
public:

	float Sight = 500.0f;

	float AttackRange = 200.0f;

	void SetPerceptionSystem();

	//2 Perception
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	class UAIPerceptionComponent* SightPerception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	class UAIPerceptionComponent* AttackPerception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	class UAISenseConfig_Sight* AttackConfig;

	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);

	UFUNCTION()
	void OnTargetAttack(AActor* actor, FAIStimulus const Stimulus);
	
	void UpdatedTarget(AActor* actor);

};
