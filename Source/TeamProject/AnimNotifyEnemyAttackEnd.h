// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotifyEnemyAttackEnd.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UAnimNotifyEnemyAttackEnd : public UAnimNotify
{
	GENERATED_BODY()

public:

	UAnimNotifyEnemyAttackEnd();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
	
};
