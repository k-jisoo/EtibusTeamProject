// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TPGameInstance.generated.h"

struct FST_Character;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Username;

	FST_Character* MyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* CharacterData;

	UFUNCTION(Server, Reliable)
	void ReqGetCharacterRowData(FName name);

	UFUNCTION(NetMulticast, Reliable)
	void GetCharacterRowData(FName name);

};
