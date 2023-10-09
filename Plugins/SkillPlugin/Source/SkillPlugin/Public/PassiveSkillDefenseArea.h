// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PassiveSkillBase.h"
#include "PassiveSkillDefenseArea.generated.h"

/**
 * 
 */
UCLASS()
class SKILLPLUGIN_API APassiveSkillDefenseArea : public APassiveSkillBase
{
	GENERATED_BODY()

public:
	APassiveSkillDefenseArea();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) override;


	void ApplySkillDamage();

	FTimerHandle Th_ProcessBeginOverlap;

	AActor* Target;

	UFUNCTION()
	virtual void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
