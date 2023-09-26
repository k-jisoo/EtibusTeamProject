// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillManagementComponent.h"
#include "SkillBase.h"
#include "ActiveSkillLightning.h"
#include "ActiveSkillStorm.h"
#include "ActiveSkillWaterBall.h"
#include "PassiveSkillDefenseArea.h"
#include "Containers/Array.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Core/Public/Math/RandomStream.h"  // 랜덤 함수를 사용하기 위한 헤더

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
}




void USkillManagementComponent::SkillLevelUp(class ASkillBase* targetSkill)
{	

	
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

	OnRep_SkillLevel();

	/*for (int i = 0; i < SkillDatas.Num(); i++)
	{
		if (SkillDatas[i]->SkillName == targetSkill->SkillName)
		{
			SkillDatas[i]->Level += 1;
			UE_LOG(LogTemp, Warning, TEXT("LevelUp"));
		}
	}*/
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
			// 랜덤 시드 초기화 (옵션)
			FRandomStream RandomStream(FMath::Rand());

			// 랜덤한 인덱스 선택
			int32 RandomIndex = RandomStream.RandRange(0, SkillDatas.Num() - 1);

			// 선택한 요소를 DestinationArray에 추가
			RandomSkills.Add(SkillDatas[RandomIndex]);

			UE_LOG(LogTemp, Warning, TEXT("%d"), RandomIndex);
		}
	}

	return RandomSkills;
}



void USkillManagementComponent::GetSkill(ASkillBase* Skill)
{
	// 스킬 배열에 없으면 추가, 있으면 레벨 증가

	if (Skill == nullptr)
	{
		return;
	}
	
	if (SkillDatas.Find(Skill) == false)
	{
		SkillDatas.Add(Skill);
	}
	else
	{
		Skill->Level += 1;
	}
}

bool USkillManagementComponent::IsCanUse(ASkillBase* Skill)
{
	if (Skill == nullptr)
	{
		return false;
	}
	
	if (Skill->Level >= 1)
	{
		return true;
	}

	return false;

	// 스킬 사용이 가능하며 해당 캐릭터의 애로우 컴포넌트에서 스킬을 스폰하는 함수를 캐릭터에서 실행

}

void USkillManagementComponent::OnRep_SkillLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_SkillLevel"));
	if (Fuc_Dele_UpdateSkillLevel.IsBound())
		Fuc_Dele_UpdateSkillLevel.Broadcast(SkillDatas);
}

void USkillManagementComponent::SpawnSkill(ASkillBase* skill)
{
}


