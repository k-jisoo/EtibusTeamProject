// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveSkillBase.h"
#include "ActiveSkillLightning.generated.h"

/**
 * 
 */
UCLASS()
class SKILLPLUGIN_API AActiveSkillLightning : public AActiveSkillBase
{
	GENERATED_BODY()

public:
	AActiveSkillLightning();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) override;

	AActor* Target;

	void ApplySkillDamage();

};
