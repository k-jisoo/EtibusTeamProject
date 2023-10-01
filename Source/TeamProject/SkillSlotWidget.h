// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API USkillSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void BuySkill(class ASkillBase* target);

	UFUNCTION(BlueprintCallable)
	bool IsCanBuySkill(class ASkillBase* target);
};
