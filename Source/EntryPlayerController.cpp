// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AEntryPlayerController::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* myPlayerController = GetWorld()->GetFirstPlayerController();

    if (!myPlayerController)
        return;

    if (!(myPlayerController->IsLocalPlayerController()))
        return;

    EntryWidget = CreateWidget<UUserWidget>(GetWorld(), EntryWidgetClass);
    EntryWidget->AddToViewport();

    myPlayerController->SetInputMode(FInputModeUIOnly());
    myPlayerController->bShowMouseCursor = true;
}
