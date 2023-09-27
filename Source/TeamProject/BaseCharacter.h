// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UAnimMontage;
class UAnimBlueprint;
class USphereComponent;
class UStaticMeshComponent;
class AWeapon;
class UParticleSystem;

USTRUCT(BlueprintType)
struct FST_Character : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacter> CharacterBp;
};

UCLASS()
class TEAMPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

public:
	void Look(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);

	void StopJump(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ReqAttack();
	
	UFUNCTION(NetMulticast, Reliable)
	void AttackSwitch();

	void ComboAttackSave();

	void ResetCombo();

	void SetInputPossible();

	UFUNCTION(Server, Reliable)
	void ReqSetCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void RecSetCharacter();


	void SetWeapon();

	UFUNCTION(Server, Reliable)
	void ReqPlayAnimMontage(UAnimMontage* animMontage);

	UFUNCTION(NetMulticast, Reliable)
	void ClientPlayAnimMontage(UAnimMontage* animMontage);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UInputAction* IA_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UAnimMontage* LevelStartMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UAnimMontage* FirstAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UAnimMontage* SecondAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UAnimMontage* ThirdAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UAnimMontage* FourthAttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	AWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	bool IsAttacking;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	bool IsSaveAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	bool IsInputPossible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	int AttackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float CurHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FVector BoxCollisionExt;

	FST_Character* ST_Character;

	FTimerHandle MyTimerHandle;
};
