// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "ActiveSkillBase.generated.h"

/**
 * 
 */
UCLASS()
class SKILLPLUGIN_API AActiveSkillBase : public ASkillBase
{
	GENERATED_BODY()
	
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Active
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "component")
	class UProjectileMovementComponent* SkillMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Cooldown;

	virtual void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) override;



};
