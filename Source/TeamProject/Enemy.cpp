// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAIController.h"
#include "EnemyAnim.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 추후 false 수정 예정
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));

	GetMesh()->SetRelativeScale3D(FVector(0.84f));

	//AIControllerClass 설정
	AIControllerClass = AEnemyAIController::StaticClass();
	//AI생성 옵션 - 앞으로 생성되는 AEnemy마다 AI를 잡아줌
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MaxHp = 100;
	CurHp = 100;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
//{
//	OnAttackEnd.Broadcast();
//}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EventInstigator == nullptr)
		return 0.0f;

	UpdateHp(DamageAmount * -1);

	return DamageAmount;
}

void AEnemy::UpdateHp(float Amount)
{
	CurHp += Amount;
	CurHp = FMath::Clamp(CurHp, 0.0f, MaxHp);
}
