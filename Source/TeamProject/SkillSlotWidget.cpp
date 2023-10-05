// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSlotWidget.h"
#include "SkillManagementComponent.h"
#include "SkillBase.h"
#include "MainPlayerController.h"

void USkillSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillSlotWidget::BuySkill(ASkillBase* target)
{
	
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());

	if (target == nullptr)
	{
		return;
	}

	if (PC)
	{
		USkillManagementComponent* SkillManager = Cast<USkillManagementComponent>(PC->FindComponentByClass<USkillManagementComponent>());

		if (SkillManager)
		{	
			SkillManager->SkillLevelUp(target);
			PC->GetSkill(target);
			UE_LOG(LogTemp, Warning, TEXT("AddSkillToInventory"));

			PC->Gold -= target->Price;
			PC->OnUpdateMyGold(PC->Gold);
			UE_LOG(LogTemp, Warning, TEXT("%d"), PC->Gold);
			UE_LOG(LogTemp, Warning, TEXT("Success Buy Skill"));
		}
	}
}

bool USkillSlotWidget::IsCanBuySkill(ASkillBase* target)
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC)
	{
		if (target->Price <= PC->Gold)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
