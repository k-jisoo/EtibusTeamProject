// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "PassiveSkillBase.generated.h"

/**
 * 
 */
UCLASS()
class SKILLPLUGIN_API APassiveSkillBase : public ASkillBase
{
	GENERATED_BODY()
	
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	virtual void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) override;
};
