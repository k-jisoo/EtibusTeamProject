// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillInvenWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SKILLPLUGIN_API USkillInvenWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTextBlock* SkillLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTexture2D* SkillIcon;

	UFUNCTION()
	void SetSkill();

};
