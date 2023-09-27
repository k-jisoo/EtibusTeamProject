// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BaseCharacter.h"
#include "TPGameInstance.h"
#include "Engine/World.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());

	if (LocalPlayerSystem && DefaultMappingContext)
	{
		LocalPlayerSystem->AddMappingContext(DefaultMappingContext, 0);
	}
	else
	{
		if (!LocalPlayerSystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Found LocalPlayerSystem"));
		}
	}

	GetWorldTimerManager().SetTimer(MyHandle, this, &AMainPlayerController::InitCharacter, 1.0f, false);
}

void AMainPlayerController::InitCharacter()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	UTPGameInstance* GI = Cast<UTPGameInstance>(GetWorld()->GetGameInstance());

	if (!(GI->MyCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("!MyCharacter"));
		return;
	}

	ABaseCharacter* ABC = Cast<ABaseCharacter>(GetPawn());

	if (!ABC)
	{
		UE_LOG(LogTemp, Warning, TEXT("!ABC"));
		return;
	}

	ABC->ReqSetCharacter(GI->MyCharacter->SkeletalMesh, GI->MyCharacter->AnimBP, GI->MyCharacter->FirstAttackMontage, GI->MyCharacter->SecondAttackMontage, GI->MyCharacter->ThirdAttackMontage, GI->MyCharacter->FourthAttackMontage, GI->MyCharacter->LevelStartMontage, GI->MyCharacter->MaxHp, GI->MyCharacter->Damage, GI->MyCharacter->Speed, GI->MyCharacter->CapsuleHeight, GI->MyCharacter->CapsuleRadius, GI->MyCharacter->BoxCollisionExt, GI->MyCharacter->HitParticle);

	ABC->ReqSetWeapon();
}
