// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillInvenWidgetBase.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

void USkillInvenWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SkillLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("CostT")));
	if (SkillLevel)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillLevel"));
	}

	SkillIcon = Cast<UTexture2D>(GetWidgetFromName(TEXT("SkillThumbnailImage")));
	if (SkillIcon)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillIcon"));
	}
}

void USkillInvenWidgetBase::SetSkill()
{
}
