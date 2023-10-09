// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class TEAMPROJECT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


//
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemy> EnemyClass;

	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION(Server, Reliable)
	void ReqSpawnEnemy();

	UFUNCTION(NetMulticast, Reliable)
	void ResSpawnEnemy();

	FTimerHandle Th_SpeedHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Categroy=Spawn)
	float SpawnDelay = 3.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Categroy = Spawn)
	bool SpawnRoop = true;

};
