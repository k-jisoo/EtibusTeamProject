// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

AMainGameMode::AMainGameMode()
{
	DeadPlayerNum = 0;
	MonsterCnt = 1;
	KillCnt = 0;
}

void AMainGameMode::RoundFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("RoundFinished, KillCnt = %d, MonsterCnt = %d"), KillCnt, MonsterCnt);

	if (KillCnt == MonsterCnt)
	{
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++)
		{
			AMainPlayerController* PC = Cast<AMainPlayerController>(*Iter);
			if (!PC)
				return;

			PC->MulticastOnWidgetCreated();
		}
	}
}

void AMainGameMode::GameOver()
{
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(It->Get());

		if (MainPlayerController->IsAlive == false)
		{
			DeadPlayerNum++;
		}
	}

	if (DeadPlayerNum == GetWorld()->GetNumPlayerControllers() - 1)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();

		timerManager.SetTimer(TH_Timer, this, &AMainGameMode::MoveToLobby, 2.0f, false);
	}

	UE_LOG(LogTemp, Warning, TEXT("players : %d"), GetWorld()->GetNumPlayerControllers());
}

void AMainGameMode::MoveToLobby()
{
	GetWorld()->ServerTravel(TEXT("Lobby"));
}
