// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAIController.h"
#include "EnemyAnim.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DropItemActor.h"
#include <Components/CapsuleComponent.h>
#include "Engine/World.h"


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

void AEnemy::Attack()
{
	ReqPlayAnimMontage(AttackMontage);
}

//# 킬뎃 & 아이템
void AEnemy::Die()
{
	
	AIController = Cast<AEnemyAIController>(GetController());

	AIController->GetBlackboardComponent()->SetValueAsEnum(AEnemyAIController::State, static_cast<uint8>(EEnemyState::MoveCrystal));
	AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("bLive"), false);

	ADropItemActor* Item = GetWorld()->SpawnActor<ADropItemActor>(DropItem, this->GetActorLocation(), this->GetActorRotation());

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemy::DestoryCharactor, 5.0f , false);

	ReqPlayAnimMontage(DieMontage);
}

void AEnemy::DestoryCharactor()
{
	Destroy();
}


float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (bLive == false)
		return 0.0f;

	if (EventInstigator != nullptr)
	{
		AActor* actor = EventInstigator->GetPawn();

		TArray<FName> ActorTags = actor->Tags;
		for (FName Tag : ActorTags)
		{
			if (Tag != "Player")
			{
				return 0.0f;
			}
		}
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemy::TakeDamage not has EventInstigator"));
	}

	

	UpdateHp(DamageAmount * -1);

	UE_LOG(LogTemp, Warning, TEXT("has Damage = %f"), DamageAmount);

	return DamageAmount;
}

void AEnemy::UpdateHp(float Amount)
{
	CurHp += Amount;

	CurHp = FMath::Clamp(CurHp, 0.0f, MaxHp);

	UE_LOG(LogTemp, Warning, TEXT("Enemy CurHP = %f"), CurHp);

	if (CurHp <= 0)
	{
		bLive = false;

		if (DieMontage != nullptr)
		{
			Die(); 
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AEnemy::UpdateHp() : not has DieMontage!"));
		}
	}
	else 
	{
		if (DamageMontage != nullptr)
		{
			ReqPlayAnimMontage(DamageMontage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AEnemy::UpdateHp() : not has DamageMontage!"));
		}
	}
}

void AEnemy::ReqPlayAnimMontage_Implementation(UAnimMontage* animMontage)
{
	UE_LOG(LogTemp,Warning,TEXT("ReqPlayAnimMontage"));

	ClientPlayAnimMontage(animMontage);
}

void AEnemy::ClientPlayAnimMontage_Implementation(UAnimMontage* animMontage)
{
	this->PlayAnimMontage(animMontage);
}

