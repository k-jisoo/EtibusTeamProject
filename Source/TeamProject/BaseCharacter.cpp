// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon.h"
#include "TPGameInstance.h"
#include "Particles/ParticleSystem.h"
#include "MainPlayerController.h"
#include "SkillManagementComponent.h"
#include "StatManagementComponent.h"
#include "ActiveSkillStorm.h"
#include "ActiveSkillLightning.h"
#include "ActiveSkillWaterBall.h"
#include "PassiveSkillDefenseArea.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "StatManagementComponent.h"
#include "GameFramework/PlayerController.h"
#include "MainGameMode.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 100.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SkillSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ch_Arrow"));
	SkillSpawnPoint->SetupAttachment(GetRootComponent());
	SkillSpawnPoint->AddLocalOffset(FVector(200.0f, 0, 0.0f));

	IsAttacking = false;
	IsSaveAttack = false;
	AttackCount = 0;
	IsInputPossible = false;

	bReplicates = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UStatManagementComponent* StatComponent = FindComponentByClass<UStatManagementComponent>();

	if (!StatComponent)
		return 0.0f;

	StatComponent->CurHp -= DamageAmount;

	if (StatComponent->CurHp <= 0)
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

		PC->ReqDieProcess(GetMesh());
	}

	UE_LOG(LogTemp, Warning, TEXT("%f"), StatComponent->CurHp);

	return DamageAmount;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);

		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ABaseCharacter::Jump);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ABaseCharacter::StopJump);

		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &ABaseCharacter::Attack);

		EnhancedInputComponent->BindAction(IA_Skill_First, ETriggerEvent::Started, this, &ABaseCharacter::UsingSkill_First);

		EnhancedInputComponent->BindAction(IA_Skill_Second, ETriggerEvent::Started, this, &ABaseCharacter::UsingSkill_Second);

		EnhancedInputComponent->BindAction(IA_Skill_Third, ETriggerEvent::Started, this, &ABaseCharacter::UsingSkill_Third);

		EnhancedInputComponent->BindAction(IA_Skill_Fourth, ETriggerEvent::Started, this, &ABaseCharacter::UsingSkill_Fourth);

		EnhancedInputComponent->BindAction(IA_OpenWidgetTest, ETriggerEvent::Started, this, &ABaseCharacter::CloseWidget);
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, StormClass);
	DOREPLIFETIME(ABaseCharacter, LightningClass);
	DOREPLIFETIME(ABaseCharacter, WaterBallClass);
	DOREPLIFETIME(ABaseCharacter, DefenseAreaClass);
	DOREPLIFETIME(ABaseCharacter, storm);
	DOREPLIFETIME(ABaseCharacter, waterBall);
	DOREPLIFETIME(ABaseCharacter, lightning);
	DOREPLIFETIME(ABaseCharacter, defenseArea);

}

void ABaseCharacter::EventGetItem_Implementation(EItemType itemType)
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

	if (PC)
	{
		UStatManagementComponent* StatManager = Cast<UStatManagementComponent>(PC->FindComponentByClass<UStatManagementComponent>());

		if (StatManager)
		{
			switch (itemType)
			{
			case EItemType::IT_RecoveryHp:
			{
				UE_LOG(LogTemp, Warning, TEXT("IT_RecoveryHp"));
				StatManager->RecurberyHp();
				break;
			}
			case EItemType::IT_RecoveryMp:
			{
				UE_LOG(LogTemp, Warning, TEXT("IT_RecoveryMp"));
				StatManager->RecurberyMp();
				break;
			}
			case EItemType::IT_SpeedUp:
			{
				UE_LOG(LogTemp, Warning, TEXT("IT_SpeedUp"));
				StatManager->BurfSpeed();
				break;
			}
			case EItemType::IT_PowerUp:
			{
				UE_LOG(LogTemp, Warning, TEXT("IT_PowerUp"));
				StatManager->BurfPower();
				break;
			}
			case EItemType::IT_Gold:
			{
				UE_LOG(LogTemp, Warning, TEXT("IT_Gold"));
				PC->Gold += 50;
				PC->OnUpdateMyGold(PC->Gold);
				break;
			}
			}
		}
	}

}

void ABaseCharacter::SpawnActor(AMainPlayerController* spawnUser, ASkillBase* spawnSkill, UBoxComponent* SkillArea, UParticleSystemComponent* SkillBody, float SkillDamage, FVector Collision, FVector skillSize)
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(spawnUser);

	if (PC)
	{
		USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

		if (SkillManager)
		{
			FVector Location = SkillSpawnPoint->GetComponentLocation();

			FRotator Rotation = SkillSpawnPoint->GetComponentRotation();

			FVector ForwardVector = Rotation.Vector();

			//UE_LOG(LogTemp, Warning, TEXT("%s"), *spawnSkill->SkillName);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *spawnSkill->GetName());

			if (spawnSkill->SkillName == "Fire Storm")
			{
				if (!HasAuthority()) return;

				storm = GetWorld()->SpawnActor<AActiveSkillStorm>(StormClass, Location + (ForwardVector * 200 * SkillManager->StormLevel), Rotation);

				if (storm)
				{
					storm->Damage = SkillDamage;
					storm->SkillArea->SetBoxExtent(Collision);
					storm->SkillBody->SetRelativeScale3D(skillSize);
					storm->OwnChar = this;
					UParticleSystemComponent* ParticleComponent = storm->FindComponentByClass<UParticleSystemComponent>();
					if (ParticleComponent)
					{
						ParticleComponent->Activate();
					}
					UE_LOG(LogTemp, Warning, TEXT("%d"), SkillManager->StormLevel);
					storm->SetActorHiddenInGame(false);
					// 해당 액터를 리플리케이트합니다.
					storm->ForceNetUpdate(); // 액터의 속성을 변경한 경우, 이것을 호출하여 업데이트를 강제로 보낼 수 있습니다.

				}

				SkillManager->UsingSkill(spawnSkill);

			}
			else if (spawnSkill->SkillName == "Lightning")
			{
				lightning = GetWorld()->SpawnActor<AActiveSkillLightning>(LightningClass, Location + (ForwardVector * 100 * SkillManager->LightningLevel), Rotation);

				if (lightning)
				{
					lightning->Damage = SkillDamage;
					lightning->SkillArea->SetBoxExtent(Collision);
					lightning->SkillBody->SetRelativeScale3D(skillSize);
					lightning->OwnChar = this;
					UParticleSystemComponent* ParticleComponent = lightning->FindComponentByClass<UParticleSystemComponent>();
					if (ParticleComponent)
					{
						ParticleComponent->Activate();
					}
					UE_LOG(LogTemp, Warning, TEXT("%d"), SkillManager->LightningLevel);
					lightning->SetActorHiddenInGame(false);
					// 해당 액터를 리플리케이트합니다.
					lightning->ForceNetUpdate(); // 액터의 속성을 변경한 경우, 이것을 호출하여 업데이트를 강제로 보낼 수 있습니다.

				}

				SkillManager->UsingSkill(spawnSkill);

			}
			else if (spawnSkill->SkillName == "Water Ball")
			{
				waterBall = GetWorld()->SpawnActor<AActiveSkillWaterBall>(WaterBallClass, Location + (ForwardVector * 50 * SkillManager->WaterBallLevel), Rotation);

				if (waterBall)
				{
					waterBall->Damage = SkillDamage;
					waterBall->SkillArea->SetBoxExtent(Collision);
					waterBall->SkillBody->SetRelativeScale3D(skillSize);
					waterBall->OwnChar = this;
					UParticleSystemComponent* ParticleComponent = waterBall->FindComponentByClass<UParticleSystemComponent>();
					if (ParticleComponent)
					{
						ParticleComponent->Activate();
					}
					UE_LOG(LogTemp, Warning, TEXT("%d"), SkillManager->WaterBallLevel);
					waterBall->SetActorHiddenInGame(false);
					// 해당 액터를 리플리케이트합니다.
					waterBall->ForceNetUpdate(); // 액터의 속성을 변경한 경우, 이것을 호출하여 업데이트를 강제로 보낼 수 있습니다.

				}

				SkillManager->UsingSkill(spawnSkill);

			}
			else if (spawnSkill->SkillName == "Defense Area")
			{
				defenseArea = GetWorld()->SpawnActor<APassiveSkillDefenseArea>(DefenseAreaClass, Location, Rotation);

				if (defenseArea)
				{
					defenseArea->Damage = SkillDamage;
					defenseArea->SkillArea->SetBoxExtent(Collision);
					defenseArea->SkillBody->SetRelativeScale3D(skillSize);
					defenseArea->OwnChar = this;
					UParticleSystemComponent* ParticleComponent = defenseArea->FindComponentByClass<UParticleSystemComponent>();
					if (ParticleComponent)
					{
						ParticleComponent->Activate();
					}
					UE_LOG(LogTemp, Warning, TEXT("%d"), SkillManager->DefenseAreaLevel);
					defenseArea->SetActorHiddenInGame(false);
					// 해당 액터를 리플리케이트합니다.
					defenseArea->ForceNetUpdate(); // 액터의 속성을 변경한 경우, 이것을 호출하여 업데이트를 강제로 보낼 수 있습니다.

				}

				SkillManager->UsingSkill(spawnSkill);
			}
		}
	}
}

void ABaseCharacter::OnRep_IsSimulatingPhysics()
{
	
}


void ABaseCharacter::ResSpawnSkillActor_Implementation(AMainPlayerController* spawnUser, ASkillBase* spawnSkill, UBoxComponent* SkillArea, UParticleSystemComponent* SkillBody, float SkillDamage, FVector Collision, FVector skillSize)
{

}


void ABaseCharacter::ReqServerSpawnSkillActor_Implementation(AMainPlayerController* spawnUser, ASkillBase* spawnSkill, UBoxComponent* SkillArea, UParticleSystemComponent* SkillBody, float SkillDamage, FVector Collision, FVector skillSize)
{
	SpawnActor(spawnUser, spawnSkill, SkillArea, SkillBody, SkillDamage, Collision, skillSize);

	ResSpawnSkillActor(spawnUser, spawnSkill, SkillArea, SkillBody, SkillDamage, Collision, skillSize);
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	if (!IsInputPossible)
		return;

	FVector2D v = Value.Get<FVector2D>();
	AddControllerYawInput(v.X);
	AddControllerPitchInput(v.Y);
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	if (!IsInputPossible)
		return;

	FVector2D MoveVector = Value.Get<FVector2D>();

	FRotator controlRot = GetControlRotation();
	FRotator controlYawRot = FRotator(0, controlRot.Yaw, 0);

	FVector PawnForwardVector = UKismetMathLibrary::GetForwardVector(controlYawRot);
	FVector PawnRightVector = UKismetMathLibrary::GetRightVector(controlYawRot);

	AddMovementInput(PawnForwardVector, MoveVector.Y);
	AddMovementInput(PawnRightVector, MoveVector.X);
}

void ABaseCharacter::Jump(const FInputActionValue& Value)
{
	if (!IsInputPossible)
		return;

	ACharacter::Jump();
}

void ABaseCharacter::StopJump(const FInputActionValue& Value)
{
	ACharacter::StopJumping();
}

void ABaseCharacter::Attack(const FInputActionValue& Value)
{
	ReqAttack();
}

void ABaseCharacter::UsingSkill_First(const FInputActionValue& Value)
{
	if (HasAuthority())
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

		USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

		if (PC && PC->PlayerSkills.Num() > 0 && PC->IsCanUseSkill(PC->PlayerSkills[0]) && SkillManager->GetSkillColldown(PC->PlayerSkills[0]))
		{
			// 서버에서는 바로 액터 스폰
			SpawnActor(PC, PC->PlayerSkills[0], PC->PlayerSkills[0]->SkillArea, PC->PlayerSkills[0]->SkillBody, PC->PlayerSkills[0]->Damage * (SkillManager->GetSkillLevel(PC->PlayerSkills[0])),
				PC->PlayerSkills[0]->CollisionSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[0])),
				PC->PlayerSkills[0]->SkillSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[0])));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill 1 Casting Fail"))
				return;
		}
	}
	else if (!HasAuthority())
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

		USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

		if (PC && PC->PlayerSkills.Num() > 0 && PC->IsCanUseSkill(PC->PlayerSkills[0]) && SkillManager->GetSkillColldown(PC->PlayerSkills[0]))
		{
			ReqServerSpawnSkillActor(PC, PC->PlayerSkills[0], PC->PlayerSkills[0]->SkillArea, PC->PlayerSkills[0]->SkillBody, PC->PlayerSkills[0]->Damage * (SkillManager->GetSkillLevel(PC->PlayerSkills[0])),
				PC->PlayerSkills[0]->CollisionSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[0])),
				PC->PlayerSkills[0]->SkillSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[0])));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill 1 Casting Fail"))
				return;
		}
	}


}

void ABaseCharacter::UsingSkill_Second(const FInputActionValue& Value)
{
	if (HasAuthority())
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

		USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

		if (PC && PC->PlayerSkills.Num() > 1 && PC->IsCanUseSkill(PC->PlayerSkills[1]) && SkillManager->GetSkillColldown(PC->PlayerSkills[1]))
		{
			// 서버에서는 바로 액터 스폰
			SpawnActor(PC, PC->PlayerSkills[1], PC->PlayerSkills[1]->SkillArea, PC->PlayerSkills[1]->SkillBody, PC->PlayerSkills[1]->Damage * (SkillManager->GetSkillLevel(PC->PlayerSkills[1])),
				PC->PlayerSkills[1]->CollisionSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[1])),
				PC->PlayerSkills[1]->SkillSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[1])));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill 1 Casting Fail"))
				return;
		}
	}
	else if (!HasAuthority())
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

		USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

		if (PC && PC->PlayerSkills.Num() > 1 && PC->IsCanUseSkill(PC->PlayerSkills[1]) && SkillManager->GetSkillColldown(PC->PlayerSkills[1]))
		{
			ReqServerSpawnSkillActor(PC, PC->PlayerSkills[1], PC->PlayerSkills[1]->SkillArea, PC->PlayerSkills[1]->SkillBody, PC->PlayerSkills[1]->Damage * (SkillManager->GetSkillLevel(PC->PlayerSkills[1])),
				PC->PlayerSkills[1]->CollisionSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[1])),
				PC->PlayerSkills[1]->SkillSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[1])));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill 1 Casting Fail"))
				return;
		}
	}
}

void ABaseCharacter::UsingSkill_Third(const FInputActionValue& Value)
{

	if (HasAuthority())
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

		USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

		if (PC && PC->PlayerSkills.Num() > 2 && PC->IsCanUseSkill(PC->PlayerSkills[2]) && SkillManager->GetSkillColldown(PC->PlayerSkills[2]))
		{
			// 서버에서는 바로 액터 스폰
			SpawnActor(PC, PC->PlayerSkills[2], PC->PlayerSkills[2]->SkillArea, PC->PlayerSkills[2]->SkillBody, PC->PlayerSkills[2]->Damage * (SkillManager->GetSkillLevel(PC->PlayerSkills[2])),
				PC->PlayerSkills[2]->CollisionSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[2])),
				PC->PlayerSkills[2]->SkillSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[2])));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill 1 Casting Fail"))
				return;
		}
	}
	else if (!HasAuthority())
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

		USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

		if (PC && PC->PlayerSkills.Num() > 2 && PC->IsCanUseSkill(PC->PlayerSkills[2]) && SkillManager->GetSkillColldown(PC->PlayerSkills[2]))
		{
			ReqServerSpawnSkillActor(PC, PC->PlayerSkills[2], PC->PlayerSkills[2]->SkillArea, PC->PlayerSkills[2]->SkillBody, PC->PlayerSkills[2]->Damage * (SkillManager->GetSkillLevel(PC->PlayerSkills[2])),
				PC->PlayerSkills[2]->CollisionSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[2])),
				PC->PlayerSkills[2]->SkillSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[2])));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill 1 Casting Fail"))
				return;
		}
	}
}

void ABaseCharacter::UsingSkill_Fourth(const FInputActionValue& Value)
{

	if (HasAuthority())
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

		USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

		if (PC && PC->PlayerSkills.Num() > 3 && PC->IsCanUseSkill(PC->PlayerSkills[3]) && SkillManager->GetSkillColldown(PC->PlayerSkills[3]))
		{
			// 서버에서는 바로 액터 스폰
			SpawnActor(PC, PC->PlayerSkills[3], PC->PlayerSkills[3]->SkillArea, PC->PlayerSkills[0]->SkillBody, PC->PlayerSkills[3]->Damage * (SkillManager->GetSkillLevel(PC->PlayerSkills[3])),
				PC->PlayerSkills[3]->CollisionSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[3])),
				PC->PlayerSkills[3]->SkillSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[3])));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill 1 Casting Fail"))
				return;
		}
	}
	else if (!HasAuthority())
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

		USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

		if (PC && PC->PlayerSkills.Num() > 3 && PC->IsCanUseSkill(PC->PlayerSkills[3]) && SkillManager->GetSkillColldown(PC->PlayerSkills[3]))
		{
			ReqServerSpawnSkillActor(PC, PC->PlayerSkills[3], PC->PlayerSkills[3]->SkillArea, PC->PlayerSkills[0]->SkillBody, PC->PlayerSkills[3]->Damage * (SkillManager->GetSkillLevel(PC->PlayerSkills[3])),
				PC->PlayerSkills[3]->CollisionSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[3])),
				PC->PlayerSkills[3]->SkillSizeVector * (SkillManager->GetSkillLevel(PC->PlayerSkills[3])));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill 1 Casting Fail"))
				return;
		}
	}

}

void ABaseCharacter::CloseWidget(const FInputActionValue& Value)
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(this->GetController());

	if (PC)
	{
		PC->CloseSkillShopWidget();
	}
}

void ABaseCharacter::ReqAttack_Implementation()
{
	if (!IsInputPossible)
		return;

	if (IsAttacking)
	{
		IsSaveAttack = true;
		return;
	}

	IsAttacking = true;

	AttackSwitch();
}

void ABaseCharacter::AttackSwitch_Implementation()
{

	switch (AttackCount)
	{
	case 0:
		AttackCount = 1;
		if(FirstAttackMontage)
			PlayAnimMontage(FirstAttackMontage);
		break;
	case 1:
		AttackCount = 2;
		if (SecondAttackMontage)
			PlayAnimMontage(SecondAttackMontage);
		break;
	case 2:
		AttackCount = 3;
		if (ThirdAttackMontage)
			PlayAnimMontage(ThirdAttackMontage);
		break;
	case 3:
		AttackCount = 0;
		if (FourthAttackMontage)
			PlayAnimMontage(FourthAttackMontage);
		break;
	}
}

void ABaseCharacter::ComboAttackSave()
{
	if (!IsSaveAttack)
		return;

	IsSaveAttack = false;
	
	AttackSwitch();
}

void ABaseCharacter::ResetCombo()
{
	AttackCount = 0;
	IsSaveAttack = false;
	IsAttacking = false;
}

void ABaseCharacter::SetInputPossible()
{
	IsInputPossible = true;
	
}

void ABaseCharacter::SetSpectatorMode()
{

}

APlayerController* ABaseCharacter::GetTargetPlayerController()
{
	return nullptr;
}

void ABaseCharacter::ReqSetLobbyCharacter_Implementation(USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBP)
{
	if (HasAuthority())
	{
		SetLobbyCharacter(SkeletalMesh, AnimBP);
	}
}

void ABaseCharacter::SetLobbyCharacter_Implementation(USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBP)
{
	GetMesh()->SetSkeletalMesh(SkeletalMesh);
	GetMesh()->SetAnimInstanceClass(AnimBP->GetAnimBlueprintGeneratedClass());
}

void ABaseCharacter::ReqSetCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp, UAnimMontage* firstAttackMontage, UAnimMontage* secondAttackMontage, UAnimMontage* thirdAttackMontage, UAnimMontage* fourthAttackMontage, UAnimMontage* levelStartMontage, float maxHp, float damage, float speed, float capsuleHeight, float capsuleRadius, FVector boxCollisionExt, UParticleSystem* hitParticle)
{
	if(HasAuthority())
	{
		RecSetCharacter(skeletalMesh, animBp, firstAttackMontage, secondAttackMontage, thirdAttackMontage, fourthAttackMontage, levelStartMontage, maxHp, damage, speed, capsuleHeight, capsuleRadius, boxCollisionExt, hitParticle);
		UE_LOG(LogTemp, Warning, TEXT("ServerReqSetCharacter"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ClientReqSetCharacter"));
	}
}

void ABaseCharacter::RecSetCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp, UAnimMontage* firstAttackMontage, UAnimMontage* secondAttackMontage, UAnimMontage* thirdAttackMontage, UAnimMontage* fourthAttackMontage, UAnimMontage* levelStartMontage, float maxHp, float damage, float speed, float capsuleHeight, float capsuleRadius, FVector boxCollisionExt, UParticleSystem* hitParticle)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerRecSetCharacter"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ClientRecSetCharacter"));
	}
	UStatManagementComponent* StatComponent = Cast<UStatManagementComponent>(FindComponentByClass<UStatManagementComponent>());

	if (!StatComponent)
		return;

	GetMesh()->SetSkeletalMesh(skeletalMesh);
	GetMesh()->SetAnimInstanceClass(animBp->GetAnimBlueprintGeneratedClass());
	GetMesh()->SetRelativeLocation(FVector(0, 0, capsuleHeight * -1));

	FirstAttackMontage = firstAttackMontage;
	SecondAttackMontage = secondAttackMontage;
	ThirdAttackMontage = thirdAttackMontage;
	FourthAttackMontage = fourthAttackMontage;
	LevelStartMontage = levelStartMontage;
	StatComponent->MaxHp = maxHp;
	StatComponent->Power = damage;
	HitParticle = hitParticle;
	StatComponent->CurHp = maxHp;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (!CapsuleComp)
		return;

	CapsuleComp->SetCapsuleHalfHeight(capsuleHeight);
	CapsuleComp->SetCapsuleRadius(capsuleRadius);


	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();

	if (!CharacterMovementComponent)
		return;

	CharacterMovementComponent->MaxWalkSpeed = speed;

	ReqPlayAnimMontage(LevelStartMontage);
}

void ABaseCharacter::ReqSetWeapon_Implementation(FVector boxCollisionExt)
{
	RecSetWeapon(boxCollisionExt);
}

void ABaseCharacter::RecSetWeapon_Implementation(FVector boxCollisionExt)
{
	UTPGameInstance* GI = Cast<UTPGameInstance>(GetWorld()->GetGameInstance());

	if (!GI || !GI->MyCharacter)
		return;

	Weapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), FVector(0, 0, 0), FRotator::ZeroRotator);

	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weaponCollision"));

	Weapon->Box->SetBoxExtent(boxCollisionExt);
	Weapon->Box->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Weapon->Box->SetRelativeLocation(FVector(0.0f, boxCollisionExt.X, 0.0f));

	Weapon->OwnChar = this;
}


void ABaseCharacter::ReqLog_Implementation(const FString& message)
{


}

void ABaseCharacter::ClientLog_Implementation(const FString& message)
{

}

void ABaseCharacter::ReqPlayAnimMontage_Implementation(UAnimMontage* animMontage)
{
	ClientPlayAnimMontage(animMontage);
}

void ABaseCharacter::ClientPlayAnimMontage_Implementation(UAnimMontage* animMontage)
{
	this->PlayAnimMontage(animMontage);
}
