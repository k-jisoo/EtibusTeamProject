// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAIController.h"
#include "EnemyAnim.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 추후 false 수정 예정
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));

	GetMesh()->SetRelativeScale3D(FVector(0.84f));

	GetMesh()->SetCanEverAffectNavigation(true);

	anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

	//AIControllerClass 설정
	AIControllerClass = AEnemyAIController::StaticClass();
	//AI생성 옵션 - 앞으로 생성되는 AEnemy마다 AI를 잡아줌
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Controller = Cast<AEnemyAIController>(AIControllerClass);

	MaxHp = 100;

	CurHp = 100;

	

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	bLive = true;

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

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Take Damage!"));

	/*if (bLive == false)
		return 0.0f;

	if (CurHp < DamageAmount) {
		bLive = false;
	}
	

	if (EventInstigator == nullptr)
	{
		return 0.0f;
	}*/

	//Controller->GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::State, static_cast<uint8>(EEnemyState::Damage));
		
	UpdateHp(DamageAmount * -1);

	UE_LOG(LogTemp, Warning, TEXT("has Damage = %d"), DamageAmount);

	ReqPlayAnimMontage(DamageMontage);

	return DamageAmount;
}

void AEnemy::Die()
{
	Controller->GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::State, static_cast<uint8>(EEnemyState::Die));
}

void AEnemy::UpdateHp(float Amount)
{
	CurHp += Amount;
	CurHp = FMath::Clamp(CurHp, 0.0f, MaxHp);
}

void AEnemy::ReqPlayAnimMontage_Implementation(UAnimMontage* animMontage)
{
	ClientPlayAnimMontage(animMontage);
}

void AEnemy::ClientPlayAnimMontage_Implementation(UAnimMontage* animMontage)
{
	this->PlayAnimMontage(animMontage);
}

