// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillManagementComponent.h"
#include "SkillBase.h"
#include "ActiveSkillLightning.h"
#include "ActiveSkillStorm.h"
#include "ActiveSkillWaterBall.h"
#include "PassiveSkillDefenseArea.h"
#include "Containers/Array.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Core/Public/Math/RandomStream.h"  // ���� �Լ��� ����ϱ� ���� ���
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
USkillManagementComponent::USkillManagementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void USkillManagementComponent::BeginPlay()
{
	Super::BeginPlay();

	SkillDatas.Empty();

	Lightning = NewObject<AActiveSkillLightning>(ASkillBase::StaticClass(), AActiveSkillLightning::StaticClass());
	Storm = NewObject<AActiveSkillStorm>(ASkillBase::StaticClass(), AActiveSkillStorm::StaticClass());
	WaterBall = NewObject<AActiveSkillWaterBall>(ASkillBase::StaticClass(), AActiveSkillWaterBall::StaticClass());
	DefenseArea = NewObject<APassiveSkillDefenseArea>(ASkillBase::StaticClass(), APassiveSkillDefenseArea::StaticClass());

	SkillDatas.Add(Lightning);
	SkillDatas.Add(Storm);
	SkillDatas.Add(WaterBall);
	SkillDatas.Add(DefenseArea);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Lightning->SkillName));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Storm->SkillName));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(WaterBall->SkillName));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(DefenseArea->SkillName));

	DefenseAreaLevel = 0;
	LightningLevel = 0;
	StormLevel = 0;
	WaterBallLevel = 0;

	LightningCooldown = 10.0f;
	StormCooldown = 15.0f;
	DefenseAreaCooldown = 5.0f;
	WaterBallCooldown = 7.0f;
}



// Called every frame
void USkillManagementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//if(Lightning)
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Lightning->SkillName));
}

void USkillManagementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkillManagementComponent, LightningLevel);
	DOREPLIFETIME(USkillManagementComponent, StormLevel);
	DOREPLIFETIME(USkillManagementComponent, WaterBallLevel);
	DOREPLIFETIME(USkillManagementComponent, DefenseAreaLevel);
	DOREPLIFETIME(USkillManagementComponent, LightningCooldown);
	DOREPLIFETIME(USkillManagementComponent, StormCooldown);
	DOREPLIFETIME(USkillManagementComponent, WaterBallCooldown);
	DOREPLIFETIME(USkillManagementComponent, DefenseAreaCooldown);
}




void USkillManagementComponent::SkillLevelUp(class ASkillBase* targetSkill)
{
	if (targetSkill == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("targetSkill is null"));
		return;
	}

	if (targetSkill == Lightning)
	{
		LightningLevel += 1;

	}
	else if (targetSkill == WaterBall)
	{
		WaterBallLevel += 1;

	}
	else if (targetSkill == Storm)
	{
		StormLevel += 1;

	}
	else if (targetSkill == DefenseArea)
	{
		DefenseAreaLevel += 1;

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SKill Level Up fail"));
		return;
	}

	OnRep_SkillLevel();
}

float USkillManagementComponent::GetSkillLevel(ASkillBase* targetSkill)
{
	if (targetSkill == Lightning)
	{
		return LightningLevel;
	}
	else if (targetSkill == WaterBall)
	{
		return WaterBallLevel;
	}
	else if (targetSkill == Storm)
	{
		return StormLevel;
	}
	else if (targetSkill == DefenseArea)
	{
		return DefenseAreaLevel;
	}
	else
	{
		return 0.0f;
	}


}

bool USkillManagementComponent::GetSkillColldown(ASkillBase* targetSkill)
{
	if (targetSkill == nullptr)
	{
		return false;
	}

	if (targetSkill->SkillName == "Lightning")
	{
		if (LightningCooldown < 10.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can not Use Skill"));
			return false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Can Use Skill"));
			return true;
		}
	}
	else if (targetSkill->SkillName == "Water Ball")
	{
		if (WaterBallCooldown < 7.0f)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if (targetSkill->SkillName == "Fire Storm")
	{
		if (StormCooldown < 15.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can not Use Skill"));
			return false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Can Use Skill"));
			return true;
		}
	}
	else if (targetSkill->SkillName == "Defense Area")
	{
		if (DefenseAreaCooldown < 5.0f)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Input failed"));
	return false;
}

void USkillManagementComponent::UsingSkill(ASkillBase* targetSkill)
{
	if (targetSkill == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Using Skill failed"))
			return;
	}

	if (targetSkill->SkillName == "Lightning")
	{
		GetWorld()->GetTimerManager().SetTimer(LightningCooldownHandle, this, &USkillManagementComponent::LightningCooldownCounter, 1.0f, true);
	}
	else if (targetSkill->SkillName == "Water Ball")
	{
		GetWorld()->GetTimerManager().SetTimer(WaterBallCooldownHandle, this, &USkillManagementComponent::WaterBallCooldownCounter, 1.0f, true);
	}
	else if (targetSkill->SkillName == "Fire Storm")
	{
		GetWorld()->GetTimerManager().SetTimer(StormCooldownHandle, this, &USkillManagementComponent::StormCooldownCounter, 1.0f, true);
	}
	else if (targetSkill->SkillName == "Defense Area")
	{
		GetWorld()->GetTimerManager().SetTimer(DefenseAreaCooldownHandle, this, &USkillManagementComponent::DefenseAreaCooldownCounter, 1.0f, true);
	}
	else
	{
		return;
	}
}

void USkillManagementComponent::LightningCooldownCounter()
{
	LightningCooldown -= 1;
	UE_LOG(LogTemp, Warning, TEXT("%f"), LightningCooldown);
	OnRep_LightningCooldown();
	if (LightningCooldown <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(LightningCooldownHandle); // Ÿ�̸� ����
		LightningCooldown = 10;
		OnRep_LightningCooldown();
	}
}

void USkillManagementComponent::StormCooldownCounter()
{
	StormCooldown -= 1;
	UE_LOG(LogTemp, Warning, TEXT("%f"), StormCooldown);
	OnRep_StormCooldown();
	if (StormCooldown <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(StormCooldownHandle); // Ÿ�̸� ����
		StormCooldown = 15;
		OnRep_StormCooldown();
	}
}

void USkillManagementComponent::WaterBallCooldownCounter()
{
	WaterBallCooldown -= 1;
	UE_LOG(LogTemp, Warning, TEXT("%f"), WaterBallCooldown);
	OnRep_WaterBallCooldown();
	if (WaterBallCooldown <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(WaterBallCooldownHandle); // Ÿ�̸� ����
		WaterBallCooldown = 7;
		OnRep_WaterBallCooldown();
	}
}

void USkillManagementComponent::DefenseAreaCooldownCounter()
{
	DefenseAreaCooldown -= 1;
	UE_LOG(LogTemp, Warning, TEXT("%f"), DefenseAreaCooldown);
	OnRep_DefenseAreaCooldown();
	if (DefenseAreaCooldown <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(DefenseAreaCooldownHandle); // Ÿ�̸� ����
		DefenseAreaCooldown = 5;
		OnRep_DefenseAreaCooldown();
	}
}

TArray<class ASkillBase*> USkillManagementComponent::GetRandomSkills()
{
	UE_LOG(LogTemp, Warning, TEXT("GetRandomSkills Start"));

	// RandomSkills.Empty();
	if (SkillDatas.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
	}

	RandomSkills.Empty();

	for (int i = 0; i < SkillDatas.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("for AllSkillDatas Start"));

		if (SkillDatas.Num() > 0)
		{
			// ���� �õ� �ʱ�ȭ (�ɼ�)
			FRandomStream RandomStream(FMath::Rand());

			// ������ �ε��� ����
			int32 RandomIndex = RandomStream.RandRange(0, SkillDatas.Num() - 1);

			// ������ ��Ҹ� DestinationArray�� �߰�
			RandomSkills.Add(SkillDatas[RandomIndex]);

			UE_LOG(LogTemp, Warning, TEXT("%d"), RandomIndex);
		}
	}

	return RandomSkills;
}

void USkillManagementComponent::OnRep_SkillLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_SkillLevel"));
	if (Fuc_Dele_UpdateSkillLevel.IsBound())
		Fuc_Dele_UpdateSkillLevel.Broadcast(SkillDatas);
}


void USkillManagementComponent::OnRep_LightningCooldown()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_LightningCooldown"));
	if (Fuc_Dele_UpdateLightningCooldown.IsBound())
		Fuc_Dele_UpdateLightningCooldown.Broadcast(LightningCooldown);
}

void USkillManagementComponent::OnRep_DefenseAreaCooldown()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_DefenseAreaCooldown"));
	if (Fuc_Dele_UpdateDefenseAreaCooldown.IsBound())
		Fuc_Dele_UpdateDefenseAreaCooldown.Broadcast(DefenseAreaCooldown);
}

void USkillManagementComponent::OnRep_WaterBallCooldown()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_WaterBallCooldown"));
	if (Fuc_Dele_UpdateWaterBallCooldown.IsBound())
		Fuc_Dele_UpdateWaterBallCooldown.Broadcast(WaterBallCooldown);
}

void USkillManagementComponent::OnRep_StormCooldown()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_DefenseAreaCooldown"));
	if (Fuc_Dele_UpdateStormCooldown.IsBound())
		Fuc_Dele_UpdateStormCooldown.Broadcast(StormCooldown);
}



