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
	IsInputPossible = true;

	bReplicates = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//UTPGameInstance* GI = Cast<UTPGameInstance>(GetWorld()->GetGameInstance());
	
	//if (!(GI->MyCharacter))
	//	return;
	
	//ReqSetCharacter(GI->MyCharacter->SkeletalMesh, GI->MyCharacter->AnimBP, GI->MyCharacter->FirstAttackMontage, GI->MyCharacter->SecondAttackMontage, GI->MyCharacter->ThirdAttackMontage, GI->MyCharacter->FourthAttackMontage, GI->MyCharacter->LevelStartMontage, GI->MyCharacter->MaxHp, GI->MyCharacter->Damage, GI->MyCharacter->Speed, GI->MyCharacter->CapsuleHeight, GI->MyCharacter->CapsuleRadius, GI->MyCharacter->BoxCollisionExt, GI->MyCharacter->HitParticle);

	//SetWeapon();

	//Message = FString("First Message");
	//ReqLog(FString("Second Message"));

	//UE_LOG(LogTemp, Warning, TEXT("WorldName : %s, name : %s, %f, %f, %f, %f"), *GetWorld()->GetName(), *GI->MyCharacter->SkeletalMesh->GetName(), MaxHp, Damage, GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetCapsuleComponent()->GetScaledCapsuleRadius());

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

void ABaseCharacter::ResSpawnSkillActor_Implementation(ASkillBase* spawnSkill)
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());

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
			return;
			if (spawnSkill->SkillName == "Fire Storm")
			{
				AActiveSkillStorm* storm = GetWorld()->SpawnActor<AActiveSkillStorm>(StormClass, Location + (ForwardVector * 150 * SkillManager->StormLevel), Rotation);

				if (storm)
				{
					storm->Damage *= SkillManager->StormLevel;
					storm->PartX *= SkillManager->StormLevel;
					storm->PartY *= SkillManager->StormLevel;
					storm->PartZ *= SkillManager->StormLevel;
					storm->SkillSize *= SkillManager->StormLevel;
					storm->SkillArea->SetBoxExtent(FVector(storm->PartX, storm->PartY, storm->PartZ));
					storm->SkillBody->SetRelativeScale3D(FVector(storm->SkillSize, storm->SkillSize, storm->SkillSize));
					UParticleSystemComponent* ParticleComponent = storm->FindComponentByClass<UParticleSystemComponent>();
					if (ParticleComponent)
					{
						ParticleComponent->Activate();
					}
					UE_LOG(LogTemp, Warning, TEXT("%d"), SkillManager->StormLevel);
					storm->SetActorHiddenInGame(false);
				}
				SkillManager->UsingSkill(spawnSkill);
			}
			else if (spawnSkill->SkillName == "Lightning")
			{
				AActiveSkillLightning* lightning = GetWorld()->SpawnActor<AActiveSkillLightning>(LightningClass, Location + (ForwardVector * 100 * SkillManager->LightningLevel), Rotation);

				if (lightning)
				{
					lightning->Damage *= SkillManager->LightningLevel;
					lightning->PartX *= SkillManager->LightningLevel;
					lightning->PartY *= SkillManager->LightningLevel;
					lightning->PartZ *= SkillManager->LightningLevel;
					lightning->SkillSize *= SkillManager->LightningLevel;
					lightning->SkillArea->SetBoxExtent(FVector(lightning->PartX, lightning->PartY, lightning->PartZ));
					lightning->SkillBody->SetRelativeScale3D(FVector(lightning->SkillSize, lightning->SkillSize, lightning->SkillSize));
					UParticleSystemComponent* ParticleComponent = lightning->FindComponentByClass<UParticleSystemComponent>();
					if (ParticleComponent)
					{
						ParticleComponent->Activate();
					}
					UE_LOG(LogTemp, Warning, TEXT("%d"), SkillManager->LightningLevel);
					lightning->SetActorHiddenInGame(false);
				}
				SkillManager->UsingSkill(spawnSkill);
			}
			else if (spawnSkill->SkillName == "Water Ball")
			{
				AActiveSkillWaterBall* waterBall = GetWorld()->SpawnActor<AActiveSkillWaterBall>(WaterBallClass, Location + (ForwardVector * 50 * SkillManager->WaterBallLevel), Rotation);

				if (waterBall)
				{
					waterBall->Damage *= SkillManager->WaterBallLevel;
					waterBall->PartX *= SkillManager->WaterBallLevel;
					waterBall->PartY *= SkillManager->WaterBallLevel;
					waterBall->PartZ *= SkillManager->WaterBallLevel;
					waterBall->SkillSize *= SkillManager->WaterBallLevel;
					waterBall->SkillArea->SetBoxExtent(FVector(waterBall->PartX, waterBall->PartY, waterBall->PartZ));
					waterBall->SkillBody->SetRelativeScale3D(FVector(waterBall->SkillSize, waterBall->SkillSize, waterBall->SkillSize));
					UParticleSystemComponent* ParticleComponent = waterBall->FindComponentByClass<UParticleSystemComponent>();
					if (ParticleComponent)
					{
						ParticleComponent->Activate();
					}
					UE_LOG(LogTemp, Warning, TEXT("%d"), SkillManager->WaterBallLevel);
					waterBall->SetActorHiddenInGame(false);
				}
				SkillManager->UsingSkill(spawnSkill);
			}
			else if (spawnSkill->SkillName == "Defense Area")
			{
				APassiveSkillDefenseArea* defenseArea = GetWorld()->SpawnActor<APassiveSkillDefenseArea>(DefenseAreaClass, Location, Rotation);

				if (defenseArea)
				{
					defenseArea->Damage *= SkillManager->DefenseAreaLevel;
					defenseArea->PartX *= SkillManager->DefenseAreaLevel;
					defenseArea->PartY *= SkillManager->DefenseAreaLevel;
					defenseArea->PartZ *= SkillManager->DefenseAreaLevel;
					defenseArea->SkillSize *= SkillManager->DefenseAreaLevel;
					defenseArea->SkillArea->SetBoxExtent(FVector(defenseArea->PartX, defenseArea->PartY, defenseArea->PartZ));
					defenseArea->SkillBody->SetRelativeScale3D(FVector(defenseArea->SkillSize, defenseArea->SkillSize, defenseArea->SkillSize));
					UParticleSystemComponent* ParticleComponent = defenseArea->FindComponentByClass<UParticleSystemComponent>();
					if (ParticleComponent)
					{
						ParticleComponent->Activate();
					}
					defenseArea->SkillArea->SetSimulatePhysics(false);
					AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("spine_01"));
					UE_LOG(LogTemp, Warning, TEXT("%d"), SkillManager->DefenseAreaLevel);
					defenseArea->SetActorHiddenInGame(false);
				}
				SkillManager->UsingSkill(spawnSkill);
			}
		}
	}
	
}


void ABaseCharacter::ReqSpawnSkillActor_Implementation(ASkillBase* spawnSkill)
{
	if (!spawnSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("spawnSkill"));
		return;
	}

	ResSpawnSkillActor(spawnSkill);
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
	
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());

	USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

	if (PC && PC->PlayerSkills.Num() > 0 && PC->IsCanUseSkill(PC->PlayerSkills[0]) && SkillManager->GetSkillColldown(PC->PlayerSkills[0]))
	{
		// 서버에서는 바로 액터 스폰
		ReqSpawnSkillActor(PC->PlayerSkills[0]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill 1 Casting Fail"))
			return;
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
			ReqSpawnSkillActor(PC->PlayerSkills[1]);
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
			// 서버에서는 바로 액터 스폰
			ReqSpawnSkillActor(PC->PlayerSkills[1]);
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
			ReqSpawnSkillActor(PC->PlayerSkills[2]);
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
			// 서버에서는 바로 액터 스폰
			ReqSpawnSkillActor(PC->PlayerSkills[2]);
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
			ReqSpawnSkillActor(PC->PlayerSkills[3]);
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
			// 서버에서는 바로 액터 스폰
			ReqSpawnSkillActor(PC->PlayerSkills[3]);
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
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);

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
	}
}

void ABaseCharacter::RecSetCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp, UAnimMontage* firstAttackMontage, UAnimMontage* secondAttackMontage, UAnimMontage* thirdAttackMontage, UAnimMontage* fourthAttackMontage, UAnimMontage* levelStartMontage, float maxHp, float damage, float speed, float capsuleHeight, float capsuleRadius, FVector boxCollisionExt, UParticleSystem* hitParticle)
{
	GetMesh()->SetSkeletalMesh(skeletalMesh);
	GetMesh()->SetAnimInstanceClass(animBp->GetAnimBlueprintGeneratedClass());
	GetMesh()->SetRelativeLocation(FVector(0, 0, capsuleHeight * -1));

	FirstAttackMontage = firstAttackMontage;
	SecondAttackMontage = secondAttackMontage;
	ThirdAttackMontage = thirdAttackMontage;
	FourthAttackMontage = fourthAttackMontage;
	LevelStartMontage = levelStartMontage;
	MaxHp = maxHp;
	Damage = damage;
	HitParticle = hitParticle;
	CurHp = maxHp;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (!CapsuleComp)
		return;

	CapsuleComp->SetCapsuleHalfHeight(capsuleHeight);
	CapsuleComp->SetCapsuleRadius(capsuleRadius);


	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();

	if (!CharacterMovementComponent)
		return;

	CharacterMovementComponent->MaxWalkSpeed = speed;
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
	Message = message;

	if (HasAuthority())
	{
		ClientLog(message);
	}
}

void ABaseCharacter::ClientLog_Implementation(const FString& message)
{
	Message = message;
}

void ABaseCharacter::ReqPlayAnimMontage_Implementation(UAnimMontage* animMontage)
{
	ClientPlayAnimMontage(animMontage);
}

void ABaseCharacter::ClientPlayAnimMontage_Implementation(UAnimMontage* animMontage)
{
	this->PlayAnimMontage(animMontage);
}
