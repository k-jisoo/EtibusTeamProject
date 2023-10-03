// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BaseCharacter.h"
#include "TPGameInstance.h"
#include "Engine/World.h"
#include "SkillManagementComponent.h"
#include "StatManagementComponent.h"
#include "Blueprint/UserWidget.h"
#include "SkillBase.h"
#include "ActiveSkillLightning.h"
#include "ActiveSkillStorm.h"
#include "ActiveSkillWaterBall.h"
#include "PassiveSkillDefenseArea.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "StatManagementComponent.h"
#include "StatEnhancementObjectBase.h"
#include "EnhancementMaxHp.h"
#include "EnhancementMaxMp.h"
#include "EnhancementPower.h"
#include "EnhancementSpeed.h"
#include "BaseCharacter.h"

AMainPlayerController::AMainPlayerController()
{
	SkillManager = CreateDefaultSubobject<USkillManagementComponent>(TEXT("SkillManager"));

	StatManager = CreateDefaultSubobject<UStatManagementComponent>(TEXT("StatManager"));

	bReplicates = true;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());

	if (LocalPlayerSystem && DefaultMappingContext)
	{
		LocalPlayerSystem->AddMappingContext(DefaultMappingContext, 0);
	}
	else
	{
		if (!LocalPlayerSystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Found LocalPlayerSystem"));
		}
	}

	GetWorldTimerManager().SetTimer(MyHandle, this, &AMainPlayerController::InitCharacter, 2.0f, false);

	AllSkillDatas.Empty();

	Lightning = NewObject<AActiveSkillLightning>(ASkillBase::StaticClass(), AActiveSkillLightning::StaticClass());
	Storm = NewObject<AActiveSkillStorm>(ASkillBase::StaticClass(), AActiveSkillStorm::StaticClass());
	WaterBall = NewObject<AActiveSkillWaterBall>(ASkillBase::StaticClass(), AActiveSkillWaterBall::StaticClass());
	DefenseArea = NewObject<APassiveSkillDefenseArea>(ASkillBase::StaticClass(), APassiveSkillDefenseArea::StaticClass());

	AllSkillDatas.Add(Lightning);
	AllSkillDatas.Add(Storm);
	AllSkillDatas.Add(WaterBall);
	AllSkillDatas.Add(DefenseArea);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Lightning->SkillName));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Storm->SkillName));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(WaterBall->SkillName));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(DefenseArea->SkillName));

	StatEnhancementObjs.Empty();

	EnhancementMaxHp = NewObject<AEnhancementMaxHp>(ASkillBase::StaticClass(), AEnhancementMaxHp::StaticClass());
	EnhancementMaxMp = NewObject<AEnhancementMaxMp>(ASkillBase::StaticClass(), AEnhancementMaxMp::StaticClass());
	EnhancementSpeed = NewObject<AEnhancementSpeed>(ASkillBase::StaticClass(), AEnhancementSpeed::StaticClass());
	EnhancementPower = NewObject<AEnhancementPower>(ASkillBase::StaticClass(), AEnhancementPower::StaticClass());

	StatEnhancementObjs.Add(EnhancementMaxHp);
	StatEnhancementObjs.Add(EnhancementMaxMp);
	StatEnhancementObjs.Add(EnhancementSpeed);
	StatEnhancementObjs.Add(EnhancementPower);

	PlayerSkills.Empty();

	CreateSkillShopWidget();
}

void AMainPlayerController::InitCharacter()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	UTPGameInstance* GI = Cast<UTPGameInstance>(GetWorld()->GetGameInstance());

	if (!(GI->MyCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("!MyCharacter"));
		return;
	}

	ABaseCharacter* ABC = Cast<ABaseCharacter>(GetPawn());

	if (!ABC)
	{
		UE_LOG(LogTemp, Warning, TEXT("!ABC"));
		return;
	}

	ABC->ReqSetCharacter(GI->MyCharacter->SkeletalMesh, GI->MyCharacter->AnimBP, GI->MyCharacter->FirstAttackMontage, GI->MyCharacter->SecondAttackMontage, GI->MyCharacter->ThirdAttackMontage, GI->MyCharacter->FourthAttackMontage, GI->MyCharacter->LevelStartMontage, GI->MyCharacter->MaxHp, GI->MyCharacter->Damage, GI->MyCharacter->Speed, GI->MyCharacter->CapsuleHeight, GI->MyCharacter->CapsuleRadius, GI->MyCharacter->BoxCollisionExt, GI->MyCharacter->HitParticle);

	ABC->ReqSetWeapon(GI->MyCharacter->BoxCollisionExt);
}

void AMainPlayerController::CreateSkillShopWidget()
{
	SkillShopWidget = CreateWidget<UUserWidget>(GetWorld(), SkillShopWidgetClass);
	SkillShopWidget->AddToViewport();

	this->SetInputMode(FInputModeGameAndUI());
	this->bShowMouseCursor = true;

	BindSkillSData();
	BindEnhancedObjData();
	BindPlayerInfo();
	OnUpdateMyGold(Gold);
	BindStatManagers();
}

void AMainPlayerController::CloseSkillShopWidget()
{
	SkillShopWidget->RemoveFromParent();
}

void AMainPlayerController::BindSkillSData()
{
	ABaseCharacter* Char = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Char)
	{
		USkillManagementComponent* skillManager = Cast<USkillManagementComponent>(this->FindComponentByClass<USkillManagementComponent>());

		if (skillManager)
		{

			TArray<class ASkillBase*> RandSkills = skillManager->GetRandomSkills();

			OnUpdateSkills(RandSkills);

			for (int i = 0; i < RandSkills.Num(); i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("%f"), RandSkills[i]->PartZ);
			}

			UE_LOG(LogTemp, Warning, TEXT("BindSkillSData Success"));
		}
	}
}

void AMainPlayerController::BindEnhancedObjData()
{
	ABaseCharacter* Char = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Char)
	{
		OnUpdateEnhancementObjs(StatEnhancementObjs);

		for (int i = 0; i < StatEnhancementObjs.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(StatEnhancementObjs[i]->ObjName));
		}

		UE_LOG(LogTemp, Warning, TEXT("BindEnhancedItemData Success"));
	}
}

void AMainPlayerController::BindPlayerInfo()
{
	ABaseCharacter* Char = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Char)
	{
		USkillManagementComponent* skillManager = Cast<USkillManagementComponent>(GetWorld()->GetFirstPlayerController()->FindComponentByClass<USkillManagementComponent>());

		skillManager->Fuc_Dele_UpdateSkillLevel.AddDynamic(this, &AMainPlayerController::OnUpdateMySkillLevel);

		OnUpdateMySkillLevel(AllSkillDatas);

		for (int i = 0; i < AllSkillDatas.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%f"), AllSkillDatas[i]->PartZ);
		}

		UE_LOG(LogTemp, Warning, TEXT("BindEnhancedItemData Success"));
	}
}

void AMainPlayerController::BindStatManagers()
{
	UStatManagementComponent* statManager = Cast<UStatManagementComponent>(this->FindComponentByClass<UStatManagementComponent>());

	if (statManager)
	{
		statManager->Fuc_Dele_UpdateHp.AddDynamic(this, &AMainPlayerController::OnUpdateMyMaxHp);
		OnUpdateMyMaxHp(StatManager->CurHp, StatManager->MaxHp);

		statManager->Fuc_Dele_UpdateMp.AddDynamic(this, &AMainPlayerController::OnUpdateMyMaxMp);
		OnUpdateMyMaxMp(StatManager->CurMp, StatManager->MaxMp);

		statManager->Fuc_Dele_UpdateSpeed.AddDynamic(this, &AMainPlayerController::OnUpdateMySpeed);
		OnUpdateMySpeed(StatManager->Speed);

		statManager->Fuc_Dele_UpdatePower.AddDynamic(this, &AMainPlayerController::OnUpdateMyPower);
		OnUpdateMyPower(StatManager->Power);
	}

	/*FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(th_BindMyStatManager, this, &AMainPlayerController::BindStatManagers, 0.1f, false);*/
}

void AMainPlayerController::AddSkillDataToSkillManager(TArray<class ASkillBase*>& SkillDatas)
{
	ABaseCharacter* Char = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Char)
	{
		USkillManagementComponent* skillManager = Cast<USkillManagementComponent>(this->FindComponentByClass<USkillManagementComponent>());

		if (skillManager)
		{
			skillManager->SkillDatas = SkillDatas;
		}
	}
}

void AMainPlayerController::OnUpdateSkills_Implementation(const TArray<class ASkillBase*>& SkillDatas)
{
}

void AMainPlayerController::OnUpdateEnhancementObjs_Implementation(const TArray<class AStatEnhancementObjectBase*>& objDatas)
{
}

void AMainPlayerController::OnUpdateMySkillLevel_Implementation(const TArray<class ASkillBase*>& SkillDatas)
{
}

void AMainPlayerController::OnUpdateMyMaxHp_Implementation(float CurHp, float MaxHp)
{
}

void AMainPlayerController::OnUpdateMyMaxMp_Implementation(float CurHp, float MaxHp)
{
}

void AMainPlayerController::OnUpdateMySpeed_Implementation(float Speed)
{
}

void AMainPlayerController::OnUpdateMyPower_Implementation(float Power)
{
}

void AMainPlayerController::OnUpdateMyGold_Implementation(int32 coin)
{
}

void AMainPlayerController::GetSkill(ASkillBase* Skill)
{
	if (Skill == nullptr)
	{
		return;
	}

	if (!PlayerSkills.Contains(Skill))
	{
		UE_LOG(LogTemp, Warning, TEXT("Client: GetSkill"));
		PlayerSkills.Add(Skill);
	}
}

bool AMainPlayerController::IsCanUseSkill(ASkillBase* Skill)
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
	
	USkillManagementComponent* skillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

	if (PC)
	{
		if (skillManager)
		{
			if (skillManager->GetSkillLevel(Skill) > 0.0f)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	

	return false;
}