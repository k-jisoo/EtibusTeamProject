// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHp = 100;
	CurHp = 100;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	AActor::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EventInstigator == nullptr)
		return 0.0f;
	
	UpdateHp(DamageAmount * -1);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
	//FString::Printf(TEXT("CurHp = %s"), CurHp);

	return DamageAmount;
}

void AEnemy::UpdateHp(float Amount)
{
	CurHp += Amount;
	CurHp = FMath::Clamp(CurHp, 0.0f, MaxHp);
}

