// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "MainPlayerController.h"
#include "BaseCharacter.h"
#include "TPGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void AMainGameMode::BeginPlay()
{

}

void AMainGameMode::InitCharacter()
{
    UTPGameInstance* GI = Cast<UTPGameInstance>(GetWorld()->GetGameInstance());

    if (!GI)
        return;

    if (!GetWorld() || !(GetWorld()->GetFirstPlayerController()))
        return;

    ABaseCharacter* CurrentPawn = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

    for (auto iter = GetWorld()->GetControllerIterator(); iter; iter--)
    {
        UE_LOG(LogTemp, Warning, TEXT("Iter : %d"), *iter->Get()->GetName());
        AMainPlayerController* PC = Cast<AMainPlayerController>(iter->Get());

        if (!PC)
        {
            UE_LOG(LogTemp, Warning, TEXT("!PC"));
            return;
        }

        ABaseCharacter* ABC = Cast<ABaseCharacter>(PC->GetPawn());

        if (!ABC)
        {
            UE_LOG(LogTemp, Warning, TEXT("!ABC"));
            return;
        }

        ABC->ReqSetCharacter(GI->MyCharacter->SkeletalMesh, GI->MyCharacter->AnimBP, GI->MyCharacter->FirstAttackMontage, GI->MyCharacter->SecondAttackMontage, GI->MyCharacter->ThirdAttackMontage, GI->MyCharacter->FourthAttackMontage, GI->MyCharacter->LevelStartMontage, GI->MyCharacter->MaxHp, GI->MyCharacter->Damage, GI->MyCharacter->Speed, GI->MyCharacter->CapsuleHeight, GI->MyCharacter->CapsuleRadius, GI->MyCharacter->BoxCollisionExt, GI->MyCharacter->HitParticle);

        ABC->ReqSetWeapon();
    }
}
