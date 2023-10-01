// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveSkillBase.h"
#include "ActiveSkillStorm.generated.h"

/**
 * 
 */
UCLASS()
class SKILLPLUGIN_API AActiveSkillStorm : public AActiveSkillBase
{
	GENERATED_BODY()
	
public:
	AActiveSkillStorm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) override;

	FTimerHandle Th_ProcessBeginOverlap;

	AActor* Target;

	void ApplySkillDamage();

	UFUNCTION()
	virtual void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
};
