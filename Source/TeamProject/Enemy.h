// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class TEAMPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 적 옵션 관련
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=EnemyOption)
	float CurHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyOption)
	float MaxHp = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyOption)
	float Sight = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyOption)
	float AttackRange = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyOption)
	float Damage = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyOption)
	float Speed = 200.0f;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UAnimMontage* DamageMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UAnimMontage* DieMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TSubclassOf<class ADropItemActor> DropItem;

	bool bAttacking;

	bool bLive;



//Anim 관련
public:
	class UEnemyAnim* anim;

	class AEnemyAIController* AIController;

//Anim 관련 함수
public :

	void Attack();

	void Die();

	void DestoryCharactor();

	UFUNCTION(Server, Reliable)
	void ReqPlayAnimMontage(UAnimMontage* animMontage);

	UFUNCTION(NetMulticast, Reliable)
	void ClientPlayAnimMontage(UAnimMontage* animMontage);

public:

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

protected:

	void UpdateHp(float Amount);

};
