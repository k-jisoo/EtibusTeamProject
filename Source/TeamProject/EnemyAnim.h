// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAIController.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnim();

	class AEnemy* me;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEnemyState animState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIController)
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIController)
	bool bAttackPlay = false;

	UFUNCTION(BlueprintCallable, Category = FSMEvent)
	void OnAttackEndAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
	void PlayDamageAnim(FName sectionName);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool bDieDone = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	UAnimSequenceBase* AttackSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	UAnimMontage* AttackMontage;

	bool bAttacking;
};
