// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UENUM()
enum class EItemType : uint8
{
	IT_RecoveryHp UMETA(DisplayName = "RecoveryHp"),
	IT_RecoveryMp UMETA(DisplayName = "RecoveryMp"),
	IT_SpeedUp UMETA(DisplayName = "SpeedUp"),
	IT_PowerUp UMETA(DisplayName = "PowerUp"),
	IT_Gold UMETA(DisplayName = "Gold"),
};


UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ITEMPLUGIN_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Event")
	void EventGetItem(EItemType itemType);

};
