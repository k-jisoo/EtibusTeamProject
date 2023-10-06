// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "InputActionValue.h"
#include "ItemInterface.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UAnimMontage;
class UAnimBlueprint;
class USphereComponent;
class UStaticMeshComponent;
class UArrowComponent;
class AWeapon;
class UParticleSystem;

enum class EItemType : uint8;

USTRUCT(BlueprintType)
struct FST_Character : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimBlueprint* AnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* LevelStartMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* FirstAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SecondAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ThirdAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* FourthAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BoxCollisionExt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* HitParticle;
};

UCLASS()
class TEAMPROJECT_API ABaseCharacter : public ACharacter, public IItemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	void Look(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);

	void StopJump(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void UsingSkill_First(const FInputActionValue& Value);

	void UsingSkill_Second(const FInputActionValue& Value);

	void UsingSkill_Third(const FInputActionValue& Value);

	void UsingSkill_Fourth(const FInputActionValue& Value);

	void CloseWidget(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ReqAttack();
	
	UFUNCTION(NetMulticast, Reliable)
	void AttackSwitch();

	void ComboAttackSave();

	void ResetCombo();

	void SetInputPossible();

	void SetSpectatorMode();

	APlayerController* GetTargetPlayerController();

	UFUNCTION(Server, Reliable)
	void ReqSetLobbyCharacter(USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBP);

	UFUNCTION(NetMulticast, Reliable)
	void SetLobbyCharacter(USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBP);

	UFUNCTION(Server, Reliable)
	void ReqSetCharacter(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp, UAnimMontage* firstAttackMontage, UAnimMontage* secondAttackMontage, UAnimMontage* thirdAttackMontage, UAnimMontage* fourthAttackMontage, UAnimMontage* levelStartMontage, float maxHp, float damage, float speed, float capsuleHeight, float capsuleRadius, FVector boxCollisionExt, UParticleSystem* hitParticle);

	UFUNCTION(NetMulticast, Reliable)
	void RecSetCharacter(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp, UAnimMontage* firstAttackMontage, UAnimMontage* secondAttackMontage, UAnimMontage* thirdAttackMontage, UAnimMontage* fourthAttackMontage, UAnimMontage* levelStartMontage, float maxHp, float damage, float speed, float capsuleHeight, float capsuleRadius, FVector boxCollisionExt, UParticleSystem* hitParticle);

	UFUNCTION(Server, Reliable)
	void ReqSetWeapon(FVector boxCollisionExt);

	UFUNCTION(NetMulticast, Reliable)
	void RecSetWeapon(FVector boxCollisionExt);

	UFUNCTION(Server, Reliable)
	void ReqPlayAnimMontage(UAnimMontage* animMontage);

	UFUNCTION(NetMulticast, Reliable)
	void ClientPlayAnimMontage(UAnimMontage* animMontage);

	UFUNCTION(Server, Reliable)
	void ReqLog(const FString& message);

	UFUNCTION(NetMulticast, Reliable)
	void ClientLog(const FString& message);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventGetItem(EItemType itemType);

	void EventGetItem_Implementation(EItemType itemType) override;

	UFUNCTION(Server, Reliable)
	void ReqServerSpawnSkillActor(AMainPlayerController* spawnUser, ASkillBase* spawnSkill, UBoxComponent* SkillArea, UParticleSystemComponent* SkillBody, float SkillDamage, FVector Collision, FVector skillSize);

	UFUNCTION(NetMulticast, Reliable)
	void ResSpawnSkillActor(AMainPlayerController* spawnUser, ASkillBase* spawnSkill, UBoxComponent* SkillArea, UParticleSystemComponent* SkillBody, float SkillDamage, FVector Collision, FVector skillSize);

	UFUNCTION()
	void SpawnActor(AMainPlayerController* spawnUser, ASkillBase* spawnSkill, UBoxComponent* SkillArea, UParticleSystemComponent* SkillBody, float SkillDamage, FVector Collision, FVector skillSize);

	UFUNCTION()
	void OnRep_IsSimulatingPhysics();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UArrowComponent* SkillSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Skill_First;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Skill_Second;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Skill_Third;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Skill_Fourth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_OpenWidgetTest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UAnimMontage* LevelStartMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UAnimMontage* FirstAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UAnimMontage* SecondAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UAnimMontage* ThirdAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UAnimMontage* FourthAttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UParticleSystem* HitParticle;

	FST_Character* ST_Character;

	FTimerHandle MyTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TSubclassOf<class AActiveSkillStorm> StormClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TSubclassOf<class AActiveSkillLightning> LightningClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TSubclassOf<class AActiveSkillWaterBall> WaterBallClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TSubclassOf<class APassiveSkillDefenseArea> DefenseAreaClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	AActiveSkillStorm* storm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	AActiveSkillLightning* lightning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	AActiveSkillWaterBall* waterBall;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	APassiveSkillDefenseArea* defenseArea;


public:
	bool IsAttacking;
	bool IsSaveAttack;
	bool IsInputPossible;
	int AttackCount;
};
