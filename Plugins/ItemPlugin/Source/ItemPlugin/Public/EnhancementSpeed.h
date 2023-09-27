// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatEnhancementObjectBase.h"
#include "EnhancementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class ITEMPLUGIN_API AEnhancementSpeed : public AStatEnhancementObjectBase
{
	GENERATED_BODY()
	
public:
	AEnhancementSpeed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EnhancementPlayerStat() override;
};
