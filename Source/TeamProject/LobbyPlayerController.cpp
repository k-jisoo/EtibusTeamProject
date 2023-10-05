// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUserWidget.h"
#include "Net/UnrealNetwork.h"

void ALobbyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bReplicates = true;

    APlayerController* myPlayerController = GetWorld()->GetFirstPlayerController();

    if (!myPlayerController)
        return;
    
    if (!(myPlayerController->IsLocalPlayerController()))
        return;
    
    if (!LobbyWidgetClass)
        return;

    LobbyWidget = CreateWidget<UUserWidget>(GetWorld(), LobbyWidgetClass);
    LobbyWidget->AddToViewport();
    

    myPlayerController->SetInputMode(FInputModeUIOnly());
    myPlayerController->bShowMouseCursor = true;
    IsReady = false;

    
}

bool ALobbyPlayerController::C2S_SendMessage_Validate(FText const& Message)
{
    return true;
}

void ALobbyPlayerController::C2S_SendMessage_Implementation(FText const& Message)
{
    for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++)
    {
        ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(*Iter);
        if (!PC)
            return;

        PC->S2C_SendMessage(Message);
    }

}

void ALobbyPlayerController::S2C_SendMessage_Implementation(FText const& Message)
{
    //UI 메세지 추가
    ULobbyUserWidget* Widget = Cast<ULobbyUserWidget>(LobbyWidget);
    if (!Widget)
        return;

    Widget->AddMessage(Message);
}

void ALobbyPlayerController::ReqSetReady_Implementation()
{
    RecSetReady();
}

void ALobbyPlayerController::RecSetReady_Implementation()
{
    IsReady = true;
}

void ALobbyPlayerController::ReqCancelReady_Implementation()
{   
    RecCancelReady();
}

void ALobbyPlayerController::RecCancelReady_Implementation()
{
    IsReady = false;
}
