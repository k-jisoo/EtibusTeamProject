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

public:

	//FOnAttackEndDelegate OnAttackEnd;

	//UFUNCTION(BlueprintCallable, Category = AnimMontage)
	//void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//class UCharacterMovementComponent* Movement;

//데미지 관련
public:

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser) override;

	void UpdateHp(float Amount);

public:
	float CurHp;
	float MaxHp;

};
