// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryUserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TPGameInstance.h"

void UEntryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartServerButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonStartServer")));
	if (!StartServerButton)
		return;

	if(GetWorld()->GetFirstPlayerController()->HasAuthority())
			StartServerButton->OnClicked.AddDynamic(this, &UEntryUserWidget::StartServer);

	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonConnect")));
	if (!ConnectButton)
		return;

	ConnectButton->OnClicked.AddDynamic(this, &UEntryUserWidget::ConnectClient);
}

void UEntryUserWidget::StartServer()
{
	SetUserName();

	UGameplayStatics::OpenLevel(GetWorld(), "Lobby", true, "listen");
}

void UEntryUserWidget::SetUserName()
{
	UTPGameInstance* gameInstance = Cast<UTPGameInstance>(
	UGameplayStatics::GetGameInstance(GetWorld()));
	if (!gameInstance) 
		return;

	NameInputBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("InputBoxName")));
	if (!NameInputBox) 
		return;

	gameInstance->Username = NameInputBox->GetText().ToString();
}

void UEntryUserWidget::ConnectClient()
{
	SetUserName();

	AddressInputBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("InputBoxAddress")));
	if (!AddressInputBox)
		return;

	FText InputText = AddressInputBox->GetText();

	UGameplayStatics::OpenLevel(GetWorld(), FName(InputText.ToString()));
}
