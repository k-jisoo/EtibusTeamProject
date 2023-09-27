// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EntryUserWidget.generated.h"

class UScrollBox;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UEntryUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEditableTextBox* AddressInputBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEditableTextBox* NameInputBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* StartServerButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* ConnectButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AGameModeBase> LobbyGameModeClass;

	UFUNCTION()
	void SetUserName();

	UFUNCTION()
	void StartServer();

	UFUNCTION()
	void ConnectClient();


};
