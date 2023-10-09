// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "EnemyAIController.h"
#include "Net/UnrealNetwork.h"


UEnemyAnim::UEnemyAnim()
{
	
}

void UEnemyAnim::OnAttackEndAnimation()
{
	me = Cast<AEnemy>(TryGetPawnOwner());
	me->bAttacking = false;
}




