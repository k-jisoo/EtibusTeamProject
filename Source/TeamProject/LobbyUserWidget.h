// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ULobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UScrollBox* ChatScroll;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEditableTextBox* InputBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* StartGameButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* ReadyButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* CancelReadyButton;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* YinButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* TerraButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* RelevantButton;


	UFUNCTION()
	void OnChangedText(const FText& Text);

	UFUNCTION()
	void OnCommitedText(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void Ready();

	UFUNCTION()
	void CancelReady();

	UFUNCTION()
	void OnButtonClicked(int num);

	UFUNCTION()
	void OnYinButtonClicked();

	UFUNCTION()
	void OnTerraButtonClicked();

	UFUNCTION()
	void OnRevenantButtonClicked();


	void AddMessage(FText const& Message);

};
