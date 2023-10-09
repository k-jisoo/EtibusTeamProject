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
#include "MainGameMode.h"

AMainPlayerController::AMainPlayerController()
{
	SkillManager = CreateDefaultSubobject<USkillManagementComponent>(TEXT("SkillManager"));

	StatManager = CreateDefaultSubobject<UStatManagementComponent>(TEXT("StatManager"));

	IsAlive = true;
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

	if (IsLocalController())
	{
		if (HasAuthority())
		{
			ServerCreateAndSyncUIWidget();

		}
		else if (!HasAuthority())
		{
			CreateUIWidget();
		}

	}

	// 상점 여는 함수
	// OpenSkillShop();
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
	APlayerController* MyController = UGameplayStatics::GetPlayerController(this, 0); // 현재 플레이어 컨트롤러 가져오기

	if (MyController)
	{
		if (MyController->IsLocalController()) // 현재 컨트롤러가 로컬 플레이어인지 확인
		{
			UserInterfaceWidget = CreateWidget<UUserWidget>(GetWorld(), UserInterfaceWidgetClass);
			UserInterfaceWidget->AddToViewport();

			SkillShopWidget = CreateWidget<UUserWidget>(GetWorld(), SkillShopWidgetClass);

			if (SkillShopWidget)
			{
				SkillShopWidget->AddToViewport();

				this->SetInputMode(FInputModeGameAndUI());
				this->bShowMouseCursor = true;

				BindSkillSData();
				BindEnhancedObjData();
				BindPlayerInfo();
				OnUpdateMyGold(Gold);
				BindStatManagers();
			}
		}
	}

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

		skillManager->Fuc_Dele_UpdateLightningCooldown.AddDynamic(this, &AMainPlayerController::OnUpdateLightningCooldown);
		//OnUpdateLightningCooldown(skillManager->LightningCooldown, PlayerSkills, Lightning);

		skillManager->Fuc_Dele_UpdateStormCooldown.AddDynamic(this, &AMainPlayerController::OnUpdateStormCooldown);
		//OnUpdateStormCooldown(skillManager->StormCooldown, PlayerSkills, Storm);

		skillManager->Fuc_Dele_UpdateWaterBallCooldown.AddDynamic(this, &AMainPlayerController::OnUpdateWaterBallCooldown);
		//OnUpdateWaterBallCooldown(skillManager->WaterBallCooldown, PlayerSkills, WaterBall);

		skillManager->Fuc_Dele_UpdateDefenseAreaCooldown.AddDynamic(this, &AMainPlayerController::OnUpdateDefenseAreaCooldown);
		//OnUpdateDefenseAreaCooldown(skillManager->DefenseAreaCooldown, PlayerSkills, DefenseArea);

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

		statManager->Fuc_Dele_UpdateHp.AddDynamic(this, &AMainPlayerController::OnUpdateMyCurHp);

		statManager->Fuc_Dele_UpdateMp.AddDynamic(this, &AMainPlayerController::OnUpdateMyMaxMp);
		OnUpdateMyMaxMp(StatManager->CurMp, StatManager->MaxMp);

		statManager->Fuc_Dele_UpdateSpeed.AddDynamic(this, &AMainPlayerController::OnUpdateMySpeed);
		OnUpdateMySpeed(StatManager->Speed);

		statManager->Fuc_Dele_UpdatePower.AddDynamic(this, &AMainPlayerController::OnUpdateMyPower);
		OnUpdateMyPower(StatManager->Power);
	}

}

void AMainPlayerController::ReqDieProcess_Implementation(USkeletalMeshComponent* skMesh)
{
	FVector ActorLoc = GetCharacter()->GetActorLocation();

	skMesh->SetSimulatePhysics(true);
	skMesh->SetWorldLocation(ActorLoc);

	DisableInput(this);

	IsAlive = false;

	skMesh->SetCollisionProfileName(FName("Ragdoll"));

	RecDieProcess(skMesh);

	if (HasAuthority())
	{
		AMainGameMode* GM = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());

		GM->GameOver();
	}
}

void AMainPlayerController::RecDieProcess_Implementation(USkeletalMeshComponent* skMesh)
{
	FVector ActorLoc = GetCharacter()->GetActorLocation();

	skMesh->SetSimulatePhysics(true);
	skMesh->SetWorldLocation(ActorLoc);

	DisableInput(this);

	IsAlive = false;

	skMesh->SetCollisionProfileName(FName("Ragdoll"));
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

void AMainPlayerController::OnUpdateMyCurHp(float CurHp, float MaxHp)
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

void AMainPlayerController::OnUpdateStormCooldown_Implementation(float skillcooldown, const TArray<class ASkillBase*>& SkillDatas, ASkillBase* skillData)
{
}

void AMainPlayerController::OnUpdateLightningCooldown_Implementation(float skillcooldown, const TArray<class ASkillBase*>& SkillDatas, ASkillBase* skillData)
{
}

void AMainPlayerController::OnUpdateWaterBallCooldown_Implementation(float skillcooldown, const TArray<class ASkillBase*>& SkillDatas, ASkillBase* skillData)
{
}

void AMainPlayerController::OnUpdateDefenseAreaCooldown_Implementation(float skillcooldown, const TArray<class ASkillBase*>& SkillDatas, ASkillBase* skillData)
{
}

void AMainPlayerController::OnUpdateMyGold_Implementation(int32 coin)
{
}

void AMainPlayerController::OnUpdateSkillSlot_Implementation(ASkillBase* skillData, int32 count)
{
}

void AMainPlayerController::GetSkill(ASkillBase* Skill)
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());

	USkillManagementComponent* skillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());
	
	if (Skill == nullptr)
	{
		return;
	}

	if (!PlayerSkills.Contains(Skill))
	{
		UE_LOG(LogTemp, Warning, TEXT("Client: GetSkill"));
		PlayerSkills.Add(Skill);
		SkillArrNumCount += 1;
		skillManager->PlayerSkills.Add(Skill);
		OnUpdateSkillSlot(Skill, SkillArrNumCount);
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

void AMainPlayerController::ClientCreateAndSyncUIWidget_Implementation()
{
	if (!HasAuthority())
	{
		// 서버에서 위젯을 생성하고 클라이언트에 동기화하는 RPC 호출
		ServerCreateAndSyncUIWidget();
	}
}

void AMainPlayerController::MulticastOnUIWidgetCreated_Implementation()
{
	if (HasAuthority())
	{
		UserInterfaceWidget = CreateWidget<UUserWidget>(GetWorld(), UserInterfaceWidgetClass);
		UserInterfaceWidget->AddToViewport();

		if (UserInterfaceWidget)
		{
			this->SetInputMode(FInputModeGameOnly());
			this->bShowMouseCursor = false;

			BindStatManagers();
		}
	}
}

void AMainPlayerController::ServerCreateAndSyncUIWidget_Implementation()
{
	if (HasAuthority())
	{
		UserInterfaceWidget = CreateWidget<UUserWidget>(GetWorld(), UserInterfaceWidgetClass);
		UserInterfaceWidget->AddToViewport();

		if (UserInterfaceWidget)
		{
			this->SetInputMode(FInputModeGameOnly());
			this->bShowMouseCursor = false;

			BindStatManagers();
		}

		MulticastOnUIWidgetCreated();
	}
}




void AMainPlayerController::ServerCreateAndSyncWidget_Implementation()
{
	// 서버에서만 실행되도록 로직 작성
	if (HasAuthority())
	{
		// 실제로 위젯을 생성하는 코드 작성
		if (!SkillShopWidget)
		{

			SkillShopWidget = CreateWidget<UUserWidget>(this, SkillShopWidgetClass);
			if (SkillShopWidget)
			{
				SkillShopWidget->AddToViewport();

				this->SetInputMode(FInputModeGameAndUI());
				this->bShowMouseCursor = true;

				BindSkillSData();
				BindEnhancedObjData();
				BindPlayerInfo();
				OnUpdateMyGold(Gold);
				BindStatManagers();

				// 클라이언트에 위젯 생성 완료를 알리기 위한 다른 RPC 함수 호출
				MulticastOnWidgetCreated();
			}
		}
	}
}

void AMainPlayerController::MulticastOnWidgetCreated_Implementation()
{
	if (!SkillShopWidget)
	{
		SkillShopWidget = CreateWidget<UUserWidget>(this, SkillShopWidgetClass);
		if (SkillShopWidget)
		{

			SkillShopWidget->AddToViewport();

			this->SetInputMode(FInputModeGameAndUI());
			this->bShowMouseCursor = true;

			BindSkillSData();
			BindEnhancedObjData();
			BindPlayerInfo();
			OnUpdateMyGold(Gold);
			BindStatManagers();
		}
	}
}

void AMainPlayerController::ClientCreateAndSyncWidget_Implementation()
{
	if (!HasAuthority())
	{
		// 서버에서 위젯을 생성하고 클라이언트에 동기화하는 RPC 호출
		ServerCreateAndSyncWidget();
	}
}

void AMainPlayerController::CreateUIWidget()
{
	APlayerController* MyController = UGameplayStatics::GetPlayerController(this, 0); // 현재 플레이어 컨트롤러 가져오기

	if (MyController)
	{
		if (MyController->IsLocalController()) // 현재 컨트롤러가 로컬 플레이어인지 확인
		{
			UserInterfaceWidget = CreateWidget<UUserWidget>(GetWorld(), UserInterfaceWidgetClass);
			UserInterfaceWidget->AddToViewport();

			if (UserInterfaceWidget)
			{
				this->SetInputMode(FInputModeGameOnly());
				this->bShowMouseCursor = false;

				BindStatManagers();
			}
		}
	}
}

void AMainPlayerController::OpenSkillShop()
{
	if (IsLocalController())
	{
		if (HasAuthority())
		{
			ServerCreateAndSyncWidget();

		}
		else if (!HasAuthority())
		{
			CreateSkillShopWidget();
		}

	}
}
