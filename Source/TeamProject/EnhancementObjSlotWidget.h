// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhancementObjSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UEnhancementObjSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void EnhancementPlayerStat(class AStatEnhancementObjectBase* target);

	UFUNCTION(BlueprintCallable)
	bool IsCanBuyObj(class AStatEnhancementObjectBase* target);
};
