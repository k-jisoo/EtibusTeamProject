// Fill out your copyright notice in the Description page of Project Settings.


#include "TPGameInstance.h"
#include "BaseCharacter.h"
#include "Engine/DataTable.h"

void UTPGameInstance::ReqGetCharacterRowData_Implementation(FName name)
{
	GetCharacterRowData(name);
}

void UTPGameInstance::GetCharacterRowData_Implementation(FName name)
{
	if (!CharacterData)
		return;

	MyCharacter = CharacterData->FindRow<FST_Character>(name, TEXT("")) ? CharacterData->FindRow<FST_Character>(name, TEXT("")) : nullptr;
}
