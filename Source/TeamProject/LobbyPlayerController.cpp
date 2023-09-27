// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUserWidget.h"
#include "BaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"

void ALobbyPlayerController::BeginPlay()
{
    Super::BeginPlay();

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

void ALobbyPlayerController::ReqSetLobbyCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp)
{
    RecSetLobbyCharacter(skeletalMesh, animBp);
}

void ALobbyPlayerController::RecSetLobbyCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp)
{
    ABaseCharacter* MyChar = Cast<ABaseCharacter>(GetPawn());

    if (!MyChar)
        return;

    MyChar->GetMesh()->SetSkeletalMesh(skeletalMesh);
    MyChar->GetMesh()->SetAnimInstanceClass(animBp->GetAnimBlueprintGeneratedClass());
}

void ALobbyPlayerController::ReqPlayAnimMontage_Implementation(UAnimMontage* levelStartMontage)
{
    RecPlayAnimMontage(levelStartMontage);
}

void ALobbyPlayerController::RecPlayAnimMontage_Implementation(UAnimMontage* levelStartMontage)
{
    ABaseCharacter* MyChar = Cast<ABaseCharacter>(GetPawn());

    if (!MyChar)
        return;

    MyChar->PlayAnimMontage(levelStartMontage);
}

