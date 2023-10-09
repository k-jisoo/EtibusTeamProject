// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMainGameMode();
	
	void RoundFinished();

	void GameOver();

	void MoveToLobby();

	FTimerHandle TH_Timer;

	int DeadPlayerNum;

	int MonsterCnt;

	int KillCnt;
};
