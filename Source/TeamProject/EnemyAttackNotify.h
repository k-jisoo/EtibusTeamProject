// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnemyAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UEnemyAttackNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName Socket = TEXT("RightHand");

};
