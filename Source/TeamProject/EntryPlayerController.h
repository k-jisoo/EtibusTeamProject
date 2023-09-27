// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EntryPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AEntryPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> EntryWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* EntryWidget;
};
