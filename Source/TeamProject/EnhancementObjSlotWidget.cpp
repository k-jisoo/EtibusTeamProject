// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancementObjSlotWidget.h"
#include "StatEnhancementObjectBase.h"
#include "StatManagementComponent.h"
#include "MainPlayerController.h"

void UEnhancementObjSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnhancementObjSlotWidget::EnhancementPlayerStat(AStatEnhancementObjectBase* target)
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC)
	{
		UStatManagementComponent* StatManager = Cast<UStatManagementComponent>(PC->FindComponentByClass<UStatManagementComponent>());

		if (StatManager)
		{
			StatManager->ComparisonObjType(target);
			UE_LOG(LogTemp, Warning, TEXT("EnhancementPlayerStat"));

			PC->Gold -= target->price;
			PC->OnUpdateMyGold(PC->Gold);
			UE_LOG(LogTemp, Warning, TEXT("%d"), PC->Gold);
			UE_LOG(LogTemp, Warning, TEXT("Success Buy Skill"));
		}
	}
}

bool UEnhancementObjSlotWidget::IsCanBuyObj(AStatEnhancementObjectBase* target)
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC)
	{
		if (target->price <= PC->Gold)
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
