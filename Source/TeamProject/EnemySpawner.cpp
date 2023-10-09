// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(Th_SpeedHandle, [&]()
		{
			ReqSpawnEnemy();
		}, 3.0f, true);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemy()
{
	AEnemy* SpawnEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, GetActorLocation(), GetActorRotation());
	if (!SpawnEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemySpawner::SpawnEnemy : not spawn Enemy"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy it"));
	}
}

void AEnemySpawner::ReqSpawnEnemy_Implementation()
{
	SpawnEnemy();
}

void AEnemySpawner::ResSpawnEnemy_Implementation()
{

}